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
#include "../include/GXDLMSActivityCalendar.h"
#include <sstream>

CGXDLMSActivityCalendar::CGXDLMSActivityCalendar() :
    CGXDLMSActivityCalendar("0.0.13.0.0.255", 0)
{
}

CGXDLMSActivityCalendar::CGXDLMSActivityCalendar(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR, ln, sn)
{

}

CGXDLMSActivityCalendar::CGXDLMSActivityCalendar(std::string ln) :
    CGXDLMSActivityCalendar(ln, 0)
{

}

CGXDLMSActivityCalendar::~CGXDLMSActivityCalendar()
{
    for (std::vector<CGXDLMSSeasonProfile*>::iterator it = m_SeasonProfileActive.begin(); it != m_SeasonProfileActive.end(); ++it)
    {
        delete *it;
    }
    m_SeasonProfileActive.clear();
    for (std::vector<CGXDLMSWeekProfile*>::iterator it = m_WeekProfileTableActive.begin(); it != m_WeekProfileTableActive.end(); ++it)
    {
        delete *it;
    }
    m_WeekProfileTableActive.clear();
    for (std::vector<CGXDLMSDayProfile*>::iterator it = m_DayProfileTableActive.begin(); it != m_DayProfileTableActive.end(); ++it)
    {
        delete *it;
    }
    m_DayProfileTableActive.clear();
    for (std::vector<CGXDLMSSeasonProfile*>::iterator it = m_SeasonProfilePassive.begin(); it != m_SeasonProfilePassive.end(); ++it)
    {
        delete *it;
    }
    m_SeasonProfilePassive.clear();
    for (std::vector<CGXDLMSWeekProfile*>::iterator it = m_WeekProfileTablePassive.begin(); it != m_WeekProfileTablePassive.end(); ++it)
    {
        delete *it;
    }
    m_WeekProfileTablePassive.clear();
    for (std::vector<CGXDLMSDayProfile*>::iterator it = m_DayProfileTablePassive.begin(); it != m_DayProfileTablePassive.end(); ++it)
    {
        delete *it;
    }
    m_DayProfileTablePassive.clear();
}


std::string CGXDLMSActivityCalendar::GetCalendarNameActive()
{
    return m_CalendarNameActive;
}
void CGXDLMSActivityCalendar::SetCalendarNameActive(std::string value)
{
    m_CalendarNameActive = value;
}

std::vector<CGXDLMSSeasonProfile*>& CGXDLMSActivityCalendar::GetSeasonProfileActive()
{
    return m_SeasonProfileActive;
}
void CGXDLMSActivityCalendar::SetSeasonProfileActive(std::vector<CGXDLMSSeasonProfile*>& value)
{
    m_SeasonProfileActive = value;
}

std::vector<CGXDLMSWeekProfile*>& CGXDLMSActivityCalendar::GetWeekProfileTableActive()
{
    return m_WeekProfileTableActive;
}
void CGXDLMSActivityCalendar::SetWeekProfileTableActive(std::vector<CGXDLMSWeekProfile*>& value)
{
    m_WeekProfileTableActive = value;
}

std::vector<CGXDLMSDayProfile*>& CGXDLMSActivityCalendar::GetDayProfileTableActive()
{
    return m_DayProfileTableActive;
}
void CGXDLMSActivityCalendar::SetDayProfileTableActive(std::vector<CGXDLMSDayProfile*>& value)
{
    m_DayProfileTableActive = value;
}

std::string CGXDLMSActivityCalendar::GetCalendarNamePassive()
{
    return m_CalendarNamePassive;
}
void CGXDLMSActivityCalendar::SetCalendarNamePassive(std::string value)
{
    m_CalendarNamePassive = value;
}

std::vector<CGXDLMSSeasonProfile*>& CGXDLMSActivityCalendar::GetSeasonProfilePassive()
{
    return m_SeasonProfilePassive;
}

void CGXDLMSActivityCalendar::SetSeasonProfilePassive(std::vector<CGXDLMSSeasonProfile*>& value)
{
    m_SeasonProfilePassive = value;
}

