//
// PrimeAdapter.h: Adapter for PRIME networks
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#ifndef _PLCHACK_ADAPTERS_PRIMEADAPTER_H
#define _PLCHACK_ADAPTERS_PRIMEADAPTER_H

#include <Topology/Adapter.h>
#include <QObject>
#include <QTimer>

#define PLCTOOL_ADAPTER_TIMER_TIMEOUT_MS 100

namespace PLCTool {
  class PrimeAdapterImpl;
  class PrimeAdapter;

  class PrimeAdapter : public QObject, public Adapter {
    Q_OBJECT

    QTimer workTimer;
    PrimeAdapterImpl *p_impl = nullptr;
    friend class PrimeAdapterImpl;

  public:
    PrimeAdapter(
        QObject *parent,
        StringParams const &params);

    PrimeAdapter(StringParams const &params);
    ~PrimeAdapter();
    virtual bool initialize(void) override;
    virtual bool work(void) override;

    void writeFrame(const void *data, size_t size);
    void writeFrame(std::vector<uint8_t> const &data);
    void setLcd(int row, std::string const &text);
    void setLeds(int leds);

    static std::string idToSna(NodeId id);

  public slots:
    void onTimeout(void);

  signals:
    void subnetAnnounce(PLCTool::Concentrator *, uint64_t times);
    void meterFound(PLCTool::Concentrator *, PLCTool::Meter *);
    void frameReceived(
        PLCTool::Concentrator *,
        bool downlink,
        const void *data,
        size_t size);
    void dataReceived(
        PLCTool::Meter *meter,
        bool downlink,
        const void *data,
        size_t size);
  };
}

#endif // _PLCHACK_ADAPTERS_PRIMEADAPTER_H
