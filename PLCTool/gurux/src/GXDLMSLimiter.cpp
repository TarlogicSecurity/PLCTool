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

#include "../include/GXDLMSLimiter.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXDLMSActionSet.h"
#include <sstream>

//Constructor.
CGXDLMSLimiter::CGXDLMSLimiter() :
    CGXDLMSLimiter("", 0)
{
}

//SN Constructor.
CGXDLMSLimiter::CGXDLMSLimiter(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_LIMITER, ln, sn)
{
    m_MonitoredValue = NULL;
    m_MinOverThresholdDuration = 0;
    m_MinUnderThresholdDuration = 0;
    m_EmergencyProfileActive = false;
    m_MonitoredAttributeIndex = 0;
}

//LN Constructor.
CGXDLMSLimiter::CGXDLMSLimiter(std::string ln) : CGXDLMSLimiter(ln, 0)
{
}

/**
Defines an attribute of an object to be monitored.
*/
CGXDLMSObject* CGXDLMSLimiter::GetMonitoredValue()
{
    return m_MonitoredValue;
}
void CGXDLMSLimiter::SetMonitoredValue(CGXDLMSObject* value)
{
    m_MonitoredValue = value;
}

/**
 Provides the active threshold value to which the attribute monitored is compared.
*/
CGXDLMSVariant& CGXDLMSLimiter::GetThresholdActive()
{
    return m_ThresholdActive;
}
void CGXDLMSLimiter::SetThresholdActive(CGXDLMSVariant& value)
{
    m_ThresholdActive = value;
}

/**
 Provides the threshold value to which the attribute monitored
 is compared when in normal operation.
*/
CGXDLMSVariant& CGXDLMSLimiter::GetThresholdNormal()
{
    return m_ThresholdNormal;
}
void CGXDLMSLimiter::SetThresholdNormal(CGXDLMSVariant& value)
{
    m_ThresholdNormal = value;
}

/**
 Provides the threshold value to which the attribute monitored
 is compared when an emergency profile is active.
*/
CGXDLMSVariant& CGXDLMSLimiter::GetThresholdEmergency()
{
    return m_ThresholdEmergency;
}
void CGXDLMSLimiter::SetThresholdEmergency(CGXDLMSVariant& value)
{
    m_ThresholdEmergency = value;
}

/**
 Defines minimal over threshold duration in seconds required
 to execute the over threshold action.
*/
long CGXDLMSLimiter::GetMinOverThresholdDuration()
{
    return m_MinOverThresholdDuration;
}
void CGXDLMSLimiter::SetMinOverThresholdDuration(long value)
{
    m_MinOverThresholdDuration = value;
}

/**
 Defines minimal under threshold duration in seconds required to
 execute the under threshold action.
*/
long CGXDLMSLimiter::GetMinUnderThresholdDuration()
{
    return m_MinUnderThresholdDuration;
}
void CGXDLMSLimiter::SetMinUnderThresholdDuration(long value)
{
    m_MinUnderThresholdDuration = value;
}
CGXDLMSEmergencyProfile& CGXDLMSLimiter::GetEmergencyProfile()
{
    return m_EmergencyProfile;
}
void CGXDLMSLimiter::SetEmergencyProfile(CGXDLMSEmergencyProfile value)
{
    m_EmergencyProfile = value;
}

std::vector<int>& CGXDLMSLimiter::GetEmergencyProfileGroupIDs()
{
    return m_EmergencyProfileGroupIDs;
}
void CGXDLMSLimiter::SetEmergencyProfileGroupIDs(std::vector<int> value)
{
    m_EmergencyProfileGroupIDs = value;
}

/**
 Is Emergency Profile active.
*/
bool CGXDLMSLimiter::GetEmergencyProfileActive()
{
    return m_EmergencyProfileActive;
}
void CGXDLMSLimiter::SetEmergencyProfileActive(bool value)
{
    m_EmergencyProfileActive = value;
}

/**
 Defines the scripts to be executed when the monitored value
 crosses the threshold for minimal duration time.
*/
CGXDLMSActionItem& CGXDLMSLimiter::GetActionOverThreshold()
{
    return m_ActionOverThreshold;
}
void CGXDLMSLimiter::SetActionOverThreshold(CGXDLMSActionItem value)
{
    m_ActionOverThreshold = value;
}

