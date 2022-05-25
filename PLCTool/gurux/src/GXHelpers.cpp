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
#define NDEBUG
#include "../include/GXDate.h"
#include "../include/GXTime.h"
#include "../include/GXHelpers.h"
#include "../include/GXDLMSTranslatorStructure.h"
#include "../include/GXDLMSClient.h"

/**
    * Get array from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * index
    *            starting index.
    * Returns  CGXDLMSVariant array.
    */
static int GetArray(CGXByteBuffer& buff, CGXDataInfo& info, int index, CGXDLMSVariant& value)
{
    int ret;
    unsigned long cnt = 0;
    CGXDataInfo info2;
    CGXDLMSVariant tmp;
    if (info.GetCount() == 0)
    {
        if ((ret = GXHelpers::GetObjectCount(buff, cnt)) != 0)
        {
            return ret;
        }
        info.SetCount(cnt);
        value.Arr.reserve(cnt);
    }
    if (info.GetXml() != NULL)
    {
        std::string tmp;
        info.GetXml()->IntegerToHex((unsigned long)info.GetCount(), 2, tmp);
        info.GetXml()->AppendStartTag(DATA_TYPE_OFFSET | info.GetType(), "Qty", tmp);
    }
    int size = buff.GetSize() - buff.GetPosition();
    if (info.GetCount() != 0 && size < 1)
    {
        info.SetComplete(false);
        return 0;
    }
    int startIndex = index;
    value.vt = DLMS_DATA_TYPE_ARRAY;
    // Position where last row was found. Cache uses this info.
    int pos = info.GetIndex();
    for (; pos != info.GetCount(); ++pos)
    {
        info2.Clear();
        tmp.Clear();
        info2.SetXml(info.GetXml());
        if ((ret = GXHelpers::GetData(buff, info2, tmp)) != 0)
        {
            return ret;
        }
        if (!info2.IsComplete())
        {
            buff.SetPosition(startIndex);
            info.SetComplete(false);
            break;
        }
        else
        {
            if (info2.GetCount() == info2.GetIndex())
            {
                startIndex = buff.GetPosition();
                value.Arr.push_back(tmp);
            }
        }
    }
    if (info.GetXml() != NULL)
    {
        info.GetXml()->AppendEndTag(DATA_TYPE_OFFSET + info.GetType());
    }
    info.SetIndex(pos);
    return 0;
}

/**
    * Get time from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  Parsed time.
    */
int GetTime(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    int ms, ret;
    unsigned char hour, minute, second, ch;
    if (buff.GetSize() - buff.GetPosition() < 4)
    {
        // If there is not enough data available.
        info.SetComplete(false);
        return 0;
    }
    std::string str;
    if (info.GetXml() != NULL)
    {
        str = buff.ToHexString(buff.GetPosition(), 4, false);
    }

    // Get time.
    if ((ret = buff.GetUInt8(&hour)) != 0)
    {
        return ret;
    }
    if ((ret = buff.GetUInt8(&minute)) != 0)
    {
        return ret;
    }
    if ((ret = buff.GetUInt8(&second)) != 0)
    {
        return ret;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0xFF)
    {
        ms = 10 * ch;
    }
    else
    {
        ms = -1;
    }
    CGXTime dt(hour, minute, second, ms);
    value = dt;
    if (info.GetXml() != NULL)
    {
        info.GetXml()->AppendComment(dt.ToString());
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", str);
    }
    return 0;
}

/**
    * Get date from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  Parsed date.
    */
int GetDate(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    unsigned short year;
    int ret;
    unsigned char ch, month, day;
    if (buff.GetSize() - buff.GetPosition() < 5)
    {
        // If there is not enough data available.
        info.SetComplete(false);
        return 0;
    }
    std::string str;
    if (info.GetXml() != NULL)
    {
        str = buff.ToHexString(buff.GetPosition(), 5, false);
    }
    // Get year.
    if ((ret = buff.GetUInt16(&year)) != 0)
    {
        return ret;
    }
    // Get month
    if ((ret = buff.GetUInt8(&month)) != 0)
    {
        return ret;
    }
    int extra = DATE_TIME_EXTRA_INFO_NONE;
    if (month == 0xFE)
    {
        //Daylight savings begin.
        month = 1;
        extra |= DATE_TIME_EXTRA_INFO_DST_BEGIN;
    }
    else if (month == 0xFD)
    {
        // Daylight savings end.
        month = 1;
        extra |= DATE_TIME_EXTRA_INFO_DST_END;
    }
    // Get day
    if ((ret = buff.GetUInt8(&day)) != 0)
    {
        return ret;
    }
    if (day == 0xFD)
    {
        // 2nd last day of month.
        day = 1;
        extra |= DATE_TIME_EXTRA_INFO_LAST_DAY2;
    }
    else if (day == 0xFE)
    {
        //Last day of month
        day = 1;
        extra |= DATE_TIME_EXTRA_INFO_LAST_DAY;
    }
    CGXDate dt(year, month, day);
    dt.SetExtra((DATE_TIME_EXTRA_INFO)(extra));
    // Skip week day
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch == 0xFF)
    {
        dt.SetSkip((DATETIME_SKIPS)(dt.GetSkip() | DATETIME_SKIPS_DAYOFWEEK));
    }
    value = dt;
    if (info.GetXml() != NULL)
    {
        info.GetXml()->AppendComment(dt.ToString());
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", str);
    }
    return 0;
}

/**
* Get date and time from DLMS data.
*
* buff
*            Received DLMS data.
* info
*            Data info.
* Returns  Parsed date and time.
*/
int GetDateTime(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    DATETIME_SKIPS skip = DATETIME_SKIPS_NONE;
    struct tm tm;
    unsigned short year;
    short deviation;
    int ret, ms, status;
    unsigned char ch;

    memset(&tm, 0, sizeof(struct tm));

    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 12)
    {
        info.SetComplete(false);
        return 0;
    }
    std::string str;
    if (info.GetXml() != NULL)
    {
        str = buff.ToHexString(buff.GetPosition(), 12, false);
    }
    // Get year.
    if ((ret = buff.GetUInt16(&year)) != 0)
    {
        return ret;
    }
    tm.tm_year = year;
    // Get month
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    tm.tm_mon = ch;
    // Get day
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    tm.tm_mday = ch;
    // Skip week day
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    tm.tm_wday = ch;
    // Get time.
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    tm.tm_hour = ch;
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    tm.tm_min = ch;
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    tm.tm_sec = ch;
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    ms = ch;
    if (ms != 0xFF)
    {
        ms *= 10;
    }
    else
    {
        skip = (DATETIME_SKIPS)(skip | DATETIME_SKIPS_MS);
        ms = 0;
    }
    if ((ret = buff.GetInt16(&deviation)) != 0)
    {
        return ret;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    status = ch;
    CGXDateTime dt;
    if (status == 0xFF)
    {
        status = 0;
        skip = (DATETIME_SKIPS)(skip | DATETIME_SKIPS_STATUS);
    }
    dt.SetStatus((DLMS_CLOCK_STATUS)status);
    if (year < 1 || year == 0xFFFF)
    {
        skip = (DATETIME_SKIPS)(skip | DATETIME_SKIPS_YEAR);
        tm.tm_year = 0;
    }
    else
    {
        tm.tm_year -= 1900;
    }
    if (tm.tm_wday < 0 || tm.tm_wday > 7)
    {
        tm.tm_wday = 0;
        skip = (DATETIME_SKIPS)(skip | DATETIME_SKIPS_DAYOFWEEK);
    }
    if (tm.tm_mon == 0xFE)
    {
        //Daylight savings begin.
        tm.tm_mon = 0;
        dt.SetExtra((DATE_TIME_EXTRA_INFO)(dt.GetExtra() | DATE_TIME_EXTRA_INFO_DST_BEGIN));
    }
    else if (tm.tm_mon == 0xFD)
    {
        // Daylight savings end.
        tm.tm_mon = 0;
        dt.SetExtra((DATE_TIME_EXTRA_INFO)(dt.GetExtra() | DATE_TIME_EXTRA_INFO_DST_END));
    }
    else if (tm.tm_mon < 1 || tm.tm_mon > 12)
    {
        skip = (DATETIME_SKIPS)(skip | DATETIME_SKIPS_MONTH);
        tm.tm_mon = 0;
    }
    else
    {
        tm.tm_mon -= 1;
    }
    if (tm.tm_mday == 0xFD)
    {
        // 2nd last day of month.
        tm.tm_mday = 1;
        dt.SetExtra((DATE_TIME_EXTRA_INFO)(dt.GetExtra() | DATE_TIME_EXTRA_INFO_LAST_DAY2));
    }
    else if (tm.tm_mday == 0xFE)
    {
        //Last day of month
        tm.tm_mday = 1;
        dt.SetExtra((DATE_TIME_EXTRA_INFO)(dt.GetExtra() | DATE_TIME_EXTRA_INFO_LAST_DAY));
    }
    else if (tm.tm_mday < 1 || tm.tm_mday > 31)
    {
        skip = (DATETIME_SKIPS)(skip | DATETIME_SKIPS_DAY);
        tm.tm_mday = 1;
    }
    if (tm.tm_hour < 0 || tm.tm_hour > 24)
    {
        skip = (DATETIME_SKIPS)(skip | DATETIME_SKIPS_HOUR);
        tm.tm_hour = 0;
    }
    if (tm.tm_min < 0 || tm.tm_min > 60)
    {
        skip = (DATETIME_SKIPS)(skip | DATETIME_SKIPS_MINUTE);
        tm.tm_min = 0;
    }
    if (tm.tm_sec < 0 || tm.tm_sec > 60)
    {
        skip = (DATETIME_SKIPS)(skip | DATETIME_SKIPS_SECOND);
        tm.tm_sec = 0;
    }
    // If ms is Zero it's skipped.
    if (ms < 1 || ms > 1000)
    {
        ms = 0;
    }
    tm.tm_isdst = (status & DLMS_CLOCK_STATUS_DAYLIGHT_SAVE_ACTIVE) != 0;
    dt.SetValue(tm);
    dt.SetDeviation(deviation);
    dt.SetSkip(skip);
    value = dt;
    if (info.GetXml() != NULL)
    {
        info.GetXml()->AppendComment(dt.ToString());
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", str);
    }
    return 0;
}

