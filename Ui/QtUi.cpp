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
QtUi::loadingMessage(QString text)
{
  this->loadingDialog->setStatus(text);
}

void
QtUi::setAdapter(PLCTool::Adapter *adapter)
{
  if (adapter != nullptr) {
    this->frameCounter = 0;
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
QtUi::pushFrame(
    const PLCTool::Concentrator *concentrator,
    QDateTime timeStamp,
    bool downlink,
    const void *data,
    size_t size)
{
  if (this->frameLogUi == nullptr) {
    QSaneMdiSubWindow *sw = this->mainWindow->findWindow("FrameLog");

    if (sw != nullptr)
      this->frameLogUi = static_cast<FrameLogUI *>(sw->widget());
  }

  ++this->frameCounter;

  if (this->frameLogUi != nullptr) {
    this->frameLogUi->pushFrame(
          concentrator,
          timeStamp,
          downlink,
          data,
          size);

    if (this->loadingDialog->isVisible())
      this->loadingDialog->setStatus(
            QString::number(this->frameCounter) + " frames");
    this->breathe();
  }
}

void
QtUi::refreshFrames(void)
{
  if (this->frameLogUi != nullptr)
    this->frameLogUi->refreshFrames();

  if (this->dlmsLogUi != nullptr)
    this->dlmsLogUi->refreshMessages();
}

void
QtUi::pushCreds(
    const PLCTool::Concentrator *dc,
    QDateTime timeStamp,
    PLCTool::NodeId meter,
    QString password)
{
  QSaneMdiSubWindow *sw = this->mainWindow->findWindow("CredsLog");

  if (sw != nullptr) {
    CredentialsUI *ui = static_cast<CredentialsUI *>(sw->widget());

    ui->pushCreds(dc, timeStamp, meter, password);
    this->breathe();
  }
}

void
QtUi::pushData(
    const PLCTool::Concentrator *dc,
    QDateTime timeStamp,
    PLCTool::NodeId meter,
    bool downlink,
    const void *data,
    size_t size)
{
  if (this->dlmsLogUi == nullptr) {
    QSaneMdiSubWindow *sw = this->mainWindow->findWindow("DlmsLog");

    if (sw != nullptr)
      this->dlmsLogUi = static_cast<DLMSLogUI *>(sw->widget());
  }

  if (this->dlmsLogUi != nullptr) {
    this->dlmsLogUi->pushMessage(dc, timeStamp, meter, downlink, data, size);
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
