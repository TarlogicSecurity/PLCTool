#include "QtUi.h"

#include "CredentialsUI.h"
#include "FrameLogUI.h"
#include "DLMSLogUI.h"
#include "TranslatorUI.h"

QtUi::QtUi(QObject *parent) : QObject(parent)
{
  this->mainWindow = new MainWindow(nullptr);
  this->modemDialog = new ModemDialog(nullptr);

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
QtUi::setAdapter(PLCTool::Adapter *adapter)
{
  if (adapter != nullptr) {
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
    (void) this->mainWindow->openWindow(
          "FrameLog",
          "Frame logger",
          new FrameLogUI);
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
    (void) this->mainWindow->openWindow(
          "DlmsLog",
          "DLMS Message Logger",
          new DLMSLogUI);
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
    bool downlink,
    const void *data,
    size_t size)
{
  QSaneMdiSubWindow *sw = this->mainWindow->findWindow("FrameLog");

  if (sw != nullptr) {
    FrameLogUI *ui = static_cast<FrameLogUI *>(sw->widget());

    ui->pushFrame(
          concentrator,
          downlink,
          data,
          size);
  }
}

void
QtUi::pushCreds(
    const PLCTool::Concentrator *dc,
    PLCTool::NodeId meter,
    QString password)
{
  QSaneMdiSubWindow *sw = this->mainWindow->findWindow("CredsLog");

  if (sw != nullptr) {
    CredentialsUI *ui = static_cast<CredentialsUI *>(sw->widget());

    ui->pushCreds(dc, meter, password);
  }
}

void
QtUi::pushData(
    const PLCTool::Concentrator *dc,
    PLCTool::NodeId meter,
    bool downlink,
    const void *data,
    size_t size)
{
  QSaneMdiSubWindow *sw = this->mainWindow->findWindow("DlmsLog");

  if (sw != nullptr) {
    DLMSLogUI *ui = static_cast<DLMSLogUI *>(sw->widget());

    ui->pushMessage(dc, meter, downlink, data, size);
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
