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

#ifndef GXCURRENCY_H
#define GXCURRENCY_H

#include "enums.h"
#include <string>

/**
 * Used currency.<br>
 * Online help:<br>
 * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCurrency
 */
class CGXCurrency
{
    /**
    * Currency name.
    */
    std::string m_Name;
    /**
     * Currency scale.
     */
    char m_Scale;
    /**
     * Currency unit.
     */
    DLMS_CURRENCY m_Unit;

public:
    // Constructor.
    CGXCurrency();

    /**
     * @return Currency name.
     */
    std::string& GetName();

    /**
     * @param value
     *            Currency name.
     */
    void SetName(std::string& value);

    /**
     * @return Currency scale.
     */
    char GetScale();

    /**
     * @param value
     *            Currency scale.
     */
    void SetScale(char value);

    /**
     * @return Currency unit.
     */
    DLMS_CURRENCY GetUnit();

    /**
     * @param value
     *            Currency unit.
     */
    void SetUnit(DLMS_CURRENCY value);

    std::string ToString();
};
#endif //GXCURRENCY_H