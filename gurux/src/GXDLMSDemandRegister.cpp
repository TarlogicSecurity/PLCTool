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
#include "../include/GXDLMSConverter.h"
#include "../include/GXDLMSDemandRegister.h"

bool CGXDLMSDemandRegister::IsRead(int index)
{
    if (index == 4)
    {
        return m_Unit != 0;
    }
    return CGXDLMSObject::IsRead(index);
}

CGXDLMSDemandRegister::CGXDLMSDemandRegister() :
    CGXDLMSDemandRegister("", 0)
{
}

CGXDLMSDemandRegister::CGXDLMSDemandRegister(std::string ln) :
    CGXDLMSDemandRegister(ln, 0)
{
}

CGXDLMSDemandRegister::CGXDLMSDemandRegister(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_DEMAND_REGISTER, ln, sn)
{
    m_Period = m_NumberOfPeriods = m_Unit = m_Scaler = 0;
}

CGXDLMSVariant& CGXDLMSDemandRegister::GetCurrentAverageValue()
{
    return m_CurrentAverageValue;
}
void CGXDLMSDemandRegister::SetCurrentAverageValue(CGXDLMSVariant& value)
{
    m_CurrentAverageValue = value;
}

void CGXDLMSDemandRegister::SetCurrentAverageValue(CGXDLMSVariant value)
{
    m_CurrentAverageValue = value;
}

CGXDLMSVariant& CGXDLMSDemandRegister::GetLastAverageValue()
{
    return m_LastAverageValue;
}
void CGXDLMSDemandRegister::SetLastAverageValue(CGXDLMSVariant& value)
{
    m_LastAverageValue = value;
}
void CGXDLMSDemandRegister::SetLastAverageValue(CGXDLMSVariant value)
{
    m_LastAverageValue = value;
}

double CGXDLMSDemandRegister::GetScaler()
{
    return pow((float)10, m_Scaler);
}

void CGXDLMSDemandRegister::SetScaler(double value)
{
    m_Scaler = (signed char)log10(value);
}

int CGXDLMSDemandRegister::GetUnit()
{
    return m_Unit;
}

void CGXDLMSDemandRegister::SetUnit(unsigned char value)
{
    m_Unit = value;
}

CGXDLMSVariant& CGXDLMSDemandRegister::GetStatus()
{
    return m_Status;
}
void CGXDLMSDemandRegister::SetStatus(CGXDLMSVariant& value)
{
    m_Status = value;
}

void CGXDLMSDemandRegister::SetStatus(CGXDLMSVariant value)
{
    m_Status = value;
}

CGXDateTime& CGXDLMSDemandRegister::GetCaptureTime()
{
    return m_CaptureTime;
}
void CGXDLMSDemandRegister::SetCaptureTime(CGXDateTime value)
{
    m_CaptureTime = value;
}

CGXDateTime& CGXDLMSDemandRegister::GetStartTimeCurrent()
{
    return m_StartTimeCurrent;
}
void CGXDLMSDemandRegister::SetStartTimeCurrent(CGXDateTime& value)
{
    m_StartTimeCurrent = value;
}

unsigned long CGXDLMSDemandRegister::GetPeriod()
{
    return m_Period;
}

void CGXDLMSDemandRegister::SetPeriod(unsigned long value)
{
    m_Period = value;
}

int CGXDLMSDemandRegister::GetNumberOfPeriods()
{
    return m_NumberOfPeriods;
}

void CGXDLMSDemandRegister::SetNumberOfPeriods(int value)
{
    m_NumberOfPeriods = value;
}

int CGXDLMSDemandRegister::Reset(CGXDLMSClient* client,
    std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int CGXDLMSDemandRegister::NextPeriod(CGXDLMSClient* client,
    std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 2, data, reply);
}

void CGXDLMSDemandRegister::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_CurrentAverageValue.ToString());
    values.push_back(m_LastAverageValue.ToString());
    std::string str = "Scaler: ";
    //if there is no fractal part.
    double s = GetScaler();
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
    values.push_back(m_Status.ToString());
    values.push_back(m_CaptureTime.ToString());
    values.push_back(m_StartTimeCurrent.ToString());
    values.push_back(CGXDLMSVariant(m_Period).ToString());
    values.push_back(CGXDLMSVariant(m_NumberOfPeriods).ToString());
}

