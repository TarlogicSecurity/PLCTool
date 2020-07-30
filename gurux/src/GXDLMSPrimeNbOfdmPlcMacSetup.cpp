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

#include "../include/GXDLMSPrimeNbOfdmPlcMacSetup.h"

//Constructor.
CGXDLMSPrimeNbOfdmPlcMacSetup::CGXDLMSPrimeNbOfdmPlcMacSetup() :
    CGXDLMSPrimeNbOfdmPlcMacSetup("0.0.28.2.0.255", 0)
{
}

//SN Constructor.
CGXDLMSPrimeNbOfdmPlcMacSetup::CGXDLMSPrimeNbOfdmPlcMacSetup(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_SETUP, ln, sn)
{
    m_MacMinSwitchSearchTime = m_MacMaxPromotionPdu = m_MacPromotionPduTxPeriod = 0;
    m_MacBeaconsPerFrame = m_MacScpMaxTxAttempts = m_MacCtlReTxTimer = m_MacMaxCtlReTx = 0;
}

//LN Constructor.
CGXDLMSPrimeNbOfdmPlcMacSetup::CGXDLMSPrimeNbOfdmPlcMacSetup(std::string ln) :
    CGXDLMSPrimeNbOfdmPlcMacSetup(ln, 0)
{

}

unsigned char CGXDLMSPrimeNbOfdmPlcMacSetup::GetMacMinSwitchSearchTime()
{
    return m_MacMinSwitchSearchTime;
}

void CGXDLMSPrimeNbOfdmPlcMacSetup::SetMacMinSwitchSearchTime(unsigned char value)
{
    m_MacMinSwitchSearchTime = value;
}

unsigned char CGXDLMSPrimeNbOfdmPlcMacSetup::GetMacMaxPromotionPdu()
{
    return m_MacMaxPromotionPdu;
}

void CGXDLMSPrimeNbOfdmPlcMacSetup::SetMacMaxPromotionPdu(unsigned char value)
{
    m_MacMaxPromotionPdu = value;
}

unsigned char CGXDLMSPrimeNbOfdmPlcMacSetup::GetMacPromotionPduTxPeriod()
{
    return m_MacPromotionPduTxPeriod;
}

void CGXDLMSPrimeNbOfdmPlcMacSetup::SetMacPromotionPduTxPeriod(unsigned char value)
{
    m_MacPromotionPduTxPeriod = value;
}

unsigned char CGXDLMSPrimeNbOfdmPlcMacSetup::GetMacBeaconsPerFrame()
{
    return m_MacBeaconsPerFrame;
}

void CGXDLMSPrimeNbOfdmPlcMacSetup::SetMacBeaconsPerFrame(unsigned char value)
{
    m_MacBeaconsPerFrame = value;
}

unsigned char CGXDLMSPrimeNbOfdmPlcMacSetup::GetMacScpMaxTxAttempts()
{
    return m_MacScpMaxTxAttempts;
}

void CGXDLMSPrimeNbOfdmPlcMacSetup::SetMacScpMaxTxAttempts(unsigned char value)
{
    m_MacScpMaxTxAttempts = value;
}

unsigned char CGXDLMSPrimeNbOfdmPlcMacSetup::GetMacCtlReTxTimer()
{
    return m_MacCtlReTxTimer;
}

void CGXDLMSPrimeNbOfdmPlcMacSetup::SetMacCtlReTxTimer(unsigned char value)
{
    m_MacCtlReTxTimer = value;
}

unsigned char CGXDLMSPrimeNbOfdmPlcMacSetup::GetMacMaxCtlReTx()
{
    return m_MacMaxCtlReTx;
}

void CGXDLMSPrimeNbOfdmPlcMacSetup::SetMacMaxCtlReTx(unsigned char value)
{
    m_MacMaxCtlReTx = value;
}

// Returns amount of attributes.
int CGXDLMSPrimeNbOfdmPlcMacSetup::GetAttributeCount()
{
    return 8;
}

// Returns amount of methods.
int CGXDLMSPrimeNbOfdmPlcMacSetup::GetMethodCount()
{
    return 0;
}

void CGXDLMSPrimeNbOfdmPlcMacSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSVariant(m_MacMinSwitchSearchTime).ToString());
    values.push_back(CGXDLMSVariant(m_MacMaxPromotionPdu).ToString());
    values.push_back(CGXDLMSVariant(m_MacPromotionPduTxPeriod).ToString());
    values.push_back(CGXDLMSVariant(m_MacBeaconsPerFrame).ToString());
    values.push_back(CGXDLMSVariant(m_MacScpMaxTxAttempts).ToString());
    values.push_back(CGXDLMSVariant(m_MacCtlReTxTimer).ToString());
    values.push_back(CGXDLMSVariant(m_MacMaxCtlReTx).ToString());
}

void CGXDLMSPrimeNbOfdmPlcMacSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // MacMinSwitchSearchTime
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // MacMaxPromotionPdu
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // MacPromotionPduTxPeriod
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
    // MacBeaconsPerFrame
    if (all || CanRead(5)) {
        attributes.push_back(5);
    }
    // MacScpMaxTxAttempts
    if (all || CanRead(6)) {
        attributes.push_back(6);
    }
    // MacCtlReTxTimer
    if (all || CanRead(7)) {
        attributes.push_back(7);
    }
    // MacMaxCtlReTx
    if (all || CanRead(8)) {
        attributes.push_back(8);
    }
}

int CGXDLMSPrimeNbOfdmPlcMacSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
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
    case 8:
        type = DLMS_DATA_TYPE_UINT8;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

// Returns value of given attribute.
int CGXDLMSPrimeNbOfdmPlcMacSetup::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
        e.SetValue(m_MacMinSwitchSearchTime);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue(m_MacMaxPromotionPdu);
    }
    else if (e.GetIndex() == 4)
    {
        e.SetValue(m_MacPromotionPduTxPeriod);
    }
    else if (e.GetIndex() == 5)
    {
        e.SetValue(m_MacBeaconsPerFrame);
    }
    else if (e.GetIndex() == 6)
    {
        e.SetValue(m_MacScpMaxTxAttempts);
    }
    else if (e.GetIndex() == 7)
    {
        e.SetValue(m_MacCtlReTxTimer);
    }
    else if (e.GetIndex() == 8)
    {
        e.SetValue(m_MacMaxCtlReTx);
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int CGXDLMSPrimeNbOfdmPlcMacSetup::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_MacMinSwitchSearchTime = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_MacMaxPromotionPdu = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_MacPromotionPduTxPeriod = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_MacBeaconsPerFrame = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 6)
    {
        m_MacScpMaxTxAttempts = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 7)
    {
        m_MacCtlReTxTimer = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 8)
    {
        m_MacMaxCtlReTx = e.GetValue().ToInteger();
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}
