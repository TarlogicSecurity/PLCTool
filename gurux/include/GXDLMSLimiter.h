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

#ifndef GXDLMSLIMITER_H
#define GXDLMSLIMITER_H

#include "GXDLMSObject.h"
#include "GXDLMSEmergencyProfile.h"
#include "GXDLMSActionItem.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSLimiter
*/
class CGXDLMSLimiter : public CGXDLMSObject
{
    CGXDLMSObject* m_MonitoredValue;
    int m_MonitoredAttributeIndex;
    CGXDLMSVariant m_ThresholdActive;
    CGXDLMSVariant m_ThresholdNormal;
    CGXDLMSVariant m_ThresholdEmergency;
    long m_MinOverThresholdDuration;
    long m_MinUnderThresholdDuration;
    CGXDLMSEmergencyProfile m_EmergencyProfile;
    std::vector<int> m_EmergencyProfileGroupIDs;
    bool m_EmergencyProfileActive;
    CGXDLMSActionItem m_ActionOverThreshold;
    CGXDLMSActionItem m_ActionUnderThreshold;
public:
    //Constructor.
    CGXDLMSLimiter();

    //SN Constructor.
    CGXDLMSLimiter(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSLimiter(std::string ln);


    /**
     Defines an attribute of an object to be monitored.
    */
    CGXDLMSObject* GetMonitoredValue();
    void SetMonitoredValue(CGXDLMSObject* value);

    /**
     Provides the active threshold value to which the attribute monitored is compared.
    */
    CGXDLMSVariant& GetThresholdActive();
    void SetThresholdActive(CGXDLMSVariant& value);

    /**
     Provides the threshold value to which the attribute monitored
     is compared when in normal operation.
    */
    CGXDLMSVariant& GetThresholdNormal();
    void SetThresholdNormal(CGXDLMSVariant& value);

    /**
     Provides the threshold value to which the attribute monitored
     is compared when an emergency profile is active.
    */
    CGXDLMSVariant& GetThresholdEmergency();
    void SetThresholdEmergency(CGXDLMSVariant& value);

    /**
     Defines minimal over threshold duration in seconds required
     to execute the over threshold action.
    */
    long GetMinOverThresholdDuration();
    void SetMinOverThresholdDuration(long value);

    /**
     Defines minimal under threshold duration in seconds required to
     execute the under threshold action.
    */
    long GetMinUnderThresholdDuration();
    void SetMinUnderThresholdDuration(long value);

    CGXDLMSEmergencyProfile& GetEmergencyProfile();
    void SetEmergencyProfile(CGXDLMSEmergencyProfile value);

    std::vector<int>& GetEmergencyProfileGroupIDs();
    void SetEmergencyProfileGroupIDs(std::vector<int> value);

    /**
     Is Emergency Profile active.
    */
    bool GetEmergencyProfileActive();
    void SetEmergencyProfileActive(bool value);

    /**
     Defines the scripts to be executed when the monitored value
     crosses the threshold for minimal duration time.
    */
    CGXDLMSActionItem& GetActionOverThreshold();
    void SetActionOverThreshold(CGXDLMSActionItem value);

    /**
     Defines the scripts to be executed when the monitored value
     crosses the threshold for minimal duration time.
    */
    CGXDLMSActionItem& GetActionUnderThreshold();
    void SetActionUnderThreshold(CGXDLMSActionItem value);

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
#endif //GXDLMSLIMITER_H