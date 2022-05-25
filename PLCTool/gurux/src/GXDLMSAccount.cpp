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

#include "../include/GXDLMSAccount.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXBitString.h"

//Constructor.
CGXDLMSAccount::CGXDLMSAccount() :
    CGXDLMSAccount("", 0)
{
}

//SN Constructor.
CGXDLMSAccount::CGXDLMSAccount(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_ACCOUNT, ln, sn)
{
    m_PaymentMode = DLMS_ACCOUNT_PAYMENT_MODE_CREDIT;
    m_AccountStatus = DLMS_ACCOUNT_STATUS_NEW_INACTIVE_ACCOUNT;
    m_CurrentCreditInUse = 0;
    m_CurrentCreditStatus = DLMS_ACCOUNT_CREDIT_STATUS_IN_CREDIT;
    m_AvailableCredit = 0;
    m_AmountToClear = 0;
    m_ClearanceThreshold = 0;
    m_AggregatedDebt = 0;
    m_LowCreditThreshold = 0;
    m_NextCreditAvailableThreshold = 0;
    m_MaxProvision = 0;
    m_MaxProvisionPeriod = 0;
}

//LN Constructor.
CGXDLMSAccount::CGXDLMSAccount(std::string ln) :
    CGXDLMSAccount(ln, 0)
{

}

// Returns amount of attributes.
int CGXDLMSAccount::GetAttributeCount()
{
    return 19;
}

// Returns amount of methods.
int CGXDLMSAccount::GetMethodCount()
{
    return 3;
}

void AppendList(std::vector<std::string>& list, std::string& result)
{
    result.clear();
    for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); ++it)
    {
        result.append(*it);
        result.append(", ");
    }
}

void AppendList(std::vector<CGXCreditChargeConfiguration>& list, std::string& result)
{
    result.clear();
    for (std::vector<CGXCreditChargeConfiguration>::iterator it = list.begin(); it != list.end(); ++it)
    {
        result.append("{");
        result.append(it->ToString());
        result.append("}, ");
    }
}

void AppendList(std::vector<CGXTokenGatewayConfiguration>& list, std::string& result)
{
    result.clear();
    for (std::vector<CGXTokenGatewayConfiguration>::iterator it = list.begin(); it != list.end(); ++it)
    {
        result.append("{");
        result.append(it->ToString());
        result.append("}, ");
    }
}

void CGXDLMSAccount::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    ln = GXHelpers::IntToString(m_PaymentMode) + ", " + GXHelpers::IntToString(m_AccountStatus);
    values.push_back(ln);
    values.push_back(GXHelpers::IntToString(m_CurrentCreditInUse));
    values.push_back(GXHelpers::IntToString(m_CurrentCreditStatus));
    values.push_back(GXHelpers::IntToString(m_AvailableCredit));
    values.push_back(GXHelpers::IntToString(m_AmountToClear));
    values.push_back(GXHelpers::IntToString(m_ClearanceThreshold));
    values.push_back(GXHelpers::IntToString(m_AggregatedDebt));
    AppendList(m_CreditReferences, ln);
    values.push_back(ln);
    AppendList(m_ChargeReferences, ln);
    values.push_back(ln);
    AppendList(m_CreditChargeConfigurations, ln);
    values.push_back(ln);
    AppendList(m_TokenGatewayConfigurations, ln);
    values.push_back(ln);
    values.push_back(m_AccountActivationTime.ToString());
    values.push_back(m_AccountClosureTime.ToString());
    values.push_back(m_Currency.ToString());
    values.push_back(GXHelpers::IntToString(m_LowCreditThreshold));
    values.push_back(GXHelpers::IntToString(m_NextCreditAvailableThreshold));
    values.push_back(GXHelpers::IntToString(m_MaxProvision));
    values.push_back(GXHelpers::IntToString(m_MaxProvisionPeriod));
}

