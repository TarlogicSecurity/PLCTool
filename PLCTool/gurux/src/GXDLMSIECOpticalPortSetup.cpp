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

#include "../include/GXDLMSIECOpticalPortSetup.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXDLMSConverter.h"

//Constructor.
CGXDLMSIECLocalPortSetup::CGXDLMSIECLocalPortSetup() :
    CGXDLMSIECLocalPortSetup("0.0.20.0.0.255", 0)
{
}

//SN Constructor.
CGXDLMSIECLocalPortSetup::CGXDLMSIECLocalPortSetup(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_IEC_LOCAL_PORT_SETUP, ln, sn)
{
    m_DefaultMode = DLMS_OPTICAL_PROTOCOL_MODE_DEFAULT;
    m_DefaultBaudrate = DLMS_BAUD_RATE_300;
    m_ProposedBaudrate = DLMS_BAUD_RATE_9600;
    m_Version = 1;
}

//LN Constructor.
CGXDLMSIECLocalPortSetup::CGXDLMSIECLocalPortSetup(std::string ln) :
    CGXDLMSIECLocalPortSetup(ln, 0)
{
}

DLMS_OPTICAL_PROTOCOL_MODE CGXDLMSIECLocalPortSetup::GetDefaultMode()
{
    return m_DefaultMode;
}
void CGXDLMSIECLocalPortSetup::SetDefaultMode(DLMS_OPTICAL_PROTOCOL_MODE value)
{
    m_DefaultMode = value;
}

DLMS_BAUD_RATE CGXDLMSIECLocalPortSetup::GetDefaultBaudrate()
{
    return m_DefaultBaudrate;
}
void CGXDLMSIECLocalPortSetup::SetDefaultBaudrate(DLMS_BAUD_RATE value)
{
    m_DefaultBaudrate = value;
}

DLMS_BAUD_RATE CGXDLMSIECLocalPortSetup::GetProposedBaudrate()
{
    return m_ProposedBaudrate;
}
void CGXDLMSIECLocalPortSetup::SetProposedBaudrate(DLMS_BAUD_RATE value)
{
    m_ProposedBaudrate = value;
}

DLMS_LOCAL_PORT_RESPONSE_TIME CGXDLMSIECLocalPortSetup::GetResponseTime()
{
    return m_ResponseTime;
}
void CGXDLMSIECLocalPortSetup::SetResponseTime(DLMS_LOCAL_PORT_RESPONSE_TIME value)
{
    m_ResponseTime = value;
}

std::string CGXDLMSIECLocalPortSetup::GetDeviceAddress()
{
    return m_DeviceAddress;
}
void CGXDLMSIECLocalPortSetup::SetDeviceAddress(std::string value)
{
    m_DeviceAddress = value;
}

std::string CGXDLMSIECLocalPortSetup::GetPassword1()
{
    return m_Password1;
}
void CGXDLMSIECLocalPortSetup::SetPassword1(std::string value)
{
    m_Password1 = value;
}

std::string CGXDLMSIECLocalPortSetup::GetPassword2()
{
    return m_Password2;
}
void CGXDLMSIECLocalPortSetup::SetPassword2(std::string value)
{
    m_Password2 = value;
}

std::string CGXDLMSIECLocalPortSetup::GetPassword5()
{
    return m_Password5;
}
void CGXDLMSIECLocalPortSetup::SetPassword5(std::string value)
{
    m_Password5 = value;
}

// Returns amount of attributes.
int CGXDLMSIECLocalPortSetup::GetAttributeCount()
{
    return 9;
}

// Returns amount of methods.
int CGXDLMSIECLocalPortSetup::GetMethodCount()
{
    return 0;
}

void CGXDLMSIECLocalPortSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSVariant(m_DefaultMode).ToString());
    values.push_back(CGXDLMSConverter::ToString(m_DefaultBaudrate));
    values.push_back(CGXDLMSConverter::ToString(m_ProposedBaudrate));
    values.push_back(CGXDLMSVariant(m_ResponseTime).ToString());
    values.push_back(m_DeviceAddress);
    values.push_back(m_Password1);
    values.push_back(m_Password2);
    values.push_back(m_Password5);
}

void CGXDLMSIECLocalPortSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //DefaultMode
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //DefaultBaudrate
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //ProposedBaudrate
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //ResponseTime
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
    //DeviceAddress
    if (all || !IsRead(6))
    {
        attributes.push_back(6);
    }
    //Password1
    if (all || !IsRead(7))
    {
        attributes.push_back(7);
    }
    //Password2
    if (all || !IsRead(8))
    {
        attributes.push_back(8);
    }
    //Password5
    if (all || !IsRead(9))
    {
        attributes.push_back(9);
    }
}

int CGXDLMSIECLocalPortSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 6)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 7)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 8)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 9)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSIECLocalPortSetup::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
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
        e.SetValue(GetDefaultMode());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetValue(GetDefaultBaudrate());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetValue(GetProposedBaudrate());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        e.SetValue(GetResponseTime());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        e.GetValue().Add(&m_DeviceAddress[0], (unsigned long)m_DeviceAddress.size());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 7)
    {
        e.GetValue().Add(&m_Password1[0], (unsigned long)m_Password1.size());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 8)
    {
        e.GetValue().Add(&m_Password2[0], (unsigned long)m_Password2.size());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 9)
    {
        e.GetValue().Add(&m_Password5[0], (int)m_Password5.size());
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSIECLocalPortSetup::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        SetDefaultMode((DLMS_OPTICAL_PROTOCOL_MODE)e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 3)
    {
        SetDefaultBaudrate((DLMS_BAUD_RATE)e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 4)
    {
        SetProposedBaudrate((DLMS_BAUD_RATE)e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 5)
    {
        SetResponseTime((DLMS_LOCAL_PORT_RESPONSE_TIME)e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 6)
    {
        CGXDLMSVariant tmp;
        CGXDLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_STRING, tmp);
        SetDeviceAddress(tmp.strVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 7)
    {
        CGXDLMSVariant tmp;
        CGXDLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_STRING, tmp);
        SetPassword1(tmp.strVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 8)
    {
        CGXDLMSVariant tmp;
        CGXDLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_STRING, tmp);
        SetPassword2(tmp.strVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 9)
    {
        CGXDLMSVariant tmp;
        CGXDLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_STRING, tmp);
        SetPassword5(tmp.strVal);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}
