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
#include "../include/GXDLMSSecuritySetup.h"
#include "../include/GXDLMSConverter.h"
#include "../include/GXDLMSSecureClient.h"

//Constructor.
CGXDLMSSecuritySetup::CGXDLMSSecuritySetup() : CGXDLMSSecuritySetup("0.0.43.0.0.255", 0)
{
}

//SN Constructor.
CGXDLMSSecuritySetup::CGXDLMSSecuritySetup(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_SECURITY_SETUP, ln, sn)
{

}

//LN Constructor.
CGXDLMSSecuritySetup::CGXDLMSSecuritySetup(std::string ln) : CGXDLMSSecuritySetup(ln, 0)
{

}

unsigned char CGXDLMSSecuritySetup::GetSecurityPolicy()
{
    return m_SecurityPolicy;
}

void CGXDLMSSecuritySetup::SetSecurityPolicy(unsigned char value)
{
    m_SecurityPolicy = value;
}

DLMS_SECURITY_SUITE CGXDLMSSecuritySetup::GetSecuritySuite()
{
    return m_SecuritySuite;
}

void CGXDLMSSecuritySetup::SetSecuritySuite(DLMS_SECURITY_SUITE value)
{
    m_SecuritySuite = value;
}

CGXByteBuffer CGXDLMSSecuritySetup::GetClientSystemTitle()
{
    return m_ClientSystemTitle;
}

void CGXDLMSSecuritySetup::SetClientSystemTitle(CGXByteBuffer& value)
{
    m_ClientSystemTitle = value;
}

CGXByteBuffer CGXDLMSSecuritySetup::GetServerSystemTitle()
{
    return m_ServerSystemTitle;
}

void CGXDLMSSecuritySetup::SetServerSystemTitle(CGXByteBuffer& value)
{
    m_ServerSystemTitle = value;
}

// Returns amount of attributes.
int CGXDLMSSecuritySetup::GetAttributeCount()
{
    if (GetVersion() == 0)
    {
        return 5;
    }
    return 6;
}

// Returns amount of methods.
int CGXDLMSSecuritySetup::GetMethodCount()
{
    if (GetVersion() == 0)
    {
        return 2;
    }
    return 8;
}

int CGXDLMSSecuritySetup::Activate(
    CGXDLMSClient* client,
    DLMS_SECURITY security,
    std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data((char)security);
    return client->Method(this, 1, data, reply);
}

int CGXDLMSSecuritySetup::GlobalKeyTransfer(
    CGXDLMSClient* client,
    CGXByteBuffer& kek,
    std::vector<std::pair<DLMS_GLOBAL_KEY_TYPE, CGXByteBuffer&> >& list,
    std::vector<CGXByteBuffer>& reply)
{
    int ret =  0;
    CGXDLMSVariant data;
    CGXByteBuffer bb, tmp;
    if (list.size() == 0)
    {
        //Invalid list. It is empty.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    bb.SetUInt8((unsigned char) list.size());
    for (std::vector<std::pair<DLMS_GLOBAL_KEY_TYPE, CGXByteBuffer&> >::iterator it = list.begin(); it != list.end(); ++it)
    {
        bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        bb.SetUInt8(2);
        data = (char)it->first;
        if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_ENUM, data)) != 0 ||
            (ret = CGXDLMSSecureClient::Encrypt(kek, it->second, tmp)) != 0)
        {
            break;
        }
        data = tmp;
        if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_OCTET_STRING, data)) != 0)
        {
            break;
        }
    }
    if (ret == 0)
    {
        data = bb;
        ret = client->Method(this, 2, data, DLMS_DATA_TYPE_ARRAY, reply);
    }
    return ret;
}

int CGXDLMSSecuritySetup::Invoke(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_SecurityPolicy = e.GetParameters().ToInteger();
    }
    else if (e.GetIndex() == 2)
    {
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetParameters().Arr.begin(); it != e.GetParameters().Arr.end(); ++it)
        {
            DLMS_GLOBAL_KEY_TYPE type = (DLMS_GLOBAL_KEY_TYPE)it->Arr[0].ToInteger();
            CGXByteBuffer data, reply;
            CGXByteBuffer kek = settings.GetKek();
            data.Set(it->Arr[1].byteArr, it->Arr[1].GetSize());
            if (CGXDLMSSecureClient::Decrypt(kek, data, reply) != 0 ||
                reply.GetSize() != 16)
            {
                e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
                break;
            }
            //Keys are take in action after reply is generated.
            switch (type) {
            case DLMS_GLOBAL_KEY_TYPE_UNICAST_ENCRYPTION:
                break;
            case DLMS_GLOBAL_KEY_TYPE_BROADCAST_ENCRYPTION:
                // Invalid type
                e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
                break;
            case DLMS_GLOBAL_KEY_TYPE_AUTHENTICATION:
                break;
            case DLMS_GLOBAL_KEY_TYPE_KEK:
                break;
            default:
                e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
            }
        }
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}


