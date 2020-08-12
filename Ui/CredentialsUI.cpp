#include "CredentialsUI.h"
#include "ui_CredentialsUI.h"
#include <QTableWidgetItem>
#include "PRIME/PrimeAdapter.h"
#include <QFileDialog>
#include <QMessageBox>

CredentialsUI::CredentialsUI(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CredentialsUI)
{
  ui->setupUi(this);

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
      Creds *c = &this->credList[i];

      fprintf(
            fp,
            "%d,%s,%d,%06x,%s\n",
            i + 1,
            c->SNA.toStdString().c_str(),
            c->timeStamp.toTime_t(),
            (unsigned int) c->meter,
            c->password.toStdString().c_str());
    }

    fclose(fp);
  }
}



void
CredentialsUI::saveCreds(
    const PLCTool::Concentrator *dc,
    QDateTime timeStamp,
    PLCTool::NodeId meter,
    QString password)
{
  Creds creds;

  creds.SNA       = QString::fromStdString(
        PLCTool::PrimeAdapter::idToSna(dc->id()));
  creds.meter     = meter;
  creds.password  = password;
  creds.timeStamp = timeStamp;

  this->credList.append(creds);
}

void
CredentialsUI::pushCreds(
    const PLCTool::Concentrator *dc,
    QDateTime timeStamp,
    PLCTool::NodeId meter,
    QString password)
{
  int rows = this->ui->tableWidget->rowCount();
  char nodeIdStr[16];

  snprintf(nodeIdStr, sizeof(nodeIdStr), "%06x", (unsigned int) meter);

  QTableWidgetItem *id = new QTableWidgetItem();
  QTableWidgetItem *time = new QTableWidgetItem(timeStamp.toString());
  QTableWidgetItem *SNA =
      new QTableWidgetItem(
        QString::fromStdString(
          PLCTool::PrimeAdapter::idToSna(dc->id())));
  QTableWidgetItem *meterId = new QTableWidgetItem(nodeIdStr);
  QTableWidgetItem *pwdItem = new QTableWidgetItem(password);

  this->saveCreds(dc, timeStamp, meter, password);
  this->ui->tableWidget->insertRow(rows);

  id->setData(Qt::DisplayRole, QVariant::fromValue<int>(rows + 1));

  this->ui->tableWidget->setItem(
        rows,
        0,
        id);
  this->ui->tableWidget->setItem(
        id->row(),
        1,
        time);
  this->ui->tableWidget->setItem(
        id->row(),
        2,
        SNA);
  this->ui->tableWidget->setItem(
        id->row(),
        3,
        meterId);
  this->ui->tableWidget->setItem(
        id->row(),
        4,
        pwdItem);

  this->ui->tableWidget->resizeColumnsToContents();
  this->ui->tableWidget->scrollToBottom();
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
