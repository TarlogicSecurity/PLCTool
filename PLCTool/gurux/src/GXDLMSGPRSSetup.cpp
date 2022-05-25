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
#include "../include/GXDLMSGPRSSetup.h"

//Constructor.
CGXDLMSGPRSSetup::CGXDLMSGPRSSetup() :
    CGXDLMSGPRSSetup("", 0)
{
}

//SN Constructor.
CGXDLMSGPRSSetup::CGXDLMSGPRSSetup(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_GPRS_SETUP, ln, sn)
{
    m_PINCode = 0;
}

//LN Constructor.
CGXDLMSGPRSSetup::CGXDLMSGPRSSetup(std::string ln) :
    CGXDLMSGPRSSetup(ln, 0)
{
}

std::string& CGXDLMSGPRSSetup::GetAPN()
{
    return m_APN;
}
void CGXDLMSGPRSSetup::SetAPN(std::string& value)
{
    m_APN = value;
}

long CGXDLMSGPRSSetup::GetPINCode()
{
    return m_PINCode;
}
void CGXDLMSGPRSSetup::SetPINCode(long value)
{
    m_PINCode = value;
}

CGXDLMSQualityOfService& CGXDLMSGPRSSetup::GetDefaultQualityOfService()
{
    return m_DefaultQualityOfService;
}

CGXDLMSQualityOfService& CGXDLMSGPRSSetup::GetRequestedQualityOfService()
{
    return m_RequestedQualityOfService;
}

// Returns amount of attributes.
int CGXDLMSGPRSSetup::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSGPRSSetup::GetMethodCount()
{
    return 0;
}

void CGXDLMSGPRSSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_APN);
    values.push_back(CGXDLMSVariant(m_PINCode).ToString());
    values.push_back(m_DefaultQualityOfService.ToString() + " " + m_RequestedQualityOfService.ToString());
}

void CGXDLMSGPRSSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //APN
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //PINCode
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //DefaultQualityOfService + RequestedQualityOfService
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
}

int CGXDLMSGPRSSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_UINT16;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Returns value of given attribute.
int CGXDLMSGPRSSetup::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
        CGXByteBuffer bb;
        bb.AddString(m_APN);
        e.SetValue(bb);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetValue(m_PINCode);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(5);
        int ret;
        CGXDLMSVariant precedence = m_DefaultQualityOfService.GetPrecedence();
        CGXDLMSVariant delay = m_DefaultQualityOfService.GetDelay();
        CGXDLMSVariant reliability = m_DefaultQualityOfService.GetReliability();
        CGXDLMSVariant peak = m_DefaultQualityOfService.GetPeakThroughput();
        CGXDLMSVariant mean = m_DefaultQualityOfService.GetMeanThroughput();
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, precedence)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, delay)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, reliability)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, peak)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, mean)) != 0)
        {
            return ret;
        }
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(5);
        precedence = m_RequestedQualityOfService.GetPrecedence();
        delay = m_RequestedQualityOfService.GetDelay();
        reliability = m_RequestedQualityOfService.GetReliability();
        peak = m_RequestedQualityOfService.GetPeakThroughput();
        mean = m_RequestedQualityOfService.GetMeanThroughput();

        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, precedence)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, delay)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, reliability)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, peak)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, mean)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSGPRSSetup::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRING)
        {
            m_APN = e.GetValue().ToString();
        }
        else
        {
            m_APN.clear();
            m_APN.append(e.GetValue().byteArr, e.GetValue().byteArr + e.GetValue().GetSize());
        }
    }
    else if (e.GetIndex() == 3)
    {
        m_PINCode = e.GetValue().uiVal;
    }
    else if (e.GetIndex() == 4)
    {
        m_DefaultQualityOfService.SetPrecedence(e.GetValue().Arr[0].Arr[0].iVal);
        m_DefaultQualityOfService.SetDelay(e.GetValue().Arr[0].Arr[1].iVal);
        m_DefaultQualityOfService.SetReliability(e.GetValue().Arr[0].Arr[2].iVal);
        m_DefaultQualityOfService.SetPeakThroughput(e.GetValue().Arr[0].Arr[3].iVal);
        m_DefaultQualityOfService.SetMeanThroughput(e.GetValue().Arr[0].Arr[4].iVal);

        m_RequestedQualityOfService.SetPrecedence(e.GetValue().Arr[1].Arr[0].iVal);
        m_RequestedQualityOfService.SetDelay(e.GetValue().Arr[1].Arr[1].iVal);
        m_RequestedQualityOfService.SetReliability(e.GetValue().Arr[1].Arr[2].iVal);
        m_RequestedQualityOfService.SetPeakThroughput(e.GetValue().Arr[1].Arr[3].iVal);
        m_RequestedQualityOfService.SetMeanThroughput(e.GetValue().Arr[1].Arr[4].iVal);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