void CGXDLMSDemandRegister::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ScalerUnit
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //CurrentAverageValue
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //LastAverageValue
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //Status
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    //CaptureTime
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    //StartTimeCurrent
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
    //Period
    if (all || CanRead(8))
    {
        attributes.push_back(8);
    }
    //NumberOfPeriods
    if (all || CanRead(9))
    {
        attributes.push_back(9);
    }
}

int CGXDLMSDemandRegister::GetAttributeCount()
{
    return 9;
}

int CGXDLMSDemandRegister::GetMethodCount()
{
    return 2;
}

int CGXDLMSDemandRegister::Invoke(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_CurrentAverageValue.Clear();
        m_LastAverageValue.Clear();
        m_CaptureTime = CGXDateTime(CGXDateTime::Now());
        m_StartTimeCurrent = CGXDateTime(CGXDateTime::Now());
    }
    else if (e.GetIndex() == 2)
    {
        m_LastAverageValue = m_CurrentAverageValue;
        m_CurrentAverageValue.Clear();
        m_CaptureTime = CGXDateTime(CGXDateTime::Now());
        m_StartTimeCurrent = CGXDateTime(CGXDateTime::Now());
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMSDemandRegister::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        return CGXDLMSObject::GetDataType(index, type);
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 5)
    {
        type = m_Status.vt;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 6)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 7)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 8)
    {
        type = DLMS_DATA_TYPE_UINT32;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 9)
    {
        type = DLMS_DATA_TYPE_UINT16;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int CGXDLMSDemandRegister::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
        tmp = m_CurrentAverageValue;
        if (m_Scaler != 0)
        {
            DLMS_DATA_TYPE dt;
            if ((ret = CGXDLMSObject::GetDataType(2, dt)) != 0)
            {
                return ret;
            }
            if (dt == DLMS_DATA_TYPE_NONE)
            {
                dt = m_CurrentAverageValue.vt;
            }
            if ((ret = tmp.ChangeType(DLMS_DATA_TYPE_FLOAT64)) != 0)
            {
                return ret;
            }
            tmp = m_CurrentAverageValue.dblVal / GetScaler();
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
        tmp = m_LastAverageValue;
        if (m_Scaler != 0)
        {
            DLMS_DATA_TYPE dt;
            if ((ret = CGXDLMSObject::GetDataType(2, dt)) != 0)
            {
                return ret;
            }
            if (dt == DLMS_DATA_TYPE_NONE)
            {
                dt = m_LastAverageValue.vt;
            }
            if ((ret = tmp.ChangeType(DLMS_DATA_TYPE_FLOAT64)) != 0)
            {
                return ret;
            }
            tmp = m_LastAverageValue.dblVal / GetScaler();
            if ((ret = tmp.ChangeType(dt)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.GetValue().Clear();
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
    if (e.GetIndex() == 5)
    {
        e.SetValue(m_Status);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        e.SetValue(m_CaptureTime);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 7)
    {
        e.SetValue(m_StartTimeCurrent);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 8)
    {
        e.SetValue(m_Period);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 9)
    {
        e.SetValue(m_NumberOfPeriods);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int CGXDLMSDemandRegister::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        if (m_Scaler != 0 && e.GetValue().IsNumber())
        {
            m_CurrentAverageValue = e.GetValue().ToDouble() * GetScaler();
        }
        else
        {
            m_CurrentAverageValue = e.GetValue();
        }
    }
    else if (e.GetIndex() == 3)
    {
        if (m_Scaler != 0 && e.GetValue().IsNumber())
        {
            m_LastAverageValue = e.GetValue().ToDouble() * GetScaler();
        }
        else
        {
            m_LastAverageValue = e.GetValue();
        }
    }
    else if (e.GetIndex() == 4 && e.GetValue().vt == DLMS_DATA_TYPE_STRUCTURE)
    {
        m_Scaler = e.GetValue().Arr[0].ToInteger();
        m_Unit = e.GetValue().Arr[1].ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_Status = e.GetValue();
    }
    else if (e.GetIndex() == 6)
    {
        CGXDLMSVariant tmp;
        CGXDLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp);
        SetCaptureTime(tmp.dateTime);
    }
    else if (e.GetIndex() == 7)
    {
        CGXDLMSVariant tmp;
        CGXDLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp);
        SetStartTimeCurrent(tmp.dateTime);
    }
    else if (e.GetIndex() == 8)
    {
        SetPeriod(e.GetValue().ulVal);
    }
    else if (e.GetIndex() == 9)
    {
        SetNumberOfPeriods(e.GetValue().ToInteger());
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
