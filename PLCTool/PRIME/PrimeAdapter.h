//
// Copyright (c) 2020, Tarlogic Security SL
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of copyright holders nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef _PLCHACK_ADAPTERS_PRIMEADAPTER_H
#define _PLCHACK_ADAPTERS_PRIMEADAPTER_H

#include <PLCTool/Topology/Adapter.h>

#include <QDateTime>
#include <QObject>
#include <QTimer>

#define PLCTOOL_ADAPTER_TIMER_TIMEOUT_MS 1000

namespace PLCTool
{
  class PrimeAdapterImpl;
  class PrimeAdapter;

  class PrimeAdapter : public QObject, public Adapter
  {
    Q_OBJECT

    QTimer workTimer;
    QString logFilePath;
    PrimeAdapterImpl *p_impl = nullptr;
    friend class PrimeAdapterImpl;

   public:
    PrimeAdapter(QObject *parent, StringParams const &params);

    PrimeAdapter(StringParams const &params);
    PrimeAdapter(QString const &path);
    ~PrimeAdapter();
    virtual bool initialize(void) override;
    virtual bool work(void) override;

    void writeFrame(const void *data, size_t size);
    void writeFrame(std::vector<uint8_t> const &data);
    void setLcd(int row, std::string const &text);
    void setLeds(int leds);
    unsigned int totalFrameCount(void) const;
    unsigned int parsedFrameCount(void) const;

    static std::string idToSna(NodeId id);

   public slots:
    void onTimeout(void);

   signals:
    void subnetAnnounce(PLCTool::Concentrator *, QDateTime, uint64_t times);
    void meterFound(PLCTool::Concentrator *, QDateTime, PLCTool::Meter *);
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
}  // namespace PLCTool

#endif  // _PLCHACK_ADAPTERS_PRIMEADAPTER_H
