#include "Ui/LoadingStatusDialog.h"
#include "ui_LoadingStatusDialog.h"

LoadingStatusDialog::LoadingStatusDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LoadingStatusDialog)
{
  ui->setupUi(this);
}

LoadingStatusDialog::~LoadingStatusDialog()
{
  delete ui;
}

void
LoadingStatusDialog::setStatus(QString text)
{
  this->ui->statusLabel->setText(text);
}
