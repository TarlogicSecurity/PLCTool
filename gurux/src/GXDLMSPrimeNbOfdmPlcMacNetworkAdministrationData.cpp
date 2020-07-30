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

#include "../include/GXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData.h"
#include "../include/GXDLMSClient.h"

//Constructor.
CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData() :
    CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData("0.0.28.5.0.255", 0)
{
}

//SN Constructor.
CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_NETWORK_ADMINISTRATION_DATA, ln, sn)
{

}

//LN Constructor.
CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData(std::string ln) :
    CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData(ln, 0)
{

}

//Destructor.
CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::~CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData()
{
    ClearMulticastEntry();
    ClearDirectTable();
    ClearAvailableSwitches();
    ClearCommunications();
}

std::vector<CGXMacMulticastEntry*>& CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetMulticastEntries()
{
    return m_MulticastEntries;
}

std::vector<short>& CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetSwitchTable()
{
    return m_SwitchTable;
}

std::vector<CGXMacDirectTable*>& CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetDirectTable()
{
    return m_DirectTable;
}

std::vector<CGXMacAvailableSwitch*>& CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetAvailableSwitches()
{
    return m_AvailableSwitches;
}

std::vector<CGXMacPhyCommunication*>& CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetCommunications()
{
    return m_Communications;
}

// Returns amount of attributes.
int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetAttributeCount()
{
    return 6;
}

// Returns amount of methods.
int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetMethodCount()
{
    return 1;
}

void CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    {
        for (std::vector<CGXMacMulticastEntry*>::iterator it = m_MulticastEntries.begin(); it != m_MulticastEntries.end(); ++it)
        {
            if (!empty)
            {
                sb << ", ";
            }
            empty = false;
            std::string str = (*it)->ToString();
            sb.write(str.c_str(), str.size());
        }
        sb << ']';
        values.push_back(sb.str());
        //Clear str.
        sb.str(std::string());
    }
    {
        sb << '[';
        empty = true;
        for (std::vector<short>::iterator it = m_SwitchTable.begin(); it != m_SwitchTable.end(); ++it)
        {
            if (!empty)
            {
                sb << ", ";
            }
            empty = false;
            std::string str = CGXDLMSVariant(*it).ToString();
            sb.write(str.c_str(), str.size());
        }
        sb << ']';
        values.push_back(sb.str());

        //Clear str.
        sb.str(std::string());
    }
    {
        sb << '[';
        empty = true;
        for (std::vector<CGXMacDirectTable*>::iterator it = m_DirectTable.begin(); it != m_DirectTable.end(); ++it)
        {
            if (!empty)
            {
                sb << ", ";
            }
            empty = false;
            std::string str = (*it)->ToString();
            sb.write(str.c_str(), str.size());
        }
        sb << ']';
        values.push_back(sb.str());
        //Clear str.
        sb.str(std::string());
    }
    {
        sb << '[';
        empty = true;
        for (std::vector<CGXMacAvailableSwitch*>::iterator it = m_AvailableSwitches.begin(); it != m_AvailableSwitches.end(); ++it)
        {
            if (!empty)
            {
                sb << ", ";
            }
            empty = false;
            std::string str = (*it)->ToString();
            sb.write(str.c_str(), str.size());
        }
        sb << ']';
        values.push_back(sb.str());
        //Clear str.
        sb.str(std::string());
    }
    {//std::vector<CGXMacPhyCommunication*> m_Communications;
        sb << '[';
        empty = true;
        for (std::vector<CGXMacPhyCommunication*>::iterator it = m_Communications.begin(); it != m_Communications.end(); ++it)
        {
            if (!empty)
            {
                sb << ", ";
            }
            empty = false;
            std::string str = (*it)->ToString();
            sb.write(str.c_str(), str.size());
        }
        sb << ']';
        values.push_back(sb.str());
    }
}


int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::Reset(CGXDLMSClient* client, std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::Invoke(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_MulticastEntries.clear();
        m_SwitchTable.clear();
        m_DirectTable.clear();
        m_AvailableSwitches.clear();
        m_Communications.clear();
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}


void CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // MulticastEntries
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // SwitchTable
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // DirectTable
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
    // AvailableSwitches
    if (all || CanRead(5)) {
        attributes.push_back(5);
    }
    // Communications
    if (all || CanRead(6)) {
        attributes.push_back(6);
    }
}

