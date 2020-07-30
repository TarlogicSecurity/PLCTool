//
// Meter.h: Meter-type node
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#ifndef _TOPOLOGY_METER_H
#define _TOPOLOGY_METER_H

#include <string>
#include <cstdint>
#include "Node.h"

namespace PLCTool {
  class Meter : public Node {

  public:
    Meter();
    Meter(SubNet *owner, NodeId id);
  };
}

#endif // _TOPOLOGY_METER_H

