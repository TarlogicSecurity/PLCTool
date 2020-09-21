//
// Node.cpp: Interface node
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/Node.h>
#include "SubNet.h"

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
  this->iListNdx = -1;
}

int
Node::allocNdx(void) const
{
  return this->iAllocNdx;
}

int
Node::listNdx(void) const
{
  return this->iListNdx;
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
Node::top(void) const
{
  SubNet *parent = this->snParent;

  while (parent != nullptr
         && parent->parent() != nullptr
         && parent->parent()->type() != CONCENTRATOR)
    parent = parent->parent()->parent();

  return parent;
}

Node *
Node::topNode(void) const
{
  SubNet *parent = this->top();

  if (parent != nullptr)
    return parent->parent();

  return nullptr;
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
Node::setMacAddr(std::string const &addr)
{
  this->mMacAddr = addr;
}

void
Node::setAllocNdx(int ndx)
{
  this->iAllocNdx = ndx;
}

void
Node::setListNdx(int ndx)
{
  this->iListNdx = ndx;
}
