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

#include "../include/GXDLMSHdlcSetup.h"
#include "../include/GXDLMSConverter.h"

//Constructor.
CGXDLMSIecHdlcSetup::CGXDLMSIecHdlcSetup() :
    CGXDLMSIecHdlcSetup("0.0.22.0.0.255", 0)
{
}

//SN Constructor.
CGXDLMSIecHdlcSetup::CGXDLMSIecHdlcSetup(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_IEC_HDLC_SETUP, ln, sn)
{
    m_CommunicationSpeed = DLMS_BAUD_RATE_9600;
    m_WindowSizeTransmit = m_WindowSizeReceive = 1;
    m_MaximumInfoLengthTransmit = m_MaximumInfoLengthReceive = 128;
    m_InactivityTimeout = 120;
    m_DeviceAddress = 0;
    m_InterCharachterTimeout = 30;
    m_Version = 1;
}

//LN Constructor.
CGXDLMSIecHdlcSetup::CGXDLMSIecHdlcSetup(std::string ln) :
    CGXDLMSIecHdlcSetup(ln, 0)
{
}

DLMS_BAUD_RATE CGXDLMSIecHdlcSetup::GetCommunicationSpeed()
{
    return m_CommunicationSpeed;
}
void CGXDLMSIecHdlcSetup::SetCommunicationSpeed(DLMS_BAUD_RATE value)
{
    m_CommunicationSpeed = value;
}

int CGXDLMSIecHdlcSetup::GetWindowSizeTransmit()
{
    return m_WindowSizeTransmit;
}
void CGXDLMSIecHdlcSetup::SetWindowSizeTransmit(int value)
{
    m_WindowSizeTransmit = value;
}

int CGXDLMSIecHdlcSetup::GetWindowSizeReceive()
{
    return m_WindowSizeReceive;
}
void CGXDLMSIecHdlcSetup::SetWindowSizeReceive(int value)
{
    m_WindowSizeReceive = value;
}

int CGXDLMSIecHdlcSetup::GetMaximumInfoLengthTransmit()
{
    return m_MaximumInfoLengthTransmit;
}
void CGXDLMSIecHdlcSetup::SetMaximumInfoLengthTransmit(int value)
{
    m_MaximumInfoLengthTransmit = value;
}

int CGXDLMSIecHdlcSetup::GetMaximumInfoLengthReceive()
{
    return m_MaximumInfoLengthReceive;
}
void CGXDLMSIecHdlcSetup::SetMaximumInfoLengthReceive(int value)
{
    m_MaximumInfoLengthReceive = value;
}

int CGXDLMSIecHdlcSetup::GetInterCharachterTimeout()
{
    return m_InterCharachterTimeout;
}
void CGXDLMSIecHdlcSetup::SetInterCharachterTimeout(int value)
{
    m_InterCharachterTimeout = value;
}

int CGXDLMSIecHdlcSetup::GetInactivityTimeout()
{
    return m_InactivityTimeout;
}
void CGXDLMSIecHdlcSetup::SetInactivityTimeout(int value)
{
    m_InactivityTimeout = value;
}

int CGXDLMSIecHdlcSetup::GetDeviceAddress()
{
    return m_DeviceAddress;
}
void CGXDLMSIecHdlcSetup::SetDeviceAddress(int value)
{
    m_DeviceAddress = value;
}

// Returns amount of attributes.
int CGXDLMSIecHdlcSetup::GetAttributeCount()
{
    return 9;
}

// Returns amount of methods.
int CGXDLMSIecHdlcSetup::GetMethodCount()
{
    return 0;
}

void CGXDLMSIecHdlcSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSConverter::ToString(m_CommunicationSpeed));
    values.push_back(CGXDLMSVariant(m_WindowSizeTransmit).ToString());
    values.push_back(CGXDLMSVariant(m_WindowSizeReceive).ToString());
    values.push_back(CGXDLMSVariant(m_MaximumInfoLengthTransmit).ToString());
    values.push_back(CGXDLMSVariant(m_MaximumInfoLengthReceive).ToString());
    values.push_back(CGXDLMSVariant(m_InterCharachterTimeout).ToString());
    values.push_back(CGXDLMSVariant(m_InactivityTimeout).ToString());
    values.push_back(CGXDLMSVariant(m_DeviceAddress).ToString());
}

void CGXDLMSIecHdlcSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //CommunicationSpeed
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //WindowSizeTransmit
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //WindowSizeReceive
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //MaximumInfoLengthTransmit
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
    //MaximumInfoLengthReceive
    if (all || !IsRead(6))
    {
        attributes.push_back(6);
    }
    //InterCharachterTimeout
    if (all || !IsRead(7))
    {
        attributes.push_back(7);
    }
    //InactivityTimeout
    if (all || !IsRead(8))
    {
        attributes.push_back(8);
    }
    //DeviceAddress
    if (all || !IsRead(9))
    {
        attributes.push_back(9);
    }
}

int CGXDLMSIecHdlcSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 6)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 7)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 8)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 9)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSIecHdlcSetup::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
    else if (e.GetIndex() == 2)
    {
        e.SetValue(m_CommunicationSpeed);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue(m_WindowSizeTransmit);
    }
    else if (e.GetIndex() == 4)
    {
        e.SetValue(m_WindowSizeReceive);
    }
    else if (e.GetIndex() == 5)
    {
        e.SetValue(m_MaximumInfoLengthTransmit);
    }
    else if (e.GetIndex() == 6)
    {
        e.SetValue(m_MaximumInfoLengthReceive);
    }
    else if (e.GetIndex() == 7)
    {
        e.SetValue(m_InterCharachterTimeout);
    }
    else if (e.GetIndex() == 8)
    {
        e.SetValue(m_InactivityTimeout);
    }
    else if (e.GetIndex() == 9)
    {
        e.SetValue(m_DeviceAddress);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int CGXDLMSIecHdlcSetup::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_CommunicationSpeed = (DLMS_BAUD_RATE)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_WindowSizeTransmit = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_WindowSizeReceive = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_MaximumInfoLengthTransmit = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 6)
    {
        m_MaximumInfoLengthReceive = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 7)
    {
        m_InterCharachterTimeout = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 8)
    {
        m_InactivityTimeout = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 9)
    {
        m_DeviceAddress = e.GetValue().ToInteger();
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
