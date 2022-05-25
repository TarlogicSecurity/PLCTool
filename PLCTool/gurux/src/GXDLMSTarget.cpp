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

#include "../include/GXDLMSTarget.h"

CGXDLMSTarget::CGXDLMSTarget() : m_Target(NULL), m_AttributeIndex(0), m_DataIndex(0)
{
};

CGXDLMSTarget::~CGXDLMSTarget()
{
    Clear();
};

void CGXDLMSTarget::Clear()
{
    m_Target = NULL;
    m_AttributeIndex = 0;
    m_DataIndex = 0;
}

CGXDLMSObject* CGXDLMSTarget::GetTarget()
{
    return m_Target;
}

void CGXDLMSTarget::SetTarget(CGXDLMSObject* value)
{
    m_Target = value;
}

unsigned char CGXDLMSTarget::GetAttributeIndex()
{
    return m_AttributeIndex;
}

void CGXDLMSTarget::SetAttributeIndex(unsigned char value)
{
    m_AttributeIndex = value;
}

unsigned char CGXDLMSTarget::GetDataIndex()
{
    return m_DataIndex;
}

void CGXDLMSTarget::SetDataIndex(unsigned char value)
{
    m_DataIndex = value;
}

CGXDLMSVariant& CGXDLMSTarget::GetValue() {
    return m_Value;
}

void CGXDLMSTarget::SetValue(CGXDLMSVariant& value) {
    m_Value = value;
}

std::string CGXDLMSTarget::ToString()
{
    if (m_Target == NULL)
    {
        return "";
    }
    std::string ln;
    std::string str = GXHelpers::IntToString(m_Target->GetObjectType());
    str.append(" ");
    m_Target->GetLogicalName(ln);
    str.append(ln);
    str.append(" ");
    str.append(GXHelpers::IntToString(m_AttributeIndex));
    str.append(" ");
    str.append(GXHelpers::IntToString(m_DataIndex));
    str.append(" ");
    str.append(m_Value.ToString());
    return str;
}