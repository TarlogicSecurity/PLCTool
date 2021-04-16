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

#ifndef BLINKUI_H
#define BLINKUI_H

#include <QWidget>
#include <QTimer>
#include <Topology/Node.h>
#include <enums.h>

namespace Ui {
  class BlinkUI;
}

using namespace PLCTool;

class BlinkUI : public QWidget
{
  Q_OBJECT
  enum State {IDLE, WAITING, RUNNING};

  Ui::BlinkUI *ui;
  enum State currentState;

  void connectAll(void);

  bool checkSNA(void);
  bool checkNID(void);
  bool checkLCID(void);
  bool checkLevel(void);
  bool checkPassword(void);
  bool checkTimeout(void);

  bool ulongFromString(ulong &number, QString numberString) const;
  std::vector<uint8_t> fromHexStringToByteArray(
      const std::string hexString,
      size_t byteArrayLength = 0) const;
  bool fromHexCharToByte(uint8_t &b, char c) const;
  DLMS_AUTHENTICATION localAuthToDLMSAuth(int authenticationLevel) const;

public:
 explicit BlinkUI(QWidget *parent = nullptr);
 ~BlinkUI();

 bool checkInput(void);

 bool setIdleState(void);
 bool setWaitingState(void);
 bool setRunningState(void);
 void setInfoStatus(const QString &message);
 void setErrorStatus(const QString &message);
 void setProgress(float progress);
 void updatePasswordField(void);

 std::vector<uint8_t> getSNA(void) const;
 NodeId getNID(void) const;
 ConnId getLCID(void) const;
 uint8_t getLevel(void) const;
 std::string getPassword(void) const;
 DLMS_AUTHENTICATION getAuthenticationLevel(void) const;
 int getMsTimeout(void) const;
 int getProgress(void) const;

public slots:
  void onChangedAuthenticationLevel(void);

signals:
  void start(void);
  void stop(void);
};

#endif // BLINKUI_H
