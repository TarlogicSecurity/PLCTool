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

#include "../include/GXDLMSPppSetup.h"
#include "../include/GXDLMSClient.h"
#include <sstream>

//Constructor.
CGXDLMSPppSetup::CGXDLMSPppSetup() :
    CGXDLMSPppSetup("", 0)
{
    m_Authentication = PPP_AUTHENTICATION_TYPE_NONE;
}

//SN Constructor.
CGXDLMSPppSetup::CGXDLMSPppSetup(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_PPP_SETUP, ln, sn)
{
    m_Authentication = PPP_AUTHENTICATION_TYPE_NONE;
}

//LN Constructor.
CGXDLMSPppSetup::CGXDLMSPppSetup(std::string ln) :
    CGXDLMSPppSetup(ln, 0)
{
    m_Authentication = PPP_AUTHENTICATION_TYPE_NONE;
}

PPP_AUTHENTICATION_TYPE CGXDLMSPppSetup::GetAuthentication()
{
    return m_Authentication;
}
void CGXDLMSPppSetup::SetAuthentication(PPP_AUTHENTICATION_TYPE value)
{
    m_Authentication = value;
}

/**
PPP authentication procedure user name.
*/
CGXByteBuffer CGXDLMSPppSetup::GetUserName()
{
    return m_UserName;
}

void CGXDLMSPppSetup::SetUserName(CGXByteBuffer value)
{
    m_UserName = value;
}

/**
 PPP authentication procedure password.
*/

CGXByteBuffer& CGXDLMSPppSetup::GetPassword()
{
    return m_Password;
}

void CGXDLMSPppSetup::SetPassword(CGXByteBuffer value)
{
    m_Password = value;
}

std::string CGXDLMSPppSetup::GetPHYReference()
{
    return m_PHYReference;
}
void CGXDLMSPppSetup::SetPHYReference(std::string value)
{
    m_PHYReference = value;
}

std::vector<CGXDLMSPppSetupLcpOption>& CGXDLMSPppSetup::GetLCPOptions()
{
    return m_LCPOptions;
}


std::vector<CGXDLMSPppSetupIPCPOption>& CGXDLMSPppSetup::GetIPCPOptions()
{
    return m_IPCPOptions;
}

// Returns amount of attributes.
int CGXDLMSPppSetup::GetAttributeCount()
{
    return 5;
}

// Returns amount of methods.
int CGXDLMSPppSetup::GetMethodCount()
{
    return 0;
}

void CGXDLMSPppSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_PHYReference);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<CGXDLMSPppSetupLcpOption>::iterator it = m_LCPOptions.begin(); it != m_LCPOptions.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = it->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());

    //Clear str.
    sb.str(std::string());
    sb << '[';
    empty = true;
    for (std::vector<CGXDLMSPppSetupIPCPOption>::iterator it = m_IPCPOptions.begin(); it != m_IPCPOptions.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = it->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
    std::string str = m_UserName.ToString();
    str.append(" ");
    str.append(m_Password.ToString());
    values.push_back(str);
}

void CGXDLMSPppSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //PHYReference
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //LCPOptions
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //IPCPOptions
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //PPPAuthentication
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
}

int CGXDLMSPppSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 5)
    {
        if (m_UserName.GetSize() == 0)
        {
            type = DLMS_DATA_TYPE_NONE;
        }
        else
        {
            type = DLMS_DATA_TYPE_STRUCTURE;
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSPppSetup::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    CGXByteBuffer data;
    if (e.GetIndex() == 1)
    {
        int ret;
        CGXDLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 2)
    {
        CGXDLMSVariant tmp;
        GXHelpers::SetLogicalName(m_PHYReference.c_str(), tmp);
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount((unsigned long)m_LCPOptions.size(), data);
        CGXDLMSVariant type, len;
        for (std::vector<CGXDLMSPppSetupLcpOption>::iterator it = m_LCPOptions.begin(); it != m_LCPOptions.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3);
            type = it->GetType();
            len = it->GetLength();
            CGXDLMSVariant tmp = it->GetData();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, type);
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, len);
            GXHelpers::SetData(data, it->GetData().vt, tmp);
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount((unsigned long)m_IPCPOptions.size(), data);
        CGXDLMSVariant type, len;
        for (std::vector<CGXDLMSPppSetupIPCPOption>::iterator it = m_IPCPOptions.begin(); it != m_IPCPOptions.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3);
            type = it->GetType();
            len = it->GetLength();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, type);
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, len);
            GXHelpers::SetData(data, it->GetData().vt, it->m_Data);
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 5)
    {
        if (m_UserName.GetSize() != 0)
        {
            e.SetByteArray(true);
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            //Add username.
            data.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            data.SetUInt8((unsigned char)m_UserName.GetSize());
            data.Set(&m_UserName, 0, -1);
            //Add password.
            data.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            data.SetUInt8((unsigned char)m_Password.GetSize());
            data.Set(&m_Password, 0, -1);
            e.SetValue(data);
        }
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSPppSetup::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRING)
        {
            m_PHYReference = e.GetValue().ToString();
        }
        else
        {
            GXHelpers::GetLogicalName(e.GetValue().byteArr, m_PHYReference);
        }
    }
    else if (e.GetIndex() == 3)
    {
        m_LCPOptions.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
            {
                CGXDLMSPppSetupLcpOption it;
                it.SetType((PPP_SETUP_LCP_OPTION_TYPE)(*item).Arr[0].ToInteger());
                it.SetLength((*item).Arr[1].ToInteger());
                it.SetData((*item).Arr[2]);
                m_LCPOptions.push_back(it);
            }
        }
    }
    else if (e.GetIndex() == 4)
    {
        m_IPCPOptions.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
            {
                CGXDLMSPppSetupIPCPOption it;
                it.SetType((PPP_SETUP_IPCP_OPTION_TYPE)(*item).Arr[0].ToInteger());
                it.SetLength((*item).Arr[1].ToInteger());
                it.SetData((*item).Arr[2]);
                m_IPCPOptions.push_back(it);
            }
        }
    }
    else if (e.GetIndex() == 5)
    {
        m_UserName.Clear();
        m_Password.Clear();
        if (e.GetValue().Arr.size() == 2)
        {
            m_UserName.Set(e.GetValue().Arr[0].byteArr, e.GetValue().Arr[0].size);
            m_Password.Set(e.GetValue().Arr[1].byteArr, e.GetValue().Arr[1].size);
        }
        else if (e.GetValue().Arr.size() != 0)
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
