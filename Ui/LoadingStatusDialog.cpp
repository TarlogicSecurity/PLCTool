#include "Ui/LoadingStatusDialog.h"
#include "ui_LoadingStatusDialog.h"

LoadingStatusDialog::LoadingStatusDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LoadingStatusDialog)
{
  ui->setupUi(this);
  this->ui->progressBar->setTextVisible(true);
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

void
LoadingStatusDialog::setLimits(unsigned int curr, unsigned int max)
{
  this->ui->progressBar->setMaximum(static_cast<int>(max));
  this->ui->progressBar->setValue(static_cast<int>(curr));
}
