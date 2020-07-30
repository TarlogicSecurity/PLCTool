//
// Hub.h: Hub-type node
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#ifndef _TOPOLOGY_SWITCH_H
#define _TOPOLOGY_SWITCH_H

#include "Hub.h"

namespace PLCTool {
  class Switch : public Hub {
  public:
    Switch();
    Switch(SubNet *, NodeId id);
  };
}

#endif // _TOPOLOGY_SWITCH_H

