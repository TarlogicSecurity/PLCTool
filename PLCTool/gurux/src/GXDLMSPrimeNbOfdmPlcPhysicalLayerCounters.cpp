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

#include "../include/GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters.h"
#include "../include/GXDLMSClient.h"

//Constructor.
CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters() :
    CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters("0.0.28.1.0.255", 0)
{
}

CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_PHYSICAL_LAYER_COUNTERS, ln, sn)
{
    m_CrcIncorrectCount = m_CrcFailedCount = m_TxDropCount = m_RxDropCount = 0;
}

CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters(std::string ln) :
    CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters(ln, 0)
{

}

unsigned short CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetCrcIncorrectCount()
{
    return m_CrcIncorrectCount;
}

void CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::SetCrcIncorrectCount(unsigned short value)
{
    m_CrcIncorrectCount = value;
}

unsigned short CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetCrcFailedCount()
{
    return m_CrcFailedCount;
}

void CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::SetCrcFailedCount(unsigned short value)
{
    m_CrcFailedCount = value;
}

unsigned short CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetTxDropCount()
{
    return m_TxDropCount;
}

void CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::SetTxDropCount(unsigned short value)
{
    m_TxDropCount = value;
}

unsigned short CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetRxDropCount()
{
    return m_RxDropCount;
}

void CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::SetRxDropCount(unsigned short value)
{
    m_RxDropCount = value;
}

// Returns amount of attributes.
int CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetAttributeCount()
{
    return 5;
}

// Returns amount of methods.
int CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetMethodCount()
{
    return 1;
}

int CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::Reset(CGXDLMSClient* client, std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::Invoke(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_CrcIncorrectCount = m_CrcFailedCount = m_TxDropCount = m_RxDropCount = 0;
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

void CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSVariant(m_CrcIncorrectCount).ToString());
    values.push_back(CGXDLMSVariant(m_CrcFailedCount).ToString());
    values.push_back(CGXDLMSVariant(m_TxDropCount).ToString());
    values.push_back(CGXDLMSVariant(m_RxDropCount).ToString());
}

void CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // CrcIncorrectCount
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // CrcFailedCount
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    // TxDropCount
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    // RxDropCount
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
}

int CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    switch (index) {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        type = DLMS_DATA_TYPE_UINT16;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

// Returns value of given attribute.
int CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        int ret;
        CGXDLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
    }
    else if (e.GetIndex() == 2)
    {
        e.SetValue(m_CrcIncorrectCount);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue(m_CrcFailedCount);
    }
    else if (e.GetIndex() == 4)
    {
        e.SetValue(m_TxDropCount);
    }
    else if (e.GetIndex() == 5)
    {
        e.SetValue(m_RxDropCount);
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_CrcIncorrectCount = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_CrcFailedCount = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_TxDropCount = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_RxDropCount = e.GetValue().ToInteger();
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}
