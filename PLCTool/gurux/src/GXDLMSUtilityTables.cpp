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

#include "../include/GXDLMSUtilityTables.h"

//Constructor.
CGXDLMSUtilityTables::CGXDLMSUtilityTables() :
    CGXDLMSUtilityTables("0.0.65.0.0.255", 0)
{
}

//SN Constructor.
CGXDLMSUtilityTables::CGXDLMSUtilityTables(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_UTILITY_TABLES, ln, sn)
{

}

//LN Constructor.
CGXDLMSUtilityTables::CGXDLMSUtilityTables(std::string ln) :
    CGXDLMSUtilityTables(ln, 0)
{
    m_TableId = 0;
}

unsigned short CGXDLMSUtilityTables::GetTableId()
{
    return m_TableId;
}

void CGXDLMSUtilityTables::SetTableId(unsigned short value)
{
    m_TableId = value;
}

CGXByteBuffer& CGXDLMSUtilityTables::GetBuffer()
{
    return m_Buffer;
}

void CGXDLMSUtilityTables::SetBuffer(CGXByteBuffer& value)
{
    m_Buffer = value;
}

// Returns amount of attributes.
int CGXDLMSUtilityTables::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSUtilityTables::GetMethodCount()
{
    return 0;
}

void CGXDLMSUtilityTables::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(GXHelpers::IntToString(m_TableId));
    values.push_back(GXHelpers::IntToString(m_TableId));
    values.push_back(GXHelpers::IntToString(m_Buffer.GetSize()));
    values.push_back(m_Buffer.ToHexString());
}

void CGXDLMSUtilityTables::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Table ID
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //Length
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //Buffer
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
}

int CGXDLMSUtilityTables::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index) {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_UINT16;
        break;
    case 3:
        type = DLMS_DATA_TYPE_UINT32;
        break;
    case 4:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSUtilityTables::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    switch (e.GetIndex())
    {
    case 1:
    {
        int ret;
        CGXDLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
    }
    break;
    case 2:
        e.SetValue(m_TableId);
        break;
    case 3:
        e.SetValue(m_Buffer.GetSize());
        break;
    case 4:
        e.SetValue(m_Buffer);
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int CGXDLMSUtilityTables::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret = DLMS_ERROR_CODE_OK;
    switch (e.GetIndex()) {
    case 1:
        ret = SetLogicalName(this, e.GetValue());
        break;
    case 2:
        m_TableId = e.GetValue().ToInteger();
        // Skip len.
        break;
    case 3:
        // Skip len.
        break;
    case 4:
        m_Buffer.Clear();
        m_Buffer.Set(e.GetValue().byteArr, e.GetValue().GetSize());
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}
