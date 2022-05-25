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

#include "../include/GXDLMSRegisterMonitor.h"
#include "../include/GXDLMSClient.h"

CGXDLMSRegisterMonitor::CGXDLMSRegisterMonitor() :
    CGXDLMSRegisterMonitor("", 0)
{
}


CGXDLMSRegisterMonitor::CGXDLMSRegisterMonitor(std::string ln) :
    CGXDLMSRegisterMonitor(ln, 0)
{
}

CGXDLMSRegisterMonitor::CGXDLMSRegisterMonitor(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_REGISTER_MONITOR, ln, sn)
{
}

CGXDLMSRegisterMonitor::~CGXDLMSRegisterMonitor()
{
    for (std::vector<CGXDLMSActionSet*>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
    {
        delete *it;
    }
    m_Actions.clear();
    m_Thresholds.clear();
}

std::vector<CGXDLMSVariant>& CGXDLMSRegisterMonitor::GetThresholds()
{
    return m_Thresholds;
}
void CGXDLMSRegisterMonitor::SetThresholds(std::vector<CGXDLMSVariant>& value)
{
    m_Thresholds = value;
}

CGXDLMSMonitoredValue& CGXDLMSRegisterMonitor::GetMonitoredValue()
{
    return m_MonitoredValue;
}
void CGXDLMSRegisterMonitor::SetMonitoredValue(CGXDLMSMonitoredValue& value)
{
    m_MonitoredValue = value;
}

std::vector<CGXDLMSActionSet*>& CGXDLMSRegisterMonitor::GetActions()
{
    return m_Actions;
}

// Returns amount of attributes.
int CGXDLMSRegisterMonitor::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSRegisterMonitor::GetMethodCount()
{
    return 0;
}

void CGXDLMSRegisterMonitor::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<CGXDLMSVariant>::iterator it = m_Thresholds.begin(); it != m_Thresholds.end(); ++it)
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
    values.push_back(m_MonitoredValue.ToString());

    //Clear str.
    sb.str(std::string());
    sb << '[';
    empty = true;
    for (std::vector<CGXDLMSActionSet*>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
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
}

void CGXDLMSRegisterMonitor::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Thresholds
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //MonitoredValue
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //Actions
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
}

int CGXDLMSRegisterMonitor::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

/*
 * Returns value of given attribute.
 */
int CGXDLMSRegisterMonitor::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    CGXByteBuffer data;
    CGXDLMSVariant ln;
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
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        GXHelpers::SetObjectCount((unsigned long)m_Thresholds.size(), data);
        int ret;
        CGXDLMSVariant tmp;
        for (std::vector<CGXDLMSVariant>::iterator it = m_Thresholds.begin(); it != m_Thresholds.end(); ++it)
        {
            tmp = *it;
            if ((ret = GXHelpers::SetData(data, it->vt, tmp)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(3);
        int ret;
        GXHelpers::SetLogicalName(m_MonitoredValue.GetLogicalName().c_str(), ln);
        CGXDLMSVariant type = m_MonitoredValue.GetObjectType();
        CGXDLMSVariant index = m_MonitoredValue.GetAttributeIndex();
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, type)) != DLMS_ERROR_CODE_OK ||  //ClassID
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != DLMS_ERROR_CODE_OK || //LN
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, index)) != DLMS_ERROR_CODE_OK)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        int ret;
        //Add count
        GXHelpers::SetObjectCount((unsigned long)m_Actions.size(), data);
        CGXDLMSVariant selector;
        for (std::vector<CGXDLMSActionSet*>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            GXHelpers::SetLogicalName((*it)->GetActionUp().GetLogicalName().c_str(), ln);
            selector = (*it)->GetActionUp().GetScriptSelector();
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != DLMS_ERROR_CODE_OK ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, selector)) != DLMS_ERROR_CODE_OK)
            {
                return ret;
            }
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            GXHelpers::SetLogicalName((*it)->GetActionDown().GetLogicalName().c_str(), ln);
            selector = (*it)->GetActionDown().GetScriptSelector();
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != DLMS_ERROR_CODE_OK ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, selector)) != DLMS_ERROR_CODE_OK)
            {
                return ret;
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

/*
 * Set value of given attribute.
 */
int CGXDLMSRegisterMonitor::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    if (e.GetIndex() == 2)
    {
        SetThresholds(e.GetValue().Arr);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        GetMonitoredValue().SetObjectType((DLMS_OBJECT_TYPE)e.GetValue().Arr[0].ToInteger());
        std::string ln;
        GXHelpers::GetLogicalName(e.GetValue().Arr[1].byteArr, ln);
        m_MonitoredValue.SetLogicalName(ln);
        m_MonitoredValue.SetAttributeIndex(e.GetValue().Arr[2].ToInteger());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        for (std::vector<CGXDLMSActionSet*>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
        {
            delete *it;
        }
        m_Actions.clear();
        std::string ln;
        for (std::vector<CGXDLMSVariant>::iterator action_set = e.GetValue().Arr.begin(); action_set != e.GetValue().Arr.end(); ++action_set)
        {
            CGXDLMSActionSet *set = new CGXDLMSActionSet();
            CGXDLMSVariant& up = action_set->Arr[0];
            GXHelpers::GetLogicalName(up.Arr[0].byteArr, ln);
            set->GetActionUp().SetLogicalName(ln);
            set->GetActionUp().SetScriptSelector(up.Arr[1].ToInteger());
            CGXDLMSVariant& down = action_set->Arr[1];
            GXHelpers::GetLogicalName(down.Arr[0].byteArr, ln);
            set->GetActionDown().SetLogicalName(ln);
            set->GetActionDown().SetScriptSelector(down.Arr[1].ToInteger());
            m_Actions.push_back(set);
        }
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}
