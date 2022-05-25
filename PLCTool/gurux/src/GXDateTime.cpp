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

#include "../include/GXDateTime.h"
#include "../include/GXHelpers.h"

#include <string.h>
#include <locale>
#include <vector>
#include <assert.h>
#include <time.h>

#if defined(_WIN32) || defined(_WIN64)//Windows
#include <windows.h>
#endif

//Destructor.
CGXDateTime::~CGXDateTime()
{

}

//Get UTC offset in minutes.
void GetUtcOffset(struct tm* timeptr, int& hours, int& minutes, int& deviation)
{
    short addH = 1, addMin = 0;
    time_t zero = 24 * 60 * 60L;
    struct tm tm;
    // local time for Jan 2, 1900 00:00 UTC
#if defined(_WIN32) || defined(_WIN64)//Windows
    TIME_ZONE_INFORMATION tz;
#if _MSC_VER > 1000
    localtime_s(&tm, &zero);
#else
    tm = *localtime(&zero);
#endif
    GetTimeZoneInformation(&tz);
    if (tz.DaylightBias % 60 == 0)
    {
        addH = (short)(-tz.DaylightBias / 60);
    }
    else
    {
        addH = (short)(-tz.DaylightBias / 60);
        addMin = (short)(-tz.DaylightBias % 60);
    }
#endif
#if defined(__linux__)
    tm = *localtime(&zero);
    short gmtoff = (short)(tm.tm_gmtoff / 60);
    addH = (short)(gmtoff / 60);
    addMin = (short)(gmtoff % 60);
#endif
    deviation = addH * 60 + addMin;
    hours = tm.tm_hour;
    if (timeptr != NULL && timeptr->tm_isdst)
    {
        hours += addH;
    }
    //If the local time is the "day before" the UTC, subtract 24 hours from the hours to get the UTC offset
    if (tm.tm_mday < 2)
    {
        hours -= 24;
    }
    minutes = tm.tm_min;
    if (timeptr != NULL && timeptr->tm_isdst)
    {
        minutes += addMin;
    }
}

static time_t GetUtcTime(struct tm* timeptr)
{
    /* gets the epoch time relative to the local time zone,
    and then adds the appropriate number of seconds to make it UTC */
    int hours, minutes, deviation;
    GetUtcOffset(timeptr, hours, minutes, deviation);
    return mktime(timeptr) + (hours * 3600) + (minutes * 60);
}

short CGXDateTime::GetCurrentTimeZone()
{
    int hours, minutes, deviation;
    GetUtcOffset(NULL, hours, minutes, deviation);
    return -(hours * 60 + minutes);
}

char CGXDateTime::GetCurrentDeviation()
{
    int hours, minutes, deviation;
    GetUtcOffset(NULL, hours, minutes, deviation);
    return deviation;
}


// Constructor.
CGXDateTime::CGXDateTime()
{
    int hours, minutes, deviation;
    struct tm dt;
    time_t tm1 = time(NULL);
#if _MSC_VER > 1000
    localtime_s(&dt, &tm1);
#else
    dt = *localtime(&tm1);
#endif
    GetUtcOffset(&dt, hours, minutes, deviation);
    m_Deviation = -(hours * 60 + minutes);
    m_Skip = DATETIME_SKIPS_NONE;
    memset(&m_Value, 0, sizeof(m_Value));
    m_Value.tm_mday = 1;
    m_Extra = DATE_TIME_EXTRA_INFO_NONE;
    m_Status = DLMS_CLOCK_STATUS_OK;
}

// Constructor.
CGXDateTime::CGXDateTime(struct tm& value)
{
    int hours, minutes, deviation;
    GetUtcOffset(&value, hours, minutes, deviation);
    m_Deviation = -(hours * 60 + minutes);
    m_Value = value;
    m_Skip = DATETIME_SKIPS_NONE;
    m_Extra = DATE_TIME_EXTRA_INFO_NONE;
    if (value.tm_isdst)
    {
        m_Status = DLMS_CLOCK_STATUS_DAYLIGHT_SAVE_ACTIVE;
    }
    else
    {
        m_Status = DLMS_CLOCK_STATUS_OK;
    }
}

// Constructor.
CGXDateTime::CGXDateTime(struct tm* value)
{
    int hours, minutes, deviation;
    GetUtcOffset(value, hours, minutes, deviation);
    m_Deviation = -(hours * 60 + minutes);
    m_Value = *value;
    m_Skip = DATETIME_SKIPS_NONE;
    m_Extra = DATE_TIME_EXTRA_INFO_NONE;
    if (value->tm_isdst)
    {
        m_Status = DLMS_CLOCK_STATUS_DAYLIGHT_SAVE_ACTIVE;
    }
    else
    {
        m_Status = DLMS_CLOCK_STATUS_OK;
    }
}

