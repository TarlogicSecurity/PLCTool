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

#include "../include/GXDLMSLlcSscsSetup.h"
#include "../include/GXDLMSClient.h"

//Constructor.
CGXDLMSLlcSscsSetup::CGXDLMSLlcSscsSetup() :
    CGXDLMSLlcSscsSetup("0.0.28.0.0.255", 0)
{
}

//SN Constructor.
CGXDLMSLlcSscsSetup::CGXDLMSLlcSscsSetup(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_LLC_SSCS_SETUP, ln, sn)
{
    m_BaseNodeAddress = m_ServiceNodeAddress = 0;
}

//LN Constructor.
CGXDLMSLlcSscsSetup::CGXDLMSLlcSscsSetup(std::string ln) :
    CGXDLMSLlcSscsSetup(ln, 0)
{

}

unsigned short CGXDLMSLlcSscsSetup::GetServiceNodeAddress()
{
    return m_ServiceNodeAddress;
}


void CGXDLMSLlcSscsSetup::SetServiceNodeAddress(unsigned short value)
{
    m_ServiceNodeAddress = value;
}

unsigned short CGXDLMSLlcSscsSetup::GetBaseNodeAddress()
{
    return m_BaseNodeAddress;
}

void CGXDLMSLlcSscsSetup::SetBaseNodeAddress(unsigned short value)
{
    m_BaseNodeAddress = value;
}


int CGXDLMSLlcSscsSetup::Reset(CGXDLMSClient* client, std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int CGXDLMSLlcSscsSetup::Invoke(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_ServiceNodeAddress = m_BaseNodeAddress = 0;
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns amount of attributes.
int CGXDLMSLlcSscsSetup::GetAttributeCount()
{
    return 3;
}

// Returns amount of methods.
int CGXDLMSLlcSscsSetup::GetMethodCount()
{
    return 1;
}

void CGXDLMSLlcSscsSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSVariant(m_ServiceNodeAddress).ToString());
    values.push_back(CGXDLMSVariant(m_BaseNodeAddress).ToString());
}

void CGXDLMSLlcSscsSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // ServiceNodeAddress
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // BaseNodeAddress
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
}

int CGXDLMSLlcSscsSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    switch (index) {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
    case 3:
        type = DLMS_DATA_TYPE_UINT16;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

// Returns value of given attribute.
int CGXDLMSLlcSscsSetup::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        int ret;
        CGXDLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
    }
    else if (e.GetIndex() == 2)
    {
        e.SetValue(m_ServiceNodeAddress);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue(m_BaseNodeAddress);
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int CGXDLMSLlcSscsSetup::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_ServiceNodeAddress = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_BaseNodeAddress = e.GetValue().ToInteger();
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}
