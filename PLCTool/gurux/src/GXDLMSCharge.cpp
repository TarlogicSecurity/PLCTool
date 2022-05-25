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

#include "../include/GXDLMSCharge.h"
#include "../include/GXBitString.h"

//Constructor.
CGXDLMSCharge::CGXDLMSCharge() :
    CGXDLMSCharge("0.0.19.20.0.255", 0)
{
}

//SN Constructor.
CGXDLMSCharge::CGXDLMSCharge(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_CHARGE, ln, sn)
{
    m_ChargeType = DLMS_CHARGE_TYPE_CONSUMPTION_BASED_COLLECTION;
}

//LN Constructor.
CGXDLMSCharge::CGXDLMSCharge(std::string ln) :
    CGXDLMSCharge(ln, 0)
{

}

// Returns amount of attributes.
int CGXDLMSCharge::GetAttributeCount()
{
    return 13;
}

// Returns amount of methods.
int CGXDLMSCharge::GetMethodCount()
{
    return 5;
}

void CGXDLMSCharge::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(GXHelpers::IntToString(m_TotalAmountPaid));
    values.push_back(GXHelpers::IntToString(m_ChargeType));
    values.push_back(GXHelpers::IntToString(m_Priority));
    values.push_back(m_UnitChargeActive.ToString());
    values.push_back(m_UnitChargePassive.ToString());
    values.push_back(m_UnitChargeActivationTime.ToString());
    values.push_back(GXHelpers::IntToString(m_Period));
    values.push_back(GXHelpers::IntToString(m_ChargeConfiguration));
    values.push_back(m_LastCollectionTime.ToString());
    values.push_back(GXHelpers::IntToString(m_LastCollectionAmount));
    values.push_back(GXHelpers::IntToString(m_TotalAmountRemaining));
    values.push_back(GXHelpers::IntToString(m_Proportion));
}

void CGXDLMSCharge::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //TotalAmountPaid
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //ChargeType
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //Priority
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    //UnitChargeActive
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    //UnitChargePassive
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    //UnitChargeActivationTime
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
    //Period
    if (all || CanRead(8))
    {
        attributes.push_back(8);
    }
    //ChargeConfiguration
    if (all || CanRead(9))
    {
        attributes.push_back(9);
    }
    //LastCollectionTime
    if (all || CanRead(10))
    {
        attributes.push_back(10);
    }
    //LastCollectionAmount
    if (all || CanRead(11))
    {
        attributes.push_back(11);
    }
    //TotalAmountRemaining
    if (all || CanRead(12))
    {
        attributes.push_back(12);
    }
    //Proportion
    if (all || CanRead(13))
    {
        attributes.push_back(13);
    }
}

int CGXDLMSCharge::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_STRUCTURE;
        break;
    case 6:
        type = DLMS_DATA_TYPE_STRUCTURE;
        break;
    case 7:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 8:
        type = DLMS_DATA_TYPE_UINT32;
        break;
    case 9:
        type = DLMS_DATA_TYPE_BIT_STRING;
        break;
    case 10:
        type = DLMS_DATA_TYPE_DATETIME;
        break;
    case 11:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 12:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 13:
        type = DLMS_DATA_TYPE_UINT16;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMSCharge::GetUIDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 7 || index == 10)
    {
        type = DLMS_DATA_TYPE_DATETIME;
    }
    else
    {
        return CGXDLMSObject::GetUIDataType(index, type);
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMSCharge::GetUnitCharge(CGXUnitCharge& charge, CGXByteBuffer& bb)
{
    int ret;
    CGXDLMSVariant tmp;
    bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    bb.SetUInt8(3);
    bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    bb.SetUInt8(2);
    tmp = charge.GetChargePerUnitScaling().GetCommodityScale();
    if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_INT8, tmp)) != 0)
    {
        return ret;
    }
    tmp = charge.GetChargePerUnitScaling().GetPriceScale();
    if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_INT8, tmp)) != 0)
    {
        return ret;
    }
    bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    bb.SetUInt8(3);
    tmp = charge.GetCommodity().GetType();
    if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_UINT16, tmp)) != 0)
    {
        return ret;
    }
    bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
    bb.SetUInt8(6);
    bb.Set(charge.GetCommodity().GetLogicalName(), 6);
    tmp = charge.GetCommodity().GetIndex();
    if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_INT8, tmp)) != 0)
    {
        return ret;
    }
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    GXHelpers::SetObjectCount((unsigned long)charge.GetChargeTables().size(), bb);
    for (std::vector<CGXChargeTable>::iterator it = charge.GetChargeTables().begin(); it != charge.GetChargeTables().end(); ++it)
    {
        bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        bb.SetUInt8(2);
        tmp = it->GetIndex();
        if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
        {
            return ret;
        }
        tmp = it->GetChargePerUnit();
        if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_INT16, tmp)) != 0)
        {
            return ret;
        }
    }
    return 0;
}

