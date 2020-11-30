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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <Topology/Meter.h>
#include "TopologyModel.h"
#include <QHash>

namespace Ui {
class MainWindow;
}

class QSaneMdiSubWindow : public QMdiSubWindow {
  Q_OBJECT

  QString name;

public:
  QSaneMdiSubWindow(QString const &name);

signals:
  void closed(QString);

protected:
  void closeEvent(QCloseEvent *closeEvent);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    TopologyModel *model = nullptr;
    QHash<QString, QSaneMdiSubWindow *> windowMap;

    void connectAll(void);

public:
    enum Button {
      MAIN_WINDOW_BUTTON_FRAME_LOG,
      MAIN_WINDOW_BUTTON_MESSAGE_LOG,
      MAIN_WINDOW_BUTTON_CREDENTIALS_LOG,
      MAIN_WINDOW_BUTTON_TRANSLATOR
    };

    explicit MainWindow(QWidget *parent = 0);

    void setSubNet(const PLCTool::SubNet *sn);
    bool connectState(void) const;
    void setConnectState(bool);

    void notifySubNetChanges(void);
    void setButtonState(Button, bool);
    QSaneMdiSubWindow *openWindow(
        QString const &name,
        QString const &title,
        QWidget *widget);
    bool closeWindow(QString const &name);
    QSaneMdiSubWindow *findWindow(QString const &name) const;

    ~MainWindow();

public slots:
    void onCloseSubWindow(QString subWindow);
    void onNodeActivated(QModelIndex);

private:
    Ui::MainWindow *ui;

signals:
    void loadFile(void);
    void toggleStart(void);
    void openModemDialog(void);
    void toggleFrameLog(bool);
    void toggleMessageLog(bool);
    void toogleCredentialsLog(bool);
    void toggleTranslator(bool);
    void closeSubWindow(QString);
    void openMeterInfo(PLCTool::Meter *);
};

#endif // MAINWINDOW_H