CGXDateTime::CGXDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
{
    if (year != -1 && month != -1 && day != -1 && hour != -1 && minute != -1)
    {
        int hours, minutes, deviation;
        struct tm dt;
        dt.tm_year = year;
        dt.tm_mon = month;
        dt.tm_mday = day;
        dt.tm_hour = hour;
        dt.tm_min = minute;
        dt.tm_sec = second;
        time_t tm1 = time(NULL);
#if _MSC_VER > 1000
        localtime_s(&dt, &tm1);
#else
        dt = *localtime(&tm1);
#endif
        GetUtcOffset(&dt, hours, minutes, deviation);
        Init(year, month, day, hour, minute, second, millisecond, -(hours * 60 + minutes));
    }
    else
    {
        Init(year, month, day, hour, minute, second, millisecond, 0xFFFF);
    }
}

// Constructor.
CGXDateTime::CGXDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int devitation)
{
    Init(year, month, day, hour, minute, second, millisecond, devitation);
}

void CGXDateTime::GetSystemDateTimeFormat(std::string& value)
{
    char buff[50];
    struct tm order;

    memset(&order, 0, sizeof(struct tm));

    order.tm_year = 0;
    order.tm_mday = 1;
    order.tm_mon = 0;//Month is zero based.
    order.tm_hour = 13;
    strftime(buff, sizeof(buff), "%x %X", &order);
    value = buff;
}

int CGXDateTime::GetDateFormat2(GXDLMS_DATE_FORMAT& format, char& separator)
{
    int ret = 0, value, lastPos = 0, pos;
    char buff[11];
    struct tm order;

    memset(&order, 0, sizeof(struct tm));

    order.tm_year = 0;
    order.tm_mday = 1;
    order.tm_mon = 1;//Month is zero based.
    ret = (int)strftime(buff, sizeof(buff), "%x", &order);
    if (ret > 0)
    {
        for (pos = 0; pos != ret; ++pos)
        {
            //If numeric value
            if (buff[pos] >= '0' && buff[pos] <= '9')
            {

            }
            else //If date time separator.
            {
                separator = buff[pos];
#if _MSC_VER > 1000
                if (sscanf_s(buff + lastPos, "%d", &value) == 1)
#else
                if (sscanf(buff + lastPos, "%d", &value) == 1)
#endif
                {
                    if (value == 1)
                    {
                        format = lastPos == 0 ? GXDLMS_DATE_FORMAT_DMY : GXDLMS_DATE_FORMAT_YDM;
                        break;
                    }
                    else if (value == 2)
                    {
                        format = lastPos == 0 ? GXDLMS_DATE_FORMAT_MDY : GXDLMS_DATE_FORMAT_YMD;
                        break;
                    }
                }
                lastPos = pos + 1;
            }
        }
        ret = 0;
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_DATE_TIME;
    }
    return ret;
}


int CGXDateTime::GetTimeFormat2(char& separator, char& use24HourClock)
{
    int ret = 0, pos;
    char buff[15];
    char pm[15];
    struct tm order;

    memset(&order, 0, sizeof(struct tm));

    order.tm_hour = 13;
    order.tm_min = 0;
    order.tm_sec = 0;
    separator = 0;
    ret = (int)strftime(buff, 15, "%X", &order);
    if (ret > 0)
    {
        for (pos = 0; pos != ret; ++pos)
        {
            //If numeric value
            if (buff[pos] >= '0' && buff[pos] <= '9')
            {

            }
            else//If date time separator.
            {
                separator = buff[pos];
                strftime(pm, 15, "%p", &order);
                use24HourClock = strchr(buff, pm[0]) == 0;
                break;
            }
        }
        ret = 0;
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_DATE_TIME;
    }
    return ret;
}

