//
// Adapter.cpp: Interface for all PLC adapters (abstract)
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/Adapter.h>
#include <Topology/Exception.h>
#include <Topology/Concentrator.h>
#include <Topology/Meter.h>
#include <Topology/Switch.h>

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
  for (auto p : this->listenerList)
    p->onNodeUp(node);
}

void
AdapterEventDispatcher::onNodeUpdated(Node *node)
{
  for (auto p : this->listenerList)
    p->onNodeUpdated(node);
}

void
AdapterEventDispatcher::onNodeDown(Node *node)
{
  for (auto p : this->listenerList)
    p->onNodeDown(node);
}

void
AdapterEventDispatcher::onHubAnnounce(Hub *hub)
{
  for (auto p : this->listenerList)
    p->onHubAnnounce(hub);
}

void
AdapterEventDispatcher::onLinkFrame(
    std::vector<uint8_t> const &frame)
{
  for (auto p : this->listenerList)
    p->onLinkFrame(frame);
}

void
AdapterEventDispatcher::onNodeFrame(
    Node *source,
    Node *dest,
    std::vector<uint8_t> const &frame)
{
  for (auto p : this->listenerList)
    p->onNodeFrame(source, dest, frame);
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
Adapter::assertMeter(NodeId dc, NodeId id)
{
  if (this->subNet != nullptr) {
    Node *dcNode = this->nodes()[dc];

    if (dcNode != nullptr) {
      Concentrator *dcCon = static_cast<Concentrator *>(dcNode);
      Node *node = dcCon->subNet()[id];
      Meter *existing = nullptr;

      if (node == nullptr) {
        existing = new Meter(&(dcCon->subNet()), id);
        if (!dcCon->subNet().registerNode(existing)) {
          delete existing;
          return nullptr;
        }

        this->eventDispatcher.onNodeUp(existing);
      } else if (node->type() == METER) {
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
