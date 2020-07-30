//
// Hub.h: Hub-type node
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#ifndef _TOPOLOGY_HUB_H
#define _TOPOLOGY_HUB_H

#include <string>
#include <cstdint>
#include "SubNet.h"
#include "Node.h"

namespace PLCTool {
  class SubNet;

  class Hub: public Node {
    SubNet *hubSubNet = nullptr;

  public:
    Hub();
    Hub(SubNet *parent, NodeType, NodeId id);
    ~Hub();

    SubNet &subNet(void);
  };
}

#endif // _TOPOLOGY_HUB_H

