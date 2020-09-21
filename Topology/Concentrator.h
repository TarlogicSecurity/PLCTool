//
// Hub.h: Hub-type node
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#ifndef _TOPOLOGY_CONCENTRATOR_H
#define _TOPOLOGY_CONCENTRATOR_H

#include "Hub.h"
#include <map>
#include <list>

namespace PLCTool {
  class Switch;
  class Meter;
  class Concentrator : public Hub {

    std::list<Hub *> dummyHubList;
    std::map<SubNetId, Hub *> switchTable;

    Hub *allocateDummyHub(SubNetId);
    bool deleteDummyHub(Hub *);

  public:
    Concentrator();
    Concentrator(SubNet *, NodeId id);
    ~Concentrator();

    SubNet *getSubNetFor(SubNetId sid);
    void promote(Meter *, SubNetId);
    Hub *assertSwitch(SubNetId);
  };
}

#endif // _TOPOLOGY_CONCENTRATOR_H