int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetMulticastEntries(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret = 0;
    CGXByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    GXHelpers::SetObjectCount((unsigned long)m_MulticastEntries.size(), bb);
    for (std::vector<CGXMacMulticastEntry*>::iterator it = m_MulticastEntries.begin(); it != m_MulticastEntries.end(); ++it)
    {
        CGXDLMSVariant a = (*it)->GetId(), b = (*it)->GetMembers();
        if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
            (ret = bb.SetUInt8(2)) != 0 ||
            (ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_INT8, a)) != 0 ||
            (ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_INT16, b)) != 0)
        {
            break;
        }
    }
    if (ret == 0)
    {
        e.SetValue(bb);
    }
    return ret;
}

int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetSwitchTable(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret = 0;
    CGXByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    GXHelpers::SetObjectCount((unsigned long)m_SwitchTable.size(), bb);
    for (std::vector<short>::iterator it = m_SwitchTable.begin(); it != m_SwitchTable.end(); ++it)
    {
        if ((ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT16, *it)) != 0)
        {
            break;
        }
    }
    if (ret == 0)
    {
        e.SetValue(bb);
    }
    return ret;
}

int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetDirectTable(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret = 0;
    CGXByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    GXHelpers::SetObjectCount((unsigned long)m_DirectTable.size(), bb);
    for (std::vector<CGXMacDirectTable*>::iterator it = m_DirectTable.begin(); it != m_DirectTable.end(); ++it)
    {
        if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
            (ret = bb.SetUInt8(7)) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT16, (*it)->GetSourceSId())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT16, (*it)->GetSourceLnId())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT16, (*it)->GetSourceLcId())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT16, (*it)->GetDestinationSId())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT16, (*it)->GetDestinationLnId())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT16, (*it)->GetDestinationLcId())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_OCTET_STRING, (*it)->GetDid())) != 0)
        {
            break;
        }
    }
    if (ret == 0)
    {
        e.SetValue(bb);
    }
    return ret;
}

int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetAvailableSwitches(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret = 0;
    CGXByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    GXHelpers::SetObjectCount((unsigned long)m_AvailableSwitches.size(), bb);
    for (std::vector<CGXMacAvailableSwitch*>::iterator it = m_AvailableSwitches.begin(); it != m_AvailableSwitches.end(); ++it)
    {
        if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
            (ret = bb.SetUInt8(5)) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_OCTET_STRING, (*it)->GetSna())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT16, (*it)->GetLsId())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT8, (*it)->GetLevel())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT8, (*it)->GetRxLevel())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT8, (*it)->GetRxSnr())) != 0)
        {
            break;
        }
    }
    if (ret == 0)
    {
        e.SetValue(bb);
    }
    return ret;
}

int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetCommunications(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret = 0;
    CGXByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    GXHelpers::SetObjectCount((unsigned long)m_Communications.size(), bb);
    for (std::vector<CGXMacPhyCommunication*>::iterator it = m_Communications.begin(); it != m_Communications.end(); ++it)
    {
        if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
            (ret = bb.SetUInt8(9)) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_OCTET_STRING, (*it)->GetEui())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT8, (*it)->GetTxPower())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT8, (*it)->GetTxCoding())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT8, (*it)->GetRxCoding())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT8, (*it)->GetRxLvl())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT8, (*it)->GetSnr())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT8, (*it)->GetTxPowerModified())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT8, (*it)->GetTxCodingModified())) != 0 ||
            (ret = GXHelpers::SetData2(bb, DLMS_DATA_TYPE_INT8, (*it)->GetRxCodingModified())) != 0)
        {
            break;
        }
    }
    if (ret == 0)
    {
        e.SetValue(bb);
    }
    return ret;
}

// Returns value of given attribute.
int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
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
        ret = GetMulticastEntries(settings, e);
        break;
    case 3:
        ret = GetSwitchTable(settings, e);
        break;
    case 4:
        ret = GetDirectTable(settings, e);
        break;
    case 5:
        ret = GetAvailableSwitches(settings, e);
        break;
    case 6:
        ret = GetCommunications(settings, e);
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

void CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::ClearMulticastEntry()
{
    for (std::vector<CGXMacMulticastEntry*>::iterator it = m_MulticastEntries.begin(); it != m_MulticastEntries.end(); ++it)
    {
        delete (*it);
    }
    m_MulticastEntries.clear();
}

void CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::ClearDirectTable()
{
    for (std::vector<CGXMacDirectTable*>::iterator it = m_DirectTable.begin(); it != m_DirectTable.end(); ++it)
    {
        delete (*it);
    }
    m_DirectTable.clear();
}

void CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::ClearAvailableSwitches()
{
    for (std::vector<CGXMacAvailableSwitch*>::iterator it = m_AvailableSwitches.begin(); it != m_AvailableSwitches.end(); ++it)
    {
        delete (*it);
    }
    m_AvailableSwitches.clear();
}

void CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::ClearCommunications()
{
    for (std::vector<CGXMacPhyCommunication*>::iterator it = m_Communications.begin(); it != m_Communications.end(); ++it)
    {
        delete (*it);
    }
    m_Communications.clear();
}

int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::SetMulticastEntry(std::vector<CGXDLMSVariant>& list)
{
    int ret = 0;
    ClearMulticastEntry();
    for (std::vector<CGXDLMSVariant>::iterator it = list.begin(); it != list.end(); ++it)
    {
        CGXMacMulticastEntry* v = new CGXMacMulticastEntry();
        v->SetId(it->Arr.at(0).ToInteger());
        v->SetMembers(it->Arr.at(1).ToInteger());
        m_MulticastEntries.push_back(v);
    }
    return ret;
}

int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::SetSwitchTable(std::vector<CGXDLMSVariant>& list)
{
    m_SwitchTable.clear();
    for (std::vector<CGXDLMSVariant>::iterator it = list.begin(); it != list.end(); ++it)
    {
        m_SwitchTable.push_back(it->ToInteger());
    }
    return 0;
}

int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::SetDirectTable(std::vector<CGXDLMSVariant>& list)
{
    ClearDirectTable();
    for (std::vector<CGXDLMSVariant>::iterator it = list.begin(); it != list.end(); ++it)
    {
        CGXByteBuffer tmp;
        CGXMacDirectTable* v = new CGXMacDirectTable();
        v->SetSourceSId(it->Arr.at(0).ToInteger());
        v->SetSourceLnId(it->Arr.at(1).ToInteger());
        v->SetSourceLcId(it->Arr.at(2).ToInteger());
        v->SetDestinationSId(it->Arr.at(3).ToInteger());
        v->SetDestinationLnId(it->Arr.at(4).ToInteger());
        v->SetDestinationLcId(it->Arr.at(5).ToInteger());
        tmp.Set(it->Arr.at(6).byteArr, it->Arr.at(6).GetSize());
        v->SetDid(tmp);
        m_DirectTable.push_back(v);
    }
    return 0;
}

int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::SetAvailableSwitches(std::vector<CGXDLMSVariant>& list)
{
    ClearAvailableSwitches();
    for (std::vector<CGXDLMSVariant>::iterator it = list.begin(); it != list.end(); ++it)
    {
        CGXByteBuffer tmp;
        tmp.Set(it->Arr.at(0).byteArr, it->Arr.at(0).GetSize());
        CGXMacAvailableSwitch* v = new CGXMacAvailableSwitch();
        v->SetSna(tmp);
        v->SetLsId(it->Arr.at(1).ToInteger());
        v->SetLevel(it->Arr.at(2).ToInteger());
        v->SetRxLevel(it->Arr.at(3).ToInteger());
        v->SetRxSnr(it->Arr.at(4).ToInteger());
        m_AvailableSwitches.push_back(v);
    }
    return 0;
}

int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::SetCommunications(std::vector<CGXDLMSVariant>& list)
{
    ClearCommunications();
    for (std::vector<CGXDLMSVariant>::iterator it = list.begin(); it != list.end(); ++it)
    {
        CGXByteBuffer tmp;
        tmp.Set(it->Arr.at(0).byteArr, it->Arr.at(0).GetSize());
        CGXMacPhyCommunication* v = new CGXMacPhyCommunication();
        v->SetEui(tmp);
        v->SetTxPower(it->Arr.at(1).ToInteger());
        v->SetTxCoding(it->Arr.at(1).ToInteger());
        v->SetRxCoding(it->Arr.at(1).ToInteger());
        v->SetRxLvl(it->Arr.at(1).ToInteger());
        v->SetSnr(it->Arr.at(1).ToInteger());
        v->SetTxPowerModified(it->Arr.at(1).ToInteger());
        v->SetTxCodingModified(it->Arr.at(1).ToInteger());
        v->SetRxCodingModified(it->Arr.at(1).ToInteger());
        m_Communications.push_back(v);
    }
    return 0;
}

// Set value of given attribute.
int CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret;
    switch (e.GetIndex())
    {
    case 1:
        ret = SetLogicalName(this, e.GetValue());
        break;
    case 2:
        ret = SetMulticastEntry(e.GetValue().Arr);
        break;
    case 3:
        ret = SetSwitchTable(e.GetValue().Arr);
        break;
    case 4:
        ret = SetDirectTable(e.GetValue().Arr);
        break;
    case 5:
        ret = SetAvailableSwitches(e.GetValue().Arr);
        break;
    case 6:
        ret = SetCommunications(e.GetValue().Arr);
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}
