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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QMap>
#include <QObject>

#include "DLMSProcessor.h"
#include "MeterInfo.h"
#include "PLCTool/Attacks/AttackManager.h"
#include "PLCTool/Attacks/AttackPluginLoader.h"
#include "PLCTool/PRIME/PrimeAdapter.h"
#include "PLCTool/Topology/Concentrator.h"
#include "PLCTool/Topology/Meter.h"
#include "PLCTool/Ui/QtUi.h"
#include "PRIMEProcessor.h"
#include "QVector"

#define WINDOW_ICON_PATH ":/Icons/Tarlogic/Tarlogic-5.png"
#define WINDOW_TITLE "Tarlogic PLCTool - A PRIME and DLMS graphical swiss-knife"
#define DEFAULT_ATTACK_PLUGIN_SEARCH_PATH_USER \
  {                                            \
    ".plctool", ".local/lib/plctool"           \
  }
#define DEFAULT_ATTACK_PLUGIN_SEARCH_PATH                        \
  {                                                              \
    "/lib/plctool", "/usr/lib/plctool", "/usr/local/lib/plctool" \
  }

class Application : public QApplication
{
  Q_OBJECT

  // Data model objects
  PLCTool::PrimeAdapter *adapter = nullptr;
  PLCTool::AttackManager *attackManager = nullptr;
  PLCTool::AttackPluginLoader *attackPluginLoader = nullptr;
  QVector<PLCTool::SubNet *> subNetHistory;
  QMap<PLCTool::NodeId, MeterInfo *> meterInfo;

  QThread *primeProcThread = nullptr;
  QThread *dlmsProcThread = nullptr;

  PRIMEProcessor *primeProcessor = nullptr;
  DLMSProcessor *dlmsProcessor = nullptr;

  // UI objects
  QtUi *ui = nullptr;

  void parseDataFrame(
      PLCTool::Meter *meter,
      QDateTime,
      bool downlink,
      const void *data,
      size_t size);

  void connectAdapter(void);
  void connectUi(void);
  void connectProcessors(void);

  void createAttackManager(PLCTool::PrimeAdapter *parent);

  void clearMeterInfo(void);
  MeterInfo *assertMeterInfo(PLCTool::Meter *);

 public:
  explicit Application(int &argc, char *argv[]);
  ~Application();

  bool openAdapter(QString const &path, unsigned int baud = 921600);
  bool loadLogFile(QString const &path);
  bool closeAdapter(void);

  bool work(void);

 signals:
  void frameReceived(
      quint64 dcId,
      QDateTime timeStamp,
      bool downlink,
      QVector<uint8_t>);

  void messageReceived(
      QString SNA,
      QDateTime timeStamp,
      quint64 id,
      bool downlink,
      QVector<uint8_t>);

 public slots:
  void onSubnetAnnounce(PLCTool::Concentrator *, QDateTime, uint64_t times);
  void onMeterFound(PLCTool::Concentrator *, QDateTime, PLCTool::Meter *);
  void onFrameReceived(
      PLCTool::Concentrator *,
      QDateTime,
      bool downlink,
      const void *data,
      size_t size);
  void onDataReceived(
      PLCTool::Meter *meter,
      QDateTime,
      bool downlink,
      const void *data,
      size_t size);

  void onAdapterStatusMessage(QString);
  void onAdapterClosed(void);
  void onAdapterRefreshRequested(void);
  void onOpenAdapter(void);
  void onOpenLogFile(QString);
  void onCloseAdapter(void);
  void onOpenMeterInfo(PLCTool::Meter *meter);
  void onProcessedFrame(Frame);
  void onProcessedDlmsMessage(DlmsMessage);
  void onNewUIAttackController(PLCTool::UIAttackController *controller);
};

#endif  // APPLICATION_H
