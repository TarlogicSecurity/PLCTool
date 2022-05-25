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

#ifndef _TOPOLOGY_NODE_H
#define _TOPOLOGY_NODE_H

#include <string>
#include <cstdint>
#include "StringParams.h"

namespace PLCTool {
  typedef uint64_t NodeId;
  typedef uint64_t ConnId;

  class SubNet;

  enum NodeType {
    UNDEFINED,
    METER,
    SWITCH,
    CONCENTRATOR,
    HUB
  };

  class Node {
    NodeId niId;
    int iAllocNdx;
    int iListNdx;
    std::string nodeName;
    std::string mMacAddr;
    SubNet *snParent;
    StringParams sParams;
    friend class SubNet;

    void setParent(SubNet *parent);
    void setAllocNdx(int);
    void setListNdx(int);

  protected:
    NodeType eType;
    Node(SubNet *parent, NodeType, NodeId id);

  public:
    Node();

    int allocNdx(void) const;
    int listNdx(void) const;

    // Getters first
    NodeType type(void) const;
    std::string const &name(void) const;
    NodeId id(void) const;
    SubNet *top(void) const;
    Node *topNode(void) const;
    SubNet *parent(void) const;

    // Setters
    void setName(std::string const &);
    void setMacAddr(std::string const &);

    // String Parameters visibility
    StringParams &
    params(void)
    {
      return this->sParams;
    }

    bool
    isHub(void) const
    {
      return this->eType == SWITCH
           || this->eType == CONCENTRATOR
           || this->eType == HUB;
    }

    std::string
    macAddr(void) const
    {
      return this->mMacAddr;
    }
  };
}

#endif // _TOPOLOGY_NODE_H

