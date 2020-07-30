//
// SubNet.cpp: Subnetwork object
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/SubNet.h>
#include <Topology/Exception.h>

using namespace PLCTool;

//
// TODO: Iplement function to traverse allocation in search for orphan
// nodes that must be cleared.
//

SubNet::SubNet(Node *parent)
{
  this->parentNode = parent;
}

Node *
SubNet::parent(void) const
{
  return this->parentNode;
}

Node *
SubNet::operator[] (std::string const &key)
{
  std::map<std::string, Node *>::iterator it;

  it = this->nameMap.find(key);
  if (it == this->nameMap.end())
    return nullptr;

  return it->second;
}

Node *
SubNet::operator[] (NodeId id)
{
  std::map<NodeId, Node *>::iterator it;

  it = this->idMap.find(id);
  if (it == this->idMap.end())
    return nullptr;

  return it->second;
}

size_t
SubNet::length(void) const
{
  return this->allocation.size();
}

Node *
SubNet::nodeAt(size_t index) const
{
  if (index >= this->allocation.size())
    return nullptr;

  return this->allocation[index];
}

bool
SubNet::remove(Node *node)
{
  PH_CHECK_PARAM(node->parent() == this, "Parent subnetwork for node");
  PH_CHECK_PARAM(
      node->allocNdx() >= 0
        || static_cast<unsigned>(node->allocNdx()) < this->allocation.size(),
      "Allocation index");
  PH_ASSERT(this->allocation[node->allocNdx()] == node);

  (void) this->idMap.erase(node->id());

  if (node->name().length() > 0)
    (void) this->nameMap.erase(node->name());

  this->allocation[node->allocNdx()] = nullptr;
  this->lastFree = node->allocNdx();

  delete node;

  return true;
}

bool
SubNet::remove(std::string const &key)
{
  Node *entry = (*this)[key];

  if (entry == nullptr)
      return false;

  return this->remove(entry);
}

bool
SubNet::remove(NodeId id)
{
  Node *entry = (*this)[id];

  if (entry == nullptr)
      return false;

  return this->remove(entry);
}

bool
SubNet::contains(std::string const &key) const
{
  return this->nameMap.find(key) != this->nameMap.end();
}

bool
SubNet::contains(NodeId id) const
{
  return this->idMap.find(id) != this->idMap.end();
}

bool
SubNet::registerNode(Node *node)
{
  Node *existing;

  PH_CHECK_PARAM(node != nullptr, "Pointer to node object");

  existing = (*this)[node->id()];

  PH_CHECK_PARAM(
      existing == nullptr || node == existing,
      "Uniquely-registered node");

  if (existing == nullptr) {
    int where;
    if (this->lastFree != -1) {
      where = this->lastFree;
      this->allocation[where] = node;
      this->lastFree = -1;
    } else {
      where = this->allocation.size();
      this->allocation.push_back(node);
    }

    node->setAllocNdx(where);
    node->setParent(this);
    this->idMap.emplace(node->id(), node);
  }

  if (node->name().size() > 0)
    this->nameMap.emplace(node->name(), node);

  return true;
}
