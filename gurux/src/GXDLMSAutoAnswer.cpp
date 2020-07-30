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
#include "../include/GXDLMSAutoAnswer.h"
#include <sstream>

CGXDLMSAutoAnswer::CGXDLMSAutoAnswer() :
    CGXDLMSAutoAnswer("0.0.2.2.0.255", 0)
{
}

CGXDLMSAutoAnswer::CGXDLMSAutoAnswer(std::string ln) :
    CGXDLMSAutoAnswer(ln, 0)
{
}

CGXDLMSAutoAnswer::CGXDLMSAutoAnswer(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_AUTO_ANSWER, ln, sn)
{
    m_NumberOfRingsInListeningWindow = m_NumberOfRingsOutListeningWindow = 0;
    m_Mode = DLMS_AUTO_ANSWER_MODE_DEVICE;
    m_Status = AUTO_ANSWER_STATUS_INACTIVE;
    m_NumberOfCalls = 0;
}

DLMS_AUTO_ANSWER_MODE CGXDLMSAutoAnswer::GetMode()
{
    return m_Mode;
}
void CGXDLMSAutoAnswer::SetMode(DLMS_AUTO_ANSWER_MODE value)
{
    m_Mode = value;
}

std::vector<std::pair< CGXDateTime, CGXDateTime> >& CGXDLMSAutoAnswer::GetListeningWindow()
{
    return m_ListeningWindow;
}
void CGXDLMSAutoAnswer::SetListeningWindow(std::vector<std::pair< CGXDateTime, CGXDateTime> >& value)
{
    m_ListeningWindow = value;
}

AUTO_ANSWER_STATUS CGXDLMSAutoAnswer::GetStatus()
{
    return m_Status;
}
void CGXDLMSAutoAnswer::SetStatus(AUTO_ANSWER_STATUS value)
{
    m_Status = value;
}

int CGXDLMSAutoAnswer::GetNumberOfCalls()
{
    return m_NumberOfCalls;
}
void CGXDLMSAutoAnswer::SetNumberOfCalls(int value)
{
    m_NumberOfCalls = value;
}

// Number of rings within the window defined by ListeningWindow.
int CGXDLMSAutoAnswer::GetNumberOfRingsInListeningWindow()
{
    return m_NumberOfRingsInListeningWindow;
}
void CGXDLMSAutoAnswer::SetNumberOfRingsInListeningWindow(int value)
{
    m_NumberOfRingsInListeningWindow = value;
}

//Number of rings outside the window defined by ListeningWindow.
int CGXDLMSAutoAnswer::GetNumberOfRingsOutListeningWindow()
{
    return m_NumberOfRingsOutListeningWindow;
}
void CGXDLMSAutoAnswer::SetNumberOfRingsOutListeningWindow(int value)
{
    m_NumberOfRingsOutListeningWindow = value;
}

// Returns amount of attributes.
int CGXDLMSAutoAnswer::GetAttributeCount()
{
    return 6;
}

// Returns amount of methods.
int CGXDLMSAutoAnswer::GetMethodCount()
{
    return 0;
}

void CGXDLMSAutoAnswer::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSVariant(m_Mode).ToString());
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<std::pair< CGXDateTime, CGXDateTime> >::iterator it = m_ListeningWindow.begin(); it != m_ListeningWindow.end(); ++it)
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
    values.push_back(CGXDLMSVariant(m_Status).ToString());
    values.push_back(CGXDLMSVariant(m_NumberOfCalls).ToString());
    //Clean
    sb.str(std::string());
    sb << m_NumberOfRingsInListeningWindow;
    sb << "/";
    sb << m_NumberOfRingsOutListeningWindow;
    values.push_back(sb.str());
}

void CGXDLMSAutoAnswer::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Mode is static and read only once.
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //ListeningWindow is static and read only once.
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //Status is not static.
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }

    //NumberOfCalls is static and read only once.
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
    //NumberOfRingsInListeningWindow is static and read only once.
    if (all || !IsRead(6))
    {
        attributes.push_back(6);
    }
}

int CGXDLMSAutoAnswer::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_ENUM;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_UINT8;
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
int CGXDLMSAutoAnswer::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
        e.SetValue(GetMode());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        int ret;
        unsigned long cnt = (unsigned long)m_ListeningWindow.size();
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        GXHelpers::SetObjectCount(cnt, data);
        CGXDLMSVariant f, s;
        for (std::vector<std::pair<CGXDateTime, CGXDateTime> >::iterator it =
            m_ListeningWindow.begin(); it != m_ListeningWindow.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2); //Count
            f = it->first;
            s = it->second;
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, f)) != 0 || //start_time
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, s)) != 0) //end_time
            {
                return ret;
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetValue(GetStatus());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        e.SetValue(GetNumberOfCalls());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        e.SetByteArray(true);
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        GXHelpers::SetObjectCount(2, data);
        CGXDLMSVariant in = m_NumberOfRingsInListeningWindow;
        CGXDLMSVariant out = m_NumberOfRingsOutListeningWindow;
        int ret;
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, in)) != DLMS_ERROR_CODE_OK ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, out)) != DLMS_ERROR_CODE_OK)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

/*
 * Set value of given attribute.
 */
int CGXDLMSAutoAnswer::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    if (e.GetIndex() == 2)
    {
        SetMode((DLMS_AUTO_ANSWER_MODE)e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        m_ListeningWindow.clear();
        for (std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
        {
            CGXDLMSVariant start, end;
            CGXDLMSClient::ChangeType((*item).Arr[0], DLMS_DATA_TYPE_DATETIME, start);
            CGXDLMSClient::ChangeType((*item).Arr[1], DLMS_DATA_TYPE_DATETIME, end);
            m_ListeningWindow.push_back(std::pair< CGXDateTime, CGXDateTime>(start.dateTime, end.dateTime));
        }
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        SetStatus((AUTO_ANSWER_STATUS)e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        SetNumberOfCalls(e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        m_NumberOfRingsInListeningWindow = m_NumberOfRingsOutListeningWindow = 0;
        if (e.GetValue().vt != DLMS_DATA_TYPE_NONE)
        {
            m_NumberOfRingsInListeningWindow = e.GetValue().Arr[0].ToInteger();
            m_NumberOfRingsOutListeningWindow = e.GetValue().Arr[1].ToInteger();
        }
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}
