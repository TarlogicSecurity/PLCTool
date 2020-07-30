//
// --------------------------------------------------------------------------
//  Gurux Ltd
//
//
//
// Filename:        $HeadURL$
//
// Version:         $Revision$,
//                  $Date$
//                  $Author$
//
// Copyright (c) Gurux Ltd
//
//---------------------------------------------------------------------------
//
//  DESCRIPTION
//
// This file is a part of Gurux Device Framework.
//
// Gurux Device Framework is Open Source software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#ifndef GXCHARGE_PER_UNIT_SCALING_H
#define GXCHARGE_PER_UNIT_SCALING_H

#include <string>

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
*/
class CGXChargePerUnitScaling
{
    signed char m_CommodityScale;
    signed char m_PriceScale;
public:
    // Constructor.
    CGXChargePerUnitScaling();

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Commodity scale.
     */
    signed char GetCommodityScale();

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Commodity scale.
     */
    void SetCommodityScale(signed char value);

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Price scale.
     */
    signed char GetPriceScale();

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Price scale.
     */
    void SetPriceScale(signed char value);

    std::string ToString();
};
#endif //GXCHARGE_PER_UNIT_SCALING_H
