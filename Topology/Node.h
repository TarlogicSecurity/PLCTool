//
// Node.h: Network node
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
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
    CONCENTRATOR
  };

  class Node {
    NodeType eType;
    NodeId niId;
    int iAllocNdx;
    std::string nodeName;
    SubNet *snParent;
    StringParams sParams;
    friend class SubNet;

    void setParent(SubNet *parent);
    void setAllocNdx(int);

  protected:
    Node(SubNet *parent, NodeType, NodeId id);

  public:
    Node();

    int allocNdx(void) const;

    // Getters first
    NodeType type(void) const;
    std::string const &name(void) const;
    NodeId id(void) const;
    SubNet *parent(void) const;

    // Setters
    void setName(std::string const &);

    // String Parameters visibility
    StringParams &
    params(void)
    {
      return this->sParams;
    }

    bool
    isHub(void) const
    {
      return this->eType == SWITCH || this->eType == CONCENTRATOR;
    }
  };
}

#endif // _TOPOLOGY_NODE_H

