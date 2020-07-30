#include "ModemDialog.h"
#include "ui_ModemDialog.h"
#include <QFileDialog>

ModemDialog::ModemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModemDialog)
{
  ui->setupUi(this);

  this->connectAll();
}

QString
ModemDialog::path(void) const
{
  return this->ui->pathEdit->text();
}

int
ModemDialog::baud(void) const
{
  return this->ui->baudSpin->value();
}

void
ModemDialog::connectAll(void)
{
  connect(
        this->ui->browseButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onBrowse()));
}

ModemDialog::~ModemDialog()
{
    delete ui;
}

//////////////////////////////// Slots ////////////////////////////////////////
void
ModemDialog::onBrowse()
{
  QFileDialog dialog(this);

  dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
  dialog.setNameFilter("Serial devices (tty*)");
  dialog.setViewMode(QFileDialog::Detail);
  dialog.setDirectory("/dev");
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  if (dialog.exec()) {
    QStringList list = dialog.selectedFiles();

    if (list.size() > 0)
      this->ui->pathEdit->setText(list.at(0));
  }
}
