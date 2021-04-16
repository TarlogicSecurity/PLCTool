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
