//
// PrimeAdapter.cpp: PRIME 1.3 Adapter
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//now

#include "PrimeAdapter.h"
#include <Topology/Adapter.h>
#include <Topology/Meter.h>
#include <Topology/Exception.h>
#include <pthread.h>
#include <fcntl.h>
#include "libprime/prime.h"
#include "spip/spip.h"
#include <pthread.h>
#include <cstring>
#include <list>
#include <cmath>
#include <vector>

#define PRIME_ADAPTER_BAUD_RATE 921600

using namespace PLCTool;

namespace PLCTool {
  struct RawPDU {
    struct timeval timeStamp;
    std::vector<uint8_t> data;

    RawPDU();
    RawPDU(struct spip_pdu *, struct timeval timeStamp);
    RawPDU(const char *hexString, qreal timeStamp);
  };

  typedef std::list<RawPDU> RawPDUList;

  class PrimeAdapterImpl {
    spip_iface_t iface = spip_iface_INITIALIZER;
    FILE *fp = nullptr;
    RawPDUList pduList;
    PrimeAdapter *owner = nullptr;
    prime13_layer_t *layer = nullptr;

    pthread_t readerThread;
    bool readerThreadRunning = false;

    pthread_mutex_t readerMutex;
    bool readerMutexInitialized = false;

    bool blinkState = 0;
    bool halting = false;
    bool halted = false;

    void lock(void);
    void unlock(void);

    bool popPdu(RawPDU &);
    QDateTime currentTimeStamp(void) const;

    void commonInit(PrimeAdapter *);

  public:
    PrimeAdapterImpl(PrimeAdapter *owner, StringParams const &params);
    PrimeAdapterImpl(PrimeAdapter *owner, QString path);
    void spipWarmup(void);
    bool work(void);

    void writeFrame(const void *data, size_t size);
    void setLcd(int row, std::string const &text);
    void setLeds(int leds);

    ~PrimeAdapterImpl();

    inline bool
    isHalted(void) const
    {
      return this->halted;
    }

    static inline NodeId
    getPrimeNodeId(const prime13_node_t *node)
    {
      return node->nid;
    }

    static inline NodeId
    snaToNodeId(const uint8_t *sna)
    {
      unsigned int i;
      NodeId id = 0x0ull;

      for (i = 0; i < 6; ++i)
        id |= static_cast<NodeId>(sna[i]) << ((5 - i) * 8ull);

      return id;
    }

    static inline NodeId
    getPrimeSubNetId(const prime13_node_t *node)
    {
      return snaToNodeId(node->subnet->sna);
    }

    static void *onMeter(
        struct prime13_layer *self,
        prime13_node_t *node);

    static BOOL onSubNet(
        struct prime13_layer *self,
        const uint8_t *sna,
        uint64_t times,
        void *userdata);

    static BOOL onFrame(
        void *userdata,
        const uint8_t *sna,
        BOOL direction,
        const void *data,
        size_t size);

    static BOOL onData(
        struct prime13_layer *self,
        prime13_node_t *node,
        void *userdata,
        BOOL downstream,
        const void *data,
        size_t size);


    static void onDispose(void *userdata);

    static void onDestroy(void *cbdata);

    static void *spipReaderThreadFunc(void *);
    static void *logFileReaderThreadFunc(void *);
  };
}

RawPDU::RawPDU()
{
  gettimeofday(&this->timeStamp, nullptr);
}

RawPDU::RawPDU(struct spip_pdu *pdu, struct timeval tv)
{
  this->timeStamp = tv;
  this->data.resize(pdu->size);
  std::copy(
        pdu->data,
        pdu->data + pdu->size,
        std::begin(this->data));
}

RawPDU::RawPDU(const char *hexString, qreal timeStamp)
{
  unsigned int byte;
  size_t len = strlen(hexString);

  this->timeStamp.tv_sec  = std::floor(timeStamp);
  this->timeStamp.tv_usec = std::round(
        1e6 * (timeStamp - std::floor(timeStamp)));
  this->data.clear();

  for (size_t i = 0; i < len; i += 2) {
    if (sscanf(hexString + i, "%02x", &byte) != 1)
      break;

    this->data.push_back(byte);
  }
}

