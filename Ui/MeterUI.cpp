#include "MeterUI.h"
#include "ui_MeterUI.h"
#include "PRIME/PrimeAdapter.h"
#include "XMLHighlighter.h"

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

  this->highlighter = new XMLHighlighter(this->ui->xmlEdit->document());

  this->savedHtml = this->ui->hexEdit->toHtml();
  this->savedText = this->ui->xmlEdit->toPlainText();

  for (auto p : *(info->credList()))
    this->onCredentialsFound(p.timeStamp, p.password, p.context);

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
  connect(
        this->ui->frameView,
        SIGNAL(activated(const QModelIndex &)),
        this,
        SLOT(onFrameCellActivated(const QModelIndex &)));

  connect(
        this->ui->frameView,
        SIGNAL(clicked(const QModelIndex &)),
        this,
        SLOT(onFrameCellActivated(const QModelIndex &)));

  connect(
        this->ui->frameView->selectionModel(),
        SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
        this,
        SLOT(onFrameCurrentChanged(QModelIndex,QModelIndex)));

  connect(
        this->ui->dlmsMessageView,
        SIGNAL(activated(const QModelIndex &)),
        this,
        SLOT(onDlmsCellActivated(const QModelIndex &)));

  connect(
        this->ui->dlmsMessageView,
        SIGNAL(clicked(const QModelIndex &)),
        this,
        SLOT(onDlmsCellActivated(const QModelIndex &)));

  connect(
        this->ui->dlmsMessageView->selectionModel(),
        SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
        this,
        SLOT(onDlmsCurrentChanged(QModelIndex,QModelIndex)));

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

  connect(
        this->info,
        SIGNAL(credentialsFound(QDateTime,QString,QString)),
        this,
        SLOT(onCredentialsFound(QDateTime,QString,QString)));
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

  this->ui->macLabel->setText(this->info->mac());
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

void
MeterUI::onFrameCellActivated(const QModelIndex &index)
{
  Frame *frame = nullptr;
  int row = index.row();

  if (row >= 0 && row < this->frameProxy->rowCount()) {
    QModelIndex trueIndex = this->frameProxy->mapToSource(index);

    int ndx = this->frameModel->data(trueIndex, Qt::UserRole).value<int>();

    if (ndx >= 0 && ndx < this->info->frameList()->count())
      frame = &(*this->info->frameList())[ndx];
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
MeterUI::onFrameCurrentChanged(QModelIndex curr, QModelIndex)
{
  onFrameCellActivated(curr);
}

void
MeterUI::onDlmsCellActivated(QModelIndex const &index)
{
  DlmsMessage *msg = nullptr;
  int row = index.row();

  if (row >= 0 && row < this->dlmsProxy->rowCount()) {
    QModelIndex trueIndex = this->dlmsProxy->mapToSource(index);
    int ndx = this->dlmsModel->data(trueIndex, Qt::UserRole).value<int>();

    if (ndx >= 0 && ndx < this->info->messageList()->count())
      msg = &(*this->info->messageList())[ndx];
  }

  if (msg != nullptr) {
    this->ui->xmlEdit->setText(msg->toText());
  } else {
    this->ui->xmlEdit->setText(this->savedText);
  }
}

void
MeterUI::onDlmsCurrentChanged(QModelIndex curr, QModelIndex)
{
  onDlmsCellActivated(curr);
}

void
MeterUI::onCredentialsFound(QDateTime timeStamp, QString passwd, QString ctx)
{
  QTableWidgetItem *timeItem = new QTableWidgetItem(timeStamp.toString());
  QTableWidgetItem *typeItem = new QTableWidgetItem(QString("LLS"));
  QTableWidgetItem *pwdItem = new QTableWidgetItem(passwd);
  QTableWidgetItem *ctxItem = new QTableWidgetItem(ctx);

  int rows = this->ui->credTableWidget->rowCount();

  this->ui->credTableWidget->insertRow(rows);

  this->ui->credTableWidget->setItem(
        rows,
        0,
        timeItem);
  this->ui->credTableWidget->setItem(
        timeItem->row(),
        1,
        typeItem);
  this->ui->credTableWidget->setItem(
        timeItem->row(),
        2,
        pwdItem);
  this->ui->credTableWidget->setItem(
        timeItem->row(),
        3,
        ctxItem);

  this->ui->credTableWidget->resizeColumnsToContents();
}
