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
#include "../include/GXDLMSDisconnectControl.h"
#include "../include/GXDLMSConverter.h"

//Constructor.
CGXDLMSDisconnectControl::CGXDLMSDisconnectControl() :
    CGXDLMSDisconnectControl("", 0)
{
}

//SN Constructor.
CGXDLMSDisconnectControl::CGXDLMSDisconnectControl(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_DISCONNECT_CONTROL, ln, sn)
{

}

//LN Constructor.
CGXDLMSDisconnectControl::CGXDLMSDisconnectControl(std::string ln) :
    CGXDLMSDisconnectControl(ln, 0)
{

}

/**
 Output state of COSEM Disconnect Control object.
*/
bool CGXDLMSDisconnectControl::GetOutputState()
{
    return m_OutputState;
}
void CGXDLMSDisconnectControl::SetOutputState(bool value)
{
    m_OutputState = value;
}

/**
 Control state of COSEM Disconnect Control object.
*/
DLMS_CONTROL_STATE CGXDLMSDisconnectControl::GetControlState()
{
    return m_ControlState;
}
void CGXDLMSDisconnectControl::SetControlState(DLMS_CONTROL_STATE value)
{
    m_ControlState = value;
}

/**
Control mode of COSEM Disconnect Control object.
*/
DLMS_CONTROL_MODE CGXDLMSDisconnectControl::GetControlMode()
{
    return m_ControlMode;
}
void CGXDLMSDisconnectControl::SetControlMode(DLMS_CONTROL_MODE value)
{
    m_ControlMode = value;
}

int CGXDLMSDisconnectControl::RemoteDisconnect(CGXDLMSClient* client, std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int CGXDLMSDisconnectControl::RemoteReconnect(CGXDLMSClient* client, std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 2, data, reply);
}

// Returns amount of attributes.
int CGXDLMSDisconnectControl::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSDisconnectControl::GetMethodCount()
{
    return 2;
}

void CGXDLMSDisconnectControl::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSVariant(m_OutputState).ToString());
    values.push_back(CGXDLMSConverter::ToString(m_ControlState));
    values.push_back(CGXDLMSConverter::ToString(m_ControlMode));
}

void CGXDLMSDisconnectControl::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //OutputState
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //ControlState
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //ControlMode
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
}

int CGXDLMSDisconnectControl::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_BOOLEAN;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSDisconnectControl::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
        e.SetValue(m_OutputState);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetValue(m_ControlState);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetValue(m_ControlMode);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSDisconnectControl::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_OutputState = e.GetValue().boolVal;
    }
    else if (e.GetIndex() == 3)
    {
        m_ControlState = (DLMS_CONTROL_STATE)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_ControlMode = (DLMS_CONTROL_MODE)e.GetValue().ToInteger();
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
