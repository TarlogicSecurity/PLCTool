//
// Hub.cpp: Hub-type node
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/Hub.h>

using namespace PLCTool;

Hub::Hub() : Node()
{
  this->hubSubNet = nullptr;
}

Hub::Hub(SubNet *parent, NodeType type, NodeId id): Node(parent, type, id)
{
  this->hubSubNet = new SubNet(this);
}

SubNet &
Hub::subNet(void)
{
  return *this->hubSubNet;
}

Hub::~Hub()
{
  if (this->hubSubNet != nullptr)
    delete this->hubSubNet;
}
