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

#include "MainWindow.h"
#include "ui_MainWindow.h"

QSaneMdiSubWindow::QSaneMdiSubWindow(QString const &name)
{
  this->name = name;
}

QString
QSaneMdiSubWindow::getName(void) const
{
  return this->name;
}


void
QSaneMdiSubWindow::closeEvent(QCloseEvent *)
{
  emit closed(this->name);
}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  this->model = new TopologyModel(nullptr, this);
  this->ui->topologyView->setModel(this->model);
  this->ui->topologyView->setSelectionBehavior(QAbstractItemView::SelectRows);
  this->connectAll();
}

MainWindow::MainWindow(QString const &iconPath, QWidget *parent)
    : MainWindow(parent)
{
  this->icon = new QIcon(iconPath);
}

void
MainWindow::connectAll(void)
{
  connect(
        this->ui->actionConnect,
        SIGNAL(toggled(bool)),
        this,
        SIGNAL(toggleStart()));

  connect(
      this->ui->mainToolBar,
      SIGNAL(actionTriggered(QAction *)),
      this,
      SLOT(onActionTriggered(QAction *)));

  connect(
        this->ui->actionSettings,
        SIGNAL(triggered(bool)),
        this,
        SIGNAL(openModemDialog()));

  connect(
        this->ui->actionLoad,
        SIGNAL(triggered(bool)),
        this,
        SIGNAL(loadFile()));

  connect(
        this->ui->actionFrame_logger,
        SIGNAL(toggled(bool)),
        this,
        SIGNAL(toggleFrameLog(bool)));

  connect(
        this->ui->actionMessage_logger,
        SIGNAL(toggled(bool)),
        this,
        SIGNAL(toggleMessageLog(bool)));

  connect(
        this->ui->actionCredentials,
        SIGNAL(toggled(bool)),
        this,
        SIGNAL(toogleCredentialsLog(bool)));

  connect(
        this->ui->actionDLMS_Translator,
        SIGNAL(toggled(bool)),
        this,
        SIGNAL(toggleTranslator(bool)));

  connect(
        this->ui->topologyView,
        SIGNAL(activated(QModelIndex)),
        this,
        SLOT(onNodeActivated(QModelIndex)));
}

void
MainWindow::setIconPath(QString const &iconPath)
{
  QIcon *icon = new QIcon(iconPath);

  if (this->icon)
    delete this->icon;

  this->icon = icon;
}

QSaneMdiSubWindow *
MainWindow::openWindow(
    QString const &name,
    QString const &title,
    QWidget *widget,
    QIcon const *icon)
{
  QSaneMdiSubWindow *subWindow;

  if (findWindow(name) != nullptr)
    return nullptr;

  subWindow = new QSaneMdiSubWindow(name);
  subWindow->setWidget(widget);
  subWindow->setWindowTitle(title);

  if (icon)
    subWindow->setWindowIcon(*icon);
  else if (this->icon)
    subWindow->setWindowIcon(*this->icon);

  this->windowMap[name] = subWindow;

  connect(
        subWindow,
        SIGNAL(closed(QString)),
        this,
        SLOT(onCloseSubWindow(QString)));

  this->ui->mdiArea->addSubWindow(subWindow);

  subWindow->show();

  return subWindow;
}

bool
MainWindow::closeWindow(QString const &name)
{
  QMdiSubWindow *window = this->findWindow(name);

  if (window == nullptr)
    return false;

  if (window->close()) {
    this->ui->mdiArea->removeSubWindow(window);
    window->widget()->setParent(nullptr);
    window->deleteLater();
    this->windowMap[name] = nullptr;
    return true;
  }

  return false;
}

QSaneMdiSubWindow *
MainWindow::findWindow(QString const &name) const
{
  if (this->windowMap.find(name) != this->windowMap.end())
    return this->windowMap[name];

  return nullptr;
}

QString
MainWindow::makeWindowName(QString const &baseName)
{
  return baseName + QString::number(QDateTime::currentMSecsSinceEpoch());
}

void
MainWindow::setSubNet(const PLCTool::SubNet *sn)
{
  this->model->setSubNet(sn);
}

bool
MainWindow::connectState(void) const
{
  return this->ui->actionConnect->isChecked();
}

void
MainWindow::setConnectState(bool state)
{
  this->ui->actionConnect->setChecked(state);
}

void
MainWindow::setButtonState(Button btn, bool state)
{
  switch (btn) {
    case MAIN_WINDOW_BUTTON_FRAME_LOG:
      this->ui->actionFrame_logger->setChecked(state);
      break;

    case MAIN_WINDOW_BUTTON_MESSAGE_LOG:
      this->ui->actionMessage_logger->setChecked(state);
      break;

    case MAIN_WINDOW_BUTTON_CREDENTIALS_LOG:
      this->ui->actionCredentials->setChecked(state);
      break;

    case MAIN_WINDOW_BUTTON_TRANSLATOR:
      this->ui->actionDLMS_Translator->setChecked(state);
      break;

  }
}

void
MainWindow::notifySubNetChanges(void)
{
  bool doResize = this->model->rowCount() < 2;

  this->model->notifyLayout();

  if (doResize) {
    this->ui->topologyView->resizeColumnToContents(0);
    this->ui->topologyView->resizeColumnToContents(1);
  }
}

MainWindow::~MainWindow()
{
  delete ui;
  delete icon;
}

///////////////////////////////// Slots ////////////////////////////////////////

void
MainWindow::onCloseSubWindow(QString subWindow)
{
  emit closeSubWindow(subWindow);
  this->windowMap[subWindow] = nullptr;
}

void
MainWindow::onNodeActivated(QModelIndex index)
{
  PLCTool::Node *node = this->model->node(index);

  if (node != nullptr
      && (node->type() == PLCTool::METER
          || node->type() == PLCTool::SWITCH))
    emit openMeterInfo(static_cast<PLCTool::Meter *>(node));
}

void
MainWindow::onActionTriggered(QAction *action)
{
  if (action->data().toString() == "attack")
    emit attackActionTriggered(action->text());
}

void
MainWindow::onNewAttackModuleFound(QString attackName)
{
  QIcon bombIcon(":/Icons/bomb.png");

  QAction *attackAction =
      this->ui->mainToolBar->addAction(bombIcon, attackName);
  attackAction->setData(QVariant("attack"));
}
