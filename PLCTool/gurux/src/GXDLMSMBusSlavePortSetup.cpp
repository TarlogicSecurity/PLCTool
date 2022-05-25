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

#include "../include/GXDLMSMBusSlavePortSetup.h"
#include "../include/GXDLMSConverter.h"

//Constructor.
CGXDLMSMBusSlavePortSetup::CGXDLMSMBusSlavePortSetup() :
    CGXDLMSMBusSlavePortSetup("", 0)
{
}

//SN Constructor.
CGXDLMSMBusSlavePortSetup::CGXDLMSMBusSlavePortSetup(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP, ln, sn)
{

}

//LN Constructor.
CGXDLMSMBusSlavePortSetup::CGXDLMSMBusSlavePortSetup(std::string ln) :
    CGXDLMSMBusSlavePortSetup(ln, 0)
{

}

/**
Defines the baud rate for the opening sequence.
*/
DLMS_BAUD_RATE CGXDLMSMBusSlavePortSetup::GetDefaultBaud()
{
    return m_DefaultBaud;
}
void CGXDLMSMBusSlavePortSetup::SetDefaultBaud(DLMS_BAUD_RATE value)
{
    m_DefaultBaud = value;
}
/**
 Defines the baud rate for the opening sequence.
*/
DLMS_BAUD_RATE CGXDLMSMBusSlavePortSetup::GetAvailableBaud()
{
    return m_AvailableBaud;
}
void CGXDLMSMBusSlavePortSetup::SetAvailableBaud(DLMS_BAUD_RATE value)
{
    m_AvailableBaud = value;
}

/**
 Defines whether or not the device has been assigned an address
 * since last power up of the device.
*/
DLMS_ADDRESS_STATE CGXDLMSMBusSlavePortSetup::GetAddressState()
{
    return m_AddressState;
}
void CGXDLMSMBusSlavePortSetup::SetAddressState(DLMS_ADDRESS_STATE value)
{
    m_AddressState = value;
}


/**
 Defines the baud rate for the opening sequence.
*/
int CGXDLMSMBusSlavePortSetup::GetBusAddress()
{
    return m_BusAddress;
}
void CGXDLMSMBusSlavePortSetup::SetBusAddress(int value)
{
    m_BusAddress = value;
}

// Returns amount of attributes.
int CGXDLMSMBusSlavePortSetup::GetAttributeCount()
{
    return 5;
}

// Returns amount of methods.
int CGXDLMSMBusSlavePortSetup::GetMethodCount()
{
    return 0;
}

void CGXDLMSMBusSlavePortSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSConverter::ToString(m_DefaultBaud));
    values.push_back(CGXDLMSConverter::ToString(m_AvailableBaud));
    values.push_back(CGXDLMSConverter::ToString(m_AddressState));
    values.push_back(CGXDLMSVariant(m_BusAddress).ToString());
}

void CGXDLMSMBusSlavePortSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //DefaultBaud
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //AvailableBaud
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //m_AddressState
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //BusAddress
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
}

int CGXDLMSMBusSlavePortSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
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
        type = DLMS_DATA_TYPE_UINT16;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSMBusSlavePortSetup::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
    }
    else if (e.GetIndex() == 2)
    {
        e.SetValue(m_DefaultBaud);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue(m_AvailableBaud);
    }
    else if (e.GetIndex() == 4)
    {
        e.SetValue(m_AddressState);
    }
    else if (e.GetIndex() == 5)
    {
        e.SetValue(m_BusAddress);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int CGXDLMSMBusSlavePortSetup::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_DefaultBaud = (DLMS_BAUD_RATE)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_AvailableBaud = (DLMS_BAUD_RATE)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_AddressState = (DLMS_ADDRESS_STATE)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_BusAddress = e.GetValue().ToInteger();
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
