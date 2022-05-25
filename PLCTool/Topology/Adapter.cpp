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

#include <PLCTool/Topology/Adapter.h>
#include <PLCTool/Topology/Concentrator.h>
#include <PLCTool/Topology/Exception.h>
#include <PLCTool/Topology/Meter.h>
#include <PLCTool/Topology/Switch.h>

#include <iostream>
#include <string>

using namespace PLCTool;

/////////////////////////// AdapterEventDispatcher /////////////////////////////
void
AdapterEventDispatcher::addListener(AdapterEventListener *listener)
{
  this->listenerList.push_back(listener);
}

void
AdapterEventDispatcher::onNodeUp(Node *node)
{
  for (auto p : this->listenerList) p->onNodeUp(node);
}

void
AdapterEventDispatcher::onNodeUpdated(Node *node)
{
  for (auto p : this->listenerList) p->onNodeUpdated(node);
}

void
AdapterEventDispatcher::onNodeDown(Node *node)
{
  for (auto p : this->listenerList) p->onNodeDown(node);
}

void
AdapterEventDispatcher::onHubAnnounce(Hub *hub)
{
  for (auto p : this->listenerList) p->onHubAnnounce(hub);
}

void
AdapterEventDispatcher::onLinkFrame(std::vector<uint8_t> const &frame)
{
  for (auto p : this->listenerList) p->onLinkFrame(frame);
}

void
AdapterEventDispatcher::onNodeFrame(
    Node *source,
    Node *dest,
    std::vector<uint8_t> const &frame)
{
  for (auto p : this->listenerList) p->onNodeFrame(source, dest, frame);
}

///////////////////////////////// Adapter //////////////////////////////////////
Concentrator *
Adapter::assertConcentrator(NodeId id)
{
  if (this->subNet != nullptr) {
    Node *existing = this->nodes()[id];

    if (existing == nullptr) {
      existing = new Concentrator(this->subNet, id);
      if (!this->subNet->registerNode(existing)) {
        delete existing;
        return nullptr;
      }

      this->eventDispatcher.onNodeUp(existing);
    } else if (existing->type() != CONCENTRATOR) {
      return nullptr;
    }

    return static_cast<Concentrator *>(existing);
  }

  return nullptr;
}

Concentrator *
Adapter::assertConcentrator(std::string const &string)
{
  unsigned int i;
  NodeId dc = 0;
  unsigned int entries[6];

  if (sscanf(
          string.c_str(),
          "%02x:%02x:%02x:%02x:%02x:%02x",
          entries + 0,
          entries + 1,
          entries + 2,
          entries + 3,
          entries + 4,
          entries + 5) != 6)
    return nullptr;

  for (i = 0; i < 6; ++i)
    dc |= (NodeId) ((uint8_t) entries[i]) << 8ull * (5 - i);

  return this->assertConcentrator(dc);
}

Meter *
Adapter::assertMeter(NodeId dc, NodeId sid, NodeId id)
{
  if (this->subNet != nullptr) {
    Node *dcNode = this->nodes()[dc];

    if (dcNode != nullptr) {
      Concentrator *dcCon = static_cast<Concentrator *>(dcNode);
#ifdef PLCTOOL_PROMOTE_METERS
      SubNet *sn = dcCon->getSubNetFor(sid);
#else
      SubNet *sn = &dcCon->subNet();
      (void) sid;
#endif  // PLCTOOL_PROMOTE_METERS
      Node *node = (*sn)[id];
      Meter *existing = nullptr;

      if (node == nullptr) {
        existing = new Meter(nullptr, id);
        if (!(*sn).registerNode(existing)) {
          delete existing;
          return nullptr;
        }

        this->eventDispatcher.onNodeUp(existing);
      } else if (node->type() == METER || node->type() == SWITCH) {
        existing = static_cast<Meter *>(node);
      }

      return existing;
    }
  }

  return nullptr;
}

Switch *
Adapter::assertSwitch(NodeId dc, NodeId id)
{
  if (this->subNet != nullptr) {
    Node *dcNode = this->nodes()[dc];

    if (dcNode != nullptr) {
      Concentrator *dcCon = static_cast<Concentrator *>(dcNode);
      Node *node = dcCon->subNet()[id];
      Switch *existing = nullptr;

      if (node == nullptr) {
        existing = new Switch(&(dcCon->subNet()), id);
        if (!dcCon->subNet().registerNode(existing)) {
          delete existing;
          return nullptr;
        }

        this->eventDispatcher.onNodeUp(existing);
      } else if (node->type() == METER) {
        existing = static_cast<Switch *>(node);
      }

      return existing;
    }
  }

  return nullptr;
}

void
Adapter::addListener(AdapterEventListener *listener)
{
  this->eventDispatcher.addListener(listener);
}

Adapter::Adapter(StringParams const &params)
{
  this->params = params;
  this->subNet = new SubNet();
}

Adapter::~Adapter(void)
{
  if (this->subNet != nullptr)
    delete this->subNet;
}
