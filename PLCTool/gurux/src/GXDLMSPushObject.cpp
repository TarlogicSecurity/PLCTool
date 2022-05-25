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

#include "../include/GXDLMSPushObject.h"
#include <sstream>

DLMS_OBJECT_TYPE CGXDLMSPushObject::GetType()
{
    return m_Type;
}
void CGXDLMSPushObject::SetType(DLMS_OBJECT_TYPE value)
{
    m_Type = value;
}

std::string CGXDLMSPushObject::GetLogicalName()
{
    return m_LogicalName;
}
void CGXDLMSPushObject::SetLogicalName(std::string value)
{
    m_LogicalName = value;
}


int CGXDLMSPushObject::GetAttributeIndex()
{
    return m_AttributeIndex;
}
void CGXDLMSPushObject::SetAttributeIndex(int value)
{
    m_AttributeIndex = value;
}

int CGXDLMSPushObject::GetDataIndex()
{
    return m_DataIndex;
}
void CGXDLMSPushObject::SetDataIndex(int value)
{
    m_DataIndex = value;
}

std::string CGXDLMSPushObject::ToString()
{
    std::stringstream sb;
    sb << m_Type;
    sb << " ";
    sb << m_LogicalName.c_str();
    sb << " ";
    sb << m_AttributeIndex;
    sb << " ";
    sb << m_DataIndex;
    return sb.str();
}