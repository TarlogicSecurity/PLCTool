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
}

bool
Application::work(void)
{
  this->ui->show();
  return this->exec() == 0;
}

Application::~Application()
{
  if (this->ui != nullptr)
    delete this->ui;
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
      delete this->adapter;
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
    delete this->adapter;
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
        SIGNAL(subnetAnnounce(PLCTool::Concentrator*,uint64_t)),
        this,
        SLOT(onSubnetAnnounce(PLCTool::Concentrator*,uint64_t)));

  connect(
        this->adapter,
        SIGNAL(dataReceived(PLCTool::Meter*,bool,const void*,size_t)),
        this,
        SLOT(onDataReceived(PLCTool::Meter*,bool,const void*,size_t)));

  connect(
        this->adapter,
        SIGNAL(frameReceived(PLCTool::Concentrator*,bool,const void*,size_t)),
        this,
        SLOT(onFrameReceived(PLCTool::Concentrator*,bool,const void*,size_t)));

  connect(
        this->adapter,
        SIGNAL(meterFound(PLCTool::Concentrator*,PLCTool::Meter*)),
        this,
        SLOT(onMeterFound(PLCTool::Concentrator*,PLCTool::Meter*)));
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
}

void
Application::parseDataFrame(
    PLCTool::Meter *meter,
    bool downlink,
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
  const uint8_t *p = nullptr;
  unsigned int i = 0;
  PLCTool::Concentrator *dc =
      static_cast<PLCTool::Concentrator *>(meter->parent()->parent());

  if (size < 3)
    return;

  // Skip convergence layer (90 01 01 etc)
  asBytes += 3;
  data = asBytes;
  size -= 3;

  this->ui->pushData(dc, meter->id(), downlink, data, size);

  TRY(stream = ber_stream_copy(data, size));
  TRY(ber_stream_read_uint8(stream, cmd));

  // Command AARQ: Decode
  if (cmd == DLMS_COMMAND_AARQ) {
    TRY(ber_stream_read_uint8(stream, len));

    printf("AARQ FOUND: ");
    for (i = 0; i < size; ++i)
      printf("%02x", asBytes[i]);
    putchar(10);

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
        }

        ber_type_destroy(type);
        type = nullptr;
      }
    }

    if (infoFound && pwdFound) {
      meter->params()["AARQ_FOUND"] = std::string("TRUE");
      meter->params()["MAX_PDU_SIZE"] = std::to_string(maxSize);

      this->ui->pushCreds(dc, meter->id(), QString::fromStdString(password));
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
Application::onCloseAdapter(void)
{
  if (this->closeAdapter())
    this->ui->setAdapter(nullptr);
}

void
Application::onFrameReceived(
    PLCTool::Concentrator *c,
    bool downlink,
    const void *data,
    size_t size)
{
  this->ui->pushFrame(c, downlink, data, size);
}

void
Application::onDataReceived(
    PLCTool::Meter *meter,
    bool downlink,
    const void *data,
    size_t size)
{
  this->parseDataFrame(meter, downlink, data, size);
}


void
Application::onSubnetAnnounce(PLCTool::Concentrator *, uint64_t)
{
  this->ui->notifyTopologyChange();
}

void
Application::onMeterFound(PLCTool::Concentrator *, PLCTool::Meter *)
{
  this->ui->notifyTopologyChange();
}