// Returns value of given attribute.
int CGXDLMSCharge::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret;
    CGXDLMSVariant tmp;
    CGXByteBuffer bb;
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
        e.SetValue(m_TotalAmountPaid);
        break;
    case 3:
        e.SetValue(m_ChargeType);
        break;
    case 4:
        e.SetValue(m_Priority);
        break;
    case 5:
        if ((ret = GetUnitCharge(m_UnitChargeActive, bb)) != 0)
        {
            return ret;
        }
        e.SetValue(bb);
        break;
    case 6:
        if ((ret = GetUnitCharge(m_UnitChargePassive, bb)) != 0)
        {
            return ret;
        }
        e.SetValue(bb);
        break;
    case 7:
        e.SetValue(m_UnitChargeActivationTime);
        break;
    case 8:
        e.SetValue(m_Period);
        break;
    case 9:
        e.SetValue(CGXBitString::ToBitString(m_ChargeConfiguration, 2));
        break;
    case 10:
        e.SetValue(m_LastCollectionTime);
        break;
    case 11:
        e.SetValue(m_LastCollectionAmount);
        break;
    case 12:
        e.SetValue(m_TotalAmountRemaining);
        break;
    case 13:
        e.SetValue(m_Proportion);
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMSCharge::SetUnitCharge(CGXDLMSSettings&, CGXUnitCharge& charge, CGXDLMSValueEventArg& e)
{
    std::vector<CGXDLMSVariant>& tmp = e.GetValue().Arr;
    charge.GetChargePerUnitScaling().SetCommodityScale(tmp[0].Arr[0].ToInteger());
    charge.GetChargePerUnitScaling().SetPriceScale(tmp[0].Arr[1].ToInteger());
    charge.GetCommodity().SetType((DLMS_OBJECT_TYPE)tmp[1].Arr[0].ToInteger());
    charge.GetCommodity().SetLogicalName(tmp[1].Arr[1].byteArr);
    charge.GetCommodity().SetIndex(tmp[1].Arr[2].ToInteger());
    charge.GetChargeTables().clear();
    for (std::vector<CGXDLMSVariant>::iterator it = tmp[2].Arr.begin(); it != tmp[2].Arr.end(); ++it)
    {
        CGXChargeTable item;
        item.SetIndex(it->Arr[0].strVal);
        item.SetChargePerUnit(it->Arr[1].ToInteger());
        charge.GetChargeTables().push_back(item);
    }
    return 0;
}

// Set value of given attribute.
int CGXDLMSCharge::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    int ret;
    CGXByteBuffer bb;
    CGXDLMSVariant tmp;
    switch (e.GetIndex())
    {
    case 1:
        return SetLogicalName(this, e.GetValue());
    case 2:
        m_TotalAmountPaid = e.GetValue().ToInteger();
        break;
    case 3:
        m_ChargeType = (DLMS_CHARGE_TYPE)e.GetValue().ToInteger();
        break;
    case 4:
        m_Priority = e.GetValue().ToInteger();
        break;
    case 5:
        SetUnitCharge(settings, m_UnitChargeActive, e);
        break;
    case 6:
        SetUnitCharge(settings, m_UnitChargePassive, e);
        break;
    case 7:
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            e.GetValue().GetBytes(bb);
            CGXDataInfo info;
            if ((ret = GXHelpers::GetData(bb, info, tmp)) != 0)
            {
                return ret;
            }
            m_UnitChargeActivationTime = tmp.dateTime;
        }
        else if (e.GetValue().vt == DLMS_DATA_TYPE_DATETIME)
        {
            m_UnitChargeActivationTime = e.GetValue().dateTime;
        }
        else
        {
            m_UnitChargeActivationTime.Reset();
        }
        break;
    case 8:
        m_Period = e.GetValue().ToInteger();
        break;
    case 9:
        if (e.GetValue().vt == DLMS_DATA_TYPE_BIT_STRING)
        {
            m_ChargeConfiguration = (DLMS_CHARGE_CONFIGURATION)e.GetValue().ToInteger();
            break;
        }
    case 10:
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            e.GetValue().GetBytes(bb);
            CGXDataInfo info;
            if ((ret = GXHelpers::GetData(bb, info, tmp)) != 0)
            {
                return ret;
            }
            m_LastCollectionTime = tmp.dateTime;
        }
        else
        {
            m_LastCollectionTime = e.GetValue().dateTime;
        }
        break;
    case 11:
        m_LastCollectionAmount = e.GetValue().ToInteger();
        break;
    case 12:
        m_TotalAmountRemaining = e.GetValue().ToInteger();
        break;
    case 13:
        m_Proportion = e.GetValue().ToInteger();
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
