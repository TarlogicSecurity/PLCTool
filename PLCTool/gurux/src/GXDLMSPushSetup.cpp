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

#include "../include/GXDLMSPushSetup.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXDLMSObjectFactory.h"
#include <sstream>

//Constructor.
CGXDLMSPushSetup::CGXDLMSPushSetup() :
    CGXDLMSPushSetup("0.7.25.9.0.255", 0)
{
}

//SN Constructor.
CGXDLMSPushSetup::CGXDLMSPushSetup(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_PUSH_SETUP, ln, sn)
{
    m_RandomisationStartInterval = m_NumberOfRetries = m_RepetitionDelay = 0;
    GXHelpers::SetLogicalName("0.7.25.9.0.255", m_LN);
    m_Service = DLMS_SERVICE_TYPE_TCP;
    m_Message = DLMS_MESSAGE_TYPE_COSEM_APDU;
}

//LN Constructor.
CGXDLMSPushSetup::CGXDLMSPushSetup(std::string ln) :
    CGXDLMSPushSetup(ln, 0)
{
}

DLMS_SERVICE_TYPE CGXDLMSPushSetup::GetService()
{
    return m_Service;
}
void CGXDLMSPushSetup::SetService(DLMS_SERVICE_TYPE value)
{
    m_Service = value;
}

std::string& CGXDLMSPushSetup::GetDestination()
{
    return m_Destination;
}
void CGXDLMSPushSetup::SetDestination(std::string& value)
{
    m_Destination = value;
}

DLMS_MESSAGE_TYPE CGXDLMSPushSetup::GetMessage()
{
    return m_Message;
}
void CGXDLMSPushSetup::SetMessage(DLMS_MESSAGE_TYPE value)
{
    m_Message = value;
}
// Returns amount of attributes.
int CGXDLMSPushSetup::GetAttributeCount()
{
    return 7;
}

// Returns amount of methods.
int CGXDLMSPushSetup::GetMethodCount()
{
    return 1;
}

void CGXDLMSPushSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);

    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject> >::iterator it = m_PushObjectList.begin(); it != m_PushObjectList.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str;
        it->first->GetLogicalName(str);
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());

    sb.str(std::string());
    sb << GetService();
    sb << " ";
    sb << GetDestination().c_str();
    sb << " ";
    sb << GetMessage();
    values.push_back(sb.str());

    sb.str(std::string());
    sb << '[';
    empty = true;
    for (std::vector<std::pair<CGXDateTime, CGXDateTime> >::iterator it = m_CommunicationWindow.begin(); it != m_CommunicationWindow.end(); ++it)
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

    values.push_back(CGXDLMSVariant(m_RandomisationStartInterval).ToString());
    values.push_back(CGXDLMSVariant(m_NumberOfRetries).ToString());
    values.push_back(CGXDLMSVariant(m_RepetitionDelay).ToString());
}

void CGXDLMSPushSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //PushObjectList
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }

    //SendDestinationAndMethod
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //CommunicationWindow
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    //RandomisationStartInterval
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    //NumberOfRetries
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    //RepetitionDelay
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
}

int CGXDLMSPushSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 6)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 7)
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
int CGXDLMSPushSetup::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    CGXDLMSVariant tmp;
    CGXByteBuffer buff;
    int ret;
    if (e.GetIndex() == 1)
    {
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 2)
    {
        e.SetByteArray(true);
        buff.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount((unsigned long)m_PushObjectList.size(), buff);
        for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject> >::iterator it = m_PushObjectList.begin(); it != m_PushObjectList.end(); ++it)
        {
            buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            buff.SetUInt8(4);

            tmp = it->first->GetObjectType();
            if ((ret = GXHelpers::SetData(buff, DLMS_DATA_TYPE_UINT16, tmp)) != 0)
            {
                return ret;
            }
            tmp.Clear();
            if ((ret = GetLogicalName(it->first, tmp)) != 0)
            {
                return ret;
            }
            if ((ret = GXHelpers::SetData(buff, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
            tmp = (it->second).GetAttributeIndex();
            if ((ret = GXHelpers::SetData(buff, DLMS_DATA_TYPE_INT8, tmp)) != 0)
            {
                return ret;
            }
            tmp = (it->second).GetDataIndex();
            if ((ret = GXHelpers::SetData(buff, DLMS_DATA_TYPE_UINT16, tmp)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(buff);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        buff.SetUInt8(3);
        tmp = GetService();
        if ((ret = GXHelpers::SetData(buff, DLMS_DATA_TYPE_ENUM, tmp)) != 0)
        {
            return ret;
        }

        CGXByteBuffer bb;
        bb.AddString(GetDestination().c_str());
        tmp = bb;
        if ((ret = GXHelpers::SetData(buff, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
        {
            return ret;
        }

        tmp = GetMessage();
        if ((ret = GXHelpers::SetData(buff, DLMS_DATA_TYPE_ENUM, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(buff);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        buff.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount((unsigned long)m_CommunicationWindow.size(), buff);
        for (std::vector<std::pair< CGXDateTime, CGXDateTime> >::iterator it = m_CommunicationWindow.begin(); it != m_CommunicationWindow.end(); ++it)
        {
            buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            buff.SetUInt8(2);
            tmp = it->first;
            if ((ret = GXHelpers::SetData(buff, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }

            tmp = it->second;
            if ((ret = GXHelpers::SetData(buff, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(buff);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        e.SetValue(m_RandomisationStartInterval);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        e.SetValue(m_NumberOfRetries);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 7)
    {
        e.SetValue(m_RepetitionDelay);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSPushSetup::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        std::string ln;
        m_PushObjectList.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                DLMS_OBJECT_TYPE type = (DLMS_OBJECT_TYPE)it->Arr[0].ToInteger();
                GXHelpers::GetLogicalName(it->Arr[1].byteArr, ln);
                CGXDLMSObject* obj = settings.GetObjects().FindByLN(type, ln);
                if (obj == NULL)
                {
                    obj = CGXDLMSObjectFactory::CreateObject(type);
                    if (obj != NULL)
                    {
                        settings.AddAllocateObject(obj);
                        CGXDLMSObject::SetLogicalName(obj, it->Arr[1]);
                    }
                }
                if (obj != NULL)
                {
                    CGXDLMSCaptureObject co(it->Arr[2].ToInteger(), it->Arr[3].ToInteger());
                    m_PushObjectList.push_back(std::pair<CGXDLMSObject*, CGXDLMSCaptureObject>(obj, co));
                }
            }
        }
    }
    else if (e.GetIndex() == 3)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRUCTURE)
        {
            SetService((DLMS_SERVICE_TYPE)e.GetValue().Arr[0].ToInteger());
            std::string str;
            str.append(reinterpret_cast<char const*>(e.GetValue().Arr[1].byteArr), e.GetValue().Arr[1].size);
            SetDestination(str);
            SetMessage((DLMS_MESSAGE_TYPE)e.GetValue().Arr[2].ToInteger());
        }
    }
    else if (e.GetIndex() == 4)
    {
        m_CommunicationWindow.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            int ret;
            for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                CGXDLMSVariant tmp;
                if ((ret = CGXDLMSClient::ChangeType(it->Arr[0], DLMS_DATA_TYPE_DATETIME, tmp)) != 0)
                {
                    return ret;
                }
                CGXDateTime start = tmp.dateTime;
                if ((ret = CGXDLMSClient::ChangeType(it->Arr[1], DLMS_DATA_TYPE_DATETIME, tmp)) != 0)
                {
                    return ret;
                }
                CGXDateTime end = tmp.dateTime;
                m_CommunicationWindow.push_back(std::pair<CGXDateTime, CGXDateTime>(start, end));
            }
        }
    }
    else if (e.GetIndex() == 5)
    {
        m_RandomisationStartInterval = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 6)
    {
        m_NumberOfRetries = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 7)
    {
        m_RepetitionDelay = e.GetValue().ToInteger();
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
