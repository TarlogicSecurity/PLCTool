//
// Adapter.h: Interface for all PLC adapters (abstract)
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
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
    Meter *assertMeter(NodeId, NodeId);
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
