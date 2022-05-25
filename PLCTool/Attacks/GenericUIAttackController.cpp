//
// Copyright (c) 2020, Tarlogic Security SL
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of copyright holders nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE-
// POSSIBILITY OF SUCH DAMAGE.
//

#include "GenericUIAttackController.h"

#include <PLCTool/Topology/StringParams.h>

#include <QObject>
#include <QVariant>

using namespace PLCTool;

GenericUIAttackController::GenericUIAttackController(
    QString attackName,
    QString windowName,
    QString windowTitle,
    QObject *parent)
    : UIAttackController{windowName, windowTitle, parent}
{
  this->attackName = attackName;
  this->ui = new AttackUI;
  connectUI();

  this->currentState = AWAITING_PARAMETERS;
}

QWidget *
GenericUIAttackController::getUI()
{
  return this->ui;
}

void
GenericUIAttackController::connectUI()
{
  connect(this->ui, SIGNAL(start()), this, SLOT(onStartButton()));
  connect(this->ui, SIGNAL(stop()), this, SLOT(onStopButton()));
}

void
GenericUIAttackController::disconnectUI()
{
  disconnect(this, SLOT(onStartButton()));
  disconnect(this, SLOT(onStopButton()));
}

void
GenericUIAttackController::connectAttack(Attack *attack)
{
  connect(this, SIGNAL(startAttack()), attack, SLOT(onStart()));
  connect(this, SIGNAL(cancelAttack()), attack, SLOT(onCancel()));
  connect(this, SIGNAL(endAttack()), attack, SLOT(onEnd()));

  connect(attack, SIGNAL(attackTimeout()), this, SLOT(onAttackTimeout()));
  connect(
      attack,
      SIGNAL(attackProgress(float)),
      this,
      SLOT(onAttackProgress(float)));
  connect(
      attack,
      SIGNAL(attackStatus(QString)),
      this,
      SLOT(onAttackStatus(QString)));
  connect(attack, SIGNAL(attackCompleted()), this, SLOT(onAttackCompleted()));
  connect(
      attack,
      SIGNAL(attackFailed(QString)),
      this,
      SLOT(onAttackFailed(QString)));
  connect(attack, SIGNAL(attackCancelled()), this, SLOT(onAttackCancelled()));
  connect(attack, SIGNAL(attackStarted()), this, SLOT(onAttackStarted()));
  connect(attack, SIGNAL(attackEnded()), this, SLOT(onAttackEnded()));
}

void
GenericUIAttackController::disconnectAttack(void)
{
  disconnect(this, SIGNAL(startAttack()), nullptr, nullptr);
  disconnect(this, SIGNAL(cancelAttack()), nullptr, nullptr);
  disconnect(this, SIGNAL(endAttack()), nullptr, nullptr);

  disconnect(this, SLOT(onAttackTimeout()));
  disconnect(this, SLOT(onAttackProgress(float)));
  disconnect(this, SLOT(onAttackStatus(QString)));
  disconnect(this, SLOT(onAttackCompleted()));
  disconnect(this, SLOT(onAttackFailed(QString)));
  disconnect(this, SLOT(onAttackCancelled()));
  disconnect(this, SLOT(onAttackStarted()));
  disconnect(this, SLOT(onAttackEnded()));
}

void
GenericUIAttackController::generateUI(QList<QString> parameterList)
{
  for (QString paramName : parameterList) this->addUiProperty(paramName);
}

void
GenericUIAttackController::addUiProperty(QString name)
{
  if (this->ui)
    this->ui->addProperty(name);
}

void
GenericUIAttackController::setUiProperty(QString name, QVariant value)
{
  if (this->ui)
    this->ui->setProperty(name, value);
}

QVariant
GenericUIAttackController::getUiProperty(QString name) const
{
  QVariant value;

  if (this->ui)
    value = this->ui->getProperty(name);

  return value;
}

bool
GenericUIAttackController::setUiIdleState()
{
  bool ok = false;

  if (this->ui)
    ok = this->ui->setIdleState();

  return ok;
}

bool
GenericUIAttackController::setUiWaitingState()
{
  bool ok = false;

  if (this->ui)
    ok = this->ui->setWaitingState();

  return ok;
}

bool
GenericUIAttackController::setUiRunningState()
{
  bool ok = false;

  if (this->ui)
    ok = this->ui->setRunningState();

  return ok;
}

void
GenericUIAttackController::setUiInfoStatus(const QString &message)
{
  if (this->ui)
    this->ui->setInfoStatus(message);
}

