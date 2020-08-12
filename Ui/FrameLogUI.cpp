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

Q_DECLARE_METATYPE(QVector<uint8_t>)
static bool typesRegistered = false;

FrameLogUI::FrameLogUI(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::FrameLogUI)
{
  ui->setupUi(this);

  this->model = new FrameTableModel(this, &this->frameList);
  this->proxy = new QSortFilterProxyModel(this);

  this->proxy->setSourceModel(this->model);
  this->ui->frameView->setModel(this->proxy);
  this->ui->frameView->setSortingEnabled(true);
  this->proxy->sort(-1);
  this->connectAll();

  this->processor = new PRIMEProcessor(nullptr);
  this->procThread = new QThread(nullptr);
  this->processor->moveToThread(this->procThread);
  this->connectProcessor();
  this->procThread->start();

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

void
FrameLogUI::connectAll(void)
{
  connect(
        this->ui->frameView,
        SIGNAL(activated(const QModelIndex &)),
        this,
        SLOT(onCellActivated(const QModelIndex &)));

  connect(
        this->ui->frameView,
        SIGNAL(clicked(const QModelIndex &)),
        this,
        SLOT(onCellActivated(const QModelIndex &)));

  connect(
        this->ui->frameView->selectionModel(),
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
FrameLogUI::connectProcessor(void)
{
  // Thread lifecycle management
  connect(
        this->procThread,
        SIGNAL(finished()),
        this->procThread,
        SLOT(deleteLater()));

  connect(
        this->procThread,
        SIGNAL(finished()),
        this->processor,
        SLOT(deleteLater()));

  // Object message passing
  connect(
        this,
        SIGNAL(frameReceived(quint64,QDateTime,bool,QVector<uint8_t>)),
        this->processor,
        SLOT(process(quint64,QDateTime,bool,QVector<uint8_t>)));

  connect(
        this->processor,
        SIGNAL(frame(Frame)),
        this,
        SLOT(onFrame(Frame)));
}

void
FrameLogUI::clear(void)
{
  this->onClear(false);
}

void
FrameLogUI::saveFrame(Frame const &frame)
{
  this->frameList.append(frame);
}

void
FrameLogUI::refreshFrames(void)
{
  int rows = this->frameList.size();

  this->model->refreshData();

  this->ui->lineSpin->setMinimum(rows > 0 ? 1 : 0);
  this->ui->lineSpin->setMaximum(rows);
  this->ui->lineSpin->setEnabled(rows > 0);
  this->ui->gotoButton->setEnabled(rows > 0);
  this->ui->topButton->setEnabled(rows > 0);
  this->ui->bottomButton->setEnabled(rows > 0);

  if (this->ui->autoScrollButton->isChecked())
    this->ui->frameView->scrollToBottom();

  for (int i = 0; i < 8; ++i)
    this->ui->frameView->resizeColumnToContents(i);
}

void
FrameLogUI::registerTypes(void)
{
  if (!typesRegistered) {
    qRegisterMetaType<QVector<uint8_t>>();
    typesRegistered = true;
  }
  FrameTableModel::registerTypes();
}

void
FrameLogUI::pushFrame(
    const PLCTool::Concentrator *concentrator,
    QDateTime timeStamp,
    bool downlink,
    const void *dataBytes,
    size_t size)
{
  PLCTool::NodeId dcId = concentrator->id();
  QVector<uint8_t> bytes;

  std::copy(
        static_cast<const uint8_t *>(dataBytes),
        static_cast<const uint8_t *>(dataBytes) + size,
        std::back_inserter(bytes));

  emit frameReceived(dcId, timeStamp, downlink, bytes);
}

void
FrameLogUI::selectNear(
    QDateTime const &,
    PLCTool::PrimeFrame::GenericType,
    PLCTool::NodeId)
{

}

FrameLogUI::~FrameLogUI()
{
  this->procThread->quit();

  delete ui;
}


//////////////////////////////////// Slots ////////////////////////////////////
void
FrameLogUI::onCellActivated(const QModelIndex &index)
{
  Frame *frame = nullptr;
  int row = index.row();

  if (row >= 0 && row < this->proxy->rowCount()) {
    QModelIndex trueIndex = this->proxy->mapToSource(index);

    int ndx = this->model->data(trueIndex, Qt::UserRole).value<int>();

    if (ndx >= 0 && ndx < this->frameList.count())
      frame = &this->frameList[ndx];
  }

  if (frame != nullptr) {
    emit frameSelected(*frame);
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
  onCellActivated(curr);
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
  this->refreshFrames();
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
  this->ui->frameView->scrollToTop();
}

void
FrameLogUI::onBottom(void)
{
  this->ui->frameView->scrollToBottom();
}

void
FrameLogUI::onGotoLine(void)
{
  this->ui->frameView->scrollTo(
        this->ui->frameView->model()->index(
          this->ui->lineSpin->value() - 1,
          0));
}

void
FrameLogUI::onFrame(Frame frame)
{
  this->saveFrame(frame);
}
