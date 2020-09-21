#include "CredentialsUI.h"
#include "ui_CredentialsUI.h"
#include <QTableWidgetItem>
#include "PRIME/PrimeAdapter.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include "CredInfoTableModel.h"

CredentialsUI::CredentialsUI(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CredentialsUI)
{
  ui->setupUi(this);

  this->model = new CredInfoTableModel(this, &this->credList);
  this->proxy = new QSortFilterProxyModel(this);

  this->proxy->setSourceModel(this->model);
  this->ui->tableView->setModel(this->proxy);
  this->setSortingEnabled(false);
  this->proxy->sort(1);

  this->connectAll();
}

void
CredentialsUI::connectAll(void)
{
  connect(
        this->ui->saveButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onSaveAs(bool)));

  connect(
        this->ui->clearButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onClear(bool)));
}

void
CredentialsUI::clear(void)
{
  this->credList.clear();
  this->refreshCredentials();
}

void
CredentialsUI::saveLog(QString path)
{
  FILE *fp = nullptr;

  if ((fp = fopen(path.toStdString().c_str(), "w")) == nullptr) {
    QMessageBox::critical(
          this,
          "Failed to save frame log",
          "Cannot open " + path + " for writing: " + strerror(errno));
  } else {
    int i;

    for (i = 0; i < this->credList.count(); ++i) {
      CredInfo *c = &this->credList[i];

      fprintf(
            fp,
            "%u,%s,%06lx,%s,%s,%s,%s\n",
            c->timeStamp.toTime_t(),
            c->owner->meter()->name().size() > 0
              ? c->owner->meter()->name().c_str()
              : "N/A",
            c->owner->meter()->id(),
            c->owner->meter()->macAddr().size() > 0
              ? c->owner->meter()->macAddr().c_str()
              : "N/A",
            PLCTool::PrimeAdapter::idToSna(
              c->owner->meter()->parent()->parent()->id()).c_str(),
            c->password.toStdString().c_str(),
            c->contexts.join(",").toStdString().c_str());
    }

    fclose(fp);
  }
}


void
CredentialsUI::setSortingEnabled(bool enabled)
{
  this->sortingEnabled = enabled;
  this->ui->tableView->setModel(
        enabled
        ? static_cast<QAbstractItemModel *>(this->proxy)
        : static_cast<QAbstractItemModel *>(this->model));
  this->ui->tableView->setSortingEnabled(enabled);
}

void
CredentialsUI::saveCreds(CredInfo const &creds)
{
  this->pendingList.append(creds);
}

void
CredentialsUI::refreshCredentials(void)
{
  if (this->pendingList.size() > 0) {
    int rows;
    bool firstIter = this->credList.size() == 0;
    bool oldAdjusting = this->adjusting;
    this->adjusting = true;

    this->model->appendData(this->pendingList);
    this->pendingList.clear();

    rows = this->credList.size();

    this->ui->lineSpin->setMinimum(1);
    this->ui->lineSpin->setMaximum(rows);
    this->ui->lineSpin->setEnabled(true);
    this->ui->gotoButton->setEnabled(true);
    this->ui->topButton->setEnabled(true);
    this->ui->bottomButton->setEnabled(true);

    if (this->ui->autoScrollButton->isChecked())
      this->ui->tableView->scrollToBottom();

    if (firstIter)
      this->ui->tableView->resizeColumnsToContents();

    this->adjusting = oldAdjusting;
  }
}


CredentialsUI::~CredentialsUI()
{
  delete ui;
}

/////////////////////////////// Slots //////////////////////////////////////////
void
CredentialsUI::onClear(bool)
{

}

void
CredentialsUI::onSaveAs(bool)
{
  QFileDialog dialog(this);

  dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
  dialog.setNameFilter("Password log (*.log)");
  dialog.setViewMode(QFileDialog::Detail);
  dialog.setAcceptMode(QFileDialog::AcceptSave);

  if (dialog.exec()) {
    QStringList list = dialog.selectedFiles();

    if (list.size() > 0)
      this->saveLog(list.at(0));
  }
}
