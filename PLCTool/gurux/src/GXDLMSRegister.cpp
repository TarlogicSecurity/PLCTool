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
#include "../include/GXDLMSRegister.h"
#include "../include/GXDLMSConverter.h"

bool CGXDLMSRegister::IsRead(int index)
{
    if (index == 3)
    {
        return m_Unit != 0;
    }
    return CGXDLMSObject::IsRead(index);
}

//Constructor.
CGXDLMSRegister::CGXDLMSRegister() :
    CGXDLMSRegister("", 0)
{
}

CGXDLMSRegister::CGXDLMSRegister(DLMS_OBJECT_TYPE type, std::string ln, unsigned short sn) :
    CGXDLMSObject(type, ln, sn)
{
    m_Unit = 0;
    m_Scaler = 0;
}

//SN Constructor.
CGXDLMSRegister::CGXDLMSRegister(std::string ln, unsigned short sn) :
    CGXDLMSRegister(DLMS_OBJECT_TYPE_REGISTER, ln, sn)
{
}

//LN Constructor.
CGXDLMSRegister::CGXDLMSRegister(std::string ln) :
    CGXDLMSRegister(ln, 0)
{
}

/// <summary>
/// Get value of COSEM Data object.
/// </summary>
CGXDLMSVariant& CGXDLMSRegister::GetValue()
{
    return m_Value;
}

/// <summary>
/// Set value of COSEM Data object.
/// </summary>
void CGXDLMSRegister::SetValue(CGXDLMSVariant& value)
{
    m_Value = value;
}

// Scaler of COSEM Register object.
double CGXDLMSRegister::GetScaler()
{
    return pow((float)10, m_Scaler);
}

void CGXDLMSRegister::SetScaler(double value)
{
    m_Scaler = (signed char)log10(value);
}

// Unit of COSEM Register object.
int CGXDLMSRegister::GetUnit()
{
    return m_Unit;
}

void CGXDLMSRegister::SetUnit(unsigned char value)
{
    m_Unit = value;
}

/*
 * Reset value.
 */
void CGXDLMSRegister::Reset()
{
    m_Value.Clear();
}

// Returns amount of attributes.
int CGXDLMSRegister::GetAttributeCount()
{
    return 3;
}

// Returns amount of methods.
int CGXDLMSRegister::GetMethodCount()
{
    return 1;
}

int CGXDLMSRegister::Reset(
    CGXDLMSClient* client,
    std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int CGXDLMSRegister::Invoke(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_Value.Clear();
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

void CGXDLMSRegister::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_Value.ToString());
    std::string str = "Scaler: ";
    double s = GetScaler();
    //if there is no fractal part.
    if (s - (long)s == 0)
    {
        str += CGXDLMSVariant((long)s).ToString();
    }
    else
    {
        str += CGXDLMSVariant(s).ToString();
    }
    str += " Unit: ";
    str += CGXDLMSConverter::GetUnitAsString(m_Unit);
    values.push_back(str);
}

void CGXDLMSRegister::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ScalerUnit
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //Value
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
}

int CGXDLMSRegister::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        return CGXDLMSObject::GetDataType(index, type);
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int CGXDLMSRegister::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret;
    CGXDLMSVariant tmp;
    if (e.GetIndex() == 1)
    {
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 2)
    {
        DLMS_DATA_TYPE dt;
        if ((ret = CGXDLMSObject::GetDataType(2, dt)) != 0)
        {
            return ret;
        }
        tmp = m_Value;
        if (m_Scaler != 0)
        {
            if (dt == DLMS_DATA_TYPE_NONE)
            {
                dt = m_Value.vt;
            }
            if ((ret = tmp.ChangeType(DLMS_DATA_TYPE_FLOAT64)) != 0)
            {
                return ret;
            }
            tmp = m_Value.dblVal / GetScaler();
            if ((ret = tmp.ChangeType(dt)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        CGXByteBuffer buff;
        buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        buff.SetUInt8(2);
        buff.SetUInt8(DLMS_DATA_TYPE_INT8);
        buff.SetUInt8(m_Scaler);
        buff.SetUInt8(DLMS_DATA_TYPE_ENUM);
        buff.SetUInt8(m_Unit);
        e.SetValue(buff);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int CGXDLMSRegister::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        if (m_Scaler != 0 && e.GetValue().IsNumber())
        {
            double val = GetScaler();
            val *= e.GetValue().ToDouble();
            CGXDLMSVariant tmp(val);
            SetValue(tmp);
        }
        else
        {
            SetValue(e.GetValue());
        }
    }
    else if (e.GetIndex() == 3 && e.GetValue().vt == DLMS_DATA_TYPE_STRUCTURE)
    {
        m_Scaler = e.GetValue().Arr[0].ToInteger();
        m_Unit = e.GetValue().Arr[1].ToInteger();
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
