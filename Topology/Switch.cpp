//
// Switch.cpp: Switch node
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/Switch.h>

using namespace PLCTool;

Switch::Switch() : Hub(nullptr, SWITCH, 0)
{

}

Switch::Switch(SubNet *subnet, NodeId id) :
    Hub(subnet, SWITCH, id)
{

}
