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

#ifndef METERCONTEXT_H
#define METERCONTEXT_H

#include <PLCTool/Topology/Meter.h>

#include <QObject>
#include <QVector>

#include "DLMSProcessor.h"
#include "PRIMEProcessor.h"

class MeterInfo;

struct CredInfo {
  MeterInfo *owner;
  QDateTime timeStamp;
  QString password;
  QStringList contexts;

  CredInfo(CredInfo const &);
  CredInfo();

  static void registerTypes(void);
};

class MeterInfo : public QObject
{
  Q_OBJECT

  PLCTool::Meter *mMeter;

  QDateTime lastActivity;

  QString mMac = "N/A";
  QVector<Frame> frames;
  QVector<DlmsMessage> messages;
  QVector<CredInfo> creds;

  QVector<Frame> pendingFrames;
  QVector<DlmsMessage> pendingMessages;
  QVector<CredInfo> pendingCreds;

 public:
  explicit MeterInfo(QObject *parent, PLCTool::Meter *meter);

  void pushFrame(Frame const &);
  void pushDlmsMessage(DlmsMessage const &);
  CredInfo pushCreds(
      QDateTime const &timeStamp,
      QString const &password,
      QStringList const &ctx);

  inline QVector<CredInfo> *
  pendingCredList(void)
  {
    return &this->pendingCreds;
  }

  inline QVector<CredInfo> *
  credList(void)
  {
    return &this->creds;
  }

  inline void
  commitCreds(void)
  {
    this->pendingCreds.clear();
  }

  inline QVector<Frame> *
  pendingFrameList(void)
  {
    return &this->pendingFrames;
  }

  inline QVector<Frame> *
  frameList(void)
  {
    return &this->frames;
  }

  inline void
  commitFrames(void)
  {
    this->pendingFrames.clear();
  }

  inline QVector<DlmsMessage> *
  pendingMessageList(void)
  {
    return &this->pendingMessages;
  }

  inline QVector<DlmsMessage> *
  messageList(void)
  {
    return &this->messages;
  }

  inline void
  commitMessages(void)
  {
    this->pendingMessages.clear();
  }

  inline PLCTool::Meter *
  meter(void) const
  {
    return this->mMeter;
  }

  QString
  mac(void) const
  {
    return this->mMac;
  }

 signals:
  void messageReceived(DlmsMessage);
  void frameReceived(Frame);
  void credentialsFound(CredInfo);

 public slots:
};

#endif  // METERCONTEXT_H
