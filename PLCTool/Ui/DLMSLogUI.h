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

#ifndef DLMSLOGUI_H
#define DLMSLOGUI_H

#include <PLCTool/Topology/Concentrator.h>

#include <QDateTime>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <QThread>
#include <QVector>
#include <QWidget>

#include "DLMSTableModel.h"

namespace Ui
{
  class DLMSLogUI;
}

class XMLHighlighter;

class DLMSLogUI : public QWidget
{
  Q_OBJECT

  DLMSTableModel *model = nullptr;
  QSortFilterProxyModel *proxy = nullptr;

  QVector<DlmsMessage> messageList;
  QVector<DlmsMessage> pendingList;
  QString savedText;

  XMLHighlighter *highlighter;
  bool adjusting = false;
  bool sortingEnabled = false;
  void saveLog(QString path);
  void connectAll(void);

 public:
  static void registerTypes(void);

  explicit DLMSLogUI(QWidget *parent = 0);
  void refreshMessages(void);
  int findMessage(QDateTime const &);
  void selectNear(QDateTime const &, PLCTool::NodeId);
  void setSortingEnabled(bool);
  void clear(void);
  void realize(void);

  ~DLMSLogUI();

 signals:
  void messageSelected(DlmsMessage);

 public slots:
  void saveMessage(const DlmsMessage &msg);

  void onCellActivated(QModelIndex const &);
  void onCurrentChanged(QModelIndex, QModelIndex);
  void onSaveAs(bool);
  void onClear(bool);

  void onTop(void);
  void onBottom(void);
  void onGotoLine(void);

 private:
  Ui::DLMSLogUI *ui;
};

#endif  // DLMSLOGUI_H
