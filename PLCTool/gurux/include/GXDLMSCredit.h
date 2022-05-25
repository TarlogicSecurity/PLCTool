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

#ifndef GXCREDIT_H
#define GXCREDIT_H

#include "GXDLMSObject.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCredit
*/
class CGXDLMSCredit : public CGXDLMSObject
{
    long m_CurrentCreditAmount;
    DLMS_CREDIT_TYPE m_Type;
    unsigned char m_Priority;
    long m_WarningThreshold;
    long m_Limit;
    DLMS_CREDIT_CONFIGURATION m_CreditConfiguration;
    DLMS_CREDIT_STATUS m_Status;
    long m_PresetCreditAmount;
    long m_CreditAvailableThreshold;
    CGXDateTime m_Period;
public:
    //Constructor.
    CGXDLMSCredit();

    //SN Constructor.
    CGXDLMSCredit(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSCredit(std::string ln);

    /**
    * @return Current credit amount.
    */
    long GetCurrentCreditAmount()
    {
        return m_CurrentCreditAmount;
    }

    /**
     * @param value
     *            Current credit amount.
     */
    void SetCurrentCreditAmount(long value)
    {
        m_CurrentCreditAmount = value;
    }

    /**
     * @return Type.
     */
    DLMS_CREDIT_TYPE GetType()
    {
        return m_Type;
    }

    /**
     * @param value
     *            Type.
     */
    void SetType(DLMS_CREDIT_TYPE value)
    {
        m_Type = value;
    }

    /**
     * @return Priority.
     */
    unsigned char GetPriority()
    {
        return m_Priority;
    }

    /**
     * @param value
     *            Priority.
     */
    void SetPriority(unsigned char value)
    {
        m_Priority = value;
    }

    /**
     * @return Warning threshold.
     */
    long GetWarningThreshold()
    {
        return m_WarningThreshold;
    }

    /**
     * @param value
     *            Warning threshold.
     */
    void SetWarningThreshold(long value)
    {
        m_WarningThreshold = value;
    }

    /**
     * @return Limit.
     */
    long GetLimit()
    {
        return m_Limit;
    }

    /**
     * @param value
     *            Limit.
     */
    void SetLimit(long value)
    {
        m_Limit = value;
    }

    /**
     * @return Credit configuration.
     */
    DLMS_CREDIT_CONFIGURATION GetCreditConfiguration()
    {
        return m_CreditConfiguration;
    }

    /**
     * @param value
     *            Credit configuration.
     */
    void SetCreditConfiguration(DLMS_CREDIT_CONFIGURATION value)
    {
        m_CreditConfiguration = value;
    }

    /**
     * @return Status.
     */
    DLMS_CREDIT_STATUS GetStatus()
    {
        return m_Status;
    }

    /**
     * @param value
     *            Status.
     */
    void SetStatus(DLMS_CREDIT_STATUS value)
    {
        m_Status = value;
    }

    /**
     * @return Preset credit amount.
     */
    long GetPresetCreditAmount()
    {
        return m_PresetCreditAmount;
    }

    /**
     * @param value
     *            Preset credit amount.
     */
    void SetPresetCreditAmount(long value)
    {
        m_PresetCreditAmount = value;
    }

    /**
     * @return Credit available threshold.
     */
    long GetCreditAvailableThreshold()
    {
        return m_CreditAvailableThreshold;
    }

    /**
     * @param value
     *            Credit available threshold.
     */
    void SetCreditAvailableThreshold(long value)
    {
        m_CreditAvailableThreshold = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCredit
     *
     * @return Period
     */
    CGXDateTime& GetPeriod()
    {
        return m_Period;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCredit
     *
     * @param value
     *            Period
     */
    void setPeriod(CGXDateTime& value)
    {
        m_Period = value;
    }


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
#endif //GXCREDIT_H