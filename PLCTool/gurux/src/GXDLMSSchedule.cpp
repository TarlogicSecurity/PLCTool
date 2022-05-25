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
#include "../include/GXDLMSSchedule.h"
#include "../include/GXBitString.h"

//Constructor.
CGXDLMSSchedule::CGXDLMSSchedule() :
    CGXDLMSSchedule("0.0.12.0.0.255", 0)
{
}

//SN Constructor.
CGXDLMSSchedule::CGXDLMSSchedule(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_SCHEDULE, ln, sn)
{

}

//LN Constructor.
CGXDLMSSchedule::CGXDLMSSchedule(std::string ln) :
    CGXDLMSSchedule(ln, 0)
{

}

CGXDLMSSchedule::~CGXDLMSSchedule()
{
    for (std::vector<CGXDLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
    {
        delete* it;
    }
    m_Entries.clear();
}

// Get value of COSEM Data object.
std::vector<CGXDLMSScheduleEntry*>& CGXDLMSSchedule::GetEntries()
{
    return m_Entries;
}

// Set value of COSEM Data object.
void CGXDLMSSchedule::SetEntries(std::vector<CGXDLMSScheduleEntry*>& value)
{
    m_Entries = value;
}

// Returns amount of attributes.
int CGXDLMSSchedule::GetAttributeCount()
{
    return 2;
}

// Returns amount of methods.
int CGXDLMSSchedule::GetMethodCount()
{
    return 3;
}

void CGXDLMSSchedule::GetValues(std::vector<std::string>& values)
{
    std::stringstream sb;
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    std::string str;
    sb << '[';
    bool empty = true;
    for (std::vector<CGXDLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        sb << (*it)->GetIndex();
        sb << " ";
        sb << (*it)->GetEnable();
        sb << " ";
        sb << (*it)->GetLogicalName();
        sb << " ";
        sb << (*it)->GetScriptSelector();
        sb << " ";
        (*it)->GetSwitchTime().ToFormatString(str);
        sb << str;
        sb << " ";
        sb << (*it)->GetValidityWindow();
        sb << " ";
        sb << (*it)->GetExecWeekdays();
        sb << " ";
        sb << (*it)->GetExecSpecDays();
        sb << " ";
        (*it)->GetBeginDate().ToFormatString(str);
        sb << str;
        sb << " ";
        (*it)->GetEndDate().ToFormatString(str);
        sb << str;
    }
    values.push_back(sb.str());
}

int CGXDLMSSchedule::RemoveEntry(unsigned short index)
{
    for (std::vector<CGXDLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
    {
        if ((*it)->GetIndex() == index)
        {
            delete* it;
            m_Entries.erase(it);
            break;
        }
    }
    return 0;
}

// Create a new entry.
int CreateEntry(CGXDLMSSettings&, std::vector<CGXDLMSVariant>& arr, CGXDLMSScheduleEntry*& item)
{
    int ret;
    CGXDLMSVariant tmp;
    CGXTime t;
    CGXDate d;
    item = new CGXDLMSScheduleEntry();
    item->SetIndex(arr[0].ToInteger());
    item->SetEnable(arr[1].boolVal);
    if ((ret = CGXDLMSClient::ChangeType(arr[2], DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
    {
        delete item;
        return ret;
    }
    item->SetLogicalName(tmp.ToString());
    item->SetScriptSelector(arr[3].ToInteger());
    if ((ret = CGXDLMSClient::ChangeType(arr[4], DLMS_DATA_TYPE_TIME, tmp)) != 0)
    {
        delete item;
        return ret;
    }
    t = tmp.dateTime;
    item->SetSwitchTime(t);
    item->SetValidityWindow(arr[5].ToInteger());
    item->SetExecWeekdays((DLMS_WEEKDAYS)arr[6].ToInteger());
    item->SetExecSpecDays(arr[7].strVal);
    if ((ret = CGXDLMSClient::ChangeType(arr[8], DLMS_DATA_TYPE_DATE, tmp)) != 0)
    {
        delete item;
        return ret;
    }
    d = tmp.dateTime;
    item->SetBeginDate(d);
    if ((ret = CGXDLMSClient::ChangeType(arr[9], DLMS_DATA_TYPE_DATE, tmp)) != 0)
    {
        delete item;
        return ret;
    }
    d = tmp.dateTime;
    item->SetEndDate(d);
    return ret;
}

int AddEntry(CGXDLMSSettings&, CGXDLMSScheduleEntry* it, CGXByteBuffer& data)
{
    unsigned char ln[6];
    int ret;
    if ((ret = data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
        (ret = data.SetUInt8(10)) != 0 ||
        //Add index.
        (ret = data.SetUInt8(DLMS_DATA_TYPE_UINT16)) != 0 ||
        (ret = data.SetUInt16(it->GetIndex())) != 0 ||
        //Add enable.
        (ret = data.SetUInt8(DLMS_DATA_TYPE_BOOLEAN)) != 0 ||
        (ret = data.SetUInt8(it->GetEnable())) != 0 ||
        //Add logical Name.
        (ret = data.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING)) != 0 ||
        (ret = data.SetUInt8(6)) != 0 ||
        (ret = GXHelpers::SetLogicalName(it->GetLogicalName().c_str(), ln)) != 0 ||
        (ret = data.Set(ln, 6)) != 0 ||
        //Add script selector.
        (ret = data.SetUInt8(DLMS_DATA_TYPE_UINT16)) != 0 ||
        (ret = data.SetUInt16(it->GetScriptSelector())) != 0 ||
        //Add switch time.
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_OCTET_STRING, it->GetSwitchTime())) != 0 ||
        //Add validity window.
        (ret = data.SetUInt8(DLMS_DATA_TYPE_UINT16)) != 0 ||
        (ret = data.SetUInt16(it->GetValidityWindow())) != 0 ||
        //Add exec week days.
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_BIT_STRING, CGXBitString::ToBitString((unsigned char)it->GetExecWeekdays(), 7))) != 0 ||
        //Add exec spec days.
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_BIT_STRING, it->GetExecSpecDays())) != 0 ||
        //Add begin date.
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_OCTET_STRING, it->GetBeginDate())) != 0 ||
        //Add end date.
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_OCTET_STRING, it->GetEndDate())) != 0)
    {

    }
    return ret;
}