std::vector<CGXDLMSWeekProfile*>& CGXDLMSActivityCalendar::GetWeekProfileTablePassive()
{
    return m_WeekProfileTablePassive;
}
void CGXDLMSActivityCalendar::SetWeekProfileTablePassive(std::vector<CGXDLMSWeekProfile*>& value)
{
    m_WeekProfileTablePassive = value;
}

std::vector<CGXDLMSDayProfile*>& CGXDLMSActivityCalendar::GetDayProfileTablePassive()
{
    return m_DayProfileTablePassive;
}
void CGXDLMSActivityCalendar::SetDayProfileTablePassive(std::vector<CGXDLMSDayProfile*>& value)
{
    m_DayProfileTablePassive = value;
}

CGXDateTime& CGXDLMSActivityCalendar::GetTime()
{
    return m_Time;
}
void CGXDLMSActivityCalendar::SetTime(CGXDateTime& value)
{
    m_Time = value;
}

// Returns amount of attributes.
int CGXDLMSActivityCalendar::GetAttributeCount()
{
    return 10;
}

// Returns amount of methods.
int CGXDLMSActivityCalendar::GetMethodCount()
{
    return 1;
}

void CGXDLMSActivityCalendar::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);

    values.push_back(m_CalendarNameActive);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<CGXDLMSSeasonProfile*>::iterator it = m_SeasonProfileActive.begin(); it != m_SeasonProfileActive.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = (*it)->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
    sb.str(std::string());

    sb << '[';
    empty = true;
    for (std::vector<CGXDLMSWeekProfile*>::iterator it = m_WeekProfileTableActive.begin(); it != m_WeekProfileTableActive.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = (*it)->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
    sb.str(std::string());

    sb << '[';
    empty = true;
    for (std::vector<CGXDLMSDayProfile*>::iterator it = m_DayProfileTableActive.begin(); it != m_DayProfileTableActive.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = (*it)->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
    sb.str(std::string());

    values.push_back(m_CalendarNamePassive);
    sb << '[';
    empty = true;
    for (std::vector<CGXDLMSSeasonProfile*>::iterator it = m_SeasonProfilePassive.begin(); it != m_SeasonProfilePassive.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = (*it)->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
    sb.str(std::string());

    sb << '[';
    empty = true;
    for (std::vector<CGXDLMSWeekProfile*>::iterator it = m_WeekProfileTablePassive.begin(); it != m_WeekProfileTablePassive.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = (*it)->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
    sb.str(std::string());

    sb << '[';
    empty = true;
    for (std::vector<CGXDLMSDayProfile*>::iterator it = m_DayProfileTablePassive.begin(); it != m_DayProfileTablePassive.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = (*it)->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
    sb.str(std::string());
    values.push_back(m_Time.ToString());
}

void CGXDLMSActivityCalendar::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //CalendarNameActive
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //SeasonProfileActive
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }

    //WeekProfileTableActive
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    //DayProfileTableActive
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    //CalendarNamePassive
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    //SeasonProfilePassive
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
    //WeekProfileTablePassive
    if (all || CanRead(8))
    {
        attributes.push_back(8);
    }
    //DayProfileTablePassive
    if (all || CanRead(9))
    {
        attributes.push_back(9);
    }
    //Time.
    if (all || CanRead(10))
    {
        attributes.push_back(10);
    }
}

int CGXDLMSActivityCalendar::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 6)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 7)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;

    }
    if (index == 8)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 9)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 10)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int GetSeasonProfile(std::vector<CGXDLMSSeasonProfile*>& items, CGXByteBuffer& data)
{
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    unsigned long cnt = (unsigned long)items.size();
    //Add count
    GXHelpers::SetObjectCount(cnt, data);
    CGXDLMSVariant tmp;
    for (std::vector<CGXDLMSSeasonProfile*>::iterator it = items.begin(); it != items.end(); ++it)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(3);
        tmp = (*it)->GetName();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
        tmp = (*it)->GetStart();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
        tmp = (*it)->GetWeekName();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
    }
    return 0;
}

