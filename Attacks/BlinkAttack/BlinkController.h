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

#ifndef _PRIME_BLINKCONTROLLER_H
#define _PRIME_BLINKCONTROLLER_H

#include <Attacks/Attack.h>
#include <Attacks/BlinkAttack/BlinkUI.h>
#include <Attacks/UIAttackController.h>
#include <Topology/Adapter.h>
#include <Topology/StringParams.h>

#include <QDialog>
#include <QObject>

namespace PLCTool {
  class BlinkController : public UIAttackController
  {
    Q_OBJECT
    enum State { NOT_CONNECTED, AWAITING_CREATION, RUNNING, AWAITING_CANCEL };

    BlinkUI *ui;
    QString const attackName = "ASSOCIATION";
    StringParams params;
    State currentState;

    void connectUI(void);
    void connectAttack(Attack *attack);
    void disconnectAttack(void);
    bool getParams(void);
    void liberateAttack(void);

   public:
    BlinkController(QObject *parent, QString name, QString windowTitle);

    QWidget *getUI(void) override;

  public slots:
    void onAttackCreated(Attack *attack) override;
    void onClosedWindow(void) override;

    void onStartButton(void);
    void onStopButton(void);
    void onAttackCompleted(StringParams const &params);
    void onAttackCancelled(void);
    void onAttackTimeout(void);
    void onAttackFailed(QString message);
    void onAttackProgress(float progressRatio);
    void onAttackStatus(QString message);

   signals:
    void startAttack(void);
    void cancelAttack(void);
  };
}

#endif  // _PRIME_BLINKCONTROLLER_H
