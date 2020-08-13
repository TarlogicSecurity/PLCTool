#include "Application.h"
#include <arpa/inet.h>
#include <Topology/Exception.h>
#include <Topology/Adapter.h>
#include <QMessageBox>
#include <ber/ber.h>
#include <dlms/dlms.h>

using namespace PLCTool;

Application::Application(int &argc, char *argv[]) : QApplication(argc, argv)
{
  this->ui = new QtUi(this);

  this->connectUi();

  this->dlmsProcessor = new DLMSProcessor(nullptr);
  this->dlmsProcThread = new QThread(nullptr);
  this->dlmsProcessor->moveToThread(this->dlmsProcThread);

  this->primeProcessor = new PRIMEProcessor(nullptr);
  this->primeProcThread = new QThread(nullptr);
  this->primeProcessor->moveToThread(this->primeProcThread);

  this->connectProcessors();

  this->primeProcThread->start();
  this->dlmsProcThread->start();
}

bool
Application::work(void)
{
  this->ui->show();
  return this->exec() == 0;
}

Application::~Application()
{
  if (this->primeProcThread != nullptr)
    this->primeProcThread->quit();

  if (this->dlmsProcThread != nullptr)
    this->dlmsProcThread->quit();

  if (this->ui != nullptr)
    delete this->ui;
}

void
Application::connectProcessors(void)
{
  // Thread lifecycle management
  connect(
        this->primeProcThread,
        SIGNAL(finished()),
        this->primeProcThread,
        SLOT(deleteLater()));

  connect(
        this->primeProcThread,
        SIGNAL(finished()),
        this->primeProcessor,
        SLOT(deleteLater()));

  connect(
        this->dlmsProcThread,
        SIGNAL(finished()),
        this->dlmsProcThread,
        SLOT(deleteLater()));

  connect(
        this->dlmsProcThread,
        SIGNAL(finished()),
        this->dlmsProcessor,
        SLOT(deleteLater()));

  // Object message passing
  connect(
        this,
        SIGNAL(frameReceived(quint64,QDateTime,bool,QVector<uint8_t>)),
        this->primeProcessor,
        SLOT(process(quint64,QDateTime,bool,QVector<uint8_t>)));

  connect(
        this->primeProcessor,
        SIGNAL(frame(Frame)),
        this,
        SLOT(onProcessedFrame(Frame)));

  connect(
        this,
        SIGNAL(messageReceived(QString,QDateTime,quint64,bool,QVector<uint8_t>)),
        this->dlmsProcessor,
        SLOT(process(QString,QDateTime,quint64,bool,QVector<uint8_t>)));

  connect(
        this->dlmsProcessor,
        SIGNAL(dlmsMessage(DlmsMessage)),
        this,
        SLOT(onProcessedDlmsMessage(DlmsMessage)));
}

bool
Application::loadLogFile(QString const &path)
{
  this->closeAdapter();

  try {
    this->adapter = new PLCTool::PrimeAdapter(path);
    if (!this->adapter->initialize()) {
      this->adapter->deleteLater();
      this->adapter = nullptr;

      PH_THROW(
            GENERIC,
            "Failed to initialize adapter (file "
            + path.toStdString()
            + "). Please verify that permissions are correct.");
    }
  } catch (PLCTool::Exception &e) {
    QMessageBox::critical(
          nullptr,
          "Failed to open adapter",
          QString::fromStdString(e.toString()));
  }

  if (this->adapter != nullptr)
    this->connectAdapter();

  return this->adapter != nullptr;
}

bool
Application::openAdapter(QString const &path, unsigned int baud)
{
  PLCTool::StringParams params;

  params["path"] = path.toStdString();
  params["baud"] = QString::number(baud).toStdString();

  if (this->adapter != nullptr)
    return false;

  try {
    this->adapter = new PLCTool::PrimeAdapter(params);
    if (!this->adapter->initialize()) {
      this->adapter->deleteLater();
      this->adapter = nullptr;

      PH_THROW(
            GENERIC,
            "Failed initialize adapter (device "
            + path.toStdString()
            + "). Please verify that the device is correctly plugged.");
    }
  } catch (PLCTool::Exception &e) {
    QMessageBox::critical(
          nullptr,
          "Failed to open adapter",
          QString::fromStdString(e.toString()));
  }

  if (this->adapter != nullptr)
    this->connectAdapter();

  return this->adapter != nullptr;
}

bool
Application::closeAdapter(void)
{
  if (this->adapter != nullptr) {
    this->subNetHistory.push_back(this->adapter->takeSubNet());
    this->adapter->disconnect();
    this->adapter->deleteLater();
    this->adapter = nullptr;
    return true;
  }

  return false;
}