void CGXDLMSAccount::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // PaymentMode, AccountStatus
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // CurrentCreditInUse
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    // CurrentCreditStatus
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    // AvailableCredit
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    // AmountToClear
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    // ClearanceThreshold
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
    // AggregatedDebt
    if (all || CanRead(8))
    {
        attributes.push_back(8);
    }
    // CreditReferences
    if (all || CanRead(9))
    {
        attributes.push_back(9);
    }
    // ChargeReferences
    if (all || CanRead(10))
    {
        attributes.push_back(10);
    }
    // CreditChargeConfigurations
    if (all || CanRead(11))
    {
        attributes.push_back(11);
    }
    // TokenGatewayConfigurations
    if (all || CanRead(12))
    {
        attributes.push_back(12);
    }
    // AccountActivationTime
    if (all || CanRead(13))
    {
        attributes.push_back(13);
    }
    // AccountClosureTime
    if (all || CanRead(14))
    {
        attributes.push_back(14);
    }
    // Currency
    if (all || CanRead(15)) {
        attributes.push_back(15);
    }
    // LowCreditThreshold
    if (all || CanRead(16)) {
        attributes.push_back(16);
    }
    // NextCreditAvailableThreshold
    if (all || CanRead(17))
    {
        attributes.push_back(17);
    }
    // MaxProvision
    if (all || CanRead(18))
    {
        attributes.push_back(18);
    }
    // MaxProvisionPeriod
    if (all || CanRead(19))
    {
        attributes.push_back(19);
    }
}

