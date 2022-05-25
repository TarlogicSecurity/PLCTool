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

#include "../include/GXDLMSMBusClient.h"
#include "../include/GXDLMSClient.h"
#include <sstream>

CGXDLMSMBusClient::CGXDLMSMBusClient() :
    CGXDLMSMBusClient("", 0)
{
}

CGXDLMSMBusClient::CGXDLMSMBusClient(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_MBUS_CLIENT, ln, sn)
{
    m_CapturePeriod = 0;
    m_PrimaryAddress = 0;
    m_IdentificationNumber = 0;
    m_ManufacturerID = 0;
    m_DataHeaderVersion = 0;
    m_DeviceType = 0;
    m_AccessNumber = 0;
    m_Status = 0;
    m_Alarm = 0;
    m_Configuration = 0;
    m_EncryptionKeyStatus = DLMS_MBUS_ENCRYPTION_KEY_STATUS_NO_ENCRYPTION_KEY;
}

CGXDLMSMBusClient::CGXDLMSMBusClient(std::string ln) :
    CGXDLMSMBusClient(ln, 0)
{

}

/**
Provides reference to an "M-Bus master port setup" object, used to configure
an M-Bus port, each interface allowing to exchange data with one or more
M-Bus slave devices
*/
std::string CGXDLMSMBusClient::GetMBusPortReference()
{
    return m_MBusPortReference;
}
void CGXDLMSMBusClient::SetMBusPortReference(std::string value)
{
    m_MBusPortReference = value;
}

std::vector<std::pair<std::string, std::string> >& CGXDLMSMBusClient::GetCaptureDefinition()
{
    return m_CaptureDefinition;
}

long CGXDLMSMBusClient::GetCapturePeriod()
{
    return m_CapturePeriod;
}
void CGXDLMSMBusClient::SetCapturePeriod(long value)
{
    m_CapturePeriod = value;
}

int CGXDLMSMBusClient::GetPrimaryAddress()
{
    return m_PrimaryAddress;
}
void CGXDLMSMBusClient::SetPrimaryAddress(int value)
{
    m_PrimaryAddress = value;
}

long CGXDLMSMBusClient::GetIdentificationNumber()
{
    return m_IdentificationNumber;
}
void CGXDLMSMBusClient::SetIdentificationNumber(long value)
{
    m_IdentificationNumber = value;
}

int CGXDLMSMBusClient::GetManufacturerID()
{
    return m_ManufacturerID;
}
void CGXDLMSMBusClient::SetManufacturerID(int value)
{
    m_ManufacturerID = value;
}

/*
 * Carries the Version element of the data header as specified in EN
 * 13757-3 sub-clause 5.6.
 */
int CGXDLMSMBusClient::GetDataHeaderVersion()
{
    return m_DataHeaderVersion;
}
void CGXDLMSMBusClient::SetDataHeaderVersion(int value)
{
    m_DataHeaderVersion = value;
}

int CGXDLMSMBusClient::GetDeviceType()
{
    return m_DeviceType;
}
void CGXDLMSMBusClient::SetDeviceType(int value)
{
    m_DeviceType = value;
}

int CGXDLMSMBusClient::GetAccessNumber()
{
    return m_AccessNumber;
}
void CGXDLMSMBusClient::SetAccessNumber(int value)
{
    m_AccessNumber = value;
}

int CGXDLMSMBusClient::GetStatus()
{
    return m_Status;
}
void CGXDLMSMBusClient::SetStatus(int value)
{
    m_Status = value;
}

int CGXDLMSMBusClient::GetAlarm()
{
    return m_Alarm;
}
void CGXDLMSMBusClient::SetAlarm(int value)
{
    m_Alarm = value;
}

unsigned short CGXDLMSMBusClient::GetConfiguration()
{
    return m_Configuration;
}
void CGXDLMSMBusClient::SetConfiguration(unsigned short value)
{
    m_Configuration = value;
}

DLMS_MBUS_ENCRYPTION_KEY_STATUS CGXDLMSMBusClient::GetEncryptionKeyStatus()
{
    return m_EncryptionKeyStatus;
}

void CGXDLMSMBusClient::SetEncryptionKeyStatus(DLMS_MBUS_ENCRYPTION_KEY_STATUS value)
{
    m_EncryptionKeyStatus = value;
}

// Returns amount of attributes.
int CGXDLMSMBusClient::GetAttributeCount()
{
    if (m_Version == 0)
    {
        return 12;
    }
    return 14;
}

// Returns amount of methods.
int CGXDLMSMBusClient::GetMethodCount()
{
    return 8;
}

