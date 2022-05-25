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

#include "../include/GXDLMSMessageHandler.h"
#include "../include/GXDLMSClient.h"
#include <sstream>

//Constructor.
CGXDLMSMessageHandler::CGXDLMSMessageHandler() :
    CGXDLMSMessageHandler("", 0)
{
}

//SN Constructor.
CGXDLMSMessageHandler::CGXDLMSMessageHandler(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_MESSAGE_HANDLER, ln, sn)
{

}

//LN Constructor.
CGXDLMSMessageHandler::CGXDLMSMessageHandler(std::string ln) :
    CGXDLMSMessageHandler(ln, 0)
{

}

std::vector<std::pair<CGXDateTime, CGXDateTime> >& CGXDLMSMessageHandler::GetListeningWindow()
{
    return m_ListeningWindow;
}

std::vector<std::string>& CGXDLMSMessageHandler::GetAllowedSenders()
{
    return m_AllowedSenders;
}

std::vector<std::pair<std::string, std::pair<int, CGXDLMSScriptAction> > >& CGXDLMSMessageHandler::GetSendersAndActions()
{
    return m_SendersAndActions;
}

// Returns amount of attributes.
int CGXDLMSMessageHandler::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSMessageHandler::GetMethodCount()
{
    return 0;
}

void CGXDLMSMessageHandler::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);

    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<std::pair<CGXDateTime, CGXDateTime> >::iterator it = m_ListeningWindow.begin(); it != m_ListeningWindow.end(); ++it)
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
    sb.str(std::string());

    sb << '[';
    empty = true;
    for (std::vector<std::string>::iterator it = m_AllowedSenders.begin(); it != m_AllowedSenders.end(); ++it)
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

    sb.str(std::string());
    sb << '[';
    empty = true;
    for (std::vector<std::pair<std::string, std::pair<int, CGXDLMSScriptAction> > >::iterator it = m_SendersAndActions.begin(); it != m_SendersAndActions.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        sb.write(it->first.c_str(), it->first.size());
    }
    sb << ']';
    values.push_back(sb.str());
}

void CGXDLMSMessageHandler::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ListeningWindow
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //AllowedSenders
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //SendersAndActions
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
}

int CGXDLMSMessageHandler::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    //ListeningWindow
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    //AllowedSenders
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    //SendersAndActions
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Returns value of given attribute.
int CGXDLMSMessageHandler::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
        //TODO: e.SetValue(m_ListeningWindow);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        //TODO: e.SetValue(m_AllowedSenders);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        //TODO: e.SetValue(m_SendersAndActions);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSMessageHandler::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_ListeningWindow.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                CGXDLMSVariant tmp;
                CGXDLMSClient::ChangeType(it->Arr[0], DLMS_DATA_TYPE_DATETIME, tmp);
                CGXDateTime start = tmp.dateTime;
                CGXDLMSClient::ChangeType(it->Arr[1], DLMS_DATA_TYPE_DATETIME, tmp);
                CGXDateTime end = tmp.dateTime;
                m_ListeningWindow.push_back(std::pair<CGXDateTime, CGXDateTime>(start, end));
            }
        }

    }
    else if (e.GetIndex() == 3)
    {
        m_AllowedSenders.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                std::string str;
                str.append(reinterpret_cast<char const*>(it->byteArr), it->size);
                m_AllowedSenders.push_back(str);
            }
        }
    }
    else if (e.GetIndex() == 4)
    {
        m_SendersAndActions.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                //std::string id = it->Arr[0].byteArr.ToString();
                //Object[] tmp2 = (Object[]) tmp[1];
                /*TODO:
                KeyValuePair<int, GXDLMSScriptAction> executed_script = new KeyValuePair<int, GXDLMSScriptAction>(Convert.ToInt32(tmp2[1], tmp2[2]));
                m_SendersAndActions.Add(new KeyValuePair<std::string, KeyValuePair<int, GXDLMSScriptAction>>(id, tmp[1] as GXDateTime));
                 * */
            }
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