/**
* Get double value from DLMS data.
*
* buff
*            Received DLMS data.
* info
*            Data info.
* Returns  Parsed double value.
*/
int GetDouble(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 8)
    {
        info.SetComplete(false);
        return 0;
    }
    std::string str;
    if (info.GetXml() != NULL)
    {
        str = buff.ToHexString(buff.GetPosition(), 8, false);
    }
    value.vt = DLMS_DATA_TYPE_FLOAT64;
    int ret = buff.GetDouble(&value.dblVal);
    if (info.GetXml() != NULL)
    {
        if (info.GetXml()->GetComments())
        {
            info.GetXml()->AppendComment(value.ToString());
        }
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", str);
    }
    return ret;
}

/**
    * Get float value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  Parsed float value.
    */
int GetFloat(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 4)
    {
        info.SetComplete(false);
        return 0;
    }
    std::string str;
    if (info.GetXml() != NULL)
    {
        str = buff.ToHexString(buff.GetPosition(), 4, false);
    }
    value.vt = DLMS_DATA_TYPE_FLOAT32;
    int ret = buff.GetFloat(&value.fltVal);
    if (info.GetXml() != NULL)
    {
        if (info.GetXml()->GetComments())
        {
            info.GetXml()->AppendComment(value.ToString());
        }
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", str);
    }
    return ret;
}

/**
    * Get enumeration value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  parsed enumeration value.
    */
int GetEnum(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    int ret;
    unsigned char ch;
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 1)
    {
        info.SetComplete(false);
        return 0;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    value = ch;
    value.vt = DLMS_DATA_TYPE_ENUM;
    if (info.GetXml() != NULL)
    {
        std::string tmp;
        info.GetXml()->IntegerToHex((long)ch, 2, tmp);
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", tmp);
    }
    return 0;
}

/**
    * Get UInt64 value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  parsed UInt64 value.
    */
int GetUInt64(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    int ret;
    unsigned long long val;
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 8)
    {
        info.SetComplete(false);
        return 0;
    }
    if ((ret = buff.GetUInt64(&val)) != 0)
    {
        return ret;
    }
    value = val;
    if (info.GetXml() != NULL)
    {
        std::string tmp;
        info.GetXml()->IntegerToHex(val, tmp);
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", tmp);
    }
    return 0;
}

/**
    * Get Int64 value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  parsed Int64 value.
    */
int GetInt64(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    int ret;
    long long val;
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 8)
    {
        info.SetComplete(false);
        return 0;
    }
    if ((ret = buff.GetInt64(&val)) != 0)
    {
        return ret;
    }
    value = val;
    if (info.GetXml() != NULL)
    {
        std::string tmp;
        info.GetXml()->IntegerToHex(val, tmp);
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", tmp);
    }
    return 0;
}

/**
    * Get UInt16 value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  parsed UInt16 value.
    */
int GetUInt16(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    int ret;
    unsigned short val;
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 2)
    {
        info.SetComplete(false);
        return 0;
    }
    if ((ret = buff.GetUInt16(&val)) != 0)
    {
        return ret;
    }
    value = val;
    if (info.GetXml() != NULL)
    {
        std::string tmp;
        info.GetXml()->IntegerToHex((long)val, 4, tmp);
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", tmp);
    }
    return 0;
}

/**
    * Get UInt8 value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  parsed UInt8 value.
    */
int GetUInt8(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    int ret;
    unsigned char val;
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 1)
    {
        info.SetComplete(false);
        return 0;
    }
    if ((ret = buff.GetUInt8(&val)) != 0)
    {
        return ret;
    }
    value = val;
    if (info.GetXml() != NULL)
    {
        std::string tmp;
        info.GetXml()->IntegerToHex((long)val, 2, tmp);
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", tmp);
    }
    return 0;
}

/**
    * Get Int16 value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  parsed Int16 value.
    */
int GetInt16(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    int ret;
    short val;
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 2)
    {
        info.SetComplete(false);
        return 0;
    }
    if ((ret = buff.GetInt16(&val)) != 0)
    {
        return ret;
    }
    value = val;
    if (info.GetXml() != NULL)
    {
        std::string tmp;
        info.GetXml()->IntegerToHex((long)val, 4, tmp);
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", tmp);
    }
    return 0;
}

/**
    * Get Int8 value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  parsed Int8 value.
    */
int GetInt8(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    int ret;
    char val;
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 1)
    {
        info.SetComplete(false);
        return 0;
    }
    if ((ret = buff.GetUInt8((unsigned char*)&val)) != 0)
    {
        return ret;
    }
    value = val;
    if (info.GetXml() != NULL)
    {
        std::string tmp;
        info.GetXml()->IntegerToHex((long)val, 2, tmp);
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", tmp);
    }
    return 0;
}

/**
* Get BCD value from DLMS data.
*
* buff
*            Received DLMS data.
* info
*            Data info.
* Returns  parsed BCD value.
*/
int GetBcd(CGXByteBuffer& buff, CGXDataInfo& info, bool, CGXDLMSVariant& value)
{
    unsigned char ch;
    // If there is not enough data available.
    if (buff.GetUInt8(&ch) != 0)
    {
        info.SetComplete(false);
        return 0;
    }
    value = ch;
    value.vt = DLMS_DATA_TYPE_BINARY_CODED_DESIMAL;
    if (info.GetXml() != NULL)
    {
        std::string tmp;
        info.GetXml()->IntegerToHex((long)ch, 2, tmp);
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", tmp);
    }
    return 0;
}

int GXHelpers::GetObjectCount(CGXByteBuffer& data, unsigned long& count)
{
    int ret;
    unsigned char cnt;
    if ((ret = data.GetUInt8(&cnt)) != 0)
    {
        return ret;
    }
    if (cnt > 0x80)
    {
        if (cnt == 0x81)
        {
            if ((ret = data.GetUInt8(&cnt)) != 0)
            {
                return ret;
            }
            count = cnt;
            return DLMS_ERROR_CODE_OK;
        }
        else if (cnt == 0x82)
        {
            unsigned short tmp;
            if ((ret = data.GetUInt16(&tmp)) != 0)
            {
                return ret;
            }
            count = tmp;
            return DLMS_ERROR_CODE_OK;
        }
        else if (cnt == 0x84)
        {
            unsigned long tmp;
            if ((ret = data.GetUInt32(&tmp)) != 0)
            {
                return ret;
            }
            count = tmp;
            return DLMS_ERROR_CODE_OK;
        }
        else
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
    }
    count = cnt;
    return DLMS_ERROR_CODE_OK;
}

