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

#include "../include/GXDLMSCredit.h"
#include "../include/GXBitString.h"

//Constructor.
CGXDLMSCredit::CGXDLMSCredit() :
    CGXDLMSCredit("", 0)
{
}

//SN Constructor.
CGXDLMSCredit::CGXDLMSCredit(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_CREDIT, ln, sn)
{

}

//LN Constructor.
CGXDLMSCredit::CGXDLMSCredit(std::string ln) :
    CGXDLMSCredit(ln, 0)
{

}

// Returns amount of attributes.
int CGXDLMSCredit::GetAttributeCount()
{
    return 11;
}

// Returns amount of methods.
int CGXDLMSCredit::GetMethodCount()
{
    return 3;
}

void CGXDLMSCredit::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(GXHelpers::IntToString(m_CurrentCreditAmount));
    values.push_back(GXHelpers::IntToString(m_Type));
    values.push_back(GXHelpers::IntToString(m_Priority));
    values.push_back(GXHelpers::IntToString(m_WarningThreshold));
    values.push_back(GXHelpers::IntToString(m_Limit));
    values.push_back(GXHelpers::IntToString(m_CreditConfiguration));
    values.push_back(GXHelpers::IntToString(m_Status));
    values.push_back(GXHelpers::IntToString(m_PresetCreditAmount));
    values.push_back(GXHelpers::IntToString(m_CreditAvailableThreshold));
    values.push_back(m_Period.ToString());
}

void CGXDLMSCredit::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // CurrentCreditAmount
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // Type
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    // Priority
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    // WarningThreshold
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    // Limit
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    // creditConfiguration
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
    // Status
    if (all || CanRead(8))
    {
        attributes.push_back(8);
    }
    // PresetCreditAmount
    if (all || CanRead(9))
    {
        attributes.push_back(9);
    }
    // CreditAvailableThreshold
    if (all || CanRead(10))
    {
        attributes.push_back(10);
    }
    // Period
    if (all || CanRead(11))
    {
        attributes.push_back(11);
    }
}

int CGXDLMSCredit::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index)
    {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 3:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    case 4:
        type = DLMS_DATA_TYPE_UINT8;
        break;
    case 5:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 6:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 7:
        type = DLMS_DATA_TYPE_BIT_STRING;
        break;
    case 8:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    case 9:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 10:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 11:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSCredit::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret;
    CGXDLMSVariant tmp;
    switch (e.GetIndex())
    {
    case 1:
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        break;
    case 2:
        e.SetValue(m_CurrentCreditAmount);
        break;
    case 3:
        e.SetValue(m_Type);
        break;
    case 4:
        e.SetValue(m_Priority);
        break;
    case 5:
        e.SetValue(m_WarningThreshold);
        break;
    case 6:
        e.SetValue(m_Limit);
        break;
    case 7:
        e.SetValue(CGXBitString::ToBitString(m_CreditConfiguration, 5));
        break;
    case 8:
        e.SetValue(m_Status);
        break;
    case 9:
        e.SetValue(m_PresetCreditAmount);
        break;
    case 10:
        e.SetValue(m_CreditAvailableThreshold);
        break;
    case 11:
        e.SetValue(m_Period);
        break;
    default:
        return DLMS_ERROR_CODE_READ_WRITE_DENIED;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int CGXDLMSCredit::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret;
    CGXByteBuffer bb;
    CGXDLMSVariant tmp;
    switch (e.GetIndex())
    {
    case 1:
        return SetLogicalName(this, e.GetValue());
    case 2:
        m_CurrentCreditAmount = e.GetValue().ToInteger();
        break;
    case 3:
        m_Type = (DLMS_CREDIT_TYPE)e.GetValue().ToInteger();
        break;
    case 4:
        m_Priority = e.GetValue().ToInteger();
        break;
    case 5:
        m_WarningThreshold = e.GetValue().ToInteger();
        break;
    case 6:
        m_Limit = e.GetValue().ToInteger();
        break;
    case 7:
        m_CreditConfiguration = (DLMS_CREDIT_CONFIGURATION)e.GetValue().ToInteger();
        break;
    case 8:
        m_Status = (DLMS_CREDIT_STATUS)e.GetValue().ToInteger();
        break;
    case 9:
        m_PresetCreditAmount = e.GetValue().ToInteger();
        break;
    case 10:
        m_CreditAvailableThreshold = e.GetValue().ToInteger();
        break;
    case 11:
        if (e.GetValue().vt == 0)
        {
            return DLMS_ERROR_CODE_READ_WRITE_DENIED;
        }
        else
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
                m_Period = tmp.dateTime;
            }
            else
            {
                m_Period = e.GetValue().dateTime;
            }
        }
        break;
    default:
        return DLMS_ERROR_CODE_READ_WRITE_DENIED;
    }
    return DLMS_ERROR_CODE_OK;
}