void
Application::connectAdapter(void)
{
  connect(
        this->adapter,
        SIGNAL(subnetAnnounce(PLCTool::Concentrator*,QDateTime,uint64_t)),
        this,
        SLOT(onSubnetAnnounce(PLCTool::Concentrator*,QDateTime,uint64_t)));

  connect(
        this->adapter,
        SIGNAL(dataReceived(PLCTool::Meter*,QDateTime,bool,const void*,size_t)),
        this,
        SLOT(onDataReceived(PLCTool::Meter*,QDateTime,bool,const void*,size_t)));

  connect(
        this->adapter,
        SIGNAL(frameReceived(PLCTool::Concentrator*,QDateTime,bool,const void*,size_t)),
        this,
        SLOT(onFrameReceived(PLCTool::Concentrator*,QDateTime,bool,const void*,size_t)));

  connect(
        this->adapter,
        SIGNAL(meterFound(PLCTool::Concentrator*,QDateTime,PLCTool::Meter*)),
        this,
        SLOT(onMeterFound(PLCTool::Concentrator*,QDateTime,PLCTool::Meter*)));

  connect(
        this->adapter,
        SIGNAL(closed(void)),
        this,
        SLOT(onAdapterClosed(void)));

  connect(
        this->adapter,
        SIGNAL(status(QString)),
        this,
        SLOT(onAdapterStatusMessage(QString)));

  connect(
        this->adapter,
        SIGNAL(refresh(void)),
        this,
        SLOT(onAdapterRefreshRequested(void)));
}

void
Application::connectUi(void)
{
  connect(
        this->ui,
        SIGNAL(openAdapter()),
        this,
        SLOT(onOpenAdapter()));

  connect(
        this->ui,
        SIGNAL(closeAdapter()),
        this,
        SLOT(onCloseAdapter()));

  connect(
        this->ui,
        SIGNAL(openLogFile(QString)),
        this,
        SLOT(onOpenLogFile(QString)));
}

static std::string
getMeterNameFromAPTitle(const ber_type_t *type)
{
  unsigned char *asBytes = NULL;
  unsigned int field_count = type->field_count;
  unsigned long count;
  std::string title;

  if (field_count != 1)
    goto done;

  count = type->field_list[0]->bytecount;
  asBytes = type->field_list[0]->bytebuf;

  if (count >= 3
      && isprint(asBytes[0])
      && isprint(asBytes[1])
      && isprint(asBytes[2])) {
    bool allPrintable = true;
    char hex[4];
    for (unsigned long i = 3;
         i < count && allPrintable;
         ++i)
      allPrintable = allPrintable && isprint(asBytes[i]);

    if (allPrintable) {
      std::copy(
            reinterpret_cast<char *>(asBytes),
            reinterpret_cast<char *>(asBytes) + count,
            std::back_inserter(title));
    } else {
      std::copy(
            reinterpret_cast<char *>(asBytes),
            reinterpret_cast<char *>(asBytes) + 3,
            std::back_inserter(title));

      title += " (";
      for (unsigned long i = 3; i < count; ++i) {
        snprintf(hex, 4, "%02x", asBytes[i]);
        title += hex;
      }
      title += ")";
    }
  }

done:
  return title;
}

void
Application::parseDataFrame(
    PLCTool::Meter *meter,
    QDateTime timeStamp,
    bool,
    const void *data,
    size_t size)
{
  const uint8_t *asBytes = static_cast<const uint8_t *>(data);
  uint8_t cmd;
  uint8_t len;
  ber_stream_t *stream = nullptr;
  ber_type_t *type = nullptr;
  uint32_t conformance;
  uint16_t maxSize = 0;
  bool pwdFound = false;
  bool infoFound = false;
  std::string password;
  PLCTool::Concentrator *dc =
      static_cast<PLCTool::Concentrator *>(meter->parent()->parent());
  const uint8_t *p = nullptr;

  if (size < 3)
    return;

  // Skip convergence layer (90 01 01 etc)
  asBytes += 3;
  data = asBytes;
  size -= 3;

  TRY(stream = ber_stream_copy(data, size));
  TRY(ber_stream_read_uint8(stream, cmd));

  // Command AARQ: Decode
  if (cmd == DLMS_COMMAND_AARQ) {
    TRY(ber_stream_read_uint8(stream, len));

    if (len > ber_stream_avail(stream))
      goto fail;

    while (!ber_stream_overflow(stream) && ber_stream_avail(stream) >= 2) {
      if ((type = ber_type_from_stream(stream)) != nullptr) {
        switch (type->orig_id) {
          // Password field found
          case BER_CI_CONTEXT | BER_CI_CONSTRUCTED | PDU_TYPE_CALLING_AUTHENTICATION_VALUE:
            if (type->field_count == 1
                && type->field_list[0]->orig_id == BER_CI_CONTEXT) {
              password.assign(
                  type->field_list[0]->string,
                  type->field_list[0]->string + type->field_list[0]->bytecount);
              pwdFound = true;
            }
            break;

          // User information found
          case BER_CI_CONTEXT | BER_CI_CONSTRUCTED | PDU_TYPE_USER_INFORMATION:
            if (type->field_count == 1
                && type->field_list[0]->ci == BER_CI_OCTET_STRING
                && type->field_list[0]->bytecount >= 10
                && type->field_list[0]->bytebuf[0] == DLMS_COMMAND_INITIATE_REQUEST) {
              p = type->field_list[0]->bytebuf + type->field_list[0]->bytecount - 9;

              if ((p[0] == 0x5f && p[1] == 0x1f)
                  || (p[1] == 0x5f && p[2] != 0x1f)) {
                if (p[2] == 4) {
                  memcpy(
                      static_cast<void *>(&conformance),
                      static_cast<const void *>(p + 3),
                      static_cast<unsigned long>(sizeof(uint32_t)));
                  memcpy(
                      static_cast<void *>(&maxSize),
                      static_cast<const void *>(p + 7),
                      static_cast<unsigned long>(sizeof(uint16_t)));

                  conformance = ntohl(conformance);
                  maxSize     = ntohs(maxSize);

                  infoFound = true;
                }
              }
            }
            break;

          // CallingAPTitle
          case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_CALLING_AP_TITLE:
            std::string name = getMeterNameFromAPTitle(type);
            meter->setName(name);
            break;
        }

        ber_type_destroy(type);
        type = nullptr;
      }
    }

    if (infoFound && pwdFound) {
      meter->params()["AARQ_FOUND"]   = std::string("TRUE");
      meter->params()["MAX_PDU_SIZE"] = std::to_string(maxSize);
      this->ui->pushCreds(
            dc,
            timeStamp,
            meter->id(),
            QString::fromStdString(password));
    }
  }

fail:
  if (stream != nullptr)
    ber_stream_destroy(stream);

  if (type != nullptr)
    ber_type_destroy(type);
}

