//
// Meter.cpp: Meter type node
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/Meter.h>

using namespace PLCTool;

Meter::Meter() : Switch(nullptr, 0)
{
  this->demote();
}

Meter::Meter(SubNet *subnet, NodeId id) : Switch(subnet, id)
{
  this->demote();
}

void
Meter::promote(void)
{
  this->eType = SWITCH;
}

void
Meter::demote(void)
{
  this->eType = METER;
}
