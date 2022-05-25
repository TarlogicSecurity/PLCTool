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

#ifndef _ATTACKS_PRIMEATTACK_H
#define _ATTACKS_PRIMEATTACK_H

#include <PLCTool/PRIME/PrimeAdapter.h>
#include <PLCTool/Topology/Adapter.h>

#include <QObject>

namespace PLCTool
{
  class Attack : public QObject
  {
    Q_OBJECT

   protected:
    QString const attackName;
    PrimeAdapter *adapter;

   public:
    Attack(
        QString const attackName,
        PrimeAdapter *adapter,
        QObject *parent = nullptr);
    virtual ~Attack(){};

   public slots:
    virtual void onStart(void);
    virtual void onCancel(void);
    virtual void onEnd(void);

   signals:
    void attackCreated(PLCTool::Attack *attack);
    void attackStarted(void);
    void attackProgress(float progress);
    void attackStatus(QString status);
    void attackCompleted(void);
    void attackFailed(QString message);
    void attackCancelled(void);
    void attackTimeout(void);
    void attackEnded(void);
  };
}  // namespace PLCTool

#endif  // _ATTACKS_PRIMEATTACK_H