/////////////////////////////////// Slots /////////////////////////////////////
void
Application::onOpenAdapter(void)
{
  if (this->openAdapter(this->ui->modemPath(), this->ui->modemBaud()))
    this->ui->setAdapter(static_cast<PLCTool::Adapter *>(this->adapter));
}

void
Application::onOpenLogFile(QString path)
{
  if (this->loadLogFile(path)) {
    this->ui->setLoading(true);
    this->ui->setAdapter(static_cast<PLCTool::Adapter *>(this->adapter));
  }
}

void
Application::onCloseAdapter(void)
{
  if (this->closeAdapter())
    this->ui->setAdapter(nullptr);
}

void
Application::onFrameReceived(
    PLCTool::Concentrator *dc,
    QDateTime timeStamp,
    bool downlink,
    const void *data,
    size_t size)
{
  PLCTool::NodeId dcId = dc->id();
  QVector<uint8_t> bytes;

  this->ui->setCounters(
        this->adapter->parsedFrameCount(),
        this->adapter->totalFrameCount());

  std::copy(
        static_cast<const uint8_t *>(data),
        static_cast<const uint8_t *>(data) + size,
        std::back_inserter(bytes));

  emit frameReceived(dcId, timeStamp, downlink, bytes);
}

void
Application::onDataReceived(
    PLCTool::Meter *meter,
    QDateTime timeStamp,
    bool downlink,
    const void *data,
    size_t size)
{
  const uint8_t *byteArr = static_cast<const uint8_t *>(data);
  QVector<uint8_t> bytes;
  PLCTool::Concentrator *dc =
      static_cast<PLCTool::Concentrator *>(meter->parent()->parent());

  if (size <= 3)
    return;

  switch (byteArr[0]) {
    case 0x90: // DLMS CL
      this->parseDataFrame(meter, timeStamp, downlink, data, size);

      std::copy(byteArr + 3, (byteArr + size) - 3, std::back_inserter(bytes));

      emit messageReceived(
            QString::fromStdString(
              PLCTool::PrimeAdapter::idToSna(dc->id())),
            timeStamp,
            meter->id(),
            downlink,
            bytes);
      break;

    default:
      printf("Warning: unknown CL (0x%02x)\n", byteArr[0]);
  }
}

void
Application::onSubnetAnnounce(
    PLCTool::Concentrator *,
    QDateTime,
    uint64_t)
{
  this->ui->notifyTopologyChange();
}

void
Application::onMeterFound(
    PLCTool::Concentrator *,
    QDateTime,
    PLCTool::Meter *)
{
  this->ui->notifyTopologyChange();
}

void
Application::onAdapterClosed(void)
{
  this->ui->setLoading(false);
  this->ui->refreshFrames();
  this->onCloseAdapter();
}

void
Application::onAdapterRefreshRequested(void)
{
  this->ui->refreshFrames();
}

void
Application::onAdapterStatusMessage(QString message)
{
  this->ui->loadingMessage(message);
}

void
Application::onProcessedFrame(Frame frame)
{
  this->ui->pushFrame(frame);
}

void
Application::onProcessedDlmsMessage(DlmsMessage message)
{
  this->ui->pushDlmsMessage(message);
}
