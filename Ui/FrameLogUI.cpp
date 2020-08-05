#include "Ui/FrameLogUI.h"
#include "ui_FrameLogUI.h"
#include "PRIME/PrimeAdapter.h"
#include "PRIME/PrimeFrame.h"
#include <QDateTime>
#include <cctype>
#include <cstdio>
#include <cstring>

#include <QFileDialog>
#include <QMessageBox>

Q_DECLARE_METATYPE(Frame *);

Frame::~Frame()
{
  if (this->frame != nullptr)
    delete this->frame;
}

Frame::Frame()
{

}


Frame::Frame(const Frame &original)
{
  PLCTool::PrimeFrame *dup = original.frame;

  if (dup != nullptr)
    dup = new PLCTool::PrimeFrame(*dup);

  this->SNA = original.SNA;
  this->downlink = original.downlink;
  this->bytes = original.bytes;
  this->timeStamp = original.timeStamp;
  this->frame = dup;
}

QString
Frame::toHtml(void) const
{
  size_t size = this->bytes.size();
  int rows = (size + 15) / 16;
  char string[16];
  int offset;
  QString result = "<html><body>";
  QString style = "border-right: solid; font-family: &quot;Cascadia Code PL&quot;";

  result += "<table>\n";
  for (int j = 0; j < rows; ++j) {
    offset = j << 4;
    result += "<tr><td style=\"color: red; " + style + "\">";
    snprintf(string, sizeof(string), "%04x", offset);
    result += string;
    result += "  </td>";

    result += "<td style=\"color: blue; " + style + "\">";
    for (int i = 0, offset = j << 4; i < 16; ++i, ++offset) {
      if (offset < static_cast<int>(size))
        snprintf(string, sizeof(string), "%02x ", this->bytes[offset]);
      else
        strncpy(string, "   ", sizeof(string));

      result += string;

      if (offset == 7)
        result += " ";
    }
    result += "</td>";

    result += "<td style=\"" + style + "\">";
    for (int i = 0, offset = j << 4; i < 16; ++i, ++offset) {
      if (offset < static_cast<int>(size))
        snprintf(
              string,
              sizeof(string),
              "%c",
              isprint(this->bytes[offset]) ? this->bytes[offset] : '.');
      else
        strncpy(string, " ", sizeof(string));

      result += string;
    }
    result += "</td>";

    result += "</tr>\n";
  }

  result += "</table>\n";
  result += "</body>\n";
  result += "</html>\n";

  return result;
}

FrameLogUI::FrameLogUI(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::FrameLogUI)
{
  ui->setupUi(this);

  this->connectAll();

  this->savedHtml = this->ui->hexEdit->toHtml();
}

void
FrameLogUI::saveLog(QString path)
{
  FILE *fp = nullptr;

  if ((fp = fopen(path.toStdString().c_str(), "w")) == nullptr) {
    QMessageBox::critical(
          this,
          "Failed to save frame log",
          "Cannot open " + path + " for writing: " + strerror(errno));
  } else {
    int i;

    for (i = 0; i < this->frameList.count(); ++i) {
      Frame *f = &this->frameList[i];

      fprintf(
            fp,
            "%d,%s,%c,%d,%s,%06x,%d,",
            i + 1,
            f->SNA.toStdString().c_str(),
            f->downlink ? 'D' : 'U',
            f->timeStamp.toTime_t(),
            f->frame != nullptr
              ? f->frame->typeToString().c_str()
              : "Corrupt",
            (f->frame != nullptr
              && f->frame->PDU.macType == PLCTool::PrimeFrame::GENERIC)
              ? f->frame->PDU.PKT.LNID
              : 0xffffff,
            f->bytes.size());

      for (int j = 0; j < f->bytes.size(); ++j)
        fprintf(fp, "%02x", f->bytes[j]);

      fprintf(fp, "\n");
    }

    fclose(fp);
  }
}

