//
// SubNet.h: Subnetwork object
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
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
