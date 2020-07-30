#include "DLMSLogUI.h"
#include "ui_DLMSLogUI.h"
#include <QFileDialog>
#include <algorithm>
#include <PRIME/PrimeAdapter.h>
#include <QTableWidgetItem>
#include <dlms/dlmsmsg.h>
#include "XMLHighlighter.h"

QString
DlmsMessage::toText(void) const
{
  QString asHex;
  char string[4];

  if (this->xml.size() > 0)
    return this->xml;

  for (int i = 0; i < this->pdu.size(); ++i) {
    snprintf(string, sizeof(string), "%02x", this->pdu[i]);
    asHex += string;
  }

  return asHex;
}

void
DLMSLogUI::saveLog(QString)
{

}

void
DLMSLogUI::saveMessage(const DlmsMessage &msg)
{
  int rows = this->ui->tableWidget->rowCount();

  this->messageList.push_back(msg);

  this->ui->tableWidget->insertRow(rows);

  this->ui->tableWidget->setItem(
        rows,
        0,
        new QTableWidgetItem(QString::number(rows + 1)));

  this->ui->tableWidget->setItem(
        rows,
        1,
        new QTableWidgetItem(msg.timeStamp.toString()));

  this->ui->tableWidget->setItem(
        rows,
        2,
        new QTableWidgetItem(msg.downlink ? "Downlink" : "Uplink"));

  this->ui->tableWidget->setItem(
        rows,
        3,
        new QTableWidgetItem(msg.type));

  this->ui->tableWidget->setItem(
        rows,
        4,
        new QTableWidgetItem(msg.nodeId));

  this->ui->tableWidget->setItem(
        rows,
        5,
        new QTableWidgetItem(QString::number(msg.pdu.size())));

  this->ui->tableWidget->setItem(
        rows,
        6,
        new QTableWidgetItem(msg.SNA));

  this->ui->tableWidget->resizeColumnsToContents();

  if (this->ui->autoScrollButton->isChecked())
    this->ui->tableWidget->scrollToBottom();
}


void
DLMSLogUI::connectAll(void)
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

}

void
DLMSLogUI::colorizeRow(int row, const QColor &color)
{
  int i;

  if (row >= 0 && row < this->ui->tableWidget->rowCount()) {
    for (i = 0; i < this->ui->tableWidget->columnCount(); ++i) {
      this->ui->tableWidget->item(row, i)->setBackground(QBrush(color));
    }
  }
}

void
DLMSLogUI::pushMessage(
    const PLCTool::Concentrator *concentrator,
    const PLCTool::NodeId id,
    bool downlink,
    const void *dataBytes,
    size_t size)
{
  DlmsMessage msg;
  CGXByteBuffer dataBuffer;
  std::string str;
  char nodeId[32];

  snprintf(nodeId, sizeof(nodeId), "%06lx", id);

  msg.downlink = downlink;
  msg.SNA = QString::fromStdString(
        PLCTool::PrimeAdapter::idToSna(concentrator->id()));
  msg.nodeId = nodeId;
  msg.timeStamp = QDateTime::currentDateTime();

  msg.pdu.resize(size);

  std::copy(
        static_cast<const uint8_t *>(dataBytes),
        static_cast<const uint8_t *>(dataBytes) + size,
        std::begin(msg.pdu));

  if (msg.pdu.size() > 0)
    msg.type = dlms_command_to_str(
          static_cast<DLMS_COMMAND>(msg.pdu[0]));
  else
    msg.type = "<!-- empty -->";


  dataBuffer.Set(dataBytes, size);
  if (this->translator.PduToXml(dataBuffer, str) == 0)
    msg.xml = QString::fromStdString(str);
  else
    msg.xml = "<!-- Decode failed -->";

  this->saveMessage(msg);
}

DLMSLogUI::DLMSLogUI(QWidget *parent) :
  QWidget(parent),
  translator(DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML),
  ui(new Ui::DLMSLogUI)
{
  ui->setupUi(this);

  this->connectAll();

  this->highlighter = new XMLHighlighter(this->ui->xmlEdit->document());

  this->savedText = this->ui->xmlEdit->toPlainText();
}

DLMSLogUI::~DLMSLogUI()
{
  delete ui;
}

//////////////////////////////////// Slots ////////////////////////////////////
void
DLMSLogUI::onCellActivated(int row, int)
{
  if (row >= 0 && row < this->messageList.size()) {
    this->ui->xmlEdit->setText(this->messageList[row].toText());
  } else {
    this->ui->xmlEdit->setText(this->savedText);
  }
}

void
DLMSLogUI::onCurrentChanged(QModelIndex curr, QModelIndex)
{
  onCellActivated(curr.row(), curr.column());
}

void
DLMSLogUI::onSaveAs(bool)
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
DLMSLogUI::onClear(bool)
{
  this->messageList.clear();
  this->ui->tableWidget->setRowCount(0);
  this->ui->xmlEdit->setText("");
}

