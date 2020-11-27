#include "QtUi.h"

#include "CredentialsUI.h"
#include "FrameLogUI.h"
#include "DLMSLogUI.h"
#include "TranslatorUI.h"
#include <QFileDialog>
#include <QCoreApplication>

QtUi::QtUi(QObject *parent) : QObject(parent)
{
  FrameLogUI::registerTypes();
  DLMSLogUI::registerTypes();

  this->mainWindow = new MainWindow(nullptr);
  this->modemDialog = new ModemDialog(nullptr);
  this->loadingDialog = new LoadingStatusDialog(nullptr);
  this->loadingDialog->setWindowTitle("Loading frames from file");
  this->frameLogUi = new FrameLogUI;
  this->dlmsLogUi = new DLMSLogUI;
  this->credentialsUi = new CredentialsUI;
  this->translatorUi = new TranslatorUI;

  this->refreshTimer.start();

  this->connectAll();
}

void
QtUi::connectAll(void)
{
  connect(
        this->mainWindow,
        SIGNAL(openModemDialog()),
        this,
        SLOT(onOpenConfig()));

  connect(
        this->mainWindow,
        SIGNAL(toggleStart()),
        this,
        SLOT(onToggleStart()));

  connect(
        this->mainWindow,
        SIGNAL(loadFile()),
        this,
        SLOT(onLoadFile()));

  connect(
        this->loadingDialog,
        SIGNAL(rejected()),
        this,
        SLOT(onRejectLoading()));

  connect(
        this->mainWindow,
        SIGNAL(openMeterInfo(PLCTool::Meter*)),
        this,
        SIGNAL(openMeterInfo(PLCTool::Meter*)));

  connect(
        this->frameLogUi,
        SIGNAL(frameSelected(Frame &)),
        this,
        SLOT(onSelectFrame(Frame &)));

  connect(
        this->mainWindow,
        SIGNAL(toggleFrameLog(bool)),
        this,
        SLOT(onToggleFrameLog(bool)));

  connect(
        this->mainWindow,
        SIGNAL(toggleMessageLog(bool)),
        this,
        SLOT(onToggleMessageLog(bool)));

  connect(
        this->mainWindow,
        SIGNAL(toogleCredentialsLog(bool)),
        this,
        SLOT(onToggleCredentialsLog(bool)));

  connect(
        this->mainWindow,
        SIGNAL(toggleTranslator(bool)),
        this,
        SLOT(onToggleTranslator(bool)));

  connect(
        this->mainWindow,
        SIGNAL(closeSubWindow(QString)),
        this,
        SLOT(onCloseSubWindow(QString)));
}

void
QtUi::breathe(void)
{
  if (this->refreshTimer.elapsed() > 100) {
    QCoreApplication::processEvents();
    this->frameLogUi->refreshFrames();

    this->dlmsLogUi->refreshMessages();

    this->credentialsUi->refreshCredentials();

    if (this->parsedFrameCounter == 0
        && this->totalFrameCounter > 0) {
      this->loadingMessage("Counting frames");
      this->loadingDialog->setLimits(0, 0);
    } else if (this->totalFrameCounter > 0){
      this->loadingMessage("Loading frames");
      this->loadingDialog->setLimits(
            this->parsedFrameCounter,
            this->totalFrameCounter);
    }
    this->refreshTimer.restart();
  }
}

void
QtUi::show(void)
{
  this->mainWindow->show();
}

QtUi::~QtUi()
{
  delete this->mainWindow;
  delete this->modemDialog;
}

void
QtUi::setLoading(bool state)
{
  this->frameLogUi->setSortingEnabled(!state);
  this->dlmsLogUi->setSortingEnabled(!state);
  this->credentialsUi->setSortingEnabled(!state);

  if (state)
    this->loadingDialog->show();
  else
    this->loadingDialog->hide();
}

void
QtUi::setCounters(unsigned int parsed, unsigned int total)
{
  this->parsedFrameCounter = parsed;
  this->totalFrameCounter  = total;
}

void
QtUi::loadingMessage(QString text)
{
  this->loadingDialog->setStatus(text);
}

void
QtUi::setAdapter(PLCTool::Adapter *adapter)
{
  if (adapter != nullptr) {
    if (this->frameLogUi != nullptr)
      this->frameLogUi->clear();
    if (this->dlmsLogUi != nullptr)
      this->dlmsLogUi->clear();
    if (this->credentialsUi != nullptr)
      this->credentialsUi->clear();

    for (auto p : this->meterUiMap.keys()) {
      QString windowName = "MeterInfo." + QString::number(p);
      QSaneMdiSubWindow *win = this->mainWindow->findWindow(windowName);
      if (win != nullptr)
        win->close();

      if (this->meterUiMap.contains(p)) {
        this->meterUiMap[p]->deleteLater();
        this->meterUiMap.remove(p);
      }
    }

    const PLCTool::SubNet &sn = adapter->nodes();
    this->mainWindow->setSubNet(&sn);
  } else {
    // UI goes to idle state. Last subnet is still available for browsing
  }
}

void
QtUi::notifyTopologyChange(void)
{
  this->mainWindow->notifySubNetChanges();
}

void
QtUi::closeAllMeterInfo(void)
{
  for (auto p : this->meterUiMap) {
    QMdiSubWindow *window = static_cast<QMdiSubWindow *>(p->parent());
    window->close();
    p->deleteLater();
  }
}

