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
#include "../include/GXDLMSModemConfiguration.h"
#include "../include/GXDLMSConverter.h"
#include <sstream>

CGXDLMSModemConfiguration::CGXDLMSModemConfiguration() :
    CGXDLMSModemConfiguration("0.0.2.0.0.255", 0)
{
}

CGXDLMSModemConfiguration::CGXDLMSModemConfiguration(std::string ln) :
    CGXDLMSModemConfiguration(ln, 0)
{
}

CGXDLMSModemConfiguration::CGXDLMSModemConfiguration(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_MODEM_CONFIGURATION, ln, sn)
{
    m_CommunicationSpeed = DLMS_BAUD_RATE_9600;
    m_ModemProfile.push_back("OK");
    m_ModemProfile.push_back("CONNECT");
    m_ModemProfile.push_back("RING");
    m_ModemProfile.push_back("NO CARRIER");
    m_ModemProfile.push_back("ERROR");
    m_ModemProfile.push_back("CONNECT 1200");
    m_ModemProfile.push_back("NO DIAL TONE");
    m_ModemProfile.push_back("BUSY");
    m_ModemProfile.push_back("NO ANSWER");
    m_ModemProfile.push_back("CONNECT 600");
    m_ModemProfile.push_back("CONNECT 2400");
    m_ModemProfile.push_back("CONNECT 4800");
    m_ModemProfile.push_back("CONNECT 9600");
    m_ModemProfile.push_back("CONNECT 14 400");
    m_ModemProfile.push_back("CONNECT 28 800");
    m_ModemProfile.push_back("CONNECT 33 600");
    m_ModemProfile.push_back("CONNECT 56 000");
}

DLMS_BAUD_RATE CGXDLMSModemConfiguration::GetCommunicationSpeed()
{
    return m_CommunicationSpeed;
}
void CGXDLMSModemConfiguration::SetCommunicationSpeed(DLMS_BAUD_RATE value)
{
    m_CommunicationSpeed = value;
}

std::vector<CGXDLMSModemInitialisation>& CGXDLMSModemConfiguration::GetInitialisationStrings()
{
    return m_InitialisationStrings;
}
void CGXDLMSModemConfiguration::SetInitialisationStrings(std::vector<CGXDLMSModemInitialisation>& value)
{
    m_InitialisationStrings = value;
}

std::vector< std::string >& CGXDLMSModemConfiguration::GetModemProfile()
{
    return m_ModemProfile;
}

void CGXDLMSModemConfiguration::SetModemProfile(std::vector< std::string >& value)
{
    m_ModemProfile = value;
}

// Returns amount of attributes.
int CGXDLMSModemConfiguration::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSModemConfiguration::GetMethodCount()
{
    return 0;
}

void CGXDLMSModemConfiguration::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSConverter::ToString(m_CommunicationSpeed));
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<CGXDLMSModemInitialisation>::iterator it = m_InitialisationStrings.begin(); it != m_InitialisationStrings.end(); ++it)
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
    GXHelpers::Join(", ", m_ModemProfile, ln);
    sb << ln;
    sb << ']';
    values.push_back(sb.str());

}

void CGXDLMSModemConfiguration::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
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
    //InitialisationStrings
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //ModemProfile
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
}

int CGXDLMSModemConfiguration::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSModemConfiguration::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
        e.SetValue(m_CommunicationSpeed);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        int ret;
        unsigned long cnt = (unsigned long)m_InitialisationStrings.size();
        GXHelpers::SetObjectCount(cnt, data);
        CGXDLMSVariant request, response, delay;
        for (std::vector<CGXDLMSModemInitialisation>::iterator it = m_InitialisationStrings.begin();
            it != m_InitialisationStrings.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3); //Count
            request = it->GetRequest();
            response = it->GetResponse();
            delay = it->GetDelay();
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, request)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, response)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, delay)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        int ret;
        unsigned long cnt = (unsigned long)m_ModemProfile.size();
        GXHelpers::SetObjectCount(cnt, data);
        CGXDLMSVariant tmp;
        for (std::vector< std::string >::iterator it = m_ModemProfile.begin();
            it != m_ModemProfile.end(); ++it)
        {
            tmp = *it;
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSModemConfiguration::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_CommunicationSpeed = (DLMS_BAUD_RATE)e.GetValue().bVal;
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 3)
    {
        m_InitialisationStrings.clear();
        int ret;
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            CGXDLMSModemInitialisation item;
            CGXDLMSVariant tmp;
            if ((ret = CGXDLMSClient::ChangeType(it->Arr[0], DLMS_DATA_TYPE_STRING, tmp)) != DLMS_ERROR_CODE_OK)
            {
                return ret;
            }
            item.SetRequest(tmp.ToString());
            if ((ret = CGXDLMSClient::ChangeType(it->Arr[1], DLMS_DATA_TYPE_STRING, tmp)) != DLMS_ERROR_CODE_OK)
            {
                return ret;
            }
            item.SetResponse(tmp.ToString());
            if (it->Arr.size() > 2)
            {
                item.SetDelay(it->Arr[2].uiVal);
            }
            m_InitialisationStrings.push_back(item);
        }
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 4)
    {
        m_ModemProfile.clear();
        int ret;
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            CGXDLMSVariant tmp;
            if ((ret = CGXDLMSClient::ChangeType(*it, DLMS_DATA_TYPE_STRING, tmp)) != DLMS_ERROR_CODE_OK)
            {
                return ret;
            }
            m_ModemProfile.push_back(tmp.ToString());
        }
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}
