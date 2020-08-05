//
// PrimeAdapter.cpp: PRIME 1.3 Adapter
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

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

#define PRIME_ADAPTER_BAUD_RATE 921600

using namespace PLCTool;

namespace PLCTool {
  typedef std::list<struct spip_pdu *>SPIPPduList;

  class PrimeAdapterImpl {
    spip_iface_t iface;
    SPIPPduList pduList;
    PrimeAdapter *owner = nullptr;
    prime13_layer_t *layer = nullptr;

    pthread_t readerThread;
    bool readerThreadRunning = false;

    pthread_mutex_t readerMutex;
    bool readerMutexInitialized = false;

    bool blinkState = 0;
    bool halting = false;

    void lock(void);
    void unlock(void);

    struct spip_pdu *popPdu(void);

  public:
    PrimeAdapterImpl(PrimeAdapter *owner, StringParams const &params);
    void warmup(void);
    bool work(void);

    void writeFrame(const void *data, size_t size);
    void setLcd(int row, std::string const &text);
    void setLeds(int leds);

    ~PrimeAdapterImpl();

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

    static void *readerThreadFunc(void *);
  };
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
PrimeAdapterImpl::readerThreadFunc(void *userdata)
{
  PrimeAdapterImpl *self = static_cast<PrimeAdapterImpl *>(userdata);
  struct spip_pdu *pdu = nullptr;
  struct spip_pdu *copy;

  while (!self->halting && spip_iface_read(&self->iface, &pdu)) {
    if ((copy = spip_pdu_dup(pdu)) != nullptr) {
      self->lock();
      self->pduList.push_back(copy);
      self->unlock();
    }

    spip_iface_dispose(&self->iface, pdu);
  }

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
PrimeAdapterImpl::warmup(void)
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

  emit impl->owner->meterFound(dc, meter);

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
    emit impl->owner->subnetAnnounce(dc, times);

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

  emit impl->owner->frameReceived(dc, direction != FALSE, data, size);

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

  emit impl->owner->dataReceived(meter, downstream != FALSE, data, size);

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

struct spip_pdu *
PrimeAdapterImpl::popPdu(void)
{
  struct spip_pdu *pdu = nullptr;

  this->lock();

  if (!this->pduList.empty()) {
    pdu = this->pduList.front();
    this->pduList.pop_front();
  }

  this->unlock();
  return pdu;
}

bool
PrimeAdapterImpl::work(void)
{
  struct spip_pdu *pdu = NULL;
  bool processed = false;

  while ((pdu = this->popPdu()) != nullptr) {
    if (pdu->command == SPIP_COMMAND_FRAME) {
      prime13_layer_feed(layer, pdu->data, pdu->size);
      processed = true;
    }

    free(pdu);
  }

  return processed;
}

PrimeAdapterImpl::PrimeAdapterImpl(
    PrimeAdapter *owner,
    StringParams const &params)
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

  std::string path = params["path"].asString();

  memset(&this->iface, 0, sizeof(spip_iface_t));

  if (path == "")
    path = "/dev/ttyACM0";

  PH_CHECK_GENERIC(
      spip_interface_open_serial(
          &this->iface,
          path.c_str(),
          PRIME_ADAPTER_BAUD_RATE),
      "Cannot open SPIP protocol on " + path);

  this->warmup();

  spip_iface_set_lcd(&this->iface, 0, "PLC Tool 0.1");
  spip_iface_set_lcd(&this->iface, 1, "Detecting SNA...");

  CONSTRUCT(this->layer, prime13_layer, &phy, &callbacks);

  PH_CHECK_GENERIC(pthread_create(
      &this->readerThread,
      nullptr,
      PrimeAdapterImpl::readerThreadFunc,
      this) != -1,
      "Cannot create blinker thread");
  this->readerThreadRunning = true;

  return;

fail:
  PH_THROW(GENERIC, "Constructor error");
}

PrimeAdapterImpl::~PrimeAdapterImpl()
{
  if (this->readerThreadRunning) {
    struct spip_pdu *pdu = nullptr;
    this->halting = true;

    spip_iface_close(&this->iface);

    pthread_join(this->readerThread, nullptr);

    while ((pdu = this->popPdu()) != nullptr)
      free(pdu);
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

PrimeAdapter::PrimeAdapter(QObject *parent, const StringParams &params) :
  QObject(parent), Adapter(params)
{

}

PrimeAdapter::~PrimeAdapter()
{
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
  return this->p_impl->work();
}

//////////////////////////////// Slots /////////////////////////////////////////
void
PrimeAdapter::onTimeout(void)
{
  (void) this->work();
}

