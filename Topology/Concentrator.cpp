//
// Concentrator.cpp: Data concentrator
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/Concentrator.h>
#include <Topology/Switch.h>
#include <Topology/Meter.h>

using namespace PLCTool;

Concentrator::Concentrator() : Hub(nullptr, CONCENTRATOR, 0)
{

}

Concentrator::Concentrator(SubNet *subnet, NodeId id) :
    Hub(subnet, CONCENTRATOR, id)
{

}

Concentrator::~Concentrator()
{

}

SubNet *
Concentrator::getSubNetFor(SubNetId sid)
{
  Hub *hub = this->assertSwitch(sid);

  if (hub == nullptr)
    hub = this;

  return &hub->subNet();
}

void
Concentrator::promote(Meter *meter, SubNetId sid)
{
  Hub *prev = nullptr;

  if (this->switchTable.find(sid) != this->switchTable.end())
    prev = this->switchTable[sid];

  if (meter != prev) {
    if (meter->type() == SWITCH)
      this->switchTable[meter->subNet().netId()] = nullptr;
    else
      meter->promote();

    meter->subNet().setNetId(sid);

    if (prev != nullptr) {
      for (size_t i = 0; i < prev->subNet().length(); ++i) {
        if (prev->subNet().nodeAt(i) != nullptr)
          meter->subNet().moveHere(prev->subNet().nodeAt(i));
      }

      if (prev->type() == SWITCH) {
        Meter *asMeter = static_cast<Meter *>(prev);
        asMeter->demote();
      } else if (prev->type() == HUB) {
        this->deleteDummyHub(prev);
      } else {
        fprintf(stderr, "WARNING! INCONSISTENCY!\n");
      }
    }

    this->switchTable[sid] = meter;
  }
}

Hub *
Concentrator::allocateDummyHub(SubNetId id)
{
  Hub *hub = new Hub(nullptr, HUB, static_cast<NodeId>(id) << 32);

  if (!this->subNet().registerNode(hub)) {
    delete hub;
    return nullptr;
  }

  hub->subNet().setNetId(id);
  this->dummyHubList.push_back(hub);

  return hub;
}

bool
Concentrator::deleteDummyHub(Hub *hub)
{
  if (!this->subNet().remove(hub))
    return false;

  this->dummyHubList.remove(hub);

  return true;
}

Hub *
Concentrator::assertSwitch(SubNetId sid)
{
  if (this->switchTable.find(sid) == this->switchTable.end())
    this->switchTable[sid] = this->allocateDummyHub(sid);

  return this->switchTable[sid];
}