unsigned char GXHelpers::GetObjectCountSizeInBytes(unsigned long count)
{
    if (count < 0x80)
    {
        return 1;
    }
    else if (count < 0x100)
    {
        return 2;
    }
    else if (count < 0x10000)
    {
        return 3;
    }
    else
    {
        return 5;
    }
}

void GXHelpers::SetObjectCount(unsigned long count, CGXByteBuffer& buff)
{
    if (count < 0x80)
    {
        buff.SetUInt8((unsigned char)count);
    }
    else if (count < 0x100)
    {
        buff.SetUInt8(0x81);
        buff.SetUInt8((unsigned char)count);
    }
    else if (count < 0x10000)
    {
        buff.SetUInt8(0x82);
        buff.SetUInt16((unsigned short)count);
    }
    else
    {
        buff.SetUInt8(0x84);
        buff.SetUInt32(count);
    }
}

std::vector< std::string > GXHelpers::Split(std::string& s, char separator)
{
    std::vector< std::string > items;
    int last = 0;
    int pos = -1;
    while ((pos = (int)s.find(separator, pos + 1)) != -1)
    {
        std::string str;
        str.append(s, last, pos - last);
        items.push_back(str);
        last = pos + 1;
    }
    int len = (int)s.length();
    if (last == 0 || last != len)
    {
        std::string str;
        str.append(s, last, pos);
        items.push_back(str);
    }
    return items;
}

std::vector< std::string > GXHelpers::Split(std::string& s, std::string separators, bool ignoreEmpty)
{
    std::vector< std::string > items;
    int last = 0;
    int pos = -1;
    while ((pos = (int)s.find_first_of(separators, pos + 1)) != -1)
    {
        if (!ignoreEmpty || pos - last != 0)
        {
            std::string str;
            str.append(s, last, pos - last);
            items.push_back(str);
        }
        last = pos + 1;
    }
    int len = (int)s.length();
    if (!ignoreEmpty || len - last != 0)
    {
        std::string str;
        str.append(s, last, len - last);
        items.push_back(str);
    }
    return items;
}

void GXHelpers::Replace(std::string& str, std::string oldString, std::string newString)
{
    int index;
    int len = (int)oldString.length();
    while ((index = (int)str.rfind(oldString)) != -1)
    {
        str.replace(index, len, newString);
    }
}

std::string& GXHelpers::ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

std::string& GXHelpers::rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

std::string& GXHelpers::trim(std::string& s)
{
    return ltrim(rtrim(s));
}

std::string GXHelpers::BytesToHex(unsigned char* pBytes, int count)
{
    return BytesToHex(pBytes, count, ' ');
}

std::string GXHelpers::BytesToHex(unsigned char* pBytes, int count, char addSpaces)
{
    const char hexArray[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
    std::string hexChars(addSpaces ? 3 * count : 2 * count, 0);
    int tmp;
    int index = 0;
    for (int pos = 0; pos != count; ++pos)
    {
        tmp = pBytes[pos] & 0xFF;
        hexChars[index++] = hexArray[tmp >> 4];
        hexChars[index++] = hexArray[tmp & 0x0F];
        if (addSpaces)
        {
            hexChars[index++] = ' ';
        }
    }
    //Remove last separator.
    if (addSpaces && count != 0)
    {
        hexChars.resize(hexChars.size() - 1);
    }
    return hexChars;
}
/**
* Get UTF string value from DLMS data.
*
* buff
*            Received DLMS data.
* info
*            Data info.
* Returns  parsed UTF string value.
*/
int GetUtfString(CGXByteBuffer& buff, CGXDataInfo& info, bool knownType, CGXDLMSVariant& value)
{
    int ret;
    unsigned long len = 0;
    char* tmp;
    if (knownType)
    {
        len = buff.GetSize();
    }
    else
    {
        if ((ret = GXHelpers::GetObjectCount(buff, len)) != 0)
        {
            return ret;
        }
        // If there is not enough data available.
        if (buff.GetSize() - buff.GetPosition() < len)
        {
            info.SetComplete(false);
            return 0;
        }
    }
    if (len > 0)
    {
        tmp = new char[len];
        buff.Get((unsigned char*)tmp, len);
        value.strVal.append(tmp, len);
        delete tmp;
    }
    else
    {
        value = "";
    }
    return 0;
}

/**
    * Get octect string value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  parsed octet string value.
    */
int GetOctetString(CGXByteBuffer& buff, CGXDataInfo& info, bool knownType, CGXDLMSVariant& value)
{
    int ret;
    unsigned long len = 0;
    if (knownType)
    {
        len = buff.GetSize();
    }
    else
    {
        if ((ret = GXHelpers::GetObjectCount(buff, len)) != 0)
        {
            return ret;
        }
        // If there is not enough data available.
        if (buff.GetSize() - buff.GetPosition() < len)
        {
            info.SetComplete(false);
            return 0;
        }
    }
    value.size = (unsigned short)len;
    if (len == 0)
    {
        value.byteArr = NULL;
    }
    else
    {
        value.byteArr = (unsigned char*)malloc(len);
        if ((ret = buff.Get(value.byteArr, len)) != 0)
        {
            return ret;
        }
    }
    value.vt = DLMS_DATA_TYPE_OCTET_STRING;
    if (info.GetXml() != NULL)
    {
        std::string str;
        if (info.GetXml()->GetComments() && value.GetSize() != 0)
        {
            // This might be logical name.
            if (value.GetSize() == 6 && value.byteArr[5] == 0xFF)
            {
                std::string ln;
                GXHelpers::GetLogicalName(value.byteArr, ln);
                info.GetXml()->AppendComment(ln);
            }
            else
            {
                bool isString = true;
                //Try to move octect string to DateTime, Date or time.
                if (value.GetSize() == 12 || value.GetSize() == 5 || value.GetSize() == 4)
                {
                    DLMS_DATA_TYPE type = DLMS_DATA_TYPE_NONE;
                    if (value.GetSize() == 12)
                    {
                        type = DLMS_DATA_TYPE_DATETIME;
                    }
                    else if (value.GetSize() == 5)
                    {
                        type = DLMS_DATA_TYPE_DATE;
                    }
                    else if (value.GetSize() == 4)
                    {
                        type = DLMS_DATA_TYPE_TIME;
                    }
                    if (type != DLMS_DATA_TYPE_NONE)
                    {
                        CGXDLMSVariant tmp;
                        if (CGXDLMSClient::ChangeType(value, type, tmp) == 0)
                        {
                            info.GetXml()->AppendComment(tmp.dateTime.ToString());
                            isString = false;
                        }
                    }
                }
                if (isString)
                {
                    isString = CGXByteBuffer::IsAsciiString(value.byteArr, value.GetSize());
                }
                if (isString)
                {
                    str.append((char*)value.byteArr, value.GetSize());
                    info.GetXml()->AppendComment(str);
                }
            }
        }
        str = GXHelpers::BytesToHex(value.byteArr, value.GetSize(), false);
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", str);
    }
    return 0;
}

/**
    * Get string value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  parsed string value.
    */
int GetString(CGXByteBuffer& buff, CGXDataInfo& info, bool knownType, CGXDLMSVariant& value)
{
    int ret;
    unsigned long len = 0;
    char* tmp;
    if (knownType)
    {
        len = buff.GetSize();
    }
    else
    {
        if ((ret = GXHelpers::GetObjectCount(buff, len)) != 0)
        {
            return ret;
        }
        // If there is not enough data available.
        if (buff.GetSize() - buff.GetPosition() < len)
        {
            info.SetComplete(false);
            return 0;
        }
    }
    if (len > 0)
    {
        tmp = new char[len + 1];
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        tmp[len] = '\0';
        if ((ret = buff.Get((unsigned char*)tmp, len)) != 0)
        {
            delete tmp;
            return ret;
        }
        value = tmp;
        delete tmp;
    }
    else
    {
        value = "";
    }
    if (info.GetXml() != NULL)
    {
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", value.strVal);
    }
    return 0;
}

/**
    * Get UInt32 value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  parsed UInt32 value.
    */
int GetUInt32(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 4)
    {
        info.SetComplete(false);
        return 0;
    }
    value.vt = DLMS_DATA_TYPE_UINT32;
    int ret = buff.GetUInt32(&value.ulVal);
    if (ret == 0 && info.GetXml() != NULL)
    {
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", value);
    }
    return ret;
}

