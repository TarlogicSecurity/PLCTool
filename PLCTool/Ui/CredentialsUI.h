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

#ifndef CREDENTIALSUI_H
#define CREDENTIALSUI_H

#include <PLCTool/App/MeterInfo.h>
#include <PLCTool/Topology/Concentrator.h>

#include <QDateTime>
#include <QWidget>

class QSortFilterProxyModel;
class CredInfoTableModel;

namespace Ui
{
  class CredentialsUI;
}

class CredentialsUI : public QWidget
{
  Q_OBJECT

  QVector<CredInfo> credList;
  QVector<CredInfo> pendingList;

  QSortFilterProxyModel *proxy = nullptr;
  CredInfoTableModel *model = nullptr;

  bool sortingEnabled = false;
  bool adjusting = false;

  void saveLog(QString path);
  void connectAll(void);

 public:
  explicit CredentialsUI(QWidget *parent = 0);
  ~CredentialsUI();

  void saveCreds(CredInfo const &);
  void refreshCredentials(void);
  void setSortingEnabled(bool);
  void clear(void);
  void realize(void);

 public slots:
  void onSaveAs(bool);
  void onClear(bool);

 private:
  Ui::CredentialsUI *ui;
};

#endif  // CREDENTIALSUI_H
