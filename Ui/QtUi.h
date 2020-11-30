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

#ifndef UI_UI_H
#define UI_UI_H

#include <QObject>
#include "TopologyModel.h"
#include "Topology/Adapter.h"
#include "ModemDialog.h"
#include "MainWindow.h"
#include "LoadingStatusDialog.h"
#include "FrameLogUI.h"
#include "DLMSLogUI.h"
#include "MeterUI.h"
#include "TranslatorUI.h"
#include "CredentialsUI.h"
#include <QMap>

#include <QElapsedTimer>

struct Frame;
struct DlmsMessage;
class MeterInfo;

class QtUi : public QObject
{
  Q_OBJECT

  unsigned int parsedFrameCounter = 0;
  unsigned int totalFrameCounter = 0;

  // Ui-related objects
  MainWindow *mainWindow = nullptr;
  ModemDialog *modemDialog = nullptr;

  LoadingStatusDialog *loadingDialog = nullptr;

  FrameLogUI *frameLogUi = nullptr;
  DLMSLogUI *dlmsLogUi = nullptr;
  CredentialsUI *credentialsUi = nullptr;
  TranslatorUI *translatorUi = nullptr;
  QElapsedTimer refreshTimer;
  QMap<PLCTool::NodeId, MeterUI *> meterUiMap;

  // UI state
  bool firstConnection = true;

  void breathe(void);
  void connectAll(void);

public:
  explicit QtUi(QObject *parent = 0);
  ~QtUi();

  void show(void);
  void setCounters(unsigned int, unsigned int);
  void setLoading(bool);
  void loadingMessage(QString);

  // GUI Operations
  void setAdapter(PLCTool::Adapter *);
  void notifyTopologyChange(void);

  void openMeterInfoView(MeterInfo *);
  void closeAllMeterInfo(void);
  void openFrameLog(void);

  void pushFrame(Frame const &);
  void pushDlmsMessage(DlmsMessage const &);
  void pushCreds(CredInfo const &);

  void refreshViews(void);

  // Convenience getters
  QString modemPath(void) const;
  unsigned int modemBaud(void) const;

signals:
  void openAdapter(void);
  void openMeterInfo(PLCTool::Meter *);
  void openLogFile(QString);
  void closeAdapter(void);

public slots:
  void onLoadFile(void);
  void onToggleStart(void);
  void onToggleFrameLog(bool);
  void onToggleMessageLog(bool);
  void onToggleCredentialsLog(bool);
  void onToggleTranslator(bool);
  void onOpenConfig(void);
  void onSelectFrame(Frame &);
  void onSelectDlmsMessage(DlmsMessage &);
  void onRejectLoading(void);
  void onCloseSubWindow(QString);

};

#endif // UI_UI_H
