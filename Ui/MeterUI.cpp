#include "MeterUI.h"
#include "ui_MeterUI.h"
#include "PRIME/PrimeAdapter.h"

using namespace PLCTool;

MeterUI::MeterUI(QWidget *parent, MeterInfo *info) :
    QWidget(parent),
    info(info),
    ui(new Ui::MeterUI)
{
  ui->setupUi(this);

  this->info = info;

  this->frameModel = new FrameTableModel(this, info->frameList());
  this->dlmsModel  = new DLMSTableModel(this, info->messageList());

  this->frameProxy = new QSortFilterProxyModel(this);
  this->frameProxy->setSourceModel(this->frameModel);

  this->dlmsProxy = new QSortFilterProxyModel(this);
  this->dlmsProxy->setSourceModel(this->dlmsModel);

  this->ui->frameView->setModel(this->frameProxy);
  this->ui->dlmsMessageView->setModel(this->dlmsProxy);

  this->ui->snLabel->setText(
        QString::fromStdString(
          PrimeAdapter::idToSna(info->meter()->parent()->parent()->id())));

  this->ui->nodeIdLabel->setText(
        QString().sprintf(
          "%06llx",
          static_cast<quint64>(info->meter()->id())));
  this->ui->sidLabel->setText(
        QString().sprintf(
          "0x%llx",
          static_cast<quint64>(PRIME13_SID(info->meter()->id()))));
  this->ui->lnidLabel->setText(
        QString().sprintf(
          "0x%llx",
          static_cast<quint64>(PRIME13_LNID(info->meter()->id()))));


  this->connectAll();
  this->connectMeterInfo();
  this->refreshViews();
}

MeterUI::~MeterUI()
{
  delete ui;
}

void
MeterUI::connectAll(void)
{

}

void
MeterUI::connectMeterInfo(void)
{
  connect(
        this->info,
        SIGNAL(frameReceived(Frame)),
        this,
        SLOT(onFrame(Frame)));

  connect(
        this->info,
        SIGNAL(messageReceived(DlmsMessage)),
        this,
        SLOT(onDlmsMessage(DlmsMessage)));
}

void
MeterUI::refreshViews(void)
{
  this->frameModel->refreshData();
  this->dlmsModel->refreshData();

  if (this->isVisible()) {
    this->ui->frameView->scrollToBottom();
    this->ui->dlmsMessageView->scrollToBottom();
  }

  for (int i = 0; i < 8; ++i) {
    this->ui->frameView->resizeColumnToContents(i);
    this->ui->dlmsMessageView->resizeColumnToContents(i);
  }

  if (!this->info->frameList()->empty())
    this->ui->lastActivityLabel->setText(
        this->info->frameList()->last().timeStamp.toString());

  if (this->info->meter()->name().size() > 0)
    this->ui->nameLabel->setText(
        QString::fromStdString(this->info->meter()->name()));
}

void
MeterUI::onFrame(Frame frame)
{
  this->ui->lastActivityLabel->setText(frame.timeStamp.toString());
}

void
MeterUI::onDlmsMessage(DlmsMessage message)
{
  this->ui->lastActivityLabel->setText(message.timeStamp.toString());
}

void
MeterUI::onCreds(QString, QString)
{

}
