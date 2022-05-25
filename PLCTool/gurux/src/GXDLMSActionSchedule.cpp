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
#include "../include/GXDLMSActionSchedule.h"
#include <sstream>

/**
 Constructor.
*/
CGXDLMSActionSchedule::CGXDLMSActionSchedule() :
    CGXDLMSActionSchedule("0.0.15.0.0.255", 0)
{
}

CGXDLMSActionSchedule::CGXDLMSActionSchedule(std::string ln) :
    CGXDLMSActionSchedule(ln, 0)
{
}

CGXDLMSActionSchedule::CGXDLMSActionSchedule(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_ACTION_SCHEDULE, ln, sn)
{
    m_ExecutedScriptSelector = 0;
    m_Type = DLMS_SINGLE_ACTION_SCHEDULE_TYPE1;
}

std::string& CGXDLMSActionSchedule::GetExecutedScriptLogicalName()
{
    return m_ExecutedScriptLogicalName;
}
void CGXDLMSActionSchedule::SetExecutedScriptLogicalName(std::string value)
{
    m_ExecutedScriptLogicalName = value;
}

int CGXDLMSActionSchedule::GetExecutedScriptSelector()
{
    return m_ExecutedScriptSelector;
}
void CGXDLMSActionSchedule::SetExecutedScriptSelector(int value)
{
    m_ExecutedScriptSelector = value;
}

DLMS_SINGLE_ACTION_SCHEDULE_TYPE CGXDLMSActionSchedule::GetType()
{
    return m_Type;
}
void CGXDLMSActionSchedule::SetType(DLMS_SINGLE_ACTION_SCHEDULE_TYPE value)
{
    m_Type = value;
}

std::vector<CGXDateTime>& CGXDLMSActionSchedule::GetExecutionTime()
{
    return m_ExecutionTime;
}
void CGXDLMSActionSchedule::SetExecutionTime(std::vector<CGXDateTime>& value)
{
    m_ExecutionTime = value;
}

// Returns amount of attributes.
int CGXDLMSActionSchedule::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSActionSchedule::GetMethodCount()
{
    return 0;
}

void CGXDLMSActionSchedule::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_ExecutedScriptLogicalName + " " + CGXDLMSVariant(m_ExecutedScriptSelector).ToString());
    values.push_back(CGXDLMSVariant(m_Type).ToString());
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<CGXDateTime>::iterator it = m_ExecutionTime.begin(); it != m_ExecutionTime.end(); ++it)
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
}

void CGXDLMSActionSchedule::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ExecutedScriptLogicalName is static and read only once.
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //Type is static and read only once.
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //ExecutionTime is static and read only once.
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
}

int CGXDLMSActionSchedule::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_ENUM;
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
int CGXDLMSActionSchedule::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    CGXByteBuffer data;
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
        e.SetByteArray(true);
        int ret;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        CGXDLMSVariant ln;
        GXHelpers::SetLogicalName(m_ExecutedScriptLogicalName.c_str(), ln);
        CGXDLMSVariant ss = m_ExecutedScriptSelector;
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, ss)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetValue(GetType());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        int ret;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount((unsigned long)GetExecutionTime().size(), data);
        CGXDLMSVariant val1, val2;
        for (std::vector<CGXDateTime>::iterator it = m_ExecutionTime.begin(); it != m_ExecutionTime.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            //Count
            data.SetUInt8(2);
            CGXTime t(*it);
            CGXDate d(*it);
            val1 = t;
            val2 = d;
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, val1)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, val2)) != 0)
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
int CGXDLMSActionSchedule::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        GXHelpers::GetLogicalName(e.GetValue().Arr[0].byteArr, m_ExecutedScriptLogicalName);
        SetExecutedScriptSelector(e.GetValue().Arr[1].ToInteger());
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 3)
    {
        SetType((DLMS_SINGLE_ACTION_SCHEDULE_TYPE)e.GetValue().ToInteger());
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 4)
    {
        int ret;
        m_ExecutionTime.clear();
        CGXDLMSVariant time, date;
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin();
            it != e.GetValue().Arr.end(); ++it)
        {
            if ((ret = CGXDLMSClient::ChangeType((*it).Arr[0], DLMS_DATA_TYPE_TIME, time)) != 0 ||
                (ret = CGXDLMSClient::ChangeType((*it).Arr[1], DLMS_DATA_TYPE_DATE, date)) != 0)
            {
                return ret;
            }
            time.dateTime.SetSkip((DATETIME_SKIPS)(time.dateTime.GetSkip() & ~(DATETIME_SKIPS_YEAR | DATETIME_SKIPS_MONTH | DATETIME_SKIPS_DAY | DATETIME_SKIPS_DAYOFWEEK)));
            date.dateTime.SetSkip((DATETIME_SKIPS)(date.dateTime.GetSkip() & ~(DATETIME_SKIPS_HOUR | DATETIME_SKIPS_MINUTE | DATETIME_SKIPS_SECOND | DATETIME_SKIPS_MS)));
            struct tm val = time.dateTime.GetValue();
            struct tm val2 = date.dateTime.GetValue();
            val2.tm_hour = val.tm_hour;
            val2.tm_min = val.tm_min;
            val2.tm_sec = val.tm_sec;
            CGXDateTime tmp(val2);
            tmp.SetSkip((DATETIME_SKIPS)(time.dateTime.GetSkip() | date.dateTime.GetSkip()));
            tmp.SetExtra((DATE_TIME_EXTRA_INFO)(time.dateTime.GetExtra() | date.dateTime.GetExtra()));
            m_ExecutionTime.push_back(tmp);
        }
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}