void
QtUi::openMeterInfoView(MeterInfo *info)
{
  QString windowName = "MeterInfo." + QString::number(info->meter()->id());

  if (this->mainWindow->findWindow(windowName) == nullptr) {
    MeterUI *ui = new MeterUI(nullptr, info);
    QString title;

    if (info->meter()->name().size() > 0)
      title = QString::fromStdString(
            "Meter information: " + info->meter()->name());
    else
      title = "Meter information: "
          + QString().asprintf(
            "%06llx",
            static_cast<quint64>(info->meter()->id()));

    (void) this->mainWindow->openWindow(windowName, title, ui);
    this->meterUiMap.insert(info->meter()->id(), ui);
  }
}

void
QtUi::pushFrame(Frame const &frame)
{
  this->frameLogUi->saveFrame(frame);
  this->breathe();
}

void
QtUi::refreshViews(void)
{
  this->frameLogUi->refreshFrames();
  this->dlmsLogUi->refreshMessages();
  this->credentialsUi->refreshCredentials();

  for (auto p : this->meterUiMap)
    p->refreshViews();
}

void
QtUi::pushDlmsMessage(DlmsMessage const &message)
{
  this->dlmsLogUi->saveMessage(message);
  this->breathe();
}

void
QtUi::pushCreds(CredInfo const &info)
{
  this->credentialsUi->saveCreds(info);
  this->breathe();
}

QString
QtUi::modemPath(void) const
{
  return this->modemDialog->path();
}

unsigned int
QtUi::modemBaud(void) const
{
  return static_cast<unsigned int>(this->modemDialog->baud());
}

///////////////////////////////// Slots ////////////////////////////////////////
void
QtUi::onToggleStart(void)
{
  if (this->mainWindow->connectState()) {
    if (this->firstConnection) {
      int result;

      this->modemDialog->show();
      result = this->modemDialog->exec();
      this->modemDialog->hide();

      if (result != QDialog::Accepted)
        this->mainWindow->setConnectState(false);
      else
        this->firstConnection = false;
    }

    if (this->mainWindow->connectState())
      emit openAdapter();
  } else {
    emit closeAdapter();
  }
}

void
QtUi::onOpenConfig(void)
{
  this->modemDialog->show();
  (void) this->modemDialog->exec();
  this->modemDialog->hide();
}

void
QtUi::onSelectFrame(Frame &)
{
  QSaneMdiSubWindow *dl = this->mainWindow->findWindow("DlmsLog");
  QSaneMdiSubWindow *fl = this->mainWindow->findWindow("FrameLog");

  if (dl != nullptr && fl != nullptr) {
//    DLMSLogUI  *dlui = static_cast<DLMSLogUI  *>(dl->widget());
//    FrameLogUI *flui = static_cast<FrameLogUI *>(fl->widget());

  }
}

void
QtUi::onSelectDlmsMessage(DlmsMessage &msg)
{
  QSaneMdiSubWindow *fl = this->mainWindow->findWindow("FrameLog");

  if (fl != nullptr) {
    FrameLogUI *flui = static_cast<FrameLogUI *>(fl->widget());
    flui->selectNear(msg.timeStamp, PLCTool::PrimeFrame::DATA, msg.nodeId);
  }
}

void
QtUi::onLoadFile(void)
{
  QFileDialog dialog(this->mainWindow);

  dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
  dialog.setNameFilter("Packet log (*.log)");
  dialog.setViewMode(QFileDialog::Detail);
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  if (dialog.exec()) {
    QStringList list = dialog.selectedFiles();

    if (list.size() > 0)
      emit openLogFile(list.at(0));
  }
}

void
QtUi::openFrameLog(void)
{
  this->mainWindow->setButtonState(
        MainWindow::MAIN_WINDOW_BUTTON_FRAME_LOG,
        true);
  this->onToggleFrameLog(true);
}

void
QtUi::onToggleFrameLog(bool open)
{
  QString windowName = "FrameLog";

  if (open) {
    (void) this->mainWindow->openWindow(
          windowName,
          "Frame logger",
          this->frameLogUi);
    this->frameLogUi->realize();
  } else {
    (void) this->mainWindow->closeWindow(windowName);
  }
}

void
QtUi::onToggleMessageLog(bool open)
{
  QString windowName = "DlmsLog";

  if (open) {
    (void) this->mainWindow->openWindow(
          windowName,
          "DLMS Message logger",
          this->dlmsLogUi);
    this->dlmsLogUi->realize();
  } else {
    (void) this->mainWindow->closeWindow(windowName);
  }
}

void
QtUi::onToggleCredentialsLog(bool open)
{
  QString windowName = "CredsLog";

  if (open) {
    (void) this->mainWindow->openWindow(
          windowName,
          "Credential logger",
          this->credentialsUi);
    this->credentialsUi->realize();
  } else {
    (void) this->mainWindow->closeWindow(windowName);
  }
}

void
QtUi::onToggleTranslator(bool open)
{
  QString windowName = "Translator";

  if (open)
    (void) this->mainWindow->openWindow(
          windowName,
          "DLMS Translator",
          this->translatorUi);
  else
    (void) this->mainWindow->closeWindow(windowName);
}

void
QtUi::onCloseSubWindow(QString windowName)
{
  if (windowName == "FrameLog")
    this->mainWindow->setButtonState(
        MainWindow::MAIN_WINDOW_BUTTON_FRAME_LOG,
        false);
  else if (windowName == "DlmsLog")
    this->mainWindow->setButtonState(
        MainWindow::MAIN_WINDOW_BUTTON_MESSAGE_LOG,
        false);
  else if (windowName == "CredsLog")
    this->mainWindow->setButtonState(
        MainWindow::MAIN_WINDOW_BUTTON_CREDENTIALS_LOG,
        false);
  else if (windowName == "Translator")
    this->mainWindow->setButtonState(
        MainWindow::MAIN_WINDOW_BUTTON_TRANSLATOR,
        false);
}

void
QtUi::onRejectLoading(void)
{
  emit closeAdapter();
}
