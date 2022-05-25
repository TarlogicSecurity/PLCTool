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

#ifndef GENERICUIATTACKCONTROLLER_H
#define GENERICUIATTACKCONTROLLER_H

#include <PLCTool/Attacks/UIAttackController.h>
#include <PLCTool/Ui/AttackUI.h>

#include <QObject>
#include <QWidget>

namespace PLCTool
{
  class GenericUIAttackController : public UIAttackController
  {
    Q_OBJECT
    enum State {
      AWAITING_PARAMETERS,
      NOT_CONNECTED,
      AWAITING_CREATION,
      RUNNING,
      AWAITING_CANCEL
    };

    AttackUI *ui;
    QString attackName;
    StringParams params;
    QList<QString> parameterList;

    State currentState;

    void connectUI(void);
    void disconnectUI(void);
    void connectAttack(Attack *attack);
    void disconnectAttack(void);
    bool getParams(void);
    void liberateAttack(void);
    void generateUI(QList<QString> parameterList);

    void addUiProperty(QString name);
    void setUiProperty(QString name, QVariant value);
    QVariant getUiProperty(QString name) const;

    bool setUiIdleState(void);
    bool setUiWaitingState(void);
    bool setUiRunningState(void);

    void setUiInfoStatus(const QString &message);
    void setUiErrorStatus(const QString &message);

    void setUiProgress(float progress);
    int getUiProgress(void) const;

   public:
    explicit GenericUIAttackController(
        QString attackName,
        QString windowName,
        QString windowTitle,
        QObject *parent = nullptr);

    QWidget *getUI(void) override;
    void sendRequestAttackParameters(void) override;

   public slots:
    void onAttackCreated(PLCTool::Attack *attack) override;
    void onAttackParametersReceived(QList<QString> paramList) override;
    void onClosedWindow(void) override;

    void onStartButton(void);
    void onStopButton(void);
    void onAttackCompleted(void);
    void onAttackCancelled(void);
    void onAttackTimeout(void);
    void onAttackFailed(QString message);
    void onAttackProgress(float progressRatio);
    void onAttackStatus(QString message);
    void onAttackStarted(void);
    void onAttackEnded(void);

   signals:
    void startAttack(void);
    void cancelAttack(void);
  };
}  // namespace PLCTool

#endif  // GENERICUIATTACKCONTROLLER_H
