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

#include "../include/GXDLMSMonitoredValue.h"
#include "../include/GXDLMSConverter.h"

CGXDLMSMonitoredValue::CGXDLMSMonitoredValue()
{
    m_ObjectType = DLMS_OBJECT_TYPE_NONE;
    m_AttributeIndex = 0;
}

void CGXDLMSMonitoredValue::Update(CGXDLMSObject* pObj, int attributeIndex)
{
    m_ObjectType = pObj->GetObjectType();
    pObj->GetLogicalName(m_LogicalName);
    m_AttributeIndex = attributeIndex;
}

DLMS_OBJECT_TYPE CGXDLMSMonitoredValue::GetObjectType()
{
    return m_ObjectType;
}
void CGXDLMSMonitoredValue::SetObjectType(DLMS_OBJECT_TYPE value)
{
    m_ObjectType = value;
}

std::string CGXDLMSMonitoredValue::GetLogicalName()
{
    return m_LogicalName;
}
void CGXDLMSMonitoredValue::SetLogicalName(std::string value)
{
    m_LogicalName = value;
}

int CGXDLMSMonitoredValue::GetAttributeIndex()
{
    return m_AttributeIndex;
}
void CGXDLMSMonitoredValue::SetAttributeIndex(int value)
{
    m_AttributeIndex = value;
}

std::string CGXDLMSMonitoredValue::ToString()
{
    if (m_ObjectType == DLMS_OBJECT_TYPE_NONE)
    {
        return "";
    }
    std::stringstream sb;
    sb << CGXDLMSConverter::ToString(m_ObjectType);
    sb << " ";
    sb << m_LogicalName.c_str();
    sb << " ";
    sb << m_AttributeIndex;
    return sb.str();

}