int CGXDLMSAccount::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index) {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_STRUCTURE;
        break;
    case 3:
        type = DLMS_DATA_TYPE_UINT8;
        break;
    case 4:
        type = DLMS_DATA_TYPE_BIT_STRING;
        break;
    case 5:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 6:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 7:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 8:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 9:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 10:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 11:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 12:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 13:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 14:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 15:
        type = DLMS_DATA_TYPE_STRUCTURE;
        break;
    case 16:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 17:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 18:
        type = DLMS_DATA_TYPE_UINT16;
        break;
    case 19:
        type = DLMS_DATA_TYPE_INT32;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSAccount::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    CGXDLMSVariant tmp;
    CGXByteBuffer bb;
    unsigned char ln[6];
    int ret;
    switch (e.GetIndex())
    {
    case 1:
    {
        CGXDLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
    }
    break;
    case 2:
        bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        bb.SetUInt8(2);
        bb.SetUInt8(DLMS_DATA_TYPE_ENUM);
        bb.SetUInt8(m_AccountStatus);
        bb.SetUInt8(DLMS_DATA_TYPE_ENUM);
        bb.SetUInt8(m_PaymentMode);
        e.SetValue(bb);
        break;
    case 3:
        e.SetValue(m_CurrentCreditInUse);
        break;
    case 4:
        e.SetValue(CGXBitString::ToBitString(m_CurrentCreditStatus, 8));
        break;
    case 5:
        e.SetValue(m_AvailableCredit);
        break;
    case 6:
        e.SetValue(m_AmountToClear);
        break;
    case 7:
        e.SetValue(m_ClearanceThreshold);
        break;
    case 8:
        e.SetValue(m_AggregatedDebt);
        break;
    case 9:
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount((unsigned long)m_CreditReferences.size(), bb);
        for (std::vector<std::string>::iterator it = m_CreditReferences.begin(); it != m_CreditReferences.end(); ++it)
        {
            bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            bb.SetUInt8(6);
            if ((ret = GXHelpers::SetLogicalName(it->c_str(), ln)) != 0)
            {
                return ret;
            }
            bb.Set(ln, 6);
        }
        e.SetValue(bb);
        break;
    case 10:
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount((unsigned long)m_ChargeReferences.size(), bb);
        for (std::vector<std::string>::iterator it = m_ChargeReferences.begin(); it != m_ChargeReferences.end(); ++it)
        {
            bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            bb.SetUInt8(6);
            if ((ret = GXHelpers::SetLogicalName(it->c_str(), ln)) != 0)
            {
                return ret;
            }
            bb.Set(ln, 6);
        }
        e.SetValue(bb);
        break;
    case 11:
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount((unsigned long)m_CreditChargeConfigurations.size(), bb);
        for (std::vector<CGXCreditChargeConfiguration>::iterator it = m_CreditChargeConfigurations.begin(); it != m_CreditChargeConfigurations.end(); ++it)
        {
            bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            bb.SetUInt8(3);
            bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            bb.SetUInt8(6);
            if ((ret = GXHelpers::SetLogicalName(it->GetCreditReference().c_str(), ln)) != 0)
            {
                return ret;
            }
            bb.Set(ln, 6);
            bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            bb.SetUInt8(6);
            if ((ret = GXHelpers::SetLogicalName(it->GetChargeReference().c_str(), ln)) != 0)
            {
                return ret;
            }
            bb.Set(ln, 6);
            if ((ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_BIT_STRING, CGXBitString::ToBitString(it->GetCollectionConfiguration(), 3))) != 0)
            {
                return ret;
            }
        }
        e.SetValue(bb);
        break;
    case 12:
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount((unsigned long)m_TokenGatewayConfigurations.size(), bb);
        for (std::vector<CGXTokenGatewayConfiguration>::iterator it = m_TokenGatewayConfigurations.begin(); it != m_TokenGatewayConfigurations.end(); ++it)
        {
            bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            bb.SetUInt8(2);
            bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            bb.SetUInt8(6);
            if ((ret = GXHelpers::SetLogicalName(it->GetCreditReference().c_str(), ln)) != 0)
            {
                return ret;
            }
            bb.Set(ln, 6);
            bb.SetUInt8(DLMS_DATA_TYPE_UINT8);
            bb.SetUInt8(it->GetTokenProportion());
        }
        e.SetValue(bb);
        break;
    case 13:
        e.SetValue(m_AccountActivationTime);
        break;
    case 14:
        e.SetValue(m_AccountClosureTime);
        break;
    case 15:
        bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        bb.SetUInt8(3);
        tmp = m_Currency.GetName();
        if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_STRING_UTF8, tmp)) != 0)
        {
            return ret;
        }
        tmp = m_Currency.GetScale();
        if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_INT8, tmp)) != 0)
        {
            return ret;
        }
        tmp = m_Currency.GetUnit();
        if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_ENUM, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(bb);
        break;
    case 16:
        e.SetValue(m_LowCreditThreshold);
        break;
    case 17:
        e.SetValue(m_NextCreditAvailableThreshold);
        break;
    case 18:
        e.SetValue(m_MaxProvision);
        break;
    case 19:
        e.SetValue(m_MaxProvisionPeriod);
        break;
    default:
        return DLMS_ERROR_CODE_READ_WRITE_DENIED;
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMSAccount::Activate(
    CGXDLMSClient* client,
    std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int CGXDLMSAccount::Close(
    CGXDLMSClient* client,
    std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 2, data, reply);
}

int CGXDLMSAccount::Reset(
    CGXDLMSClient* client,
    std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 3, data, reply);
}

int CGXDLMSAccount::Invoke(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_AccountStatus = DLMS_ACCOUNT_STATUS_ACTIVE;
    }
    else if (e.GetIndex() == 2)
    {
        m_AccountStatus = DLMS_ACCOUNT_STATUS_CLOSED;
    }
    else if (e.GetIndex() == 3)
    {
        //Meter must handle this.
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}


