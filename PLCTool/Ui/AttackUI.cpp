#include "AttackUI.h"

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

#include "ui_AttackUI.h"

AttackUI::AttackUI(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::AttackUI)
{
  ui->setupUi(this);

  ui->parametersLayout->setMargin(4);
  ui->parametersLayout->setSpacing(4);

  setInfoStatus("Awaiting input");
  setIdleState();

  connectAll();
}

AttackUI::~AttackUI()
{
  delete ui;
}

void
AttackUI::addProperty(QString name)
{
  QVariant value("");

  setProperty(name, value);
}

void
AttackUI::setProperty(QString name, QVariant value)
{
  QLineEdit *field = nullptr;

  if (!(field = (QLineEdit *) getField(name))) {
    field = new QLineEdit(this);
    field->setFixedHeight(24);

    this->addField(name, field);
  }

  field->setText(value.toString());
}

QVariant
AttackUI::getProperty(QString name) const
{
  QVariant value;
  QLineEdit *field = nullptr;

  if ((field = (QLineEdit *) getField(name)))
    value.setValue(field->text());

  return value;
}

void
AttackUI::addField(QString name, QWidget *field)
{
  this->ui->parametersLayout->addRow(name, field);
}

QWidget *
AttackUI::getField(QString name) const
{
  QWidget *field = nullptr;
  int i;
  int rowCount = this->ui->parametersLayout->rowCount();

  i = 0;
  while (i < rowCount && !field) {
    if (name.compare(getLabel(i)) == 0)
      field = getField(i);
    ++i;
  }

  return field;
}

QString
AttackUI::getLabel(int row) const
{
  QWidget *labelWidget =
      this->ui->parametersLayout->itemAt(row, QFormLayout::LabelRole)->widget();

  return ((QLabel *) labelWidget)->text();
}

QWidget *
AttackUI::getField(int row) const
{
  return this->ui->parametersLayout->itemAt(row, QFormLayout::FieldRole)
      ->widget();
}

void
AttackUI::connectAll(void)
{
  connect(this->ui->startButton, SIGNAL(pressed()), this, SIGNAL(start()));
  connect(this->ui->cancelButton, SIGNAL(pressed()), this, SIGNAL(stop()));
}

bool
AttackUI::setIdleState(void)
{
  ui->parametersLayout->setEnabled(true);
  ui->controlLayout->setEnabled(true);
  ui->startButton->setEnabled(true);
  ui->cancelButton->setEnabled(false);
  ui->progressBar->setEnabled(false);

  this->currentState = IDLE;

  return true;
}

bool
AttackUI::setWaitingState(void)
{
  ui->parametersLayout->setEnabled(false);
  ui->controlLayout->setEnabled(false);
  this->currentState = WAITING;

  return true;
}

bool
AttackUI::setRunningState(void)
{
  bool ok = this->currentState == WAITING;

  if (ok) {
    ui->parametersLayout->setEnabled(false);
    ui->controlLayout->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->cancelButton->setEnabled(true);
    ui->progressBar->setEnabled(true);

    this->currentState = RUNNING;
  }

  return ok;
}
void
AttackUI::setInfoStatus(const QString &message)
{
  QPalette messagePalette;
  messagePalette.setColor(QPalette::WindowText, Qt::black);
  ui->messageLabel->setPalette(messagePalette);
  ui->messageLabel->setText(message);
}

void
AttackUI::setErrorStatus(const QString &message)
{
  QPalette messagePalette;
  messagePalette.setColor(QPalette::WindowText, Qt::red);
  ui->messageLabel->setPalette(messagePalette);
  ui->messageLabel->setText(message);
}

void
AttackUI::setProgress(float progress)
{
  ui->progressBar->setValue(progress * this->ui->progressBar->maximum());
}

int
AttackUI::getProgress(void) const
{
  return ui->progressBar->value();
}
