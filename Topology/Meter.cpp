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

Meter::Meter() : Node(nullptr, METER, 0)
{

}

Meter::Meter(SubNet *subnet, NodeId id) : Node(subnet, METER, id)
{

}