void
PrimeAdapterImpl::lock(void)
{
  PH_CHECK_GENERIC(
        pthread_mutex_lock(&this->readerMutex) != -1,
        "Failed to acquire PRIME adapter mutex");
}

void
PrimeAdapterImpl::unlock(void)
{
  PH_CHECK_GENERIC(
      pthread_mutex_unlock(&this->readerMutex) != -1,
      "Failed to release PRIME adapter mutex");
}

void *
PrimeAdapterImpl::spipReaderThreadFunc(void *userdata)
{
  PrimeAdapterImpl *self = static_cast<PrimeAdapterImpl *>(userdata);
  struct spip_pdu *pdu = nullptr;

  while (!self->halting && spip_iface_read(&self->iface, &pdu)) {
    if (pdu->command == SPIP_COMMAND_FRAME) {
      struct timeval tv;
      gettimeofday(&tv, nullptr);

      self->lock();
      self->pduList.push_back(RawPDU(pdu, tv));
      self->unlock();
    }

    spip_iface_dispose(&self->iface, pdu);
  }

  self->halted = true;

  return nullptr;
}

void *
PrimeAdapterImpl::logFileReaderThreadFunc(void *userdata)
{
  PrimeAdapterImpl *self = static_cast<PrimeAdapterImpl *>(userdata);
  char *line = nullptr;

  while ((line = fread_line(self->fp)) != nullptr) {
    arg_list_t *al;

    if ((al = csv_split_line(line)) != nullptr) {
      if (al->al_argc >= 8) {
        unsigned long timeStamp;
        if (sscanf(al->al_argv[3], "%lu", &timeStamp) == 1) {
          self->lock();
          self->pduList.push_back(RawPDU(al->al_argv[7], timeStamp));
          self->unlock();
        }
      }

      free_al(al);
    }
    free(line);
  }

  self->halted = true;

  return nullptr;
}

void
PrimeAdapterImpl::writeFrame(const void *data, size_t size)
{
  (void) spip_iface_write_frame(&this->iface, data, size);
}

void
PrimeAdapterImpl::setLcd(int row, std::string const &text)
{
  (void) spip_iface_set_lcd(&this->iface, row, text.c_str());
}

void
PrimeAdapterImpl::setLeds(int leds)
{
  (void) spip_iface_set_led_mask(&this->iface, leds);
}

void
PrimeAdapterImpl::spipWarmup(void)
{
  unsigned int i;

  for (i = 0; i < 10; ++i)
    spip_iface_set_led_mask(&this->iface, 0);
}

void *
PrimeAdapterImpl::onMeter(
    struct prime13_layer *self,
    prime13_node_t *node)
{
  PrimeAdapterImpl *impl =
      static_cast<PrimeAdapterImpl *>(prime13_layer_get_userdata(self));
  Concentrator *dc = impl->owner->assertConcentrator(
        PrimeAdapterImpl::getPrimeSubNetId(node));
  Meter *meter = impl->owner->assertMeter(
        PrimeAdapterImpl::getPrimeSubNetId(node),
        PrimeAdapterImpl::getPrimeNodeId(node));

  emit impl->owner->meterFound(dc, impl->currentTimeStamp(), meter);

  return meter;
}

BOOL
PrimeAdapterImpl::onSubNet(
    struct prime13_layer *self,
    const uint8_t *sna,
    uint64_t times,
    void *)
{
  PrimeAdapterImpl *impl =
      static_cast<PrimeAdapterImpl *>(prime13_layer_get_userdata(self));
  Concentrator *dc =
      impl->owner->assertConcentrator(PrimeAdapterImpl::snaToNodeId(sna));

  if (dc != nullptr)
    emit impl->owner->subnetAnnounce(dc, impl->currentTimeStamp(), times);

  return dc != nullptr ? TRUE : FALSE;
}

