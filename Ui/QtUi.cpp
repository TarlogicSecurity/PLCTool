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

  this->refreshTimer.start();

  this->connectAll();

  this->openFrameLog();
  this->openCredsLog();
  this->openDlmsLog();
  this->openTranslator();
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
}

void
QtUi::breathe(void)
{
  if (this->refreshTimer.elapsed() > 100) {
    QCoreApplication::processEvents();
    if (this->frameLogUi != nullptr)
      this->frameLogUi->refreshFrames();

    if (this->dlmsLogUi != nullptr)
      this->dlmsLogUi->refreshMessages();

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
          + QString().sprintf(
            "%06llx",
            static_cast<quint64>(info->meter()->id()));

    (void) this->mainWindow->openWindow(windowName, title, ui);
    this->meterUiMap.insert(info->meter()->id(), ui);
  }
}

void
QtUi::openFrameLog(void)
{
  if (this->mainWindow->findWindow("FrameLog") == nullptr) {
    FrameLogUI *ui = new FrameLogUI;
    (void) this->mainWindow->openWindow("FrameLog", "Frame logger", ui);

    connect(
          ui,
          SIGNAL(frameSelected(Frame &)),
          this,
          SLOT(onSelectFrame(Frame &)));
  }
}

void
QtUi::openCredsLog(void)
{
  if (this->mainWindow->findWindow("CredsLog") == nullptr) {
    (void) this->mainWindow->openWindow(
          "CredsLog",
          "Credential logger",
          new CredentialsUI);
  }
}

void
QtUi::openDlmsLog(void)
{
  if (this->mainWindow->findWindow("DlmsLog") == nullptr) {
    DLMSLogUI *ui = new DLMSLogUI;

    (void) this->mainWindow->openWindow(
          "DlmsLog",
          "DLMS Message Logger",
          ui);

    connect(
          ui,
          SIGNAL(messageSelected(DlmsMessage &)),
          this,
          SLOT(onSelectDlmsMessage(DlmsMessage &)));
  }
}

void
QtUi::openTranslator(void)
{
  if (this->mainWindow->findWindow("DlmsTranslator") == nullptr) {
    (void) this->mainWindow->openWindow(
          "DlmsTranslator",
          "DLMS Translator",
          new TranslatorUI);
  }
}


void
QtUi::pushFrame(Frame const &frame)
{
  if (this->frameLogUi == nullptr) {
    QSaneMdiSubWindow *sw = this->mainWindow->findWindow("FrameLog");

    if (sw != nullptr)
      this->frameLogUi = static_cast<FrameLogUI *>(sw->widget());
  }

  if (this->frameLogUi != nullptr) {
    this->frameLogUi->saveFrame(frame);

    this->breathe();
  }
}

void
QtUi::refreshViews(void)
{
  if (this->frameLogUi != nullptr)
    this->frameLogUi->refreshFrames();

  if (this->dlmsLogUi != nullptr)
    this->dlmsLogUi->refreshMessages();

  for (auto p : this->meterUiMap)
    p->refreshViews();
}

void
QtUi::pushDlmsMessage(DlmsMessage const &message)
{
  if (this->dlmsLogUi == nullptr) {
    QSaneMdiSubWindow *sw = this->mainWindow->findWindow("DlmsLog");

    if (sw != nullptr)
      this->dlmsLogUi = static_cast<DLMSLogUI *>(sw->widget());
  }

  if (this->dlmsLogUi != nullptr) {
    this->dlmsLogUi->saveMessage(message);
    this->breathe();
  }
}

void
QtUi::pushCreds(
    const PLCTool::Concentrator *dc,
    QDateTime timeStamp,
    PLCTool::NodeId meter,
    QString password,
    QString conformance)
{
  QSaneMdiSubWindow *sw = this->mainWindow->findWindow("CredsLog");

  if (sw != nullptr) {
    CredentialsUI *ui = static_cast<CredentialsUI *>(sw->widget());

    ui->pushCreds(dc, timeStamp, meter, password);
    this->breathe();
  }
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
QtUi::onRejectLoading(void)
{
  emit closeAdapter();
}