int CGXDLMSSecuritySetup::ApplyKeys(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    for (std::vector<CGXDLMSVariant>::iterator it = e.GetParameters().Arr.begin(); it != e.GetParameters().Arr.end(); ++it)
    {
        DLMS_GLOBAL_KEY_TYPE type = (DLMS_GLOBAL_KEY_TYPE)it->Arr[0].ToInteger();
        CGXByteBuffer data, reply;
        CGXByteBuffer kek = settings.GetKek();
        data.Set(it->Arr[1].byteArr, it->Arr[1].GetSize());
        if (CGXDLMSSecureClient::Decrypt(kek, data, reply) != 0 ||
            reply.GetSize() != 16)
        {
            e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
            break;
        }
        switch (type) {
        case DLMS_GLOBAL_KEY_TYPE_UNICAST_ENCRYPTION:
            settings.GetCipher()->SetBlockCipherKey(reply);
            break;
        case DLMS_GLOBAL_KEY_TYPE_BROADCAST_ENCRYPTION:
            // Invalid type
            e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
            break;
        case DLMS_GLOBAL_KEY_TYPE_AUTHENTICATION:
            // if settings.Cipher is null non secure server is used.
            settings.GetCipher()->SetAuthenticationKey(reply);
            break;
        case DLMS_GLOBAL_KEY_TYPE_KEK:
            settings.SetKek(reply);
            break;
        default:
            e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
        }
    }
    return DLMS_ERROR_CODE_OK;
}

void CGXDLMSSecuritySetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    if (m_Version == 0)
    {
        values.push_back(CGXDLMSConverter::ToString((DLMS_SECURITY_POLICY)m_SecurityPolicy));
    }
    else
    {
        CGXDLMSConverter::ToString((DLMS_SECURITY_POLICY1)m_SecurityPolicy, ln);
        values.push_back(ln);
    }
    values.push_back(CGXDLMSConverter::ToString(m_SecuritySuite));
    std::string str = m_ClientSystemTitle.ToHexString();
    values.push_back(str);
    str = m_ServerSystemTitle.ToHexString();
    values.push_back(str);
    if (GetVersion() > 0)
    {
        std::stringstream sb;
        bool empty = true;
        for (std::vector<CGXDLMSCertificateInfo*>::iterator it = m_Certificates.begin(); it != m_Certificates.end(); ++it)
        {
            if (empty)
            {
                empty = false;
            }
            else
            {
                sb << ',';
            }
            sb << '[';
            sb << (int)(*it)->GetEntity();
            sb << ' ';
            sb << (int)(*it)->GetType();
            sb << ' ';
            sb << (*it)->GetSerialNumber();
            sb << ' ';
            sb << (*it)->GetIssuer();
            sb << ' ';
            sb << (*it)->GetSubject();
            sb << ' ';
            sb << (*it)->GetSubjectAltName();
            sb << ']';
        }
        values.push_back(sb.str());
    }
}

void CGXDLMSSecuritySetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //SecurityPolicy
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //SecuritySuite
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //ClientSystemTitle
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    //ServerSystemTitle
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    if (GetVersion() > 0)
    {
        //Certificates
        if (all || CanRead(6))
        {
            attributes.push_back(6);
        }
    }
}

int CGXDLMSSecuritySetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 6 && GetVersion() > 0)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSSecuritySetup::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        int ret;
        CGXDLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 2)
    {
        CGXDLMSVariant tmp = m_SecurityPolicy;
        e.SetValue(tmp);
    }
    else if (e.GetIndex() == 3)
    {
        CGXDLMSVariant tmp = m_SecuritySuite;
        e.SetValue(tmp);
    }
    else if (e.GetIndex() == 4)
    {
        e.GetValue().Add(m_ClientSystemTitle.GetData(), m_ClientSystemTitle.GetSize());
    }
    else if (e.GetIndex() == 5)
    {
        e.GetValue().Add(m_ServerSystemTitle.GetData(), m_ServerSystemTitle.GetSize());
    }
    else if (e.GetIndex() == 6)
    {
        CGXByteBuffer bb;
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount((unsigned long)m_Certificates.size(), bb);
        for (std::vector<CGXDLMSCertificateInfo*>::iterator it = m_Certificates.begin(); it != m_Certificates.end(); ++it)
        {
            bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            GXHelpers::SetObjectCount(6, bb);
            bb.SetUInt8(DLMS_DATA_TYPE_ENUM);
            bb.SetUInt8((*it)->GetEntity());
            bb.SetUInt8(DLMS_DATA_TYPE_ENUM);
            bb.SetUInt8((*it)->GetType());
            bb.AddString((*it)->GetSerialNumber());
            bb.AddString((*it)->GetIssuer());
            bb.AddString((*it)->GetSubject());
            bb.AddString((*it)->GetSubjectAltName());
        }
        e.SetValue(bb);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int CGXDLMSSecuritySetup::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_SecurityPolicy = (DLMS_SECURITY_POLICY)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_SecuritySuite = (DLMS_SECURITY_SUITE)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_ClientSystemTitle.Set(e.GetValue().byteArr, e.GetValue().size);
    }
    else if (e.GetIndex() == 5)
    {
        m_ServerSystemTitle.Set(e.GetValue().byteArr, e.GetValue().size);
    }
    else if (e.GetIndex() == 6)
    {
        m_Certificates.clear();
        if (e.GetValue().vt != DLMS_DATA_TYPE_NONE)
        {
            std::string tmp;
            for (std::vector<CGXDLMSVariant >::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                CGXDLMSCertificateInfo* info = new CGXDLMSCertificateInfo();
                info->SetEntity((DLMS_CERTIFICATE_ENTITY)it->Arr[0].ToInteger());
                info->SetType((DLMS_CERTIFICATE_TYPE)it->Arr[1].ToInteger());
                tmp = it->Arr[2].ToString();
                info->SetSerialNumber(tmp);
                tmp = it->Arr[3].ToString();
                info->SetIssuer(tmp);
                tmp = it->Arr[4].ToString();
                info->SetSubject(tmp);
                tmp = it->Arr[5].ToString();
                info->SetSubjectAltName(tmp);
                m_Certificates.push_back(info);
            }
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

std::vector<CGXDLMSCertificateInfo*>& CGXDLMSSecuritySetup::GetCertificates()
{
    return m_Certificates;
}
