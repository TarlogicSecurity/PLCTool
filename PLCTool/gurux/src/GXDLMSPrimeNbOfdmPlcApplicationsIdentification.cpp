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

#include "../include/GXDLMSPrimeNbOfdmPlcApplicationsIdentification.h"

//Constructor.
CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::CGXDLMSPrimeNbOfdmPlcApplicationsIdentification() :
    CGXDLMSPrimeNbOfdmPlcApplicationsIdentification("0.0.28.7.0.255", 0)
{
}

//SN Constructor.
CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::CGXDLMSPrimeNbOfdmPlcApplicationsIdentification(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_APPLICATIONS_IDENTIFICATION, ln, sn)
{
    m_VendorId = 0;
    m_ProductId = 0;
}

//LN Constructor.
CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::CGXDLMSPrimeNbOfdmPlcApplicationsIdentification(std::string ln) :
    CGXDLMSPrimeNbOfdmPlcApplicationsIdentification(ln, 0)
{

}

std::string& CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::GetFirmwareVersion()
{
    return m_FirmwareVersion;
}


void CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::SetFirmwareVersion(std::string& value)
{
    m_FirmwareVersion = value;
}


unsigned short CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::GetVendorId()
{
    return m_VendorId;
}


void CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::SetVendorId(unsigned short value)
{
    m_VendorId = value;
}


unsigned short CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::GetProductId()
{
    return m_ProductId;
}

void CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::SetProductId(unsigned short value)
{
    m_ProductId = value;
}

// Returns amount of attributes.
int CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::GetMethodCount()
{
    return 0;
}

void CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_FirmwareVersion);
    values.push_back(CGXDLMSVariant(m_VendorId).ToString());
    values.push_back(CGXDLMSVariant(m_ProductId).ToString());
}

void CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // FirmwareVersion
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // VendorId
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // ProductId
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
}

int CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    switch (index) {
    case 1:
    case 2:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 3:
    case 4:
        type = DLMS_DATA_TYPE_UINT16;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

// Returns value of given attribute.
int CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::GetValue(CGXDLMSSettings& , CGXDLMSValueEventArg& e)
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
        e.SetValue(m_FirmwareVersion);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue(m_VendorId);
    }
    else if (e.GetIndex() == 4)
    {
        e.SetValue(m_ProductId);
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int CGXDLMSPrimeNbOfdmPlcApplicationsIdentification::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_FirmwareVersion = e.GetValue().ChangeType(DLMS_DATA_TYPE_STRING);
    }
    else if (e.GetIndex() == 3)
    {
        m_VendorId = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_ProductId = e.GetValue().ToInteger();
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}