void
GenericUIAttackController::setUiErrorStatus(const QString &message)
{
  if (this->ui)
    this->ui->setErrorStatus(message);
}

void
GenericUIAttackController::setUiProgress(float progress)
{
  if (this->ui)
    this->ui->setProgress(progress);
}

int
GenericUIAttackController::getUiProgress() const
{
  int progress = -1;

  if (this->ui)
    progress = this->ui->getProgress();

  return progress;
}

bool
GenericUIAttackController::getParams(void)
{
  bool ok = true;
  int i = 0;
  QString parameter;
  QString property;
  StringParams params;

  do {
    parameter = this->parameterList[i];
    property = this->getUiProperty(parameter).toString();

    ok = property != "";

    if (ok)
      params[parameter.toStdString()] = property.toStdString();

    ++i;
  } while (ok && i < this->parameterList.count());

  if (ok)
    this->params = params;

  return ok;
}

void
GenericUIAttackController::liberateAttack(void)
{
  emit endAttack();
  disconnectAttack();
  this->currentState = NOT_CONNECTED;
}

void
GenericUIAttackController::sendRequestAttackParameters(void)
{
  emit requestAttackParameters(attackName);
}

//////////////////////////////////// Slots /////////////////////////////////////

void
GenericUIAttackController::onAttackCreated(Attack *attack)
{
  if (this->currentState == AWAITING_CREATION) {
    connectAttack(attack);

    if (this->ui) {
      this->setUiRunningState();
      this->setUiInfoStatus("Starting Attack...");
      this->currentState = RUNNING;
      emit startAttack();
    } else {
      this->currentState = AWAITING_CANCEL;
      emit cancelAttack();
    }
  }
}

void
GenericUIAttackController::onClosedWindow(void)
{
  this->disconnectUI();
  this->ui = nullptr;

  if (this->currentState == NOT_CONNECTED) {
    emit closeAttackController(this->name);
  } else if (this->currentState == RUNNING) {
    this->currentState = AWAITING_CANCEL;
    emit cancelAttack();
  }
}

void
GenericUIAttackController::onStartButton(void)
{
  if (this->currentState == NOT_CONNECTED) {
    if (getParams()) {
      this->setUiWaitingState();
      this->setUiInfoStatus("Requesting attack...");
      this->currentState = AWAITING_CREATION;
      emit requestAttack(this->attackName, this->params, this);
    }
  }
}

void
GenericUIAttackController::onStopButton(void)
{
  if (this->currentState == RUNNING) {
    this->setUiWaitingState();
    this->setUiInfoStatus("Cancel attack...");
    this->currentState = AWAITING_CANCEL;
    emit cancelAttack();
  }
}

void
GenericUIAttackController::onAttackParametersReceived(
    QList<QString> parameterList)
{
  if (this->currentState == AWAITING_PARAMETERS) {
    this->parameterList = parameterList;
    this->generateUI(parameterList);
    this->currentState = NOT_CONNECTED;
  }
}

void
GenericUIAttackController::onAttackCompleted(void)
{
  QString status_message;

  if (this->currentState == RUNNING) {
    liberateAttack();
    this->setUiInfoStatus(status_message);
    this->setUiIdleState();
  }
}

void
GenericUIAttackController::onAttackCancelled(void)
{
  if (this->currentState == AWAITING_CANCEL) {
    liberateAttack();
    if (this->ui) {
      this->setUiInfoStatus("Attack cancelled");
      this->setUiIdleState();
    } else {
      emit closeAttackController(this->name);
    }
  }
}

void
GenericUIAttackController::onAttackTimeout(void)
{
  if (this->currentState == RUNNING) {
    liberateAttack();
    this->setUiErrorStatus("Timeout reached...");
    this->setUiIdleState();
  }
}

void
GenericUIAttackController::onAttackFailed(QString message)
{
  if (this->currentState == RUNNING) {
    liberateAttack();
    this->setUiErrorStatus("Attack failed: " + message);
    this->setUiIdleState();
  }
}

void
GenericUIAttackController::onAttackProgress(float progressRatio)
{
  this->setUiProgress(progressRatio);
}

void
GenericUIAttackController::onAttackStatus(QString message)
{
  this->setUiInfoStatus(message);
}

void
GenericUIAttackController::onAttackStarted()
{
  this->setUiInfoStatus("Attack started");
}

void
GenericUIAttackController::onAttackEnded()
{
}