// Set value of given attribute.
int CGXDLMSAccount::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret = DLMS_ERROR_CODE_OK;
    CGXDLMSVariant tmp;
    CGXByteBuffer bb;
    std::string ln;
    switch (e.GetIndex())
    {
    case 1:
        return SetLogicalName(this, e.GetValue());
        break;
    case 2:
        m_AccountStatus = (DLMS_ACCOUNT_STATUS)e.GetValue().Arr[0].ToInteger();
        m_PaymentMode = (DLMS_ACCOUNT_PAYMENT_MODE)e.GetValue().Arr[1].ToInteger();
        break;
    case 3:
        m_CurrentCreditInUse = e.GetValue().ToInteger();
        break;
    case 4:
        if (e.GetValue().vt == DLMS_DATA_TYPE_BIT_STRING)
        {
            m_CurrentCreditStatus = (DLMS_ACCOUNT_CREDIT_STATUS)e.GetValue().ToInteger();
        }
        else
        {
            m_CurrentCreditStatus = DLMS_ACCOUNT_CREDIT_STATUS_NONE;
        }
        break;
    case 5:
        m_AvailableCredit = e.GetValue().ToInteger();
        break;
    case 6:
        m_AmountToClear = e.GetValue().ToInteger();
        break;
    case 7:
        m_ClearanceThreshold = e.GetValue().ToInteger();
        break;
    case 8:
        m_AggregatedDebt = e.GetValue().ToInteger();
        break;
    case 9:
        m_CreditReferences.clear();
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            GXHelpers::GetLogicalName(it->byteArr, ln);
            m_CreditReferences.push_back(ln);
        }
        break;
    case 10:
        m_ChargeReferences.clear();
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            GXHelpers::GetLogicalName(it->byteArr, ln);
            m_ChargeReferences.push_back(ln);
        }
        break;
    case 11:
        m_CreditChargeConfigurations.clear();
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            CGXCreditChargeConfiguration item;
            GXHelpers::GetLogicalName(it->Arr[0].byteArr, ln);
            item.SetCreditReference(ln);
            GXHelpers::GetLogicalName(it->Arr[1].byteArr, ln);
            item.SetChargeReference(ln);
            item.SetCollectionConfiguration((DLMS_CREDIT_COLLECTION_CONFIGURATION)it->Arr[2].ToInteger());
            m_CreditChargeConfigurations.push_back(item);
        }
        break;
    case 12:
        m_TokenGatewayConfigurations.clear();
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            CGXTokenGatewayConfiguration item;
            GXHelpers::GetLogicalName(it->Arr[0].byteArr, ln);
            item.SetCreditReference(ln);
            item.SetTokenProportion(it->Arr[1].ToInteger());
            m_TokenGatewayConfigurations.push_back(item);
        }
        break;
    case 13:
        if (e.GetValue().vt != 0)
        {
            if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                e.GetValue().GetBytes(bb);
                CGXDataInfo info;
                info.SetType(DLMS_DATA_TYPE_DATETIME);
                if ((ret = GXHelpers::GetData(bb, info, tmp)) != 0)
                {
                    return ret;
                }
                m_AccountActivationTime = tmp.dateTime;
            }
            else
            {
                m_AccountActivationTime = e.GetValue().dateTime;
            }
        }
        break;
    case 14:
        if (e.GetValue().vt != 0)
        {
            if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                e.GetValue().GetBytes(bb);
                CGXDataInfo info;
                info.SetType(DLMS_DATA_TYPE_DATETIME);
                if ((ret = GXHelpers::GetData(bb, info, tmp)) != 0)
                {
                    return ret;
                }
                m_AccountClosureTime = tmp.dateTime;
            }
            else
            {
                m_AccountClosureTime = e.GetValue().dateTime;
            }
        }
        break;
    case 15:
        m_Currency.SetName(e.GetValue().Arr[0].strVal);
        m_Currency.SetScale(e.GetValue().Arr[1].ToInteger());
        m_Currency.SetUnit((DLMS_CURRENCY)e.GetValue().Arr[2].ToInteger());
        break;
    case 16:
        m_LowCreditThreshold = e.GetValue().ToInteger();
        break;
    case 17:
        m_NextCreditAvailableThreshold = e.GetValue().ToInteger();
        break;
    case 18:
        m_MaxProvision = e.GetValue().ToInteger();
        break;
    case 19:
        m_MaxProvisionPeriod = e.GetValue().ToInteger();
        break;
    default:
        ret =  DLMS_ERROR_CODE_READ_WRITE_DENIED;
    }
    return ret;
}
