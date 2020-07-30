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

#ifndef CGXCOMMODITY_H
#define CGXCOMMODITY_H

#include "enums.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
*/
class CGXCommodity
{
    DLMS_OBJECT_TYPE m_Type;
    unsigned char m_LogicalName[6];
    unsigned char m_Index;
public:
    // Constructor.
    CGXCommodity();

    /**
   * @return Object type.
   */
    DLMS_OBJECT_TYPE GetType();

    /**
     * @param value
     *            Object type.
     */
    void SetType(DLMS_OBJECT_TYPE value);

    /**
   * @return Attribute index.
   */
    unsigned char* GetLogicalName();

    /**
     * @param value
     *            Attribute index.
     */
    void SetLogicalName(unsigned char* value);

    /**
    * @return Attribute index.
    */
    unsigned char GetIndex();

    /**
     * @param value
     *            Attribute index.
     */
    void SetIndex(unsigned char value);

    std::string ToString();
};
#endif //CGXCOMMODITY_H