int GetWeekProfile(std::vector<CGXDLMSWeekProfile*>& items, CGXByteBuffer& data)
{
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    unsigned long cnt = (unsigned long)items.size();
    //Add count
    GXHelpers::SetObjectCount(cnt, data);
    CGXDLMSVariant tmp;
    for (std::vector<CGXDLMSWeekProfile*>::iterator it = items.begin(); it != items.end(); ++it)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(8);
        tmp.Clear();
        tmp.Add((*it)->GetName());
        GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
        tmp = (*it)->GetMonday();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = (*it)->GetTuesday();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = (*it)->GetWednesday();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = (*it)->GetThursday();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = (*it)->GetFriday();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = (*it)->GetSaturday();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = (*it)->GetSunday();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
    }
    return 0;
}

int GetDayProfile(std::vector<CGXDLMSDayProfile*>& items, CGXByteBuffer& data)
{
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    unsigned long cnt = (unsigned long)items.size();
    //Add count
    GXHelpers::SetObjectCount(cnt, data);
    for (std::vector<CGXDLMSDayProfile*>::iterator it = items.begin(); it != items.end(); ++it)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        CGXDLMSVariant tmp2 = (*it)->GetDayId();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp2);
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        std::vector<CGXDLMSDayProfileAction*>& schedules = (*it)->GetDaySchedules();
        GXHelpers::SetObjectCount((unsigned long)schedules.size(), data);
        CGXDLMSVariant time, ln, selector;
        for (std::vector<CGXDLMSDayProfileAction*>::iterator action = schedules.begin(); action != schedules.end(); ++action)
        {
            GXHelpers::SetLogicalName((*action)->GetScriptLogicalName().c_str(), ln);
            time = (*action)->GetStartTime();
            selector = (*action)->GetScriptSelector();
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3);
            GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, time);
            GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln);
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, selector);
        }
    }
    return 0;
}