/**
 Defines the scripts to be executed when the monitored value
 crosses the threshold for minimal duration time.
*/
CGXDLMSActionItem& CGXDLMSLimiter::GetActionUnderThreshold()
{
    return m_ActionUnderThreshold;
}
void CGXDLMSLimiter::SetActionUnderThreshold(CGXDLMSActionItem value)
{
    m_ActionUnderThreshold = value;
}

// Returns amount of attributes.
int CGXDLMSLimiter::GetAttributeCount()
{
    return 11;
}

// Returns amount of methods.
int CGXDLMSLimiter::GetMethodCount()
{
    return 0;
}

void CGXDLMSLimiter::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    if (m_MonitoredValue != NULL)
    {
        values.push_back(m_MonitoredValue->GetName().ToString());
    }
    else
    {
        values.push_back("");
    }
    values.push_back(m_ThresholdActive.ToString());
    values.push_back(m_ThresholdNormal.ToString());
    values.push_back(m_ThresholdEmergency.ToString());
    values.push_back(CGXDLMSVariant(m_MinOverThresholdDuration).ToString());
    values.push_back(CGXDLMSVariant(m_MinUnderThresholdDuration).ToString());
    values.push_back(m_EmergencyProfile.ToString());
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<int>::iterator it = m_EmergencyProfileGroupIDs.begin(); it != m_EmergencyProfileGroupIDs.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = CGXDLMSVariant(*it).ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
    values.push_back(CGXDLMSVariant(m_EmergencyProfileActive).ToString());

    sb.str(std::string());
    sb << m_ActionOverThreshold.ToString().c_str();
    sb << ", ";
    sb << m_ActionUnderThreshold.ToString().c_str();
    values.push_back(sb.str());
}

void CGXDLMSLimiter::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //MonitoredValue
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }

    //ThresholdActive
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }

    //ThresholdNormal
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }

    //ThresholdEmergency
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }

    //MinOverThresholdDuration
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }

    //MinUnderThresholdDuration
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }

    //EmergencyProfile
    if (all || CanRead(8))
    {
        attributes.push_back(8);
    }
    //EmergencyProfileGroup
    if (all || CanRead(9))
    {
        attributes.push_back(9);
    }

    //EmergencyProfileActive
    if (all || CanRead(10))
    {
        attributes.push_back(10);
    }
    //Actions
    if (all || CanRead(11))
    {
        attributes.push_back(11);
    }
}

