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

#include <PLCTool/Topology/Node.h>

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

  while (parent != nullptr && parent->parent() != nullptr &&
         parent->parent()->type() != CONCENTRATOR)
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