int
FrameLogUI::saveFrame(
    const PLCTool::Concentrator *concentrator,
    bool downlink,
    const void *data,
    size_t size,
    PLCTool::PrimeFrame *dszFrame)
{
  Frame frame;

  frame.SNA          = QString::fromStdString(
        PLCTool::PrimeAdapter::idToSna(concentrator->id()));
  frame.downlink     = downlink;
  frame.bytes.reserve(size);
  frame.timeStamp    = QDateTime::currentDateTime();
  frame.frame        = dszFrame;


  std::copy(
        static_cast<const uint8_t *>(data),
        static_cast<const uint8_t *>(data) + size,
        std::back_inserter(frame.bytes));

  this->frameList.append(frame);

  return this->frameList.count() - 1;
}

void
FrameLogUI::connectAll(void)
{
  connect(
        this->ui->tableWidget,
        SIGNAL(cellActivated(int,int)),
        this,
        SLOT(onCellActivated(int,int)));

  connect(
        this->ui->tableWidget,
        SIGNAL(cellClicked(int,int)),
        this,
        SLOT(onCellActivated(int,int)));

  connect(
        this->ui->tableWidget->selectionModel(),
        SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
        this,
        SLOT(onCurrentChanged(QModelIndex,QModelIndex)));

  connect(
        this->ui->clearButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onClear(bool)));

  connect(
        this->ui->saveButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onSaveAs(bool)));

  connect(
        this->ui->topButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onTop()));

  connect(
        this->ui->bottomButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onBottom()));

  connect(
        this->ui->gotoButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onGotoLine()));
}

void
FrameLogUI::colorizeRow(int row, const QColor &color)
{
  int i;

  if (row >= 0 && row < this->ui->tableWidget->rowCount()) {
    for (i = 0; i < this->ui->tableWidget->columnCount(); ++i) {
      this->ui->tableWidget->item(row, i)->setBackground(QBrush(color));
    }
  }
}


void
FrameLogUI::pushFrame(
    const PLCTool::Concentrator *concentrator,
    bool downlink,
    const void *dataBytes,
    size_t size)
{
  int rows = this->ui->tableWidget->rowCount();
  PLCTool::NodeId dcId = concentrator->id();

  uint8_t mac[6];

  for (int i = 0; i < 6; ++i)
    mac[i] = static_cast<uint8_t>(dcId >> ((5 - i) * 8ull));

  PLCTool::PrimeFrame *frame = PLCTool::PrimeFrame::fromRawData(
        mac,
        static_cast<const uint8_t *>(dataBytes),
        size);

  const uint8_t *asBytes = static_cast<const uint8_t *>(dataBytes);
  QTableWidgetItem *id = new QTableWidgetItem();
  QTableWidgetItem *timeStamp =
      new QTableWidgetItem(QDateTime::currentDateTime().toString());
  QTableWidgetItem *dir = new QTableWidgetItem(downlink ? "Downlink" : "Uplink");
  QTableWidgetItem *type = new QTableWidgetItem(
        frame == nullptr ? "Corrupt" : frame->typeToString().c_str());
  QTableWidgetItem *sz = new QTableWidgetItem();
  QTableWidgetItem *SNA =
      new QTableWidgetItem(
        QString::fromStdString(
          PLCTool::PrimeAdapter::idToSna(concentrator->id())));
  char lnid[8] = "ffffff";
  QString byteString;

  if (frame != nullptr) {
    snprintf(
          lnid,
          sizeof(lnid),
          "%06x",
          frame->PDU.macType == PLCTool::PrimeFrame::GENERIC
           ? frame->PDU.PKT.LNID
           : 0xffffff);
  }

  for (unsigned int i = 0; i < size; ++i) {
    char byte[4];
    snprintf(byte, 4, "%02x", asBytes[i]);
    byteString += QString(byte) + " ";
  }

  QTableWidgetItem *data = new QTableWidgetItem(byteString);

  id->setData(Qt::DisplayRole, QVariant::fromValue<int>(rows + 1));
  sz->setData(Qt::DisplayRole, QVariant::fromValue<int>(size));

  this->ui->tableWidget->insertRow(rows);
  this->ui->tableWidget->setItem(
        rows,
        0,
        id);
  this->ui->tableWidget->setItem(
        id->row(),
        1,
        timeStamp);
  this->ui->tableWidget->setItem(
        id->row(),
        2,
        dir);
  this->ui->tableWidget->setItem(
        id->row(),
        3,
        type);
  this->ui->tableWidget->setItem(
        id->row(),
        4,
        new QTableWidgetItem(QString(lnid)));
  this->ui->tableWidget->setItem(
        id->row(),
        5,
        sz);
  this->ui->tableWidget->setItem(
        id->row(),
        6,
        SNA);
  this->ui->tableWidget->setItem(
        id->row(),
        7,
        data);

  this->ui->lineSpin->setMinimum(1);
  this->ui->lineSpin->setMaximum(rows);
  this->ui->lineSpin->setEnabled(true);
  this->ui->gotoButton->setEnabled(true);
  this->ui->topButton->setEnabled(true);
  this->ui->bottomButton->setEnabled(true);

  if (frame == nullptr) {
    this->colorizeRow(id->row(), QColor::fromRgb(0xff, 0x80, 0x80));
  } else if (frame->PDU.macType == PLCTool::PrimeFrame::BEACON) {
    this->colorizeRow(id->row(), QColor::fromRgb(0xc0, 0xc0, 0xc0));
  } else if (frame->PDU.macType == PLCTool::PrimeFrame::GENERIC) {
    if (frame->PDU.genType == PLCTool::PrimeFrame::DATA)
      this->colorizeRow(id->row(), QColor::fromRgb(0x80, 0xff, 0x80));
  }

  this->ui->tableWidget->resizeColumnsToContents();

  if (this->ui->autoScrollButton->isChecked())
    this->ui->tableWidget->scrollToBottom();

  // Don't delete the frame. Just save it.
  id->setData(
        Qt::UserRole,
        QVariant::fromValue(
          this->saveFrame(concentrator, downlink, dataBytes, size, frame)));

}