int CGXDLMSLimiter::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
    }
    else if (index == 3)
    {
        CGXDLMSObject::GetDataType(index, type);
    }
    else if (index == 4)
    {
        CGXDLMSObject::GetDataType(index, type);
    }
    else if (index == 5)
    {
        CGXDLMSObject::GetDataType(index, type);
    }
    else if (index == 6)
    {
        type = DLMS_DATA_TYPE_UINT32;
    }
    else if (index == 7)
    {
        type = DLMS_DATA_TYPE_UINT32;
    }
    else if (index == 8)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
    }
    else if (index == 9)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 10)
    {
        type = DLMS_DATA_TYPE_BOOLEAN;
    }
    else if (index == 11)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSLimiter::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
    else if (e.GetIndex() == 2)
    {
        e.SetByteArray(true);
        int ret;
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(3);
        std::string ln;
        CGXDLMSVariant type = 0;
        if (m_MonitoredValue != NULL)
        {
            m_MonitoredValue->GetLogicalName(ln);
            type = m_MonitoredValue->GetObjectType();
        }
        CGXDLMSVariant index = m_MonitoredAttributeIndex;
        CGXDLMSVariant tmp;
        GXHelpers::SetLogicalName(ln.c_str(), tmp);
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, type)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, index)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue(m_ThresholdActive);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 4)
    {
        e.SetValue(m_ThresholdNormal);
    }
    else if (e.GetIndex() == 5)
    {
        e.SetValue(m_ThresholdEmergency);
    }
    else if (e.GetIndex() == 6)
    {
        e.SetValue(m_MinOverThresholdDuration);
    }
    else if (e.GetIndex() == 7)
    {
        e.SetValue(m_MinUnderThresholdDuration);
    }
    else if (e.GetIndex() == 8)
    {
        e.SetByteArray(true);
        int ret;
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(3);
        CGXDLMSVariant id = m_EmergencyProfile.GetID();
        CGXDLMSVariant time = m_EmergencyProfile.GetActivationTime();
        CGXDLMSVariant duration = m_EmergencyProfile.GetDuration();
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, id)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, time)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT32, duration)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
    }
    else if (e.GetIndex() == 9)
    {
        e.SetByteArray(true);
        int ret;
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount((unsigned long)m_EmergencyProfileGroupIDs.size(), data);
        CGXDLMSVariant tmp;
        for (std::vector<int>::iterator it = m_EmergencyProfileGroupIDs.begin(); it != m_EmergencyProfileGroupIDs.end(); ++it)
        {
            tmp = *it;
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, tmp)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
    }
    else if (e.GetIndex() == 10)
    {
        e.SetValue(m_EmergencyProfileActive);
    }
    else if (e.GetIndex() == 11)
    {
        e.SetByteArray(true);
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        int ret;
        CGXDLMSVariant ln;
        GXHelpers::SetLogicalName(m_ActionOverThreshold.GetLogicalName().c_str(), ln);
        CGXDLMSVariant selector = m_ActionOverThreshold.GetScriptSelector();
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, selector)) != 0)
        {
            return ret;
        }
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        GXHelpers::SetLogicalName(m_ActionUnderThreshold.GetLogicalName().c_str(), ln);
        selector = m_ActionUnderThreshold.GetScriptSelector();
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, selector)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int CGXDLMSLimiter::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        DLMS_OBJECT_TYPE ot = (DLMS_OBJECT_TYPE)e.GetValue().Arr[0].ToInteger();
        std::string ln;
        GXHelpers::GetLogicalName(e.GetValue().Arr[1].byteArr, ln);
        m_MonitoredValue = settings.GetObjects().FindByLN(ot, ln);
        m_MonitoredAttributeIndex = e.GetValue().Arr[2].ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_ThresholdActive = e.GetValue();
    }
    else if (e.GetIndex() == 4)
    {
        m_ThresholdNormal = e.GetValue();
    }
    else if (e.GetIndex() == 5)
    {
        m_ThresholdEmergency = e.GetValue();
    }
    else if (e.GetIndex() == 6)
    {
        m_MinOverThresholdDuration = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 7)
    {
        m_MinUnderThresholdDuration = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 8)
    {
        m_EmergencyProfile.SetID(e.GetValue().Arr[0].ToInteger());
        CGXDLMSVariant tmp;
        CGXDLMSClient::ChangeType(e.GetValue().Arr[1], DLMS_DATA_TYPE_DATETIME, tmp);
        m_EmergencyProfile.SetActivationTime(tmp.dateTime);
        m_EmergencyProfile.SetDuration(e.GetValue().Arr[2].ToInteger());
    }
    else if (e.GetIndex() == 9)
    {
        m_EmergencyProfileGroupIDs.clear();
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            m_EmergencyProfileGroupIDs.push_back((*it).ToInteger());
        }
    }
    else if (e.GetIndex() == 10)
    {
        m_EmergencyProfileActive = e.GetValue().boolVal;
    }
    else if (e.GetIndex() == 11)
    {
        std::string ln;
        GXHelpers::GetLogicalName(e.GetValue().Arr[0].Arr[0].byteArr, ln);
        m_ActionOverThreshold.SetLogicalName(ln);
        m_ActionOverThreshold.SetScriptSelector(e.GetValue().Arr[0].Arr[1].ToInteger());
        GXHelpers::GetLogicalName(e.GetValue().Arr[1].Arr[0].byteArr, ln);
        m_ActionUnderThreshold.SetLogicalName(ln);
        m_ActionUnderThreshold.SetScriptSelector(e.GetValue().Arr[1].Arr[1].ToInteger());
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
