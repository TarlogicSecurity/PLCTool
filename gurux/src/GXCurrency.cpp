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

#include "../include/GXCurrency.h"
#include "../include/GXHelpers.h"

// Constructor.
CGXCurrency::CGXCurrency()
{
    m_Scale = 0;
    m_Unit = DLMS_CURRENCY_TIME;
};

/**
 * @return Currency name.
 */
std::string& CGXCurrency::GetName()
{
    return m_Name;
}

/**
 * @param value
 *            Currency name.
 */
void CGXCurrency::SetName(std::string& value)
{
    m_Name = value;
}

/**
 * @return Currency scale.
 */
char CGXCurrency::GetScale()
{
    return m_Scale;
}

/**
 * @param value
 *            Currency scale.
 */
void CGXCurrency::SetScale(char value)
{
    m_Scale = value;
}

/**
 * @return Currency unit.
 */
DLMS_CURRENCY CGXCurrency::GetUnit()
{
    return m_Unit;
}

/**
 * @param value
 *            Currency unit.
 */
void CGXCurrency::SetUnit(DLMS_CURRENCY value)
{
    m_Unit = value;
}

std::string CGXCurrency::ToString()
{
    std::string str = m_Name;
    str.append(", ");
    str.append(GXHelpers::IntToString(m_Scale));
    str.append(", ");
    str.append(GXHelpers::IntToString(m_Unit));
    return str;
}