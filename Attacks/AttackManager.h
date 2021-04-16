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

#ifndef _ATTACKS_ATTACKMANAGER_H
#define _ATTACKS_ATTACKMANAGER_H

#include <Attacks/AttackController.h>
#include <Attacks/AttackFactoryLibrary.h>
#include <Attacks/AttackManagerWorker.h>
#include <Attacks/Attack.h>
#include <Topology/Adapter.h>

#include <QObject>
#include <QThread>

namespace PLCTool {
  class AttackManager : public QObject {
    Q_OBJECT

    AttackManagerWorker *worker = nullptr;
    QThread *thread = nullptr;

    void connectController(
        AttackController const *controller,
        Attack const *attack);
    void connectWorker(AttackManagerWorker const *worker);
    void connectThread(
        QThread const *thread,
        AttackManagerWorker const *worker);

   public:
    AttackManager(QObject *parent = nullptr);
    ~AttackManager(void);

  public slots:
    void onRequestedAttack(
        QString attackName,
        StringParams const &params,
        AttackController const *controller);

  signals:
    void sendAttack(Attack *attack);
    void endAllAttacks(void);
  };
}

#endif  // _ATTACKS_ATTACKMANAGER_H
