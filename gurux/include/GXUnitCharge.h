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

#ifndef CGXUNIT_CHARGE_H
#define CGXUNIT_CHARGE_H

#include "GXChargePerUnitScaling.h"
#include "GXCommodity.h"
#include "GXChargeTable.h"
/**
* Time object. Date part is ignored.
*/
class CGXUnitCharge
{
    /**
    * Charge per unit scaling. <br>
    * Online help:<br>
    * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
    */
    CGXChargePerUnitScaling m_ChargePerUnitScaling;

    /**
     * Commodity.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     */
    CGXCommodity m_Commodity;

    /**
     * Charge tables.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     */
    std::vector<CGXChargeTable> m_ChargeTables;
public:
    // Constructor.
    CGXUnitCharge();

    /**
    * Online help:<br>
    * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
    *
    * @return Charge per unit scaling.
    */
    CGXChargePerUnitScaling& GetChargePerUnitScaling();

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Commodity
     */
    CGXCommodity& GetCommodity();

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Charge tables.
     */
    std::vector<CGXChargeTable> GetChargeTables();

    std::string ToString();

};
#endif //CGXUNIT_CHARGE_H