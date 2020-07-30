//
// AdapterEventListener.h: Interface for adapter events
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#ifndef _TOPOLOGY_ADAPTEREVENTLISTENER_H
#define _TOPOLOGY_ADAPTEREVENTLISTENER_H

#include <map>
#include "SubNet.h"
#include "Hub.h"

namespace PLCTool {
  class Adapter;

  class AdapterEventListener {
    Adapter *adapter;

  public:
    virtual ~AdapterEventListener();

    void setAdapter(Adapter *);
    virtual void onNodeUp(Node *) = 0;
    virtual void onNodeUpdated(Node *) = 0;
    virtual void onNodeDown(Node *) = 0;
    virtual void onHubAnnounce(Hub *) = 0;
    virtual void onLinkFrame(std::vector<uint8_t> const &) = 0;
    virtual void onNodeFrame(Node *, Node *, std::vector<uint8_t> const &) = 0;
  };
}

#endif // _TOPOLOGY_ADAPTEREVENTLISTENER_H