void CGXDLMSMBusClient::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_MBusPortReference);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<std::pair<std::string, std::string> >::iterator it = m_CaptureDefinition.begin(); it != m_CaptureDefinition.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        sb.write(it->first.c_str(), it->first.size());
        sb << " ";
        sb.write(it->second.c_str(), it->second.size());
    }
    sb << ']';
    values.push_back(sb.str());

    values.push_back(CGXDLMSVariant(m_CapturePeriod).ToString());
    values.push_back(CGXDLMSVariant(m_PrimaryAddress).ToString());
    values.push_back(CGXDLMSVariant(m_IdentificationNumber).ToString());
    values.push_back(CGXDLMSVariant(m_ManufacturerID).ToString());
    values.push_back(CGXDLMSVariant(m_DataHeaderVersion).ToString());
    values.push_back(CGXDLMSVariant(m_DeviceType).ToString());
    values.push_back(CGXDLMSVariant(m_AccessNumber).ToString());
    values.push_back(CGXDLMSVariant(m_Status).ToString());
    values.push_back(CGXDLMSVariant(m_Alarm).ToString());
    if (m_Version > 0)
    {
        values.push_back(CGXDLMSVariant(m_Configuration).ToString());
        values.push_back(CGXDLMSVariant(m_EncryptionKeyStatus).ToString());
    }
}

void CGXDLMSMBusClient::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //MBusPortReference
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //CaptureDefinition
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //CapturePeriod
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    //PrimaryAddress
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    //IdentificationNumber
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    //ManufacturerID
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
    //Version
    if (all || CanRead(8))
    {
        attributes.push_back(8);
    }
    //DeviceType
    if (all || CanRead(9))
    {
        attributes.push_back(9);
    }
    //AccessNumber
    if (all || CanRead(10))
    {
        attributes.push_back(10);
    }
    //Status
    if (all || CanRead(11))
    {
        attributes.push_back(11);
    }
    //Alarm
    if (all || CanRead(12))
    {
        attributes.push_back(12);
    }
    if (m_Version > 0)
    {
        //Configuration
        if (all || CanRead(13))
        {
            attributes.push_back(13);
        }
        //EncryptionKeyStatus
        if (all || CanRead(14))
        {
            attributes.push_back(14);
        }
    }
}

int CGXDLMSMBusClient::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_UINT32;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 6)
    {
        type = DLMS_DATA_TYPE_UINT32;
    }
    else if (index == 7)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 8)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 9)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 10)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 11)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 12)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 13 && m_Version > 0)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 14 && m_Version > 0)
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
int CGXDLMSMBusClient::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
        CGXDLMSVariant tmp;
        GXHelpers::SetLogicalName(m_MBusPortReference.c_str(), tmp);
        e.SetValue(tmp);
    }
    else if (e.GetIndex() == 3)
    {
        //e.SetValue(m_CaptureDefinition);//TODO;
    }
    else if (e.GetIndex() == 4)
    {
        e.SetValue(m_CapturePeriod);
    }
    else if (e.GetIndex() == 5)
    {
        e.SetValue(m_PrimaryAddress);
    }
    else if (e.GetIndex() == 6)
    {
        e.SetValue(m_IdentificationNumber);
    }
    else if (e.GetIndex() == 7)
    {
        e.SetValue(m_ManufacturerID);
    }
    else if (e.GetIndex() == 8)
    {
        e.SetValue(m_DataHeaderVersion);
    }
    else if (e.GetIndex() == 9)
    {
        e.SetValue(m_DeviceType);
    }
    else if (e.GetIndex() == 10)
    {
        e.SetValue(m_AccessNumber);
    }
    else if (e.GetIndex() == 11)
    {
        e.SetValue(m_Status);
    }
    else if (e.GetIndex() == 12)
    {
        e.SetValue(m_Alarm);
    }
    else
    {
        if (m_Version > 0)
        {
            if (e.GetIndex() == 13)
            {
                e.SetValue(m_Configuration);
            }
            if (e.GetIndex() == 14)
            {
                e.SetValue(m_EncryptionKeyStatus);
            }
        }
        else
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int CGXDLMSMBusClient::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_MBusPortReference.clear();
        GXHelpers::GetLogicalName(e.GetValue().byteArr, m_MBusPortReference);
    }
    else if (e.GetIndex() == 3)
    {
        m_CaptureDefinition.clear();
        CGXDLMSVariant tmp1, tmp2;
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            CGXDLMSClient::ChangeType((*it).Arr[0], DLMS_DATA_TYPE_OCTET_STRING, tmp1);
            CGXDLMSClient::ChangeType((*it).Arr[1], DLMS_DATA_TYPE_OCTET_STRING, tmp2);
            m_CaptureDefinition.push_back(std::pair<std::string, std::string>(tmp1.ToString(), tmp2.ToString()));
        }
    }
    else if (e.GetIndex() == 4)
    {
        m_CapturePeriod = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_PrimaryAddress = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 6)
    {
        m_IdentificationNumber = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 7)
    {
        m_ManufacturerID = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 8)
    {
        m_DataHeaderVersion = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 9)
    {
        m_DeviceType = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 10)
    {
        m_AccessNumber = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 11)
    {
        m_Status = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 12)
    {
        m_Alarm = e.GetValue().ToInteger();
    }
    else
    {
        if (m_Version > 0)
        {
            if (e.GetIndex() == 13)
            {
                m_Configuration = e.GetValue().ToInteger();
            }
            else if (e.GetIndex() == 14)
            {
                m_EncryptionKeyStatus = (DLMS_MBUS_ENCRYPTION_KEY_STATUS)e.GetValue().ToInteger();
            }
        }
        else
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
    }
    return DLMS_ERROR_CODE_OK;
}
