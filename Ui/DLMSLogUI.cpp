#include "DLMSLogUI.h"
#include "ui_DLMSLogUI.h"
#include <QFileDialog>
#include <algorithm>
#include <QTableWidgetItem>
#include <QMessageBox>
#include "XMLHighlighter.h"

void
DLMSLogUI::saveLog(QString path)
{
  FILE *fp = nullptr;

  if ((fp = fopen(path.toStdString().c_str(), "w")) == nullptr) {
    QMessageBox::critical(
          this,
          "Failed to save frame log",
          "Cannot open " + path + " for writing: " + strerror(errno));
  } else {
    int i;

    for (i = 0; i < this->messageList.count(); ++i) {
      DlmsMessage *f = &this->messageList[i];

      fprintf(
            fp,
            "MESSAGE:%d,%s,%c,%d,%s,%06lx,%d,",
            i + 1,
            f->SNA.toStdString().c_str(),
            f->downlink ? 'D' : 'U',
            f->timeStamp.toTime_t(),
            f->type.toStdString().c_str(),
            f->nodeId,
            f->pdu.size());

      for (int j = 0; j < f->pdu.size(); ++j)
        fprintf(fp, "%02x", f->pdu[j]);

      fprintf(fp, "\n");
      fwrite(f->xml.data(), f->xml.size(), 1, fp);
      fprintf(fp, "\n");
    }

    fclose(fp);
  }
}

void
DLMSLogUI::saveMessage(const DlmsMessage &msg)
{
  this->pendingList.append(msg);
}

void
DLMSLogUI::refreshMessages(void)
{
  if (this->pendingList.size() > 0) {
    int rows;
    bool firstIter = this->messageList.size() == 0;
    bool oldAdjusting = this->adjusting;
    this->adjusting = true;

    if (this->sortingEnabled)
      this->ui->messageView->setModel(this->model);

    this->model->appendData(this->pendingList);
    this->pendingList.clear();

    rows = this->messageList.size();
    if (this->sortingEnabled)
      this->ui->messageView->setModel(this->proxy);

    this->ui->lineSpin->setMinimum(1);
    this->ui->lineSpin->setMaximum(rows);
    this->ui->lineSpin->setEnabled(true);
    this->ui->gotoButton->setEnabled(true);
    this->ui->topButton->setEnabled(true);
    this->ui->bottomButton->setEnabled(true);

    if (this->ui->autoScrollButton->isChecked())
      this->ui->messageView->scrollToBottom();

    if (firstIter)
      this->ui->messageView->resizeColumnsToContents();

    this->adjusting = oldAdjusting;
  }
}

int
DLMSLogUI::findMessage(QDateTime const &date)
{
  //int ndxA, ndxB;
  
  if (this->messageList.empty())
    return -1;

 // ndxA = 0;
  //ndxB = 0;

  if (date.msecsTo(this->messageList.first().timeStamp) > 0)
    return -1;

  if (date.msecsTo(this->messageList.last().timeStamp) < 0)
    return -1;

  return 0;
}

void
DLMSLogUI::selectNear(QDateTime const &, PLCTool::NodeId )
{
  //int findMessage(QDateTime const &);
}

void
DLMSLogUI::connectAll(void)
{
  connect(
        this->ui->messageView,
        SIGNAL(activated(const QModelIndex &)),
        this,
        SLOT(onCellActivated(const QModelIndex &)));

  connect(
        this->ui->messageView,
        SIGNAL(clicked(const QModelIndex &)),
        this,
        SLOT(onCellActivated(const QModelIndex &)));

  connect(
        this->ui->messageView->selectionModel(),
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
DLMSLogUI::registerTypes(void)
{
  DLMSProcessor::registerTypes();
}

void
DLMSLogUI::clear(void)
{
  this->onClear(false);
}

void
DLMSLogUI::setSortingEnabled(bool enabled)
{
  this->sortingEnabled = enabled;
  this->ui->messageView->setModel(
        enabled
        ? static_cast<QAbstractItemModel *>(this->proxy)
        : static_cast<QAbstractItemModel *>(this->model));
  this->ui->messageView->setSortingEnabled(enabled);
}


DLMSLogUI::DLMSLogUI(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DLMSLogUI)
{
  ui->setupUi(this);

  this->model = new DLMSTableModel(this, &this->messageList);
  this->proxy = new QSortFilterProxyModel(this);

  this->proxy->setSourceModel(this->model);
  this->ui->messageView->setModel(this->proxy);
  this->setSortingEnabled(false);
  this->proxy->sort(1);

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
DLMSLogUI::onCellActivated(QModelIndex const &index)
{
  DlmsMessage *msg = nullptr;
  int row = index.row();

  if (!this->adjusting && index.isValid()) {
    if (row >= 0 && row < this->ui->messageView->model()->rowCount()) {
      QModelIndex trueIndex =
          this->ui->messageView->model() == this->proxy
          ?  this->proxy->mapToSource(index)
          : index;
      int ndx = this->model->data(trueIndex, Qt::UserRole).value<int>();

      if (ndx >= 0 && ndx < this->messageList.count())
        msg = &this->messageList[ndx];
    }

    if (msg != nullptr) {
      emit messageSelected(*msg);
      this->ui->xmlEdit->setText(msg->toText());
    } else {
      this->ui->xmlEdit->setText(this->savedText);
    }
  }
}

void
DLMSLogUI::onCurrentChanged(QModelIndex curr, QModelIndex)
{
  onCellActivated(curr);
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
  this->refreshMessages();
  this->ui->xmlEdit->setText("");
  this->ui->lineSpin->setMinimum(0);
  this->ui->lineSpin->setMaximum(0);
  this->ui->lineSpin->setEnabled(false);
  this->ui->gotoButton->setEnabled(false);
  this->ui->topButton->setEnabled(false);
  this->ui->bottomButton->setEnabled(false);
}

void
DLMSLogUI::onTop(void)
{
  this->ui->messageView->scrollToTop();
}

void
DLMSLogUI::onBottom(void)
{
  this->ui->messageView->scrollToBottom();
}

void
DLMSLogUI::onGotoLine(void)
{
  if (!this->adjusting)
    this->ui->messageView->scrollTo(
          this->ui->messageView->model()->index(
            this->ui->lineSpin->value() - 1,
            0));
}
