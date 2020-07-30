//
// AdapterEventListener.cpp: Interface for adapter events
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/AdapterEventListener.h>

using namespace PLCTool;

void
AdapterEventListener::setAdapter(Adapter *adapter)
{
  this->adapter = adapter;
}

AdapterEventListener::~AdapterEventListener(void)
{

}
