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

#include "AttackManager.h"

using namespace PLCTool;

AttackManager::AttackManager(QObject *parent) : QObject(parent)
{
  thread = new QThread();
  thread->setObjectName("AttackManagerWorker");

  worker = new AttackManagerWorker();
  worker->moveToThread(thread);
  connectThread(thread, worker);
  thread->start();
  connectWorker(worker);
}

AttackManager::~AttackManager(void)
{
  if (thread) thread->quit();
}

void
AttackManager::connectController(
    AttackController const *controller,
    Attack const *attack)
{
  connect(
      attack,
      &Attack::attackCreated,
      controller,
      &AttackController::onAttackCreated);
}

void
AttackManager::connectWorker(AttackManagerWorker const *worker)
{
  connect(
      this,
      &AttackManager::sendAttack,
      worker,
      &AttackManagerWorker::onAttackReceived);
  connect(
      this,
      &AttackManager::endAllAttacks,
      worker,
      &AttackManagerWorker::endAllAttacks);
}

void
AttackManager::connectThread(
    QThread const *thread,
    AttackManagerWorker const *worker)
{
  connect(
      thread,
      &QThread::finished,
      worker,
      &AttackManagerWorker::onThreadFinished);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);
}

//////////////////////////////////// Slots /////////////////////////////////////

void
AttackManager::onAttackRequested(
    QString attackName,
    StringParams const &params,
    AttackController const *controller)
{
  Attack *newAttack = nullptr;

  newAttack = AttackFactoryLibrary::instance().getAttack(
      attackName,
      params,
      (PrimeAdapter *) this->parent());

  /* Send Attack to worker */
  newAttack->moveToThread(thread);
  connectController(controller, newAttack);

  emit sendAttack(newAttack);
}

void
AttackManager::onAttackParametersRequested(QString attackName)
{
  bool ok;
  QList<QString> paramList;

  ok = AttackFactoryLibrary::instance().getAttackParamList(
      attackName,
      paramList);

  if (ok)
    emit attackParameters(paramList);
}