int CGXDLMSSchedule::Insert(CGXDLMSClient* client, CGXDLMSScheduleEntry* entry, std::vector<CGXByteBuffer>& reply)
{
    int ret;
    CGXByteBuffer data;
    if ((ret = AddEntry(client->m_Settings, entry, data)) == 0)
    {
        CGXDLMSVariant tmp = data;
        ret = client->Method(this, 2, tmp, DLMS_DATA_TYPE_STRUCTURE, reply);
    }
    return ret;
}

int CGXDLMSSchedule::Delete(CGXDLMSClient* client, CGXDLMSScheduleEntry* entry, std::vector<CGXByteBuffer>& reply)
{
    int ret;
    CGXByteBuffer data;
    if ((ret = data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        //Add structure size.
        (ret = data.SetUInt8(2)) == 0 &&
        //firstIndex
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT16, entry->GetIndex())) == 0 &&
        //lastIndex
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT16, entry->GetIndex())) == 0)
    {
        CGXDLMSVariant tmp = data;
        ret = client->Method(this, 3, tmp, DLMS_DATA_TYPE_STRUCTURE, reply);
    }
    return ret;
}

int CGXDLMSSchedule::Enable(CGXDLMSClient* client, CGXDLMSScheduleEntry* entry, std::vector<CGXByteBuffer>& reply)
{
    int ret;
    CGXByteBuffer data;
    if ((ret = data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        //Add structure size.
        (ret = data.SetUInt8(4)) == 0 &&
        //firstIndex
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT16, entry->GetIndex())) == 0 &&
        //lastIndex
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT16, entry->GetIndex())) == 0 &&
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT16, 0)) == 0 &&
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT16, 0)) == 0)
    {
        CGXDLMSVariant tmp = data;
        ret = client->Method(this, 1, tmp, DLMS_DATA_TYPE_STRUCTURE, reply);
    }
    return ret;
}

int CGXDLMSSchedule::Disable(CGXDLMSClient* client, CGXDLMSScheduleEntry* entry, std::vector<CGXByteBuffer>& reply)
{
    int ret;
    CGXByteBuffer data;
    if ((ret = data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        //Add structure size.
        (ret = data.SetUInt8(4)) == 0 &&
        //firstIndex
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT16, 0)) == 0 &&
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT16, 0)) == 0 &&
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT16, entry->GetIndex())) == 0 &&
        //lastIndex
        (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT16, entry->GetIndex())) == 0)
    {
        CGXDLMSVariant tmp = data;
        ret = client->Method(this, 1, tmp, DLMS_DATA_TYPE_STRUCTURE, reply);
    }
    return ret;
}

int CGXDLMSSchedule::Invoke(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    int ret = 0;
    unsigned short index;
    switch (e.GetIndex())
    {
        //Enable/disable entry
    case 1:
    {
        //Enable
        for (index = e.GetParameters().Arr[0].uiVal; index <= e.GetParameters().Arr[1].uiVal; ++index)
        {
            if (index != 0)
            {
                for (std::vector<CGXDLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
                {
                    if ((*it)->GetIndex() == index)
                    {
                        (*it)->SetEnable(true);
                        break;
                    }
                }
            }
        }
        //Disable
        for (index = e.GetParameters().Arr[2].uiVal; index <= e.GetParameters().Arr[3].uiVal; ++index)
        {
            if (index != 0)
            {
                for (std::vector<CGXDLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
                {
                    if ((*it)->GetIndex() == index)
                    {
                        (*it)->SetEnable(false);
                        break;
                    }
                }
            }
        }
    }
    break;
    //Insert entry
    case 2:
    {
        CGXDLMSScheduleEntry* entry;
        if ((ret = CreateEntry(settings, e.GetParameters().Arr, entry)) == 0)
        {
            if ((ret = RemoveEntry(entry->GetIndex())) != 0)
            {
                break;
            }
            m_Entries.push_back(entry);
        }
    }
    break;
    //Delete entry
    case 3:
    {
        for (index = e.GetParameters().Arr[0].uiVal; index <= e.GetParameters().Arr[1].uiVal; ++index)
        {
            if ((ret = RemoveEntry(index)) != 0)
            {
                break;
            }
        }
    }
    break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

void CGXDLMSSchedule::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Entries
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
}

int CGXDLMSSchedule::GetDataType(int index, DLMS_DATA_TYPE& type)
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
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Returns value of given attribute.
int CGXDLMSSchedule::GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
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
        int ret = 0;
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount((unsigned long) m_Entries.size(), data);
        for (std::vector<CGXDLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
        {
            if ((ret = AddEntry(settings, *it, data)) != 0)
            {
                break;
            }
        }
        e.SetValue(data);
        return ret;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSSchedule::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    int ret = 0;
    if (e.GetIndex() == 1)
    {
        ret = SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        for (std::vector<CGXDLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
        {
            delete* it;
        }
        m_Entries.clear();
        CGXDLMSScheduleEntry* item;
        for (std::vector<CGXDLMSVariant >::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            if ((ret = CreateEntry(settings, it->Arr, item)) != 0)
            {
                break;
            }
            m_Entries.push_back(item);
        }
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}
