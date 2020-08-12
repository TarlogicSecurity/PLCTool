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
#include <QDateTime>

#define PLCTOOL_ADAPTER_TIMER_TIMEOUT_MS 1000

namespace PLCTool {
  class PrimeAdapterImpl;
  class PrimeAdapter;

  class PrimeAdapter : public QObject, public Adapter {
    Q_OBJECT

    QTimer workTimer;
    QString logFilePath;
    PrimeAdapterImpl *p_impl = nullptr;
    friend class PrimeAdapterImpl;

  public:
    PrimeAdapter(
        QObject *parent,
        StringParams const &params);

    PrimeAdapter(StringParams const &params);
    PrimeAdapter(QString const &path);
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
    void subnetAnnounce(
        PLCTool::Concentrator *,
        QDateTime,
        uint64_t times);
    void meterFound(
        PLCTool::Concentrator *,
        QDateTime,
        PLCTool::Meter *);
    void frameReceived(
        PLCTool::Concentrator *,
        QDateTime,
        bool downlink,
        const void *data,
        size_t size);
    void dataReceived(
        PLCTool::Meter *meter,
        QDateTime,
        bool downlink,
        const void *data,
        size_t size);
    void closed(void);
    void refresh(void);
  };
}

#endif // _PLCHACK_ADAPTERS_PRIMEADAPTER_H