BOOL
PrimeAdapterImpl::onFrame(
    void *user,
    const uint8_t *sna,
    BOOL direction,
    const void *data,
    size_t size)
{
  PrimeAdapterImpl *impl = static_cast<PrimeAdapterImpl *>(user);
  NodeId dcId = PrimeAdapterImpl::snaToNodeId(sna);
  Concentrator *dc = impl->owner->assertConcentrator(dcId);

  emit impl->owner->frameReceived(
        dc,
        impl->currentTimeStamp(),
        direction != FALSE, data, size);

  return TRUE;
}

BOOL
PrimeAdapterImpl::onData(
    struct prime13_layer *self,
    prime13_node_t *node,
    void *,
    BOOL downstream,
    const void *data,
    size_t size)
{
  PrimeAdapterImpl *impl =
      static_cast<PrimeAdapterImpl *>(prime13_layer_get_userdata(self));
  Meter *meter =
      impl->owner->assertMeter(
              PrimeAdapterImpl::getPrimeSubNetId(node),
              PrimeAdapterImpl::getPrimeNodeId(node));
  emit impl->owner->dataReceived(
        meter,
        impl->currentTimeStamp(),
        downstream != FALSE, data, size);

  return TRUE;
}

void
PrimeAdapterImpl::onDispose(void *)
{
  // NO-OP
}

void
PrimeAdapterImpl::onDestroy(void *)
{
  // NO-OP
}

bool
PrimeAdapterImpl::popPdu(RawPDU &pdu)
{
  bool avail = false;

  this->lock();

  if (!this->pduList.empty()) {
    pdu = this->pduList.front();
    this->pduList.pop_front();
    avail = true;
  }

  this->unlock();

  return avail;
}

