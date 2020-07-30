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
#include "../include/GXDLMSAutoConnect.h"
#include <sstream>

//Constructor.
CGXDLMSAutoConnect::CGXDLMSAutoConnect() :
    CGXDLMSAutoConnect("0.0.2.1.0.255", 0)
{
}

//SN Constructor.
CGXDLMSAutoConnect::CGXDLMSAutoConnect(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_AUTO_CONNECT, ln, sn)
{
    m_Mode = DLMS_AUTO_CONNECT_MODE_NO_AUTO_CONNECT;
    m_RepetitionDelay = m_Repetitions = 0;
    m_Version = 2;
}

//LN Constructor.
CGXDLMSAutoConnect::CGXDLMSAutoConnect(std::string ln) :
    CGXDLMSAutoConnect(ln, 0)
{
}

DLMS_AUTO_CONNECT_MODE CGXDLMSAutoConnect::GetMode()
{
    return m_Mode;
}
void CGXDLMSAutoConnect::SetMode(DLMS_AUTO_CONNECT_MODE value)
{
    m_Mode = value;
}

int CGXDLMSAutoConnect::GetRepetitions()
{
    return m_Repetitions;
}
void CGXDLMSAutoConnect::SetRepetitions(int value)
{
    m_Repetitions = value;
}

int CGXDLMSAutoConnect::GetRepetitionDelay()
{
    return m_RepetitionDelay;
}
void CGXDLMSAutoConnect::SetRepetitionDelay(int value)
{
    m_RepetitionDelay = value;
}

std::vector<std::pair< CGXDateTime, CGXDateTime> >& CGXDLMSAutoConnect::GetCallingWindow()
{
    return m_CallingWindow;
}
void CGXDLMSAutoConnect::SetCallingWindow(std::vector<std::pair< CGXDateTime, CGXDateTime> > value)
{
    m_CallingWindow = value;
}

std::vector< std::string >& CGXDLMSAutoConnect::GetDestinations()
{
    return m_Destinations;
}

void CGXDLMSAutoConnect::SetDestinations(std::vector< std::string >& value)
{
    m_Destinations = value;
}

int CGXDLMSAutoConnect::Connect(CGXDLMSClient* client, std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

// Returns amount of attributes.
int CGXDLMSAutoConnect::GetAttributeCount()
{
    return 6;
}

// Returns amount of methods.
int CGXDLMSAutoConnect::GetMethodCount()
{
    return 1;
}

void CGXDLMSAutoConnect::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSVariant(m_Mode).ToString());
    values.push_back(CGXDLMSVariant(m_Repetitions).ToString());
    values.push_back(CGXDLMSVariant(m_RepetitionDelay).ToString());
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<std::pair< CGXDateTime, CGXDateTime> >::iterator it = m_CallingWindow.begin(); it != m_CallingWindow.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = it->first.ToString();
        sb.write(str.c_str(), str.size());
        sb << " ";
        str = it->second.ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());

    //Clear str.
    sb.str(std::string());
    sb << '[';
    empty = true;
    for (std::vector< std::string >::iterator it = m_Destinations.begin(); it != m_Destinations.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        sb.write(it->c_str(), it->size());
    }
    sb << ']';
    values.push_back(sb.str());
}

void CGXDLMSAutoConnect::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Mode
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //Repetitions
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //RepetitionDelay
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    //CallingWindow
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    //Destinations
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
}

int CGXDLMSAutoConnect::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ENUM;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_UINT8;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_UINT16;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 6)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Returns value of given attribute.
int CGXDLMSAutoConnect::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
        e.SetValue((unsigned char)GetMode());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetValue(GetRepetitions());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetValue(GetRepetitionDelay());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        e.SetByteArray(true);
        unsigned long cnt = (unsigned long)m_CallingWindow.size();
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        int ret;
        //Add count
        GXHelpers::SetObjectCount(cnt, data);
        if (cnt != 0)
        {
            CGXDLMSVariant s, e;
            for (std::vector<std::pair< CGXDateTime, CGXDateTime> >::iterator it = m_CallingWindow.begin(); it != m_CallingWindow.end(); ++it)
            {
                data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
                data.SetUInt8(2); //Count
                s = it->first;
                e = it->second;
                if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, s)) != 0 || //start_time
                    (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, e)) != 0) //end_time
                {
                    return ret;
                }
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        e.SetByteArray(true);
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        int ret;
        unsigned long cnt = (unsigned long)m_Destinations.size();
        //Add count
        GXHelpers::SetObjectCount(cnt, data);
        for (std::vector< std::string >::iterator it = m_Destinations.begin(); it != m_Destinations.end(); ++it)
        {
            CGXDLMSVariant value;
            e.GetValue().Add(&(*it)[0], (int)it->size());
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, value)) != 0) //destination
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
int CGXDLMSAutoConnect::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        SetMode((DLMS_AUTO_CONNECT_MODE)e.GetValue().ToInteger());
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 3)
    {
        SetRepetitions(e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 4)
    {
        SetRepetitionDelay(e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 5)
    {
        m_CallingWindow.clear();
        for (std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
        {
            CGXDLMSVariant tmp;
            CGXDLMSClient::ChangeType(item->Arr[0], DLMS_DATA_TYPE_DATETIME, tmp);
            CGXDateTime start = tmp.dateTime;
            CGXDLMSClient::ChangeType(item->Arr[1], DLMS_DATA_TYPE_DATETIME, tmp);
            CGXDateTime end = tmp.dateTime;
            m_CallingWindow.push_back(std::make_pair(start, end));
        }
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 6)
    {
        m_Destinations.clear();
        std::vector< std::string > items;
        for (std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
        {
            CGXDLMSVariant value;
            CGXDLMSClient::ChangeType(*item, DLMS_DATA_TYPE_STRING, value);
            items.push_back(e.GetValue().ToString());
        }
        SetDestinations(items);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}
