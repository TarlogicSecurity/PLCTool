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

#include "../include/GXDLMSCertificateInfo.h"

CGXDLMSCertificateInfo::CGXDLMSCertificateInfo()
{
    m_Entity = DLMS_CERTIFICATE_ENTITY_SERVER;
    m_Type = DLMS_CERTIFICATE_TYPE_DIGITAL_SIGNATURE;
}

DLMS_CERTIFICATE_ENTITY CGXDLMSCertificateInfo::GetEntity() {
    return m_Entity;
}

void CGXDLMSCertificateInfo::SetEntity(DLMS_CERTIFICATE_ENTITY value) {
    m_Entity = value;
}

DLMS_CERTIFICATE_TYPE CGXDLMSCertificateInfo::GetType() {
    return m_Type;
}

void CGXDLMSCertificateInfo::SetType(DLMS_CERTIFICATE_TYPE value) {
    m_Type = value;
}

std::string CGXDLMSCertificateInfo::GetSerialNumber() {
    return m_SerialNumber;
}

void CGXDLMSCertificateInfo::SetSerialNumber(std::string& value) {
    m_SerialNumber = value;
}

std::string CGXDLMSCertificateInfo::GetIssuer() {
    return m_Issuer;
}

void CGXDLMSCertificateInfo::SetIssuer(std::string& value) {
    m_Issuer = value;
}

std::string CGXDLMSCertificateInfo::GetSubject() {
    return m_Subject;
}

void CGXDLMSCertificateInfo::SetSubject(std::string& value) {
    m_Subject = value;
}

std::string CGXDLMSCertificateInfo::GetSubjectAltName() {
    return m_SubjectAltName;
}

void CGXDLMSCertificateInfo::SetSubjectAltName(std::string& value) {
    m_SubjectAltName = value;
}