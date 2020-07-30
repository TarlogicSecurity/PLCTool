//
// Concentrator.cpp: Data concentrator
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/Concentrator.h>

using namespace PLCTool;

Concentrator::Concentrator() : Hub(nullptr, CONCENTRATOR, 0)
{

}

Concentrator::Concentrator(SubNet *subnet, NodeId id) :
    Hub(subnet, CONCENTRATOR, id)
{

}
