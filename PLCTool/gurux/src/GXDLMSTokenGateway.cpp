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

#include "../include/GXDLMSTokenGateway.h"

//Constructor.
CGXDLMSTokenGateway::CGXDLMSTokenGateway() :
    CGXDLMSTokenGateway("", 0)
{
}

//SN Constructor.
CGXDLMSTokenGateway::CGXDLMSTokenGateway(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_TOKEN_GATEWAY, ln, sn)
{

}

//LN Constructor.
CGXDLMSTokenGateway::CGXDLMSTokenGateway(std::string ln) :
    CGXDLMSTokenGateway(ln, 0)
{

}

// Returns amount of attributes.
int CGXDLMSTokenGateway::GetAttributeCount()
{
    return 6;
}

// Returns amount of methods.
int CGXDLMSTokenGateway::GetMethodCount()
{
    return 1;
}

void CGXDLMSTokenGateway::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string tmp;
    GetLogicalName(tmp);
    values.push_back(tmp);
    values.push_back(m_Token.ToString());
    values.push_back(m_Time.ToString());
    tmp = "";
    for (std::vector<std::string>::iterator it = m_Descriptions.begin(); it != m_Descriptions.end(); ++it)
    {
        tmp += *it;
    }
    values.push_back(tmp);
    values.push_back(GXHelpers::IntToString(m_DeliveryMethod));
    values.push_back(GXHelpers::IntToString(m_Status) + ", " + m_DataValue);
}

void CGXDLMSTokenGateway::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // Token
    if (all || CanRead(2)) {
        attributes.push_back(2);
    }
    // Time
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // Description
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
    // DeliveryMethod
    if (all || CanRead(5)) {
        attributes.push_back(5);
    }
    // Status
    if (all || CanRead(6)) {
        attributes.push_back(6);
    }
}

int CGXDLMSTokenGateway::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index)
    {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 3:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 4:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 5:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    case 6:
        type = DLMS_DATA_TYPE_STRUCTURE;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSTokenGateway::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    CGXByteBuffer bb;
    int ret;
    CGXDLMSVariant tmp;
    switch (e.GetIndex()) {
    case 1:
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        break;
    case 2:
        e.SetValue(m_Token);
        break;
    case 3:
        e.SetValue(m_Time);
        break;
    case 4:
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        bb.SetUInt8((unsigned char)m_Descriptions.size());
        for (std::vector<std::string>::iterator it = m_Descriptions.begin(); it != m_Descriptions.end(); ++it)
        {
            bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            bb.SetUInt8((unsigned char)it->length());
            bb.Set(it->c_str(), (unsigned long)it->length());
        }
        e.SetValue(bb);
        break;
    case 5:
        e.SetValue(m_DeliveryMethod);
        break;
    case 6:
        bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        bb.SetUInt8(2);
        tmp = m_Status;
        if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_ENUM, tmp)) != 0)
        {
            return ret;
        }
        tmp = m_DataValue;
        if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_BIT_STRING, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(bb);
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int CGXDLMSTokenGateway::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret;
    CGXByteBuffer bb;
    CGXDLMSVariant tmp;
    std::string str;
    switch (e.GetIndex())
    {
    case 1:
        return SetLogicalName(this, e.GetValue());
    case 2:
        m_Token.Clear();
        m_Token.Set(e.GetValue().byteArr, e.GetValue().GetSize());
        break;
    case 3:
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            bb.Clear();
            e.GetValue().GetBytes(bb);
            CGXDataInfo info;
            info.SetType(DLMS_DATA_TYPE_DATETIME);
            if ((ret = GXHelpers::GetData(bb, info, tmp)) != 0)
            {
                return ret;
            }
            m_Time = tmp.dateTime;
        }
        else
        {
            m_Time = e.GetValue().dateTime;
        }
        break;
    case 4:
        m_Descriptions.clear();
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            str.clear();
            str.append((const char*)it->byteArr, it->GetSize());
            m_Descriptions.push_back(str);
        }
        break;
    case 5:
        m_DeliveryMethod = (DLMS_TOKEN_DELIVERY)e.GetValue().ToInteger();
        break;
    case 6:
        m_Status = (DLMS_TOKEN_STATUS_CODE)e.GetValue().Arr[0].ToInteger();
        m_DataValue = e.GetValue().Arr[1].strVal;
        break;
    default:
        return DLMS_ERROR_CODE_READ_WRITE_DENIED;
        break;
    }
    return DLMS_ERROR_CODE_OK;
}
