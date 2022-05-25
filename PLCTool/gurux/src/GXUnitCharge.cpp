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

#include "../include/GXUnitCharge.h"

CGXUnitCharge::CGXUnitCharge()
{
}

CGXChargePerUnitScaling& CGXUnitCharge::GetChargePerUnitScaling() {
    return m_ChargePerUnitScaling;
}

CGXCommodity& CGXUnitCharge::GetCommodity() {
    return m_Commodity;
}

std::vector<CGXChargeTable> CGXUnitCharge::GetChargeTables()
{
    return m_ChargeTables;
}

std::string CGXUnitCharge::ToString()
{
    std::string str = m_ChargePerUnitScaling.ToString();
    str.append(", ");
    str.append(m_Commodity.ToString());
    str.append(", {");
    for (std::vector<CGXChargeTable>::iterator it = m_ChargeTables.begin(); it != m_ChargeTables.end(); ++it)
    {
        str.append(it->ToString());
        str.append(", ");
    }
    str.append("}");
    return str;
}