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

#ifndef _TOPOLOGY_SUBNET_H
#define _TOPOLOGY_SUBNET_H

#include <vector>
#include <map>

#include "Node.h"

namespace PLCTool {
  typedef std::vector<Node *> NodeVector;
  typedef uint32_t SubNetId;
  class SubNet {
    Node *parentNode = nullptr;
    NodeVector allocation;
    SubNetId id = 0;
    std::map<NodeId, Node *> idMap;
    std::map<std::string, Node *> nameMap;
    std::vector<Node *> nodeList;

    int lastFree = -1; // Slight speedup

  public:
      SubNet(Node *parent = nullptr);

      Node *parent(void) const;

      size_t length(void) const;

      void setNetId(SubNetId);
      SubNetId netId(void) const;

      Node *operator[] (std::string const &);
      Node *operator[] (NodeId);
      Node *nodeAt(size_t) const;

      void rebuildList(void);
      size_t listCount(void) const;
      Node *listNodeAt(size_t) const;

      bool disown(Node *);
      bool remove(Node *);
      bool remove(std::string const &);
      bool remove(NodeId);

      bool contains(std::string const &) const;
      bool contains(NodeId) const;

      bool registerNode(Node *);

      bool moveHere(Node *);

      NodeVector::const_iterator
      begin(void) const
      {
        return this->allocation.cbegin();
      }

      NodeVector::const_iterator
      end(void) const
      {
        return this->allocation.cend();
      }
  };
}

#endif // _TOPOLOGY_SUBNET_H
