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

#ifndef ATTACKUI_H
#define ATTACKUI_H

#include <QMap>
#include <QWidget>

namespace Ui {
  class AttackUI;
}

class AttackUI : public QWidget
{
  Q_OBJECT
  enum State {
    IDLE,
    WAITING,
    RUNNING
  };

  Ui::AttackUI *ui;
  enum State currentState;

  void connectAll(void);

  void addField(QString name, QWidget *field);
  QWidget *getField(QString name) const;
  QWidget *getField(int row) const;
  QString getLabel(int row) const;

 public:
  explicit AttackUI(QWidget *parent = nullptr);
  ~AttackUI();

  void addProperty(QString name);
  void setProperty(QString name, QVariant value);
  QVariant getProperty(QString name) const;

  bool setIdleState(void);
  bool setWaitingState(void);
  bool setRunningState(void);

  void setInfoStatus(const QString &message);
  void setErrorStatus(const QString &message);

  void setProgress(float progress);
  int getProgress(void) const;

 signals:
  void start(void);
  void stop(void);
};

#endif // ATTACKUI_H
