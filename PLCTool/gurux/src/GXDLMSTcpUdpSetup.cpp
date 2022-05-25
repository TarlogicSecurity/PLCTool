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

#include "../include/GXDLMSVariant.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXDLMSTcpUdpSetup.h"

/**
 Constructor.
*/
CGXDLMSTcpUdpSetup::CGXDLMSTcpUdpSetup() :
    CGXDLMSTcpUdpSetup("0.0.25.0.0.255", 0)
{
}

/**
 Constructor.

 @param ln Logical Name of the object.
*/
CGXDLMSTcpUdpSetup::CGXDLMSTcpUdpSetup(std::string ln) :
    CGXDLMSTcpUdpSetup(ln, 0)
{
}

/**
 Constructor.

 @param ln Logical Name of the object.
 @param sn Short Name of the object.
*/
CGXDLMSTcpUdpSetup::CGXDLMSTcpUdpSetup(std::string ln, short sn) : CGXDLMSObject(DLMS_OBJECT_TYPE_TCP_UDP_SETUP, ln, sn)
{
    m_Port = 4059;
    m_IPReference = "127.0.0.1";
    m_MaximumSimultaneousConnections = 1;
    m_InactivityTimeout = 180;
    m_MaximumSegmentSize = 576;
}

int CGXDLMSTcpUdpSetup::GetPort()
{
    return m_Port;
}

void CGXDLMSTcpUdpSetup::SetPort(int value)
{
    m_Port = value;
}

std::string CGXDLMSTcpUdpSetup::GetIPReference()
{
    return m_IPReference;
}
void CGXDLMSTcpUdpSetup::SetIPReference(std::string value)
{
    m_IPReference = value;
}

int CGXDLMSTcpUdpSetup::GetMaximumSegmentSize()
{
    return m_MaximumSegmentSize;
}
void CGXDLMSTcpUdpSetup::SetMaximumSegmentSize(int value)
{
    m_MaximumSegmentSize = value;
}

int CGXDLMSTcpUdpSetup::GetMaximumSimultaneousConnections()
{
    return m_MaximumSimultaneousConnections;
}
void CGXDLMSTcpUdpSetup::SetMaximumSimultaneousConnections(int value)
{
    m_MaximumSimultaneousConnections = value;
}

int CGXDLMSTcpUdpSetup::GetInactivityTimeout()
{
    return m_InactivityTimeout;
}
void CGXDLMSTcpUdpSetup::SetInactivityTimeout(int value)
{
    m_InactivityTimeout = value;
}

// Returns amount of attributes.
int CGXDLMSTcpUdpSetup::GetAttributeCount()
{
    return 6;
}

// Returns amount of methods.
int CGXDLMSTcpUdpSetup::GetMethodCount()
{
    return 0;
}

void CGXDLMSTcpUdpSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSVariant(m_Port).ToString());
    values.push_back(m_IPReference);
    values.push_back(CGXDLMSVariant(m_MaximumSegmentSize).ToString());
    values.push_back(CGXDLMSVariant(m_MaximumSimultaneousConnections).ToString());
    values.push_back(CGXDLMSVariant(m_InactivityTimeout).ToString());
}

void CGXDLMSTcpUdpSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Port
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //IPReference
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //MaximumSegmentSize
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //MaximumSimultaneousConnections
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
    //InactivityTimeout
    if (all || !IsRead(6))
    {
        attributes.push_back(6);
    }
}

int CGXDLMSTcpUdpSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 6)
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
int CGXDLMSTcpUdpSetup::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
        CGXDLMSVariant tmp = GetPort();
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        CGXDLMSVariant tmp;
        GXHelpers::SetLogicalName(m_IPReference.c_str(), tmp);
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        CGXDLMSVariant tmp = GetMaximumSegmentSize();
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        CGXDLMSVariant tmp = GetMaximumSimultaneousConnections();
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        CGXDLMSVariant tmp = GetInactivityTimeout();
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSTcpUdpSetup::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        SetPort(e.GetValue().ToInteger());
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 3)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_NONE)
        {
            SetIPReference("");
        }
        else
        {
            if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                GXHelpers::GetLogicalName(e.GetValue().byteArr, m_IPReference);
            }
            else
            {
                SetIPReference(e.GetValue().ToString());
            }
        }
    }
    else if (e.GetIndex() == 4)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_NONE)
        {
            SetMaximumSegmentSize(576);
        }
        else
        {
            SetMaximumSegmentSize(e.GetValue().ToInteger());
        }
    }
    else if (e.GetIndex() == 5)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_NONE)
        {
            SetMaximumSimultaneousConnections(1);
        }
        else
        {
            SetMaximumSimultaneousConnections(e.GetValue().ToInteger());
        }
    }
    else if (e.GetIndex() == 6)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_NONE)
        {
            SetInactivityTimeout(180);
        }
        else
        {
            SetInactivityTimeout(e.GetValue().ToInteger());
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
