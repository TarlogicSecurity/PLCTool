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

#include "../include/GXCommodity.h"
#include "../include/GXHelpers.h"

CGXCommodity::CGXCommodity()
{
    m_Type = DLMS_OBJECT_TYPE_NONE;
    memset(m_LogicalName, 0, 6);
    m_Index = 0;
}

DLMS_OBJECT_TYPE CGXCommodity::GetType()
{
    return m_Type;
}

void CGXCommodity::SetType(DLMS_OBJECT_TYPE value)
{
    m_Type = value;
}

unsigned char* CGXCommodity::GetLogicalName()
{
    return m_LogicalName;
}

void CGXCommodity::SetLogicalName(unsigned char* value)
{
    memcpy(m_LogicalName, value, 6);
}

unsigned char CGXCommodity::GetIndex()
{
    return m_Index;
}

void CGXCommodity::SetIndex(unsigned char value)
{
    m_Index = value;
}

std::string CGXCommodity::ToString()
{
    std::string ln;
    std::string str = GXHelpers::IntToString(m_Type);
    str.append(" ");
    GXHelpers::GetLogicalName(m_LogicalName, ln);
    str.append(ln);
    str.append(" ");
    str = GXHelpers::IntToString(m_Index);
    return str;
}