int CGXDateTime::GetDateTimeFormat(
    std::string& value,
    GXDLMS_DATE_FORMAT& format,
    char& dateSeparator,
    char& timeSeparator,
    char& use24HourClock)
{
    int ret;
    if ((ret = GetDateFormat2(format, dateSeparator)) == 0 &&
        (ret = GetTimeFormat2(timeSeparator, use24HourClock)) == 0)
    {
        switch (format)
        {
        case GXDLMS_DATE_FORMAT_DMY:
            value.append("%d");
            value += dateSeparator;
            value.append("%m");
            value += dateSeparator;
            value.append("%Y");
            break;
        case GXDLMS_DATE_FORMAT_MDY:
            value.append("%m");
            value += dateSeparator;
            value.append("%d");
            value += dateSeparator;
            value.append("%Y");
            break;
        case GXDLMS_DATE_FORMAT_YMD:
            value.append("%Y");
            value += dateSeparator;
            value.append("%m");
            value += dateSeparator;
            value.append("%d");
            break;
        case GXDLMS_DATE_FORMAT_YDM:
            value.append("%Y");
            value += dateSeparator;
            value.append("%d");
            value += dateSeparator;
            value.append("%m");
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        value += ' ';
        if (use24HourClock)
        {
            value.append("%H");
        }
        else
        {
            value.append("%I");
        }
        value += timeSeparator;
        value.append("%M");
        value += timeSeparator;
        value.append("%S");
        if (!use24HourClock)
        {
            value += ' ';
            value.append("%p");
        }
    }
    return ret;
}

void Remove(std::string& value, const char* tag, const char sep)
{
    size_t pos;
    if (sep != 0)
    {
        char tmp[6];
        strcpy(tmp, tag);
        size_t len = strlen(tag);
        tmp[len] = sep;
        tmp[len + 1] = '\0';
        if ((pos = value.find(tmp)) != std::string::npos)
        {
            value.replace(pos, strlen(tmp), "");
        }
        else
        {
            strcpy(&tmp[1], tag);
            tmp[0] = sep;
            if ((pos = value.find(tmp)) != std::string::npos)
            {
                value.replace(pos, strlen(tmp), "");
            }
        }
    }
    if ((pos = value.find(tag)) != std::string::npos)
    {
        value.replace(pos, strlen(tag), "");
    }
}

void Replace(std::string& value, const char* tag, const char* v)
{
    size_t pos;
    if ((pos = value.find(tag)) != std::string::npos)
    {
        value.replace(pos, strlen(tag), v);
        // Trim spaces.
        value = GXHelpers::trim(value);
    }
}

int Remove(CGXDateTime* value, std::string& format, char dateSeparator, char timeSeparator)
{
    if (dynamic_cast<CGXDate*>(value) != NULL)
    {
        Remove(format, "%H", timeSeparator);
        Remove(format, "%I", timeSeparator);
        Remove(format, "%M", timeSeparator);
        Remove(format, "%S", timeSeparator);
        Remove(format, "%p", timeSeparator);
        value->SetSkip((DATETIME_SKIPS)(value->GetSkip() | DATETIME_SKIPS_HOUR | DATETIME_SKIPS_MINUTE | DATETIME_SKIPS_SECOND | DATETIME_SKIPS_MS));
    }
    else if (dynamic_cast<CGXTime*>(value) != NULL)
    {
        Remove(format, "%Y", dateSeparator);
        Remove(format, "%y", dateSeparator);
        Remove(format, "%m", dateSeparator);
        Remove(format, "%d", dateSeparator);
        value->SetSkip((DATETIME_SKIPS)(value->GetSkip() | DATETIME_SKIPS_YEAR | DATETIME_SKIPS_MONTH | DATETIME_SKIPS_DAY | DATETIME_SKIPS_DAYOFWEEK));
    }
    // Trim spaces.
    format = GXHelpers::trim(format);
    return 0;
}

static bool IsNumeric(char value)
{
    return value >= '0' && value <= '9';
}

int CGXDateTime::FromString(const char* datetime)
{
    int ret;
    std::string value = datetime;
    std::string format;
    GXDLMS_DATE_FORMAT df;
    char dateSeparator, timeSeparator, use24HourClock;

    memset(&m_Value, 0, sizeof(m_Value));
    m_Extra = DATE_TIME_EXTRA_INFO_NONE;
    m_Status = DLMS_CLOCK_STATUS_OK;
    m_Deviation = 0;
    m_Skip = DATETIME_SKIPS_NONE;
    if ((ret = GetDateTimeFormat(format, df, dateSeparator, timeSeparator, use24HourClock)) == 0)
    {
        size_t pos;
        Remove(this, format, dateSeparator, timeSeparator);
        if ((pos = value.find("BEGIN")) != std::string::npos)
        {
            value.replace(pos, pos + strlen("BEGIN"), "01");
            m_Extra = (DATE_TIME_EXTRA_INFO)(m_Extra | DATE_TIME_EXTRA_INFO_DST_BEGIN);
        }
        if ((pos = value.find("END")) != std::string::npos)
        {
            value.replace(pos, pos + strlen("END"), "01");
            m_Extra = (DATE_TIME_EXTRA_INFO)(m_Extra | DATE_TIME_EXTRA_INFO_DST_END);
        }
        if ((pos = value.find("LASTDAY2")) != std::string::npos)
        {
            value.replace(pos, pos + strlen("LASTDAY2"), "01");
            m_Extra = (DATE_TIME_EXTRA_INFO)(m_Extra | DATE_TIME_EXTRA_INFO_LAST_DAY2);
        }
        if ((pos = value.find("LASTDAY")) != std::string::npos)
        {
            value.replace(pos, pos + strlen("LASTDAY"), "01");
            m_Extra = (DATE_TIME_EXTRA_INFO)(m_Extra | DATE_TIME_EXTRA_INFO_LAST_DAY);
        }
        int skip = 0;
        std::string v = value;
        if ((pos = value.find('*')) != std::string::npos)
        {
            //Day of week is not supported when date time is give as a string.
            skip |= DATETIME_SKIPS_DAYOFWEEK;
            int lastFormatIndex = -1;
            int offset = 0;
            for (pos = 0; pos < value.size(); ++pos)
            {
                char c = value.at(pos);
                if (!IsNumeric(c))
                {
                    if (c == '*')
                    {
                        int cnt = 1;
                        ++lastFormatIndex;
                        c = format[lastFormatIndex + 1];
                        std::string val = "1";
                        while (lastFormatIndex + cnt + 1 < (int)format.size() && format[lastFormatIndex + cnt + 1] == c)
                        {
                            val += "0";
                            ++cnt;
                        }
                        v = v.substr(0, pos + offset) + val + value.substr(pos + 1);
                        offset += cnt - 1;
                        std::string tmp = format.substr(lastFormatIndex + 1, cnt);
                        tmp = GXHelpers::trim(tmp);
                        if (tmp == "Y")
                        {
                            skip |= DATETIME_SKIPS_YEAR;
                        }
                        else if (tmp == "m")
                        {
                            skip |= DATETIME_SKIPS_MONTH;
                        }
                        else if (tmp == "d")
                        {
                            skip |= DATETIME_SKIPS_DAY;
                        }
                        else if (tmp == "H" || tmp == "I")
                        {
                            skip |= DATETIME_SKIPS_HOUR;
                            if (format.find("%p") != std::string::npos)
                            {
                                Replace(format, "%p", "");
                            }
                        }
                        else if (tmp == "M")
                        {
                            skip |= DATETIME_SKIPS_MINUTE;
                        }
                        else if (tmp == "p")
                        {
                            skip |= DATETIME_SKIPS_HOUR;
                            Replace(format, "%p", "");
                        }
                        else if (tmp == "S")
                        {
                            skip |= DATETIME_SKIPS_SECOND;
                        }
                        else if (tmp.size() != 0)
                        {
                            return DLMS_ERROR_CODE_INVALID_DATE_TIME;
                        }
                    }
                    else
                    {
                        lastFormatIndex = (int)format.find(c, lastFormatIndex + 1);
                        //Dot is used time separator in some countries.
                        if (lastFormatIndex == -1 && c == ':')
                        {
                            lastFormatIndex = (int)format.find('.', lastFormatIndex + 1);
                        }
                    }
                }
            }
        }
        Replace(format, "%Y", "%d");
        Replace(format, "%d", "%d");
        Replace(format, "%m", "%d");
        Replace(format, "%H", "%d");
        Replace(format, "%I", "%d");
        Replace(format, "%M", "%d");
        Replace(format, "%S", "%d");
        Replace(format, "%p", "%s");
        int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
        char g[15];
        g[0] = 0;
        if (dynamic_cast<CGXDate*>(this) != NULL)
        {
            ret = sscanf(v.c_str(), format.c_str(), &a, &b, &c);
            if (ret != 3)
            {
                ret = DLMS_ERROR_CODE_INVALID_DATE_TIME;
            }
            else
            {
                ret = 0;
            }
        }
        else if (dynamic_cast<CGXTime*>(this) != NULL)
        {
            ret = sscanf(v.c_str(), format.c_str(), &d, &e, &f, &g);
            if (ret != 4)
            {
                if (ret == 3)
                {
                    if (!use24HourClock)
                    {
                        //Is AM is missing.
                        g[0] = '\0';
                    }
                    ret = 0;
                }
                else
                {
                    ret = DLMS_ERROR_CODE_INVALID_DATE_TIME;
                }
            }
            else
            {
                ret = 0;
            }
        }
        else
        {
            ret = sscanf(v.c_str(), format.c_str(), &a, &b, &c, &d, &e, &f, &g);
            if (ret != 7)
            {
                if (ret == 6)
                {
                    if (!use24HourClock)
                    {
                        //Is AM is missing.
                        g[0] = '\0';
                    }
                    ret = 0;
                }
                else
                {
                    ret = DLMS_ERROR_CODE_INVALID_DATE_TIME;
                }
            }
            else
            {
                ret = 0;
            }
        }
        if (ret == 0)
        {
            if (!use24HourClock && memcmp(g, "PM", 2) == 0)
            {
                d += 12;
            }
            switch (df)
            {
            case GXDLMS_DATE_FORMAT_DMY:
                Init(c, b, a, d, e, f, 0, 0);
                break;
            case GXDLMS_DATE_FORMAT_MDY:
                Init(c, a, b, d, e, f, 0, 0);
                break;
            case GXDLMS_DATE_FORMAT_YMD:
                Init(a, b, c, d, e, f, 0, 0);
                break;
            case GXDLMS_DATE_FORMAT_YDM:
                Init(a, c, b, d, e, f, 0, 0);
                break;
            default:
                ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            if (ret == 0)
            {
                m_Skip = (DATETIME_SKIPS)(m_Skip | skip | DATETIME_SKIPS_MS);
            }
        }
    }
    return ret;
}

int CGXDateTime::ToFormatString(const char* pattern, std::string& value)
{
    int ret;
    char buff[30];
    value.clear();
    if (m_Skip != 0 || m_Extra != 0)
    {
        std::string format;
        if (pattern != NULL)
        {
            format.append(pattern);
        }
        GXDLMS_DATE_FORMAT df;
        char dateSeparator = '/', timeSeparator = ':', use24HourClock = 0;
        if (pattern != NULL ||
            (ret = GetDateTimeFormat(format, df, dateSeparator, timeSeparator, use24HourClock)) == 0)
        {
            Remove(this, format, dateSeparator, timeSeparator);

            if ((m_Extra & DATE_TIME_EXTRA_INFO_DST_BEGIN) != 0)
            {
                Replace(format, "%m", "BEGIN");
            }
            else if ((m_Extra & DATE_TIME_EXTRA_INFO_DST_END) != 0)
            {
                Replace(format, "%m", "END");
            }
            else if ((m_Extra & DATE_TIME_EXTRA_INFO_LAST_DAY) != 0)
            {
                Replace(format, "%d", "LASTDAY");
            }
            else if ((m_Extra & DATE_TIME_EXTRA_INFO_LAST_DAY2) != 0)
            {
                Replace(format, "%d", "LASTDAY2");
            }
            if ((m_Skip & DATETIME_SKIPS_YEAR) != 0)
            {
                Replace(format, "%Y", "*");
                Replace(format, "%g", "*");
            }
            if ((m_Skip & DATETIME_SKIPS_MONTH) != 0)
            {
                Replace(format, "%m", "*");
            }
            if ((m_Skip & DATETIME_SKIPS_DAY) != 0)
            {
                Replace(format, "%d", "*");
            }
            if ((m_Skip & DATETIME_SKIPS_HOUR) != 0)
            {
                Replace(format, "%I", "*");
                Replace(format, "%H", "*");
                Remove(format, "%p", 0);
            }
            if ((m_Skip & DATETIME_SKIPS_SECOND) != 0)
            {
                Replace(format, "%S", "*");
            }
            if ((m_Skip & DATETIME_SKIPS_MINUTE) != 0)
            {
                Replace(format, "%M", "*");
            }
            ret = (int)strftime(buff, sizeof(buff), GXHelpers::trim(format).c_str(), &m_Value);
            if (ret != 0)
            {
                ret = 0;
                value = buff;
            }
            else
            {
                ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
        }
    }
    else
    {
        ret = (int)strftime(buff, sizeof(buff), pattern, &m_Value);
        if (ret != 0)
        {
            ret = 0;
            value = buff;
        }
        else
        {
            ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
    }
    return ret;
}

int CGXDateTime::ToFormatString(std::string& value)
{
    return ToFormatString(NULL, value);
}

// Constructor.
void CGXDateTime::Init(int year, int month, int day, int hour, int minute, int second, int millisecond, int devitation)
{
    memset(&m_Value, 0, sizeof(m_Value));
    m_Extra = DATE_TIME_EXTRA_INFO_NONE;
    m_Status = DLMS_CLOCK_STATUS_OK;
    int skip = DATETIME_SKIPS_NONE;
    if (year < 1 || year == 0xFFFF)
    {
        skip |= DATETIME_SKIPS_YEAR;
        year = 1900 + Now().m_Value.tm_year;
    }
    if (month < 1 || month == 0xFF)
    {
        skip |= DATETIME_SKIPS_MONTH;
        month = 0;
    }
    else
    {
        --month;
    }
    if (day < 1 || day == 0xFF)
    {
        skip |= DATETIME_SKIPS_DAY;
        day = 1;
    }
    else if (day == 0xFD)
    {
        day = DaysInMonth(year, month) - 1;
    }
    else if (day == 0xFE)
    {
        day = DaysInMonth(year, month);
    }
    if (hour == -1 || hour == 0xFF)
    {
        skip |= DATETIME_SKIPS_HOUR;
        hour = 1;
    }
    if (minute == -1 || minute == 0xFF)
    {
        skip |= DATETIME_SKIPS_MINUTE;
        minute = 0;
    }
    if (second == -1 || second == 0xFF)
    {
        skip |= DATETIME_SKIPS_SECOND;
        second = 0;
    }
    if (millisecond < 1 || millisecond == 0xFF)
    {
        skip |= DATETIME_SKIPS_MS;
        millisecond = 0;
    }
    m_Skip = (DATETIME_SKIPS)skip;
    if (year != 0)
    {
        m_Value.tm_year = year - 1900;
    }
    m_Value.tm_mon = month;
    m_Value.tm_mday = day;
    m_Value.tm_hour = hour;
    m_Value.tm_min = minute;
    m_Value.tm_sec = second;
    m_Deviation = devitation;
}

// Used date time value.
struct tm& CGXDateTime::GetValue()
{
    return m_Value;
}

unsigned char CGXDateTime::DaysInMonth(int year, short month)
{
    if (month == 0 || month == 2 || month == 4 ||
        month == 6 || month == 7 || month == 9 || month == 11)
    {
        return 31;
    }
    else if (month == 3 || month == 5 || month == 8 || month == 10)
    {
        return 30;
    }
    if (year % 4 == 0)
    {
        if (year % 100 == 0)
        {
            if (year % 400 == 0)
            {
                return 29;
            }
            return 28;
        }
        return 29;
    }
    return 28;
}

void CGXDateTime::SetValue(const struct tm& value)
{
    m_Value = value;
}

// Skip selected date time fields.
DATETIME_SKIPS CGXDateTime::GetSkip()
{
    return m_Skip;
}

void CGXDateTime::SetSkip(DATETIME_SKIPS value)
{
    m_Skip = value;
}

DATE_TIME_EXTRA_INFO CGXDateTime::GetExtra()
{
    return m_Extra;
}

void CGXDateTime::SetExtra(DATE_TIME_EXTRA_INFO value)
{
    m_Extra = value;
}

int CGXDateTime::GetDeviation()
{
    return m_Deviation;
}

void CGXDateTime::SetDeviation(int value)
{
    m_Deviation = value;
}

std::string CGXDateTime::ToString()
{
    char buff[50];
    int ret;
    if (m_Skip != DATETIME_SKIPS_NONE)
    {
        CGXByteBuffer ba;
        GXDLMS_DATE_FORMAT format;
        char separator;
        //Add year, month and date if used.
        if ((m_Skip & (DATETIME_SKIPS_YEAR | DATETIME_SKIPS_MONTH | DATETIME_SKIPS_DAY)) != (DATETIME_SKIPS_YEAR | DATETIME_SKIPS_MONTH | DATETIME_SKIPS_DAY))
        {
            ret = GetDateFormat2(format, separator);
            switch (format)
            {
            case GXDLMS_DATE_FORMAT_DMY:
            {
                if (m_Value.tm_mday != -1 && (m_Skip & DATETIME_SKIPS_DAY) == 0)
                {
                    ba.AddIntAsString(m_Value.tm_mday);
                }
                if (m_Value.tm_mon != -1 && (m_Skip & DATETIME_SKIPS_MONTH) == 0)
                {
                    if (ba.GetSize() != 0)
                    {
                        ba.SetUInt8(separator);
                    }
                    ba.AddIntAsString(1 + m_Value.tm_mon);
                }
                if (m_Value.tm_year != -1 && (m_Skip & DATETIME_SKIPS_YEAR) == 0)
                {
                    if (ba.GetSize() != 0)
                    {
                        ba.SetUInt8(separator);
                    }
                    ba.AddIntAsString(1900 + m_Value.tm_year);
                }
            }
            break;
            case GXDLMS_DATE_FORMAT_MDY:
            {
                if (m_Value.tm_mon != -1 && (m_Skip & DATETIME_SKIPS_MONTH) == 0)
                {
                    ba.AddIntAsString(1 + m_Value.tm_mon);
                }
                if (m_Value.tm_mday != -1 && (m_Skip & DATETIME_SKIPS_DAY) == 0)
                {
                    if (ba.GetSize() != 0)
                    {
                        ba.SetUInt8(separator);
                    }
                    ba.AddIntAsString(m_Value.tm_mday);
                }
                if (m_Value.tm_year != -1 && (m_Skip & DATETIME_SKIPS_YEAR) == 0)
                {
                    if (ba.GetSize() != 0)
                    {
                        ba.SetUInt8(separator);
                    }
                    ba.AddIntAsString(1900 + m_Value.tm_year);
                }
            }
            break;
            case GXDLMS_DATE_FORMAT_YMD:
            {
                if (m_Value.tm_year != -1 && (m_Skip & DATETIME_SKIPS_YEAR) == 0)
                {
                    ba.AddIntAsString(1900 + m_Value.tm_year);
                }
                if (m_Value.tm_mon != -1 && (m_Skip & DATETIME_SKIPS_MONTH) == 0)
                {
                    if (ba.GetSize() != 0)
                    {
                        ba.SetUInt8(separator);
                    }
                    ba.AddIntAsString(1 + m_Value.tm_mon);
                }
                if (m_Value.tm_mday != -1 && (m_Skip & DATETIME_SKIPS_DAY) == 0)
                {
                    if (ba.GetSize() != 0)
                    {
                        ba.SetUInt8(separator);
                    }
                    ba.AddIntAsString(m_Value.tm_mday);
                }
            }
            break;
            case GXDLMS_DATE_FORMAT_YDM:
            {
                if (m_Value.tm_year != -1 && (m_Skip & DATETIME_SKIPS_YEAR) == 0)
                {
                    ba.AddIntAsString(1900 + m_Value.tm_year);
                }
                if (m_Value.tm_mday != -1 && (m_Skip & DATETIME_SKIPS_DAY) == 0)
                {
                    if (ba.GetSize() != 0)
                    {
                        ba.SetUInt8(separator);
                    }
                    ba.AddIntAsString(m_Value.tm_mday);
                }
                if (m_Value.tm_mon != -1 && (m_Skip & DATETIME_SKIPS_MONTH) == 0)
                {
                    if (ba.GetSize() != 0)
                    {
                        ba.SetUInt8(separator);
                    }
                    ba.AddIntAsString(1 + m_Value.tm_mon);
                }
            }
            break;
            default:
            {
                ret = (int)strftime(buff, 50, "%X", &m_Value);
                ba.SetUInt8(' ');
                ba.Set(buff, ret);
                return 0;
            }
            }
        }

        //Add hours.
        if (m_Value.tm_hour != -1 && (m_Skip & DATETIME_SKIPS_HOUR) == 0)
        {
            if (ba.GetSize() != 0)
            {
                ba.SetUInt8(' ');
            }
#if _MSC_VER > 1000
            sprintf_s(buff, 50, "%.2d", m_Value.tm_hour);
#else
            sprintf(buff, "%.2d", m_Value.tm_hour);
#endif
            ba.AddString(buff);
        }
        //Add minutes.
        if (m_Value.tm_min != -1 && (m_Skip & DATETIME_SKIPS_MINUTE) == 0)
        {
            if (ba.GetSize() != 0)
            {
                ba.SetUInt8(':');
            }
#if _MSC_VER > 1000
            sprintf_s(buff, 50, "%.2d", m_Value.tm_min);
#else
            sprintf(buff, "%.2d", m_Value.tm_min);
#endif
            ba.AddString(buff);
        }
        //Add seconds.
        if (m_Value.tm_sec != -1 && (m_Skip & DATETIME_SKIPS_SECOND) == 0)
        {
            if (ba.GetSize() != 0)
            {
                ba.SetUInt8(':');
            }
#if _MSC_VER > 1000
            sprintf_s(buff, 50, "%.2d", m_Value.tm_sec);
#else
            sprintf(buff, "%.2d", m_Value.tm_sec);
#endif
            ba.AddString(buff);
        }
        return ba.ToString();
    }
    //If value is not set return empty std::string.
    if (m_Value.tm_year == -1)
    {
        return "";
    }
    std::string str;
    ret = (int)strftime(buff, 50, "%x %X", &m_Value);
    str.append(buff, ret);
    return str;
}

CGXDateTime CGXDateTime::Now()
{
    time_t tm1 = time(NULL);
#if _MSC_VER > 1000
    struct tm dt;
    localtime_s(&dt, &tm1);
#else
    struct tm dt = *localtime(&tm1);
#endif
    CGXDateTime now(dt);
    return now;
}

// Status of the clock.
DLMS_CLOCK_STATUS CGXDateTime::GetStatus()
{
    return m_Status;
}

void CGXDateTime::SetStatus(DLMS_CLOCK_STATUS value)
{
    m_Status = value;
}

void CGXDateTime::Reset()
{
    ResetDate();
    ResetTime();
    m_Status = DLMS_CLOCK_STATUS_OK;
    m_Deviation = 0;
    m_Skip = DATETIME_SKIPS_NONE;
    m_Extra = DATE_TIME_EXTRA_INFO_NONE;
    m_Status = DLMS_CLOCK_STATUS_OK;
}

void CGXDateTime::ResetDate()
{
    m_Value.tm_mday = 1;
    m_Value.tm_mon = m_Value.tm_wday = m_Value.tm_yday = m_Value.tm_isdst = 0;
    m_Value.tm_year = 0;
}

void CGXDateTime::ResetTime()
{
    m_Value.tm_hour = m_Value.tm_min = m_Value.tm_sec = 0;
}

int CGXDateTime::AddDays(int days)
{
    m_Value.tm_mday += days;
    if ((int)mktime(&m_Value) == -1)
    {
        return DLMS_ERROR_CODE_INVALID_DATE_TIME;
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDateTime::AddHours(int hours)
{
    m_Value.tm_hour += hours;
    if ((int)mktime(&m_Value) == -1)
    {
        return DLMS_ERROR_CODE_INVALID_DATE_TIME;
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDateTime::AddMinutes(int minutes)
{
    m_Value.tm_min += minutes;
    if ((int)mktime(&m_Value) == -1)
    {
        return DLMS_ERROR_CODE_INVALID_DATE_TIME;
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDateTime::AddSeconds(int seconds)
{
    m_Value.tm_sec += seconds;
    if ((int)mktime(&m_Value) == -1)
    {
        return DLMS_ERROR_CODE_INVALID_DATE_TIME;
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDateTime::CompareTo(CGXDateTime& antherDate)
{
    time_t time1 = mktime(&m_Value);
    time_t time2 = mktime(&antherDate.GetValue());
    if (time1 < time2)
    {
        return -1;
    }
    if (time1 > time2)
    {
        return 1;
    }
    return 0;
}

int CGXDateTime::ToLocalTime(struct tm& localTime)
{
    localTime = m_Value;
    if (m_Deviation != -32768)//0x8000
    {
        localTime.tm_min += m_Deviation;
        time_t t = GetUtcTime(&localTime);
        if (t == -1)
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
#if _MSC_VER > 1000
        localtime_s(&localTime, &t);
#else
        localTime = *localtime(&t);
#endif
    }
    return 0;
}

long CGXDateTime::GetDifference(struct tm& start, CGXDateTime& to)
{
    long diff = 0;
    // Compare seconds.
    if ((to.GetSkip() & DATETIME_SKIPS_SECOND) == 0)
    {
        if (start.tm_sec < to.m_Value.tm_sec)
        {
            diff += (to.m_Value.tm_sec - start.tm_sec) * 1000L;
        }
        else
        {
            diff -= (start.tm_sec - to.m_Value.tm_sec) * 1000L;
        }
    }
    // Compare minutes.
    if ((to.GetSkip() & DATETIME_SKIPS_MINUTE) == 0)
    {
        if (start.tm_min < to.m_Value.tm_min)
        {
            diff += (to.m_Value.tm_min - start.tm_min) * 60000L;
        }
        else
        {
            diff -= (start.tm_min - to.m_Value.tm_min) * 60000L;
        }
    }
    else if (diff < 0)
    {
        diff = 60 * 60000 + diff;
    }
    // Compare hours.
    if ((to.GetSkip() & DATETIME_SKIPS_HOUR) == 0)
    {
        if (start.tm_hour < to.m_Value.tm_hour)
        {
            diff += (to.m_Value.tm_hour - start.tm_hour) * 60 * 60000L;
        }
        else
        {
            diff -= (start.tm_hour - to.m_Value.tm_hour) * 60 * 60000L;
        }
    }
    else if (diff < 0)
    {
        diff = 60 * 60000 + diff;
    }
    // Compare days.
    if ((to.GetSkip() & DATETIME_SKIPS_DAY) == 0)
    {
        if (start.tm_mday < to.m_Value.tm_mday)
        {
            diff += (to.m_Value.tm_mday - start.tm_mday) * 24 * 60 * 60000;
        }
        else if (start.tm_mday != to.m_Value.tm_mday)
        {
            if ((to.GetSkip() & DATETIME_SKIPS_DAY) == 0)
            {
                diff += (to.m_Value.tm_mday - start.tm_mday) * 24 * 60 * 60000L;
            }
            else
            {
                diff = ((DaysInMonth(start.tm_year,
                    start.tm_mon)
                    - start.tm_mday
                    + to.m_Value.tm_mday) * 24
                    * 60 * 60000L) + diff;
            }
        }
    }
    else if (diff < 0)
    {
        diff = 24 * 60 * 60000 + diff;
    }
    // Compare months.
    if ((to.GetSkip() & DATETIME_SKIPS_MONTH) == 0)
    {
        if (start.tm_mon < to.m_Value.tm_mon)
        {
            for (int m = start.tm_mon; m != to.m_Value.tm_mon; ++m)
            {
                diff += DaysInMonth(start.tm_year, m) * 24 * 60 * 60000L;
            }
        }
        else
        {
            for (int m = to.m_Value.tm_mon; m != start.tm_mon; ++m)
            {
                diff += -DaysInMonth(start.tm_year, m) * 24 * 60 * 60000L;
            }
        }
    }
    else if (diff < 0)
    {
        diff = DaysInMonth(start.tm_year, start.tm_mon) * 24 * 60 * 60000L + diff;
    }
    return diff;
}
unsigned long CGXDateTime::ToUnixTime()
{
    return (unsigned long)mktime(&m_Value);
}