FrameLogUI::~FrameLogUI()
{
  delete ui;
}


//////////////////////////////////// Slots ////////////////////////////////////
void
FrameLogUI::onCellActivated(int row, int)
{
  Frame *frame = nullptr;

  if (row >= 0 && row < this->ui->tableWidget->rowCount()) {
    int ndx = this->ui->tableWidget->item(row, 0)->data(Qt::UserRole).value<int>();
    if (ndx >= 0 && ndx < this->frameList.count())
      frame = &this->frameList[ndx];
  }

  if (frame != nullptr) {
    this->ui->hexEdit->setHtml(frame->toHtml());
    this->ui->pktEdit->setText(frame->frame != nullptr
          ? QString::fromStdString(frame->frame->toString())
          : "");
  } else {
    this->ui->hexEdit->setHtml(this->savedHtml);
    this->ui->pktEdit->setText("");
  }
}

void
FrameLogUI::onCurrentChanged(QModelIndex curr, QModelIndex)
{
  onCellActivated(curr.row(), curr.column());
}

void
FrameLogUI::onSaveAs(bool)
{
  QFileDialog dialog(this);

  dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
  dialog.setNameFilter("Packet log (*.log)");
  dialog.setViewMode(QFileDialog::Detail);
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  if (dialog.exec()) {
    QStringList list = dialog.selectedFiles();

    if (list.size() > 0)
      this->saveLog(list.at(0));
  }
}

void
FrameLogUI::onClear(bool)
{
  this->frameList.clear();
  this->ui->tableWidget->setRowCount(0);
  this->ui->hexEdit->setHtml(this->savedHtml);
  this->ui->pktEdit->setText("");
  this->ui->lineSpin->setMinimum(0);
  this->ui->lineSpin->setMaximum(0);
  this->ui->lineSpin->setEnabled(false);
  this->ui->gotoButton->setEnabled(false);
  this->ui->topButton->setEnabled(false);
  this->ui->bottomButton->setEnabled(false);
}

void
FrameLogUI::onTop(void)
{
  this->ui->tableWidget->scrollToTop();
}

void
FrameLogUI::onBottom(void)
{
  this->ui->tableWidget->scrollToBottom();
}

void
FrameLogUI::onGotoLine(void)
{
  this->ui->tableWidget->scrollTo(
        this->ui->tableWidget->model()->index(
          this->ui->lineSpin->value() - 1,
          0));
}
