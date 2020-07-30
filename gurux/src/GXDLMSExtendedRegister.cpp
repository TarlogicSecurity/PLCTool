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
#include "../include/GXDLMSExtendedRegister.h"

bool CGXDLMSExtendedRegister::IsRead(int index)
{
    if (index == 3)
    {
        return m_Unit != 0;
    }
    return CGXDLMSObject::IsRead(index);
}
//Constructor.
CGXDLMSExtendedRegister::CGXDLMSExtendedRegister() :
    CGXDLMSExtendedRegister("", 0)
{
}

//SN Constructor.
CGXDLMSExtendedRegister::CGXDLMSExtendedRegister(std::string ln, unsigned short sn) :
    CGXDLMSRegister(DLMS_OBJECT_TYPE_EXTENDED_REGISTER, ln, sn)
{
}

//LN Constructor.
CGXDLMSExtendedRegister::CGXDLMSExtendedRegister(std::string ln) :
    CGXDLMSExtendedRegister(ln, 0)
{
}

// Get value of COSEM Data object.
CGXDLMSVariant& CGXDLMSExtendedRegister::GetValue()
{
    return CGXDLMSRegister::GetValue();
}

// Set value of COSEM Data object.
void CGXDLMSExtendedRegister::SetValue(CGXDLMSVariant& value)
{
    CGXDLMSRegister::SetValue(value);
}

/**
 Status of COSEM Extended Register object.
*/
CGXDLMSVariant& CGXDLMSExtendedRegister::GetStatus()
{
    return m_Status;
}
void CGXDLMSExtendedRegister::SetStatus(CGXDLMSVariant& value)
{
    m_Status = value;
}

/**
 Capture time of COSEM Extended Register object.
*/
CGXDateTime& CGXDLMSExtendedRegister::GetCaptureTime()
{
    return m_CaptureTime;
}

void CGXDLMSExtendedRegister::SetCaptureTime(CGXDateTime& value)
{
    m_CaptureTime = value;
}

int CGXDLMSExtendedRegister::GetUIDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_DATETIME;
    }
    else
    {
        return CGXDLMSRegister::GetUIDataType(index, type);
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns amount of attributes.
int CGXDLMSExtendedRegister::GetAttributeCount()
{
    return 5;
}

// Returns amount of methods.
int CGXDLMSExtendedRegister::GetMethodCount()
{
    return 1;
}

void CGXDLMSExtendedRegister::GetValues(std::vector<std::string>& values)
{
    CGXDLMSRegister::GetValues(values);
    values.push_back(m_Status.ToString());
    values.push_back(m_CaptureTime.ToString());
}

void CGXDLMSExtendedRegister::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    CGXDLMSRegister::GetAttributeIndexToRead(all, attributes);
    //Status
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //CaptureTime
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
}

int CGXDLMSExtendedRegister::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index < 4)
    {
        return CGXDLMSRegister::GetDataType(index, type);
    }
    if (index == 4)
    {
        return CGXDLMSObject::GetDataType(index, type);
    }
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}


int CGXDLMSExtendedRegister::Invoke(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_Value.Clear();
        m_CaptureTime = CGXDateTime(CGXDateTime::Now());
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMSExtendedRegister::GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() < 4)
    {
        return CGXDLMSRegister::GetValue(settings, e);
    }
    if (e.GetIndex() == 4)
    {
        e.SetValue(m_Status);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        e.SetValue(m_CaptureTime);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int CGXDLMSExtendedRegister::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() < 4)
    {
        return CGXDLMSRegister::SetValue(settings, e);
    }
    else if (e.GetIndex() == 4)
    {
        m_Status = e.GetValue();
    }
    else if (e.GetIndex() == 5)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            CGXDLMSVariant tmp;
            CGXDLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp);
            m_CaptureTime = tmp.dateTime;
        }
        else
        {
            m_CaptureTime = e.GetValue().dateTime;
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

