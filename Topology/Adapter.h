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

#ifndef _TOPOLOGY_ADAPTER_H
#define _TOPOLOGY_ADAPTER_H

#include <map>
#include <vector>
#include <algorithm>
#include "SubNet.h"
#include "Hub.h"
#include "AdapterEventListener.h"
#include "StringParams.h"
#include "Worker.h"

namespace PLCTool {
  class AdapterEventListener;
  class AdapterFactory;
  class Concentrator;
  class Meter;
  class Switch;


  class AdapterEventDispatcher : public AdapterEventListener {
    std::vector<AdapterEventListener *> listenerList;

  public:
    void addListener(AdapterEventListener *);

    virtual void onNodeUp(Node *) override;
    virtual void onNodeUpdated(Node *) override;
    virtual void onNodeDown(Node *) override;
    virtual void onHubAnnounce(Hub *) override;
    virtual void onLinkFrame(std::vector<uint8_t> const &) override;
    virtual void onNodeFrame(Node *, Node *, std::vector<uint8_t> const &) override;
  };

  class Adapter {
    Worker worker;
    StringParams params;
    AdapterEventDispatcher eventDispatcher;
    SubNet *subNet;

  protected:
    StringParams &
    getAdapterParams(void)
    {
      return this->params;
    }

  public:
    Adapter(StringParams const &params);
    virtual ~Adapter();
    virtual bool initialize(void) = 0;
    virtual bool work(void) = 0;

    Concentrator *assertConcentrator(NodeId);
    Concentrator *assertConcentrator(std::string const &);
    Meter *assertMeter(NodeId, NodeId, NodeId);
    Switch *assertSwitch(NodeId, NodeId);

    void addListener(AdapterEventListener *);

    SubNet *
    takeSubNet(void)
    {
      SubNet *result = this->subNet;

      this->subNet = nullptr;

      return result;
    }

    SubNet &
    nodes(void)
    {
      return *this->subNet;
    }
  };
}

#endif // _TOPOLOGY_ADAPTER_H