// Returns value of given attribute.
int CGXDLMSActivityCalendar::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
        CGXDLMSVariant tmp;
        tmp.Add(m_CalendarNameActive.c_str(), (int)m_CalendarNameActive.size());
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        GetSeasonProfile(m_SeasonProfileActive, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        GetWeekProfile(m_WeekProfileTableActive, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        e.SetByteArray(true);
        GetDayProfile(m_DayProfileTableActive, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        e.GetValue().Add(m_CalendarNamePassive);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 7)
    {
        e.SetByteArray(true);
        GetSeasonProfile(m_SeasonProfilePassive, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 8)
    {
        e.SetByteArray(true);
        GetWeekProfile(m_WeekProfileTablePassive, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 9)
    {
        e.SetByteArray(true);
        GetDayProfile(m_DayProfileTablePassive, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 10)
    {
        e.SetValue(GetTime());
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

void AddSeasonProfile(std::vector<CGXDLMSSeasonProfile*>& items, std::vector<CGXDLMSVariant>& list)
{
    for (std::vector<CGXDLMSSeasonProfile*>::iterator item = items.begin(); item != items.end(); ++item)
    {
        delete *item;
    }
    items.clear();
    CGXByteBuffer bb;
    CGXDLMSVariant tmp;
    for (std::vector<CGXDLMSVariant>::iterator item = list.begin(); item != list.end(); ++item)
    {
        CGXDLMSSeasonProfile *it = new CGXDLMSSeasonProfile();
        bb.Clear();
        bb.Set(item->Arr[0].byteArr, item->Arr[0].GetSize());
        it->SetName(bb);
        tmp.Clear();
        CGXDLMSClient::ChangeType((*item).Arr[1], DLMS_DATA_TYPE_DATETIME, tmp);
        it->SetStart(tmp.dateTime);
        tmp.Clear();
        bb.Clear();
        bb.Set(item->Arr[2].byteArr, item->Arr[2].GetSize());
        it->SetWeekName(bb);
        items.push_back(it);
    }
}

void AddWeekProfileTable(std::vector<CGXDLMSWeekProfile*>& items, std::vector<CGXDLMSVariant>& list)
{
    for (std::vector<CGXDLMSWeekProfile*>::iterator item = items.begin(); item != items.end(); ++item)
    {
        delete *item;
    }
    items.clear();
    CGXByteBuffer bb;
    for (std::vector<CGXDLMSVariant>::iterator item = list.begin(); item != list.end(); ++item)
    {
        CGXDLMSVariant tmp;
        CGXDLMSWeekProfile *it = new CGXDLMSWeekProfile();
        bb.Clear();
        bb.Set(item->Arr[0].byteArr, item->Arr[0].GetSize());
        it->SetName(bb);
        it->SetMonday((*item).Arr[1].lVal);
        it->SetTuesday((*item).Arr[2].lVal);
        it->SetWednesday((*item).Arr[3].lVal);
        it->SetThursday((*item).Arr[4].lVal);
        it->SetFriday((*item).Arr[5].lVal);
        it->SetSaturday((*item).Arr[6].lVal);
        it->SetSunday((*item).Arr[7].lVal);
        items.push_back(it);
    }
}

void AddDayProfileTable(std::vector<CGXDLMSDayProfile*>& items, std::vector<CGXDLMSVariant>& list)
{
    for (std::vector<CGXDLMSDayProfile*>::iterator item = items.begin(); item != items.end(); ++item)
    {
        delete *item;
    }
    items.clear();

    for (std::vector<CGXDLMSVariant>::iterator item = list.begin(); item != list.end(); ++item)
    {
        CGXDLMSDayProfile* it = new CGXDLMSDayProfile();
        it->SetDayId((*item).Arr[0].iVal);
        std::string ln;
        for (std::vector<CGXDLMSVariant>::iterator it2 = (*item).Arr[1].Arr.begin(); it2 != (*item).Arr[1].Arr.end(); ++it2)
        {
            CGXDLMSDayProfileAction * ac = new CGXDLMSDayProfileAction();
            CGXDLMSVariant tmp;
            CGXDLMSClient::ChangeType(it2->Arr[0], DLMS_DATA_TYPE_TIME, tmp);
            ac->SetStartTime((CGXTime&)tmp.dateTime);
            GXHelpers::GetLogicalName((*it2).Arr[1].byteArr, ln);
            ac->SetScriptLogicalName(ln);
            ac->SetScriptSelector((*it2).Arr[2].lVal);
            it->GetDaySchedules().push_back(ac);
        }
        items.push_back(it);
    }
}

// Set value of given attribute.
int CGXDLMSActivityCalendar::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        std::string name;
        name.append(e.GetValue().byteArr, e.GetValue().byteArr + e.GetValue().GetSize());
        SetCalendarNameActive(name);
    }
    else if (e.GetIndex() == 3)
    {
        AddSeasonProfile(m_SeasonProfileActive, e.GetValue().Arr);
    }
    else if (e.GetIndex() == 4)
    {
        AddWeekProfileTable(m_WeekProfileTableActive, e.GetValue().Arr);
    }
    else if (e.GetIndex() == 5)
    {
        AddDayProfileTable(m_DayProfileTableActive, e.GetValue().Arr);
    }
    else if (e.GetIndex() == 6)
    {
        std::string name;
        name.append(e.GetValue().byteArr, e.GetValue().byteArr + e.GetValue().GetSize());
        SetCalendarNamePassive(name);
    }
    else if (e.GetIndex() == 7)
    {
        AddSeasonProfile(m_SeasonProfilePassive, e.GetValue().Arr);
    }
    else if (e.GetIndex() == 8)
    {
        AddWeekProfileTable(m_WeekProfileTablePassive, e.GetValue().Arr);
    }
    else if (e.GetIndex() == 9)
    {
        AddDayProfileTable(m_DayProfileTablePassive, e.GetValue().Arr);
    }
    else if (e.GetIndex() == 10)
    {
        int ret;
        CGXDLMSVariant tmp;
        if ((ret = CGXDLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp)) != 0)
        {
            return ret;
        }
        SetTime(tmp.dateTime);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
