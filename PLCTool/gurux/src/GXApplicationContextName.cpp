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

#include "../include/GXApplicationContextName.h"
#include "../include/GXDLMSVariant.h"

/// <summary>
/// Constructor
/// </summary>
CGXApplicationContextName::CGXApplicationContextName()
{
    m_JointIsoCtt = 2;
    m_Country = 16;
    m_CountryName = 756;
    m_IdentifiedOrganization = 5;
    m_DlmsUA = 8;
    m_ApplicationContext = 1;
    m_ContextId = DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME;
}

unsigned char CGXApplicationContextName::GetJointIsoCtt()
{
    return m_JointIsoCtt;
}
void CGXApplicationContextName::SetJointIsoCtt(unsigned char value)
{
    m_JointIsoCtt = value;
}

unsigned char CGXApplicationContextName::GetCountry()
{
    return m_Country;
}
void CGXApplicationContextName::SetCountry(unsigned char value)
{
    m_Country = value;
}

unsigned short CGXApplicationContextName::GetCountryName()
{
    return m_CountryName;
}
void CGXApplicationContextName::SetCountryName(unsigned short value)
{
    m_CountryName = value;
}

unsigned char CGXApplicationContextName::GetIdentifiedOrganization()
{
    return m_IdentifiedOrganization;
}
void CGXApplicationContextName::SetIdentifiedOrganization(unsigned char value)
{
    m_IdentifiedOrganization = value;
}

unsigned char CGXApplicationContextName::GetDlmsUA()
{
    return m_DlmsUA;
}
void CGXApplicationContextName::SetDlmsUA(unsigned char value)
{
    m_DlmsUA = value;
}

unsigned char CGXApplicationContextName::GetApplicationContext()
{
    return m_ApplicationContext;
}
void CGXApplicationContextName::SetApplicationContext(unsigned char value)
{
    m_ApplicationContext = value;
}

DLMS_APPLICATION_CONTEXT_NAME CGXApplicationContextName::GetContextId()
{
    return m_ContextId;
}
void CGXApplicationContextName::SetContextId(DLMS_APPLICATION_CONTEXT_NAME value)
{
    m_ContextId = value;
}

std::string CGXApplicationContextName::ToString()
{
    CGXByteBuffer bb;
    bb.AddString(m_LogicalName.c_str());
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_JointIsoCtt);
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_Country);
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_CountryName);
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_IdentifiedOrganization);
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_DlmsUA);
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_ApplicationContext);
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_ContextId);
    return bb.ToString();
}
