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

#include "../include/GXDLMSPrimeNbOfdmPlcMacCounters.h"
#include "../include/GXDLMSClient.h"

//Constructor.
CGXDLMSPrimeNbOfdmPlcMacCounters::CGXDLMSPrimeNbOfdmPlcMacCounters() :
    CGXDLMSPrimeNbOfdmPlcMacCounters("0.0.28.4.0.255", 0)
{
}

//SN Constructor.
CGXDLMSPrimeNbOfdmPlcMacCounters::CGXDLMSPrimeNbOfdmPlcMacCounters(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_COUNTERS, ln, sn)
{
    m_TxDataPktCount = m_RxDataPktCount = m_TxCtrlPktCount = m_RxCtrlPktCount = m_CsmaFailCount = m_CsmaChBusyCount = 0;
}

//LN Constructor.
CGXDLMSPrimeNbOfdmPlcMacCounters::CGXDLMSPrimeNbOfdmPlcMacCounters(std::string ln) :
    CGXDLMSPrimeNbOfdmPlcMacCounters(ln, 0)
{

}

unsigned int CGXDLMSPrimeNbOfdmPlcMacCounters::GetTxDataPktCount()
{
    return m_TxDataPktCount;
}

void CGXDLMSPrimeNbOfdmPlcMacCounters::SetTxDataPktCount(unsigned int value)
{
    m_TxDataPktCount = value;
}

unsigned int CGXDLMSPrimeNbOfdmPlcMacCounters::GetRxDataPktCount()
{
    return m_RxDataPktCount;
}

void CGXDLMSPrimeNbOfdmPlcMacCounters::SetRxDataPktCount(unsigned int value)
{
    m_RxDataPktCount = value;
}

unsigned int CGXDLMSPrimeNbOfdmPlcMacCounters::GetTxCtrlPktCount()
{
    return m_TxCtrlPktCount;
}

void CGXDLMSPrimeNbOfdmPlcMacCounters::SetTxCtrlPktCount(unsigned int value)
{
    m_TxCtrlPktCount = value;
}

unsigned int CGXDLMSPrimeNbOfdmPlcMacCounters::GetRxCtrlPktCount()
{
    return m_RxCtrlPktCount;
}

void CGXDLMSPrimeNbOfdmPlcMacCounters::SetRxCtrlPktCount(unsigned int value)
{
    m_RxCtrlPktCount = value;
}

unsigned int CGXDLMSPrimeNbOfdmPlcMacCounters::GetCsmaFailCount()
{
    return m_CsmaFailCount;
}

void CGXDLMSPrimeNbOfdmPlcMacCounters::SetCsmaFailCount(unsigned int value)
{
    m_CsmaFailCount = value;
}

unsigned int CGXDLMSPrimeNbOfdmPlcMacCounters::GetCsmaChBusyCount()
{
    return m_CsmaChBusyCount;
}

void CGXDLMSPrimeNbOfdmPlcMacCounters::SetCsmaChBusyCount(unsigned int value)
{
    m_CsmaChBusyCount = value;
}


int CGXDLMSPrimeNbOfdmPlcMacCounters::Reset(CGXDLMSClient* client, std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int CGXDLMSPrimeNbOfdmPlcMacCounters::Invoke(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_TxDataPktCount = m_RxDataPktCount = m_TxCtrlPktCount = m_RxCtrlPktCount = m_CsmaFailCount = m_CsmaChBusyCount = 0;
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns amount of attributes.
int CGXDLMSPrimeNbOfdmPlcMacCounters::GetAttributeCount()
{
    return 7;
}

// Returns amount of methods.
int CGXDLMSPrimeNbOfdmPlcMacCounters::GetMethodCount()
{
    return 1;
}

void CGXDLMSPrimeNbOfdmPlcMacCounters::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSVariant(m_TxDataPktCount).ToString());
    values.push_back(CGXDLMSVariant(m_RxDataPktCount).ToString());
    values.push_back(CGXDLMSVariant(m_TxCtrlPktCount).ToString());
    values.push_back(CGXDLMSVariant(m_RxCtrlPktCount).ToString());
    values.push_back(CGXDLMSVariant(m_CsmaFailCount).ToString());
    values.push_back(CGXDLMSVariant(m_CsmaChBusyCount).ToString());
}

void CGXDLMSPrimeNbOfdmPlcMacCounters::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // TxDataPktCount
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }

    // RxDataPktCount
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    // TxCtrlPktCount
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    // RxCtrlPktCount
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    // CsmaFailCount
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    // CsmaChBusyCount
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
}

int CGXDLMSPrimeNbOfdmPlcMacCounters::GetDataType(int index, DLMS_DATA_TYPE& type)
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
    case 6:
    case 7:
        type = DLMS_DATA_TYPE_UINT32;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

// Returns value of given attribute.
int CGXDLMSPrimeNbOfdmPlcMacCounters::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret = DLMS_ERROR_CODE_OK;
    switch (e.GetIndex())
    {
    case 1:
    {
        int ret;
        CGXDLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
    }
    case 2:
        e.SetValue(m_TxDataPktCount);
        break;
    case 3:
        e.SetValue(m_RxDataPktCount);
        break;
    case 4:
        e.SetValue(m_TxCtrlPktCount);
        break;
    case 5:
        e.SetValue(m_RxCtrlPktCount);
        break;
    case 6:
        e.SetValue(m_CsmaFailCount);
        break;
    case 7:
        e.SetValue(m_CsmaChBusyCount);
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int CGXDLMSPrimeNbOfdmPlcMacCounters::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret = DLMS_ERROR_CODE_OK;
    switch (e.GetIndex())
    {
    case 1:
        ret = SetLogicalName(this, e.GetValue());
        break;
    case 2:
        m_TxDataPktCount = e.GetValue().ToInteger();
        break;
    case 3:
        m_RxDataPktCount = e.GetValue().ToInteger();
        break;
    case 4:
        m_TxCtrlPktCount = e.GetValue().ToInteger();
        break;
    case 5:
        m_RxCtrlPktCount = e.GetValue().ToInteger();
        break;
    case 6:
        m_CsmaFailCount = e.GetValue().ToInteger();
        break;
    case 7:
        m_CsmaChBusyCount = e.GetValue().ToInteger();
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }

    return ret;
}
