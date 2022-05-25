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

#ifndef GXDLMSACTIVITYCALENDAR_H
#define GXDLMSACTIVITYCALENDAR_H

#include "GXDLMSObject.h"
#include "GXDLMSSeasonProfile.h"
#include "GXDLMSWeekProfile.h"
#include "GXDLMSDayProfile.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSActivityCalendar
*/
class CGXDLMSActivityCalendar : public CGXDLMSObject
{
    std::string m_CalendarNameActive;
    std::string m_CalendarNamePassive;
    std::vector<CGXDLMSSeasonProfile*> m_SeasonProfileActive;
    std::vector<CGXDLMSWeekProfile*> m_WeekProfileTableActive;
    std::vector<CGXDLMSDayProfile*> m_DayProfileTableActive;
    std::vector<CGXDLMSSeasonProfile*> m_SeasonProfilePassive;
    std::vector<CGXDLMSDayProfile*> m_DayProfileTablePassive;
    std::vector<CGXDLMSWeekProfile*> m_WeekProfileTablePassive;
    CGXDateTime m_Time;
public:
    //Constructor.
    CGXDLMSActivityCalendar();

    //SN Constructor.
    CGXDLMSActivityCalendar(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSActivityCalendar(std::string ln);

    //Destructor.
    ~CGXDLMSActivityCalendar();

    std::string GetCalendarNameActive();
    void SetCalendarNameActive(std::string value);

    std::vector<CGXDLMSSeasonProfile*>& GetSeasonProfileActive();
    void SetSeasonProfileActive(std::vector<CGXDLMSSeasonProfile*>& value);

    std::vector<CGXDLMSWeekProfile*>& GetWeekProfileTableActive();
    void SetWeekProfileTableActive(std::vector<CGXDLMSWeekProfile*>& value);

    std::vector<CGXDLMSDayProfile*>& GetDayProfileTableActive();
    void SetDayProfileTableActive(std::vector<CGXDLMSDayProfile*>& value);

    std::string GetCalendarNamePassive();
    void SetCalendarNamePassive(std::string value);

    std::vector<CGXDLMSSeasonProfile*>& GetSeasonProfilePassive();
    void SetSeasonProfilePassive(std::vector<CGXDLMSSeasonProfile*>& value);

    std::vector<CGXDLMSWeekProfile*>& GetWeekProfileTablePassive();
    void SetWeekProfileTablePassive(std::vector<CGXDLMSWeekProfile*>& value);

    std::vector<CGXDLMSDayProfile*>& GetDayProfileTablePassive();
    void SetDayProfileTablePassive(std::vector<CGXDLMSDayProfile*>& value);

    CGXDateTime& GetTime();
    void SetTime(CGXDateTime& value);

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    // Returns value of given attribute.
    int GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);
};
#endif //GXDLMSACTIVITYCALENDAR_H