/**
    * Get Int32 value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  parsed Int32 value.
    */
int GetInt32(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 4)
    {
        info.SetComplete(false);
        return 0;
    }
    value.vt = DLMS_DATA_TYPE_INT32;
    int ret = buff.GetInt32(&value.lVal);
    if (ret == 0 && info.GetXml() != NULL)
    {
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", value);
    }
    return ret;
}


//Reserved for internal use.
void GXHelpers::ToBitString(CGXByteBuffer& sb, unsigned char value, int count)
{
    if (count > 0)
    {
        if (count > 8)
        {
            count = 8;
        }
        for (int pos = 7; pos != 8 - count - 1; --pos)
        {
            if ((value & (1 << pos)) != 0)
            {
                sb.SetInt8('1');
            }
            else
            {
                sb.SetInt8('0');
            }
        }
    }
}

/**
* Get bit string value from DLMS data.
*
* buff : Received DLMS data.
* info : Data info.
* Returns parsed bit string value.
*/
static int GetBitString(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    unsigned long cnt = 0;
    int ret;
    unsigned char ch;
    if ((ret = GXHelpers::GetObjectCount(buff, cnt)) != 0)
    {
        return ret;
    }
    double t = cnt;
    t /= 8;
    if (cnt % 8 != 0)
    {
        ++t;
    }
    unsigned int byteCnt = (unsigned int)t;
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < byteCnt)
    {
        info.SetComplete(false);
        return 0;
    }

    CGXByteBuffer bb;
    while (cnt > 0)
    {
        if ((ret = buff.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        GXHelpers::ToBitString(bb, ch, cnt);
        if (cnt < 8)
        {
            break;
        }
        cnt -= 8;
    }
    if (info.GetXml() != NULL)
    {
        std::string str = bb.ToString();
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()),
            "", str);
    }
    value = bb.ToString();
    value.vt = DLMS_DATA_TYPE_BIT_STRING;
    return 0;
}

/**
    * Get bool value from DLMS data.
    *
    * buff
    *            Received DLMS data.
    * info
    *            Data info.
    * Returns  parsed bool value.
    */
static int GetBool(CGXByteBuffer& buff, CGXDataInfo& info, CGXDLMSVariant& value)
{
    int ret;
    unsigned char ch;
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 1)
    {
        info.SetComplete(false);
        return 0;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    value.vt = DLMS_DATA_TYPE_BOOLEAN;
    value.boolVal = ch != 0;
    if (info.GetXml() != NULL)
    {
        std::string str = value.boolVal != 0 ? "true" : "false";
        info.GetXml()->AppendLine(info.GetXml()->GetDataType(info.GetType()), "", str);
    }
    return 0;
}


int GXHelpers::GetCompactArrayItem(
    CGXByteBuffer& buff,
    std::vector< CGXDLMSVariant>& dt,
    std::vector< CGXDLMSVariant>& list,
    int)
{
    int ret;
    CGXDLMSVariant tmp;
    tmp.vt = DLMS_DATA_TYPE_ARRAY;
    for (std::vector< CGXDLMSVariant>::iterator it = dt.begin(); it != dt.end(); ++it)
    {
        if (it->vt == DLMS_DATA_TYPE_ARRAY || it->vt == DLMS_DATA_TYPE_STRUCTURE)
        {
            if ((ret = GetCompactArrayItem(buff, it->Arr, tmp.Arr, 1)) != 0)
            {
                return ret;
            }
        }
        else
        {
            if ((ret = GetCompactArrayItem(buff, (DLMS_DATA_TYPE)it->bVal, tmp.Arr, 1)) != 0)
            {
                return ret;
            }
        }
    }
    list.push_back(tmp);
    return 0;
}

int GXHelpers::GetCompactArrayItem(
    CGXByteBuffer& buff,
    DLMS_DATA_TYPE dt,
    std::vector< CGXDLMSVariant>& list,
    int len)
{
    int ret;
    CGXDataInfo tmp;
    tmp.SetType(dt);
    unsigned long start = buff.GetPosition();
    CGXDLMSVariant value;
    if (dt == DLMS_DATA_TYPE_STRING)
    {
        while (buff.GetPosition() - start < (unsigned long)len)
        {
            value.Clear();
            tmp.Clear();
            tmp.SetType(dt);
            if ((ret = GetString(buff, tmp, false, value)) != 0)
            {
                return ret;
            }
            list.push_back(value);
            if (!tmp.IsComplete())
            {
                break;
            }
        }
    }
    else if (dt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        while (buff.GetPosition() - start < (unsigned long)len)
        {
            value.Clear();
            tmp.Clear();
            tmp.SetType(dt);
            if ((ret = GetOctetString(buff, tmp, false, value)) != 0)
            {
                return ret;
            }
            list.push_back(value);
            if (!tmp.IsComplete())
            {
                break;
            }
        }
    }
    else
    {
        while (buff.GetPosition() - start < (unsigned long)len)
        {
            tmp.Clear();
            tmp.SetType(dt);
            CGXDLMSVariant value;
            if ((ret = GetData(buff, tmp, value)) != 0)
            {
                return ret;
            }
            list.push_back(value);
            if (!tmp.IsComplete())
            {
                break;
            }
        }
    }
    return 0;
}

int GXHelpers::GetDataTypes(
    CGXByteBuffer& buff,
    std::vector<CGXDLMSVariant>& cols,
    int len)
{
    int ret;
    unsigned char ch;
    unsigned short cnt;
    DLMS_DATA_TYPE dt;
    for (int pos = 0; pos != len; ++pos)
    {
        if ((ret = buff.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        dt = (DLMS_DATA_TYPE)ch;
        if (dt == DLMS_DATA_TYPE_ARRAY)
        {
            if ((ret = buff.GetUInt16(&cnt)) != 0)
            {
                return ret;
            }
            std::vector<CGXDLMSVariant> tmp;
            CGXDLMSVariant tmp2;
            tmp2.vt = DLMS_DATA_TYPE_ARRAY;
            GetDataTypes(buff, tmp, 1);
            for (int i = 0; i != cnt; ++i) {
                tmp2.Arr.push_back(tmp[0]);
            }
            cols.push_back(tmp2);
        }
        else if (dt == DLMS_DATA_TYPE_STRUCTURE)
        {
            CGXDLMSVariant tmp;
            if ((ret = buff.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            tmp.vt = DLMS_DATA_TYPE_STRUCTURE;
            GetDataTypes(buff, tmp.Arr, ch);
            cols.push_back(tmp);
        }
        else
        {
            cols.push_back(dt);
        }
    }
    return 0;
}

int GXHelpers::AppendDataTypeAsXml(
    std::vector<CGXDLMSVariant>& cols,
    CGXDataInfo info)
{
    std::string val;
    for (std::vector<CGXDLMSVariant>::iterator it = cols.begin(); it != cols.end(); ++it)
    {
        if (it->vt == DLMS_DATA_TYPE_ARRAY)
        {
            info.GetXml()->AppendStartTag(DATA_TYPE_OFFSET + DLMS_DATA_TYPE_ARRAY, "", val);
            AppendDataTypeAsXml(it->Arr, info);
            info.GetXml()->AppendEndTag(DATA_TYPE_OFFSET + DLMS_DATA_TYPE_ARRAY);
        }
        else if (it->vt == DLMS_DATA_TYPE_STRUCTURE)
        {
            info.GetXml()->AppendStartTag(DATA_TYPE_OFFSET + DLMS_DATA_TYPE_STRUCTURE, "", val);
            AppendDataTypeAsXml(it->Arr, info);
            info.GetXml()->AppendEndTag(DATA_TYPE_OFFSET + DLMS_DATA_TYPE_STRUCTURE);
        }
        else
        {
            info.GetXml()->AppendEmptyTag(info.GetXml()->GetDataType((DLMS_DATA_TYPE)it->bVal));
        }
    }
    return 0;
}

int GXHelpers::GetCompactArray(
    CGXByteBuffer& buff,
    CGXDataInfo& info,
    CGXDLMSVariant& value)
{
    std::string str;
    int ret;
    unsigned char ch;
    unsigned long len;
    // If there is not enough data available.
    if (buff.GetSize() - buff.GetPosition() < 2)
    {
        info.SetComplete(false);
        return 0;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    DLMS_DATA_TYPE dt = (DLMS_DATA_TYPE)ch;
    if (dt == DLMS_DATA_TYPE_ARRAY)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((ret = GXHelpers::GetObjectCount(buff, len)) != 0)
    {
        return ret;
    }

    if (dt == DLMS_DATA_TYPE_STRUCTURE)
    {
        // Get data types.
        std::vector<CGXDLMSVariant> cols;
        if ((ret = GetDataTypes(buff, cols, len)) != 0)
        {
            return ret;
        }
        if ((ret = GXHelpers::GetObjectCount(buff, len)) != 0)
        {
            return ret;
        }
        if (info.GetXml() != NULL)
        {
            info.GetXml()->AppendStartTag(info.GetXml()->GetDataType(DLMS_DATA_TYPE_COMPACT_ARRAY));
            info.GetXml()->AppendStartTag(DLMS_TRANSLATOR_TAGS_CONTENTS_DESCRIPTION);
            AppendDataTypeAsXml(cols, info);
            info.GetXml()->AppendEndTag(DLMS_TRANSLATOR_TAGS_CONTENTS_DESCRIPTION);
            if (info.GetXml()->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
            {
                info.GetXml()->AppendStartTag2(DLMS_TRANSLATOR_TAGS_ARRAY_CONTENTS, true);
                str = buff.ToHexString(buff.GetPosition(), buff.Available(), true);
                info.GetXml()->Append(str);
                info.GetXml()->AppendEndTag(DLMS_TRANSLATOR_TAGS_ARRAY_CONTENTS, true);
                str = info.GetXml()->GetDataType(DLMS_DATA_TYPE_COMPACT_ARRAY);
                info.GetXml()->AppendEndTag(str);
            }
            else
            {
                info.GetXml()->AppendStartTag(DLMS_TRANSLATOR_TAGS_ARRAY_CONTENTS);
            }
        }
        value.vt = DLMS_DATA_TYPE_ARRAY;
        int start = buff.GetPosition();
        while (buff.GetPosition() - start < len)
        {
            CGXDLMSVariant row;
            row.vt = DLMS_DATA_TYPE_ARRAY;
            for (unsigned long pos = 0; pos != (unsigned long)cols.size(); ++pos)
            {
                CGXDLMSVariant& tmp = cols.at(pos);
                if (tmp.vt == DLMS_DATA_TYPE_STRUCTURE)
                {
                    if ((ret = GetCompactArrayItem(buff, tmp.Arr, row.Arr, 1)) != 0)
                    {
                        return ret;
                    }
                }
                else if (tmp.vt == DLMS_DATA_TYPE_ARRAY)
                {
                    std::vector<CGXDLMSVariant> tmp2;
                    if ((ret = GetCompactArrayItem(buff, tmp.Arr, tmp2, 1)) != 0)
                    {
                        return ret;
                    }
                    row.Arr.insert(row.Arr.end(), tmp2.at(0).Arr.begin(), tmp2.at(0).Arr.end());
                }
                else
                {
                    if ((ret = GetCompactArrayItem(buff, (DLMS_DATA_TYPE)tmp.cVal, row.Arr, 1)) != 0)
                    {
                        return ret;
                    }
                }
                if (buff.GetPosition() == buff.GetSize())
                {
                    break;
                }
            }
            // If all columns are read.
            if (row.Arr.size() >= cols.size())
            {
                value.Arr.push_back(row);
            }
            else
            {
                break;
            }
        }
        if (info.GetXml() != NULL && info.GetXml()->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
        {
            for (std::vector<CGXDLMSVariant>::iterator row = value.Arr.begin(); row != value.Arr.end(); ++row)
            {
                std::ostringstream sb;
                for (std::vector<CGXDLMSVariant>::iterator it = row->Arr.begin(); it != row->Arr.end(); ++it)
                {
                    if (it->vt == DLMS_DATA_TYPE_OCTET_STRING)
                    {
                        sb << GXHelpers::BytesToHex(it->byteArr, it->GetSize());
                    }
                    else if (it->vt == DLMS_DATA_TYPE_ARRAY ||
                        it->vt == DLMS_DATA_TYPE_STRUCTURE)
                    {
                        for (std::vector<CGXDLMSVariant>::iterator it2 = it->Arr.begin(); it2 != it->Arr.end(); ++it2)
                        {
                            if (it2->vt == DLMS_DATA_TYPE_OCTET_STRING)
                            {
                                sb << GXHelpers::BytesToHex(it2->byteArr, it2->GetSize());
                            }
                            else
                            {
                                sb << it2->ToString();
                            }
                            if (it2 + 1 != it->Arr.end())
                            {
                                sb << ';';
                            }
                        }
                    }
                    else
                    {
                        sb << it->ToString();
                    }
                    if (it + 1 != row->Arr.end())
                    {
                        sb << ';';
                    }
                }
                info.GetXml()->AppendLine(sb.str());
            }
        }
        if (info.GetXml() != NULL && info.GetXml()->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
        {
            info.GetXml()->AppendEndTag(DLMS_TRANSLATOR_TAGS_ARRAY_CONTENTS);
            str = info.GetXml()->GetDataType(DLMS_DATA_TYPE_COMPACT_ARRAY);
            info.GetXml()->AppendEndTag(str);
        }
        return 0;
    }
    else
    {
        if (info.GetXml() != NULL)
        {
            info.GetXml()->AppendStartTag(info.GetXml()->GetDataType(DLMS_DATA_TYPE_COMPACT_ARRAY));
            info.GetXml()->AppendStartTag(DLMS_TRANSLATOR_TAGS_CONTENTS_DESCRIPTION);
            info.GetXml()->AppendEmptyTag(info.GetXml()->GetDataType(dt));
            info.GetXml()->AppendEndTag(DLMS_TRANSLATOR_TAGS_CONTENTS_DESCRIPTION);
            info.GetXml()->AppendStartTag2(DLMS_TRANSLATOR_TAGS_ARRAY_CONTENTS, true);
            if (info.GetXml()->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
            {
                str = buff.ToHexString(buff.GetPosition(), buff.Available(), false);
                info.GetXml()->Append(str);
                info.GetXml()->AppendEndTag(DLMS_TRANSLATOR_TAGS_ARRAY_CONTENTS, true);
                str = info.GetXml()->GetDataType(DLMS_DATA_TYPE_COMPACT_ARRAY);
                info.GetXml()->AppendEndTag(str);
            }
        }
        GetCompactArrayItem(buff, dt, value.Arr, len);
        if (info.GetXml() != NULL && info.GetXml()->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
        {
            std::string separator = ";";
            for (std::vector<CGXDLMSVariant>::iterator it = value.Arr.begin(); it != value.Arr.end(); ++it)
            {
                str = it->ToString();
                info.GetXml()->Append(str);
                info.GetXml()->Append(separator);
            }
            if (value.Arr.size() != 0)
            {
                info.GetXml()->SetXmlLength(info.GetXml()->GetXmlLength() - 1);
            }
            info.GetXml()->AppendEndTag(DLMS_TRANSLATOR_TAGS_ARRAY_CONTENTS, true);
            str = info.GetXml()->GetDataType(DLMS_DATA_TYPE_COMPACT_ARRAY);
            info.GetXml()->AppendEndTag(str);
        }
    }
    return 0;
}

int GXHelpers::GetData(
    CGXByteBuffer& data,
    CGXDataInfo& info,
    CGXDLMSVariant& value)
{
    int ret;
    unsigned char ch;
    int startIndex = data.GetPosition();
    value.Clear();
    if (data.GetPosition() == data.GetSize())
    {
        info.SetComplete(false);
        return 0;
    }
    info.SetComplete(true);
    bool knownType = info.GetType() != DLMS_DATA_TYPE_NONE;
    // Get data type if it is unknown.
    if (!knownType)
    {
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        info.SetType((DLMS_DATA_TYPE)ch);
    }
    if (info.GetType() == DLMS_DATA_TYPE_NONE)
    {
        if (info.GetXml() != NULL)
        {
            info.GetXml()->AppendLine("<" + info.GetXml()->GetDataType(info.GetType()) + " />");
        }
        return 0;
    }
    if (data.GetPosition() == data.GetSize())
    {
        info.SetComplete(false);
        return 0;
    }
    switch (info.GetType())
    {
    case DLMS_DATA_TYPE_ARRAY:
    case DLMS_DATA_TYPE_STRUCTURE:
        ret = GetArray(data, info, startIndex, value);
        value.vt = info.GetType();
        break;
    case DLMS_DATA_TYPE_BOOLEAN:
        ret = GetBool(data, info, value);
        break;
    case DLMS_DATA_TYPE_BIT_STRING:
        ret = GetBitString(data, info, value);
        break;
    case DLMS_DATA_TYPE_INT32:
        ret = GetInt32(data, info, value);
        break;
    case DLMS_DATA_TYPE_UINT32:
        ret = GetUInt32(data, info, value);
        break;
    case DLMS_DATA_TYPE_STRING:
        ret = GetString(data, info, knownType, value);
        break;
    case DLMS_DATA_TYPE_STRING_UTF8:
        ret = GetUtfString(data, info, knownType, value);
        break;
    case DLMS_DATA_TYPE_OCTET_STRING:
        ret = GetOctetString(data, info, knownType, value);
        break;
    case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
        ret = GetBcd(data, info, knownType, value);
        break;
    case DLMS_DATA_TYPE_INT8:
        ret = GetInt8(data, info, value);
        break;
    case DLMS_DATA_TYPE_INT16:
        ret = GetInt16(data, info, value);
        break;
    case DLMS_DATA_TYPE_UINT8:
        ret = GetUInt8(data, info, value);
        break;
    case DLMS_DATA_TYPE_UINT16:
        ret = GetUInt16(data, info, value);
        break;
    case DLMS_DATA_TYPE_COMPACT_ARRAY:
        ret = GetCompactArray(data, info, value);
        break;
    case DLMS_DATA_TYPE_INT64:
        ret = GetInt64(data, info, value);
        break;
    case DLMS_DATA_TYPE_UINT64:
        ret = GetUInt64(data, info, value);
        break;
    case DLMS_DATA_TYPE_ENUM:
        ret = GetEnum(data, info, value);
        break;
    case DLMS_DATA_TYPE_FLOAT32:
        ret = GetFloat(data, info, value);
        break;
    case DLMS_DATA_TYPE_FLOAT64:
        ret = GetDouble(data, info, value);
        break;
    case DLMS_DATA_TYPE_DATETIME:
        ret = GetDateTime(data, info, value);
        break;
    case DLMS_DATA_TYPE_DATE:
        ret = GetDate(data, info, value);
        break;
    case DLMS_DATA_TYPE_TIME:
        ret = GetTime(data, info, value);
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

/**
    * Convert time to DLMS bytes.
    *
    * buff
    *            Byte buffer where data is write.
    * value
    *            Added value.
    */
static int SetTime(CGXByteBuffer& buff, CGXDLMSVariant& value)
{
    DATETIME_SKIPS skip = value.dateTime.GetSkip();
    struct tm dt = value.dateTime.GetValue();
    // Add time.
    if ((skip & DATETIME_SKIPS_HOUR) != 0)
    {
        buff.SetUInt8(0xFF);
    }
    else
    {
        buff.SetUInt8(dt.tm_hour);
    }
    if ((skip & DATETIME_SKIPS_MINUTE) != 0)
    {
        buff.SetUInt8(0xFF);
    }
    else
    {
        buff.SetUInt8(dt.tm_min);
    }
    if ((skip & DATETIME_SKIPS_SECOND) != 0)
    {
        buff.SetUInt8(0xFF);
    }
    else
    {
        buff.SetUInt8(dt.tm_sec);
    }
    if ((skip & DATETIME_SKIPS_MS) != 0)
    {
        // Hundredths of second is not used.
        buff.SetUInt8(0xFF);
    }
    else
    {
        buff.SetUInt8(0);
    }
    return 0;
}

/**
* Convert date to DLMS bytes.
*
* buff
*            Byte buffer where data is write.
* value
*            Added value.
*/
static int SetDate(CGXByteBuffer& buff, CGXDLMSVariant& value)
{
    struct tm dt = value.dateTime.GetValue();
    DATETIME_SKIPS skip = value.dateTime.GetSkip();
    // Add year.
    if ((skip & DATETIME_SKIPS_YEAR) != 0)
    {
        buff.SetUInt16(0xFFFF);
    }
    else
    {
        buff.SetUInt16(1900 + dt.tm_year);
    }
    // Add month
    if ((value.dateTime.GetExtra() & DATE_TIME_EXTRA_INFO_DST_BEGIN) != 0)
    {
        buff.SetUInt8(0xFE);
    }
    else if ((value.dateTime.GetExtra() & DATE_TIME_EXTRA_INFO_DST_END) != 0)
    {
        buff.SetUInt8(0xFD);
    }
    else if ((skip & DATETIME_SKIPS_MONTH) != 0)
    {
        buff.SetUInt8(0xFF);
    }
    else
    {
        buff.SetUInt8(dt.tm_mon + 1);
    }
    // Add day
    if ((value.dateTime.GetExtra() & DATE_TIME_EXTRA_INFO_LAST_DAY) != 0)
    {
        buff.SetUInt8(0xFE);
    }
    else if ((value.dateTime.GetExtra() & DATE_TIME_EXTRA_INFO_LAST_DAY2) != 0)
    {
        buff.SetUInt8(0xFD);
    }
    else if ((skip & DATETIME_SKIPS_DAY) != 0)
    {
        buff.SetUInt8(0xFF);
    }
    else
    {
        buff.SetUInt8(dt.tm_mday);
    }
    //Add week day
    if ((skip & DATETIME_SKIPS_DAYOFWEEK) != 0)
    {
        buff.SetUInt8(0xFF);
    }
    else
    {
        int val = dt.tm_wday;
        //If Sunday.
        if (val == 0)
        {
            val = 8;
        }
        buff.SetUInt8(val - 1);
    }
    return 0;
}

/**
* Convert date time to DLMS bytes.
*
* buff
*            Byte buffer where data is write.
* value
*            Added value.
*/
static int SetDateTime(CGXByteBuffer& buff, CGXDLMSVariant& value)
{
    //Add year.
    unsigned short year = 0xFFFF;
    struct tm dt = value.dateTime.GetValue();
    DATETIME_SKIPS skip = value.dateTime.GetSkip();
    if (dt.tm_year != -1 && (skip & DATETIME_SKIPS_YEAR) == 0)
    {
        year = 1900 + dt.tm_year;
    }
    buff.SetUInt16(year);
    //Add month
    if ((value.dateTime.GetExtra() & DATE_TIME_EXTRA_INFO_DST_BEGIN) != 0)
    {
        buff.SetUInt8(0xFE);
    }
    else if ((value.dateTime.GetExtra() & DATE_TIME_EXTRA_INFO_DST_END) != 0)
    {
        buff.SetUInt8(0xFD);
    }
    else if (dt.tm_mon != -1 && (skip & DATETIME_SKIPS_MONTH) == 0)
    {
        buff.SetUInt8(dt.tm_mon + 1);
    }
    else
    {
        buff.SetUInt8(0xFF);
    }
    //Add day
    if ((value.dateTime.GetExtra() & DATE_TIME_EXTRA_INFO_LAST_DAY) != 0)
    {
        buff.SetUInt8(0xFE);
    }
    else if ((value.dateTime.GetExtra() & DATE_TIME_EXTRA_INFO_LAST_DAY2) != 0)
    {
        buff.SetUInt8(0xFD);
    }
    else if (dt.tm_mday != -1 && (skip & DATETIME_SKIPS_DAY) == 0)
    {
        buff.SetUInt8(dt.tm_mday);
    }
    else
    {
        buff.SetUInt8(0xFF);
    }
    //Add week day
    if ((skip & DATETIME_SKIPS_DAYOFWEEK) != 0)
    {
        buff.SetUInt8(0xFF);
    }
    else
    {
        int val = dt.tm_wday;
        //If Sunday.
        if (val == 0)
        {
            val = 7;
        }
        buff.SetUInt8(val);
    }
    //Add Hours
    if (dt.tm_hour != -1 && (skip & DATETIME_SKIPS_HOUR) == 0)
    {
        buff.SetUInt8(dt.tm_hour);
    }
    else
    {
        buff.SetUInt8(0xFF);
    }
    //Add Minutes
    if (dt.tm_min != -1 && (skip & DATETIME_SKIPS_MINUTE) == 0)
    {
        buff.SetUInt8(dt.tm_min);
    }
    else
    {
        buff.SetUInt8(0xFF);
    }
    //Add seconds.
    if (dt.tm_sec != -1 && (skip & DATETIME_SKIPS_SECOND) == 0)
    {
        buff.SetUInt8(dt.tm_sec);
    }
    else
    {
        buff.SetUInt8(0xFF);
    }
    //Add ms.
    if ((skip & DATETIME_SKIPS_MS) != 0)
    {
        // Hundredths of second is not used.
        buff.SetUInt8(0xFF);
    }
    else
    {
        buff.SetUInt8(0);
    }
    // devitation not used.
    if ((skip & DATETIME_SKIPS_DEVITATION) != 0)
    {
        buff.SetUInt16(0x8000);
    }
    else
    {
        // Add devitation.
        buff.SetUInt16(value.dateTime.GetDeviation());
    }
    // Add clock_status
    if ((skip & DATETIME_SKIPS_STATUS) != 0)
    {
        // Status is not used.
        buff.SetUInt8(0xFF);
    }
    else if (dt.tm_isdst)
    {
        buff.SetUInt8(value.dateTime.GetStatus() | DLMS_CLOCK_STATUS_DAYLIGHT_SAVE_ACTIVE);
    }
    else
    {
        buff.SetUInt8(value.dateTime.GetStatus());
    }
    return 0;
}

/**
* Convert BCD to DLMS bytes.
*
* buff
*            Byte buffer where data is write.
* value
*            Added value.
*/
static int SetBcd(CGXByteBuffer& buff, CGXDLMSVariant& value)
{
    // Standard supports only size of byte.
    buff.SetUInt8(value.bVal);
    return 0;
}

/**
    * Convert Array to DLMS bytes.
    *
    * buff
    *            Byte buffer where data is write.
    * value
    *            Added value.
    */
static int SetArray(CGXByteBuffer& buff, CGXDLMSVariant& value)
{
    int ret;
    GXHelpers::SetObjectCount((unsigned long)value.Arr.size(), buff);
    for (std::vector<CGXDLMSVariant>::iterator it = value.Arr.begin(); it != value.Arr.end(); ++it)
    {
        if ((ret = GXHelpers::SetData(buff, it->vt, *it)) != 0)
        {
            return ret;
        }
    }
    return 0;
}

/**
    * Convert Octetstring to DLMS bytes.
    *
    * buff
    *            Byte buffer where data is write.
    * value
    *            Added value.
    */
static int SetOctetString(CGXByteBuffer& buff, CGXDLMSVariant& value)
{
    if (value.vt == DLMS_DATA_TYPE_STRING)
    {
        CGXByteBuffer bb;
        GXHelpers::HexToBytes(value.strVal, bb);
        GXHelpers::SetObjectCount(bb.GetSize(), buff);
        buff.Set(bb.GetData(), bb.GetSize());
    }
    else if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        GXHelpers::SetObjectCount(value.size, buff);
        buff.Set(value.byteArr, value.size);
    }
    else if (value.vt == DLMS_DATA_TYPE_NONE)
    {
        GXHelpers::SetObjectCount(0, buff);
    }
    else
    {
        // Invalid data type.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return 0;
}

/**
* Convert UTC string to DLMS bytes.
*
* buff
*            Byte buffer where data is write.
* value
*            Added value.
*/
static int SetUtfString(CGXByteBuffer& buff, CGXDLMSVariant& value)
{
    if (value.vt != DLMS_DATA_TYPE_NONE)
    {
        GXHelpers::SetObjectCount((unsigned long)value.strVal.size(), buff);
        buff.AddString(value.strVal.c_str());
    }
    else
    {
        buff.SetUInt8(0);
    }
    return 0;
}

void GXHelpers::GetLogicalName(unsigned char* buff, std::string& ln)
{
    int dataSize;
    char tmp[25];
    //If Script Action target is not set it is null
    if (buff == NULL)
    {
        ln.clear();
        ln.append("0.0.0.0.0.0");
    }
    else
    {
#if _MSC_VER > 1000
        dataSize = sprintf_s(tmp, 25, "%d.%d.%d.%d.%d.%d", buff[0], buff[1], buff[2], buff[3], buff[4], buff[5]) + 1;
#else
        dataSize = sprintf(tmp, "%d.%d.%d.%d.%d.%d", buff[0], buff[1], buff[2], buff[3], buff[4], buff[5]) + 1;
#endif
        if (dataSize > 25)
        {
            assert(0);
        }
        ln.clear();
        ln.append(tmp, dataSize - 1);
    }
}

void GXHelpers::GetLogicalName(CGXByteBuffer& buff, std::string& ln)
{
    unsigned char tmp[6];
    buff.Get(tmp, 6);
    GetLogicalName(tmp, ln);
}


int GXHelpers::SetLogicalName(const char* name, CGXDLMSVariant& value)
{
    unsigned char ln[6] = { 0 };
    int ret = 0;
    if (name != NULL && *name != '\0')
    {
        ret = SetLogicalName(name, ln);
    }
    if (ret == 0)
    {
        value.Clear();
        value.Add(ln, 6);
    }
    return ret;
}

int GXHelpers::SetLogicalName(const char* name, unsigned char ln[6])
{
    int ret;
    int v1, v2, v3, v4, v5, v6;
#if _MSC_VER > 1000
    ret = sscanf_s(name, "%u.%u.%u.%u.%u.%u", &v1, &v2, &v3, &v4, &v5, &v6);
#else
    ret = sscanf(name, "%u.%u.%u.%u.%u.%u", &v1, &v2, &v3, &v4, &v5, &v6);
#endif
    if (ret != 6)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    ln[0] = (unsigned char)v1;
    ln[1] = (unsigned char)v2;
    ln[2] = (unsigned char)v3;
    ln[3] = (unsigned char)v4;
    ln[4] = (unsigned char)v5;
    ln[5] = (unsigned char)v6;
    return DLMS_ERROR_CODE_OK;
}

/**
* Convert ASCII string to DLMS bytes.
*
* buff
*            Byte buffer where data is write.
* value
*            Added value.
*/
static int SetString(CGXByteBuffer& buff, CGXDLMSVariant& value)
{
    if (value.vt != DLMS_DATA_TYPE_NONE)
    {
        GXHelpers::SetObjectCount((unsigned long)value.strVal.size(), buff);
        buff.AddString(value.strVal.c_str());
    }
    else
    {
        buff.SetUInt8(0);
    }
    return 0;
}

int GXHelpers::SetBitString(CGXByteBuffer& buff, CGXDLMSVariant& value, bool addCount)
{
    unsigned char val = 0;
    int index = 7;
    if (value.vt == DLMS_DATA_TYPE_STRING)
    {
        if (addCount)
        {
            GXHelpers::SetObjectCount((unsigned long)value.strVal.size(), buff);
        }
        for (std::string::iterator it = value.strVal.begin(); it != value.strVal.end(); ++it)
        {
            if (*it == '1')
            {
                val |= (1 << index);
            }
            else if (*it != '0')
            {
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            --index;
            if (index == -1)
            {
                index = 7;
                buff.SetUInt8(val);
                val = 0;
            }
        }
        if (index != 7)
        {
            buff.SetUInt8(val);
        }
    }
    else if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        GXHelpers::SetObjectCount(8 * value.size, buff);
        buff.Set(value.byteArr, value.size);
    }
    else if (value.vt == DLMS_DATA_TYPE_NONE)
    {
        buff.SetUInt8(0);
    }
    else if (value.vt == DLMS_DATA_TYPE_UINT8)
    {
        SetObjectCount(8, buff);
        buff.SetUInt8(value.cVal);
    }
    else
    {
        //BitString must give as string.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

int GXHelpers::SetData2(CGXByteBuffer& buff, DLMS_DATA_TYPE type, CGXDLMSVariant value)
{
    return SetData(buff, type, value);
}

int GXHelpers::SetData(CGXByteBuffer& buff, DLMS_DATA_TYPE type, CGXDLMSVariant& value)
{
    if ((type == DLMS_DATA_TYPE_ARRAY || type == DLMS_DATA_TYPE_STRUCTURE)
        && value.vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        // If byte array is added do not add type.
        buff.Set(value.byteArr, value.size);
        return 0;
    }
    else
    {
        buff.SetUInt8(type);
    }
    if (type == DLMS_DATA_TYPE_NONE)
    {
        return 0;
    }
    switch (type)
    {
    case DLMS_DATA_TYPE_BOOLEAN:
        buff.SetUInt8(value.boolVal != 0);
        break;
    case DLMS_DATA_TYPE_INT8:
    case DLMS_DATA_TYPE_UINT8:
    case DLMS_DATA_TYPE_ENUM:
        buff.SetUInt8(value.bVal);
        break;
    case DLMS_DATA_TYPE_INT16:
    case DLMS_DATA_TYPE_UINT16:
        buff.SetUInt16(value.iVal);
        break;
    case DLMS_DATA_TYPE_INT32:
    case DLMS_DATA_TYPE_UINT32:
        buff.SetUInt32(value.lVal);
        break;
    case DLMS_DATA_TYPE_INT64:
    case DLMS_DATA_TYPE_UINT64:
        buff.SetUInt64(value.llVal);
        break;
    case DLMS_DATA_TYPE_FLOAT32:
        buff.SetFloat(value.fltVal);
        break;
    case DLMS_DATA_TYPE_FLOAT64:
        buff.SetDouble(value.dblVal);
        break;
    case DLMS_DATA_TYPE_BIT_STRING:
        return SetBitString(buff, value, true);
        break;
    case DLMS_DATA_TYPE_STRING:
        return SetString(buff, value);
        break;
    case DLMS_DATA_TYPE_STRING_UTF8:
        return SetUtfString(buff, value);
        break;
    case DLMS_DATA_TYPE_OCTET_STRING:
        if (value.vt == DLMS_DATA_TYPE_DATE)
        {
            // Add size
            buff.SetUInt8(5);
            SetDate(buff, value);
        }
        else if (value.vt == DLMS_DATA_TYPE_TIME)
        {
            // Add size
            buff.SetUInt8(4);
            SetTime(buff, value);
        }
        else if (value.vt == DLMS_DATA_TYPE_DATETIME)
        {
            // Date an calendar are always written as date time.
            buff.SetUInt8(12);
            SetDateTime(buff, value);
        }
        else
        {
            return SetOctetString(buff, value);
        }
        break;
    case DLMS_DATA_TYPE_ARRAY:
    case DLMS_DATA_TYPE_STRUCTURE:
        return SetArray(buff, value);
        break;
    case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
        return SetBcd(buff, value);
        break;
    case DLMS_DATA_TYPE_COMPACT_ARRAY:
        assert(0);
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    case DLMS_DATA_TYPE_DATETIME:
        return SetDateTime(buff, value);
        break;
    case DLMS_DATA_TYPE_DATE:
        return SetDate(buff, value);
        break;
    case DLMS_DATA_TYPE_TIME:
        return SetTime(buff, value);
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return 0;
}

unsigned char GXHelpers::GetValue(char c)
{
    unsigned char value;
    if (c > '9')
    {
        if (c > 'Z')
        {
            value = (c - 'a' + 10);
        }
        else
        {
            value = (c - 'A' + 10);
        }
    }
    else
    {
        value = (c - '0');
    }
    return value;
}

void GXHelpers::HexToBytes(std::string& value, CGXByteBuffer& buffer)
{
    buffer.Clear();
    buffer.Capacity((unsigned long)(value.length() / 2));
    int lastValue = -1;
    int index = 0;
    for (std::string::iterator ch = value.begin(); ch != value.end(); ++ch)
    {
        if (*ch >= '0' && *ch < 'g')
        {
            if (lastValue == -1)
            {
                lastValue = GetValue(*ch);
            }
            else if (lastValue != -1)
            {
                buffer.SetUInt8(lastValue << 4 | GetValue(*ch));
                lastValue = -1;
                ++index;
            }
        }
        else if (lastValue != -1)
        {
            buffer.SetUInt8(GetValue(*ch));
            lastValue = -1;
            ++index;
        }
    }
}

void GXHelpers::Write(char* fileName, char* pData, int len)
{
    if (len != 0 && pData != NULL)
    {
        std::ofstream trace;
        trace.open(fileName, std::ios::out | std::ios::app);
        trace.write(pData, len);
        trace.close();
    }
}

void GXHelpers::Write(std::string fileName, std::string data)
{
    if (data.size() != 0)
    {
        std::ofstream trace;
        trace.open(fileName.c_str(), std::ios::out | std::ios::app);
        trace.write(&data[0], data.size());
        trace.close();
    }
}

bool GXHelpers::GetBits(unsigned char& tmp, unsigned char BitMask)
{
    return (tmp & BitMask) != 0;
}

bool GXHelpers::StringCompare(const char* c1, const char* c2)
{
    return strcmp(c1, c2) == 0;
}

int GXHelpers::GetDataTypeSize(DLMS_DATA_TYPE type)
{
    int size = -1;
    switch (type)
    {
    case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
        size = 1;
        break;
    case DLMS_DATA_TYPE_BOOLEAN:
        size = 1;
        break;
    case DLMS_DATA_TYPE_DATE:
        size = 5;
        break;
    case DLMS_DATA_TYPE_DATETIME:
        size = 12;
        break;
    case DLMS_DATA_TYPE_ENUM:
        size = 1;
        break;
    case DLMS_DATA_TYPE_FLOAT32:
        size = 4;
        break;
    case DLMS_DATA_TYPE_FLOAT64:
        size = 8;
        break;
    case DLMS_DATA_TYPE_INT16:
        size = 2;
        break;
    case DLMS_DATA_TYPE_INT32:
        size = 4;
        break;
    case DLMS_DATA_TYPE_INT64:
        size = 8;
        break;
    case DLMS_DATA_TYPE_INT8:
        size = 1;
        break;
    case DLMS_DATA_TYPE_NONE:
        size = 0;
        break;
    case DLMS_DATA_TYPE_TIME:
        size = 4;
        break;
    case DLMS_DATA_TYPE_UINT16:
        size = 2;
        break;
    case DLMS_DATA_TYPE_UINT32:
        size = 4;
        break;
    case DLMS_DATA_TYPE_UINT64:
        size = 8;
        break;
    case DLMS_DATA_TYPE_UINT8:
        size = 1;
        break;
    default:
        break;
    }
    return size;
}

std::string GXHelpers::GeneralizedTime(struct tm* date)
{
    char tmp[20];
#if _MSC_VER > 1000
    sprintf_s(tmp, 20, "%.4d%.2d%.2d%.2d%.2d%.2dZ", date->tm_year, date->tm_mon, date->tm_mday, date->tm_hour, date->tm_min, date->tm_sec);
#else
    sprintf(tmp, "%.4d%.2d%.2d%.2d%.2d%.2dZ", date->tm_year, date->tm_mon, date->tm_mday, date->tm_hour, date->tm_min, date->tm_sec);
#endif
    return tmp;
}

std::string GXHelpers::IntToString(int value)
{
    char buff[20];
#if _MSC_VER > 1000
    sprintf_s(buff, 20, "%d", value);
#else
    sprintf(buff, "%d", value);
#endif
    return buff;
}

void GXHelpers::Join(std::string separator, std::vector< std::string >& list, std::string& res)
{
    bool empty = true;
    for (std::vector< std::string >::iterator it = list.begin(); it != list.end(); ++it)
    {
        if (!empty)
        {
            res.append(separator);
        }
        empty = false;
        res.append(*it);
    }
}

unsigned char GXHelpers::SwapBits(unsigned char value)
{
    unsigned char ret = 0, pos;
    for (pos = 0; pos != 8; ++pos)
    {
        ret = (unsigned char)((ret << 1) | (value & 0x01));
        value = (unsigned char)(value >> 1);
    }
    return ret;
}