QDateTime
PrimeAdapterImpl::currentTimeStamp(void) const
{
  struct timeval tv = prime13_layer_get_feed_time(this->layer);
  return QDateTime::fromMSecsSinceEpoch(
        tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

bool
PrimeAdapterImpl::work(void)
{
  RawPDU pdu;
  bool processed = false;

  while (this->popPdu(pdu)) {
    prime13_layer_feed_extended(
          layer,
          &pdu.timeStamp,
          pdu.data.data(),
          pdu.data.size());
    processed = true;
  }

  return processed;
}

void
PrimeAdapterImpl::commonInit(PrimeAdapter *owner)
{
  struct prime13_phy_adapter phy = prime13_phy_adapter_INITIALIZER;
  struct prime13_callbacks callbacks = prime13_callbacks_INITIALIZER;

  PH_CHECK_GENERIC(
      pthread_mutex_init(&this->readerMutex, nullptr) != -1,
      "Failed to acquire reader mutex");

  this->readerMutexInitialized = true;

  this->owner = owner;

  callbacks.userdata  = this;
  callbacks.on_meter  = PrimeAdapterImpl::onMeter;
  callbacks.on_data   = PrimeAdapterImpl::onData;
  callbacks.on_frame  = PrimeAdapterImpl::onFrame;
  callbacks.on_subnet = PrimeAdapterImpl::onSubNet;
  callbacks.dispose   = PrimeAdapterImpl::onDispose;
  callbacks.destroy   = PrimeAdapterImpl::onDestroy;

  CONSTRUCT(this->layer, prime13_layer, &phy, &callbacks);

  return;

fail:
  PH_THROW(GENERIC, "Constructor error");
}


PrimeAdapterImpl::PrimeAdapterImpl(
    PrimeAdapter *owner,
    StringParams const &params)
{

  std::string path = params["path"].asString();

  this->commonInit(owner);

  memset(&this->iface, 0, sizeof(spip_iface_t));

  if (path == "")
    path = "/dev/ttyACM0";

  PH_CHECK_GENERIC(
      spip_interface_open_serial(
          &this->iface,
          path.c_str(),
          PRIME_ADAPTER_BAUD_RATE),
      "Cannot open SPIP protocol on " + path);

  this->spipWarmup();

  spip_iface_set_lcd(&this->iface, 0, "PLC Tool 0.1");
  spip_iface_set_lcd(&this->iface, 1, "Capturing...");

  PH_CHECK_GENERIC(pthread_create(
      &this->readerThread,
      nullptr,
      PrimeAdapterImpl::spipReaderThreadFunc,
      this) != -1,
      "Cannot create SPIP reader thread");
  this->readerThreadRunning = true;
}

PrimeAdapterImpl::PrimeAdapterImpl(
    PrimeAdapter *owner,
    QString path)
{
  this->commonInit(owner);

  PH_CHECK_GENERIC(
        fp = fopen(path.toStdString().c_str(), "r"),
        "Failed to open log file: " + std::string(strerror(errno)));

  PH_CHECK_GENERIC(pthread_create(
      &this->readerThread,
      nullptr,
      PrimeAdapterImpl::logFileReaderThreadFunc,
      this) != -1,
      "Cannot create file reader thread");
  this->readerThreadRunning = true;
}

PrimeAdapterImpl::~PrimeAdapterImpl()
{
  if (this->readerThreadRunning) {
    this->halting = true;

    if (this->fp != nullptr)
      fclose(this->fp);

    spip_iface_close(&this->iface);

    pthread_join(this->readerThread, nullptr);
  }

  if (this->readerMutexInitialized)
    pthread_mutex_destroy(&this->readerMutex);

  if (this->layer != nullptr)
    prime13_layer_destroy(this->layer);
}

////////////////////////// PrimeAdapter interface /////////////////////////////
PrimeAdapter::PrimeAdapter(StringParams const &params) :
  QObject(nullptr), Adapter(params)
{

}

PrimeAdapter::PrimeAdapter(QString const &path) :
  QObject(nullptr), Adapter(StringParams())
{
  this->logFilePath = path;
}

PrimeAdapter::PrimeAdapter(QObject *parent, const StringParams &params) :
  QObject(parent), Adapter(params)
{

}

PrimeAdapter::~PrimeAdapter()
{
  this->workTimer.stop();

  if (this->p_impl != nullptr)
    delete this->p_impl;
}

std::string
PrimeAdapter::idToSna(NodeId id)
{
  std::string result = "";
  char byte[8];
  unsigned int i;

  for (i = 0; i < 6; ++i) {
    snprintf(
          byte,
          8,
          "%02x%c",
          (uint8_t) (id >> (5 - i) * 8),
          i != 5
            ? ':'
            : '\0');
    result += byte;
  }

  return result;
}

bool
PrimeAdapter::initialize(void)
{
  try {
    if (this->logFilePath.size() > 0)
      this->p_impl = new PrimeAdapterImpl(this, this->logFilePath);
    else
      this->p_impl = new PrimeAdapterImpl(this, this->getAdapterParams());

    connect(&this->workTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    this->workTimer.start(PLCTOOL_ADAPTER_TIMER_TIMEOUT_MS);
  } catch (PLCTool::Exception &e) {
    e.printInfo();
    return false;
  }
  return true;
}

void
PrimeAdapter::writeFrame(const void *data, size_t size)
{
  this->p_impl->writeFrame(data, size);
}

void
PrimeAdapter::writeFrame(std::vector<uint8_t> const &data)
{
  this->p_impl->writeFrame(data.data(), data.size());
}

void
PrimeAdapter::setLcd(int row, std::string const &text)
{
  this->p_impl->setLcd(row, text);
}

void
PrimeAdapter::setLeds(int leds)
{
  this->p_impl->setLeds(leds);
}

bool
PrimeAdapter::work(void)
{
  bool result = this->p_impl->work();

  if (!result && this->p_impl->isHalted())
    emit closed();

  return result;
}

//////////////////////////////// Slots /////////////////////////////////////////
void
PrimeAdapter::onTimeout(void)
{
  (void) this->work();
}

