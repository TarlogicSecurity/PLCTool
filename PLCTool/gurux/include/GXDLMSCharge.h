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

#ifndef GXCHARGE_H
#define GXCHARGE_H

#include "GXDLMSObject.h"
#include "GXUnitCharge.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
*/
class CGXDLMSCharge : public CGXDLMSObject
{
    long m_TotalAmountPaid;
    DLMS_CHARGE_TYPE m_ChargeType;
    unsigned char m_Priority;
    CGXUnitCharge m_UnitChargeActive;
    CGXUnitCharge m_UnitChargePassive;
    CGXDateTime m_UnitChargeActivationTime;
    unsigned long m_Period;
    DLMS_CHARGE_CONFIGURATION m_ChargeConfiguration;
    CGXDateTime m_LastCollectionTime;
    long m_LastCollectionAmount;
    long m_TotalAmountRemaining;
    unsigned short m_Proportion;

    static int GetUnitCharge(CGXUnitCharge& charge, CGXByteBuffer& bb);
    static int SetUnitCharge(CGXDLMSSettings& settings, CGXUnitCharge& charge, CGXDLMSValueEventArg& e);
public:
    //Constructor.
    CGXDLMSCharge();

    //SN Constructor.
    CGXDLMSCharge(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSCharge(std::string ln);

    /**
    * Online help:<br>
    * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
    *
    * @return Total amount paid.
    */
    long GetTotalAmountPaid()
    {
        return m_TotalAmountPaid;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Total amount paid.
     */
    void SetTotalAmountPaid(long value)
    {
        m_TotalAmountPaid = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Charge type.
     */
    DLMS_CHARGE_TYPE GetChargeType()
    {
        return m_ChargeType;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Charge type.
     */
    void SetChargeType(DLMS_CHARGE_TYPE value)
    {
        m_ChargeType = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Priority
     */
    unsigned char GetPriority()
    {
        return m_Priority;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Priority
     */
    void SetPriority(unsigned char  value)
    {
        m_Priority = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Unit charge active.
     */
    CGXUnitCharge& GetUnitChargeActive()
    {
        return m_UnitChargeActive;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Unit charge passive.
     */
    CGXUnitCharge& GetUnitChargePassive()
    {
        return m_UnitChargePassive;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Unit charge activation time.
     */
    CGXDateTime& GetUnitChargeActivationTime()
    {
        return m_UnitChargeActivationTime;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Unit charge activation time.
     */
    void SetUnitChargeActivationTime(CGXDateTime& value)
    {
        m_UnitChargeActivationTime = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Period.
     */
    unsigned long GetPeriod()
    {
        return m_Period;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Period.
     */
    void SetPeriod(unsigned long value)
    {
        m_Period = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Charge configuration.
     */
    DLMS_CHARGE_CONFIGURATION GetChargeConfiguration()
    {
        return m_ChargeConfiguration;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Charge configuration.
     */
    void SetChargeConfiguration(DLMS_CHARGE_CONFIGURATION value)
    {
        m_ChargeConfiguration = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Last collection time.
     */
    CGXDateTime& GetLastCollectionTime()
    {
        return m_LastCollectionTime;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Last collection time.
     */
    void SetLastCollectionTime(CGXDateTime& value)
    {
        m_LastCollectionTime = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Last collection amount.
     */
    long GetLastCollectionAmount()
    {
        return m_LastCollectionAmount;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Last collection amount.
     */
    void SetLastCollectionAmount(long value)
    {
        m_LastCollectionAmount = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Total amount remaining
     */
    long GetTotalAmountRemaining()
    {
        return m_TotalAmountRemaining;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Total amount remaining
     */
    void SetTotalAmountRemaining(long value)
    {
        m_TotalAmountRemaining = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Proportion
     */
    unsigned short GetProportion()
    {
        return m_Proportion;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Proportion
     */
    void SetProportion(unsigned short value)
    {
        m_Proportion = value;
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
    int GetUIDataType(int index, DLMS_DATA_TYPE& type);

    // Returns value of given attribute.
    int GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);
};
#endif //GXCHARGE_H