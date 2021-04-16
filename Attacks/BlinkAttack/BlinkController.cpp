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
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "BlinkController.h"

using namespace PLCTool;

BlinkController::BlinkController(
    QObject *parent,
    QString name,
    QString windowTitle) :
  UIAttackController(parent, name, windowTitle)
{
  this->ui = new BlinkUI;
  connectUI();
  this->currentState = NOT_CONNECTED;
}

QWidget *
BlinkController::getUI(void)
{
  return this->ui;
}

void
BlinkController::connectUI(void)
{
  connect(
        this->ui,
        SIGNAL(start(void)),
        this,
        SLOT(onStartButton(void)));
  connect(
        this->ui,
        SIGNAL(stop(void)),
        this,
        SLOT(onStopButton(void)));
}

void
BlinkController::connectAttack(Attack *attack)
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
  connect(
      attack,
      SIGNAL(attackCompleted(StringParams const &)),
      this,
      SLOT(onAttackCompleted(StringParams const &)));
  connect(
      attack,
      SIGNAL(attackFailed(QString)),
      this,
      SLOT(onAttackFailed(QString)));
  connect(attack, SIGNAL(attackCancelled()), this, SLOT(onAttackCancelled()));
}

void
BlinkController::disconnectAttack(void)
{
  disconnect(this, SIGNAL(startAttack()), nullptr, nullptr);
  disconnect(this, SIGNAL(cancelAttack()), nullptr, nullptr);
  disconnect(this, SIGNAL(endAttack()), nullptr, nullptr);
}

bool
BlinkController::getParams(void)
{
  bool ok = this->ui->checkInput();

  if (ok) {
    std::vector<uint8_t> sna = this->ui->getSNA();
    this->params["sna"] = std::string(sna.begin(), sna.end());
    this->params["nid"] = QString::number(this->ui->getNID()).toStdString();
    this->params["lcid"] = QString::number(this->ui->getLCID()).toStdString();
    this->params["level"] = QString::number(this->ui->getLevel()).toStdString();
    this->params["authenticationLevel"] =
        QString::number(this->ui->getAuthenticationLevel()).toStdString();
    this->params["password"] = this->ui->getPassword();
    this->params["msTimeout"] =
        QString::number(this->ui->getMsTimeout()).toStdString();
  }

  return ok;
}

void
BlinkController::liberateAttack(void)
{
  emit endAttack();
  disconnectAttack();
  this->currentState = NOT_CONNECTED;
}

//////////////////////////////////// Slots /////////////////////////////////////

void
BlinkController::onAttackCreated(Attack *attack)
{
  if (this->currentState == AWAITING_CREATION) {
    connectAttack(attack);
    if (this->ui) {
      this->ui->setRunningState();
      this->ui->setInfoStatus("Sending AARQ probes...");
      this->currentState = RUNNING;
      emit startAttack();
    } else {
      this->currentState = AWAITING_CANCEL;
      emit cancelAttack();
    }
  }
}

void
BlinkController::onClosedWindow(void)
{
  this->ui = nullptr;

  if (this->currentState == NOT_CONNECTED) {
    emit closeAttackController(this->name);
  } else if (this->currentState == RUNNING) {
    this->currentState = AWAITING_CANCEL;
    emit cancelAttack();
  }
}

void
BlinkController::onStartButton(void)
{
  if (this->currentState == NOT_CONNECTED) {
    if (getParams()) {
      this->ui->setWaitingState();
      this->ui->setInfoStatus("Requesting attack...");
      this->currentState = AWAITING_CREATION;
      emit requestAttack(this->attackName, this->params, this);
    }
  }
}

void
BlinkController::onStopButton(void)
{
  if (this->currentState == RUNNING) {
    this->ui->setWaitingState();
    this->ui->setInfoStatus("Cancel attack...");
    this->currentState = AWAITING_CANCEL;
    emit cancelAttack();
  }
}

void
BlinkController::onAttackCompleted(StringParams const &params)
{
  QString status_message;

  if (this->currentState == RUNNING) {
    liberateAttack();
    this->ui->setInfoStatus(status_message);
    this->ui->setIdleState();
  }
}

void
BlinkController::onAttackCancelled(void)
{
  if (this->currentState == AWAITING_CANCEL) {
    liberateAttack();
    if (this->ui) {
      this->ui->setInfoStatus("Attack cancelled");
      this->ui->setIdleState();
    } else {
      emit closeAttackController(this->name);
    }
  }
}

void
BlinkController::onAttackTimeout(void)
{
  if (this->currentState == RUNNING) {
    liberateAttack();
    this->ui->setErrorStatus("Timeout reached...");
    this->ui->setIdleState();
  }
}

void
BlinkController::onAttackFailed(QString message)
{
  if (this->currentState == RUNNING) {
    liberateAttack();
    this->ui->setErrorStatus("Attack failed: " + message);
    this->ui->setIdleState();
  }
}

void
BlinkController::onAttackProgress(float progressRatio)
{
  this->ui->setProgress(progressRatio);
}

void
BlinkController::onAttackStatus(QString message)
{
  this->ui->setInfoStatus(message);
}
