//
// Node.cpp: Interface node
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/Node.h>

using namespace PLCTool;

Node::Node() : Node(nullptr, UNDEFINED, 0)
{

}

Node::Node(SubNet *parent, NodeType type, NodeId id)
{
  this->snParent = parent;
  this->eType = type;
  this->niId = id;
  this->iAllocNdx = -1;
}

int
Node::allocNdx(void) const
{
  return this->iAllocNdx;
}

NodeType
Node::type(void) const
{
  return this->eType;
}

std::string const &
Node::name(void) const
{
  return this->nodeName;
}

NodeId
Node::id(void) const
{
  return this->niId;
}

SubNet *
Node::parent(void) const
{
  return this->snParent;
}

void
Node::setParent(SubNet *net)
{
  this->snParent = net;
}

void
Node::setName(std::string const &name)
{
  this->nodeName = name;
}

void
Node::setAllocNdx(int ndx)
{
  this->iAllocNdx = ndx;
}
