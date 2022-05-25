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
#include "../include/GXDLMSObjectFactory.h"
#include "../include/GXDLMSAssociationLogicalName.h"
#include "../include/GXDLMSServer.h"
#include "../include/GXBitString.h"

void CGXDLMSAssociationLogicalName::UpdateAccessRights(CGXDLMSObject* pObj, CGXDLMSVariant data)
{
    for (std::vector<CGXDLMSVariant >::iterator it = data.Arr[0].Arr.begin(); it != data.Arr[0].Arr.end(); ++it)
    {
        int id = it->Arr[0].ToInteger();
        DLMS_ACCESS_MODE mode = (DLMS_ACCESS_MODE)it->Arr[1].ToInteger();
        pObj->SetAccess(id, mode);
    }
    for (std::vector<CGXDLMSVariant >::iterator it = data.Arr[1].Arr.begin(); it != data.Arr[1].Arr.end(); ++it)
    {
        int id = it->Arr[0].ToInteger();
        DLMS_METHOD_ACCESS_MODE mode = (DLMS_METHOD_ACCESS_MODE)it->Arr[1].ToInteger();
        pObj->SetMethodAccess(id, mode);
    }
}

int CGXDLMSAssociationLogicalName::GetAccessRights(CGXDLMSObject* pItem, CGXDLMSServer* server, CGXByteBuffer& data)
{
    int ret;
    int cnt = pItem->GetAttributeCount();
    data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    data.SetUInt8(2);
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    GXHelpers::SetObjectCount(cnt, data);
    CGXDLMSValueEventArg e(server, pItem, 0);

    CGXDLMSVariant index, access, empty;
    for (int pos = 0; pos != cnt; ++pos)
    {
        e.SetIndex(pos + 1);
        index = pos + 1;
        if (server != NULL)
        {
            access = server->GetAttributeAccess(&e);
        }
        else
        {
            access = DLMS_ACCESS_MODE_READ_WRITE;
        }
        //attribute_access_item
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(3);
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, index)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_ENUM, access)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_NONE, empty)) != 0)
        {
            return ret;
        }
    }
    cnt = pItem->GetMethodCount();
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    GXHelpers::SetObjectCount(cnt, data);
    for (int pos = 0; pos != cnt; ++pos)
    {
        e.SetIndex(pos + 1);
        index = pos + 1;
        if (server != NULL)
        {
            access = server->GetMethodAccess(&e);
        }
        else
        {
            access = DLMS_METHOD_ACCESS_MODE_ACCESS;
        }
        //attribute_access_item
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, index);
        GXHelpers::SetData(data, DLMS_DATA_TYPE_ENUM, access);
    }
    return DLMS_ERROR_CODE_OK;
}


// Updates secret.
int CGXDLMSAssociationLogicalName::UpdateSecret(CGXDLMSClient* client, std::vector<CGXByteBuffer>& reply)
{
    if (m_AuthenticationMechanismName.GetMechanismId() == DLMS_AUTHENTICATION_NONE)
    {
#if defined(_WIN32) || defined(_WIN64)//Windows
        printf("Invalid authentication level in MechanismId.\n");
#endif
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (m_AuthenticationMechanismName.GetMechanismId() == DLMS_AUTHENTICATION_HIGH_GMAC)
    {
#if defined(_WIN32) || defined(_WIN64)//Windows
        printf("HighGMAC secret is updated using Security setup.\n");
#endif
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (m_AuthenticationMechanismName.GetMechanismId() == DLMS_AUTHENTICATION_LOW)
    {
        return client->Write(this, 7, reply);
    }
    //Action is used to update High authentication password.
    CGXDLMSVariant tmp = m_Secret;
    return client->Method(this, 2, tmp, reply);
}

// Add user to user list.
int CGXDLMSAssociationLogicalName::AddUser(CGXDLMSClient* client, unsigned char id, std::string name, std::vector<CGXByteBuffer>& reply)
{
    CGXByteBuffer data;
    data.SetUInt8((unsigned char)DLMS_DATA_TYPE_STRUCTURE);
    //Add structure size.
    data.SetUInt8(2);
    CGXDLMSVariant tmp = id;
    GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
    tmp = name;
    GXHelpers::SetData(data, DLMS_DATA_TYPE_STRING, tmp);
    tmp = data;
    return client->Method(this, 5, tmp, reply);
}

// Remove user fro user list.
int CGXDLMSAssociationLogicalName::RemoveUser(CGXDLMSClient* client, unsigned char id, std::string name, std::vector<CGXByteBuffer>& reply)
{
    CGXByteBuffer data;
    data.SetUInt8((unsigned char)DLMS_DATA_TYPE_STRUCTURE);
    //Add structure size.
    data.SetUInt8(2);
    CGXDLMSVariant tmp = id;
    GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
    tmp = name;
    GXHelpers::SetData(data, DLMS_DATA_TYPE_STRING, tmp);
    tmp = data;
    return client->Method(this, 6, tmp, reply);
}

// Returns LN Association View.
int CGXDLMSAssociationLogicalName::GetObjects(
    CGXDLMSSettings& settings,
    CGXDLMSValueEventArg& e,
    CGXByteBuffer& data)
{
    int ret;
    unsigned long pos = 0;
    //Add count only for first time.
    if (settings.GetIndex() == 0)
    {
        settings.SetCount((unsigned short)m_ObjectList.size());
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        GXHelpers::SetObjectCount((unsigned long)m_ObjectList.size(), data);
    }
    unsigned char gbt = (settings.GetNegotiatedConformance() & DLMS_CONFORMANCE_GENERAL_BLOCK_TRANSFER) != 0;
    for (CGXDLMSObjectCollection::iterator it = m_ObjectList.begin(); it != m_ObjectList.end(); ++it)
    {
        ++pos;
        if (!(pos <= settings.GetIndex()))
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(4);//Count
            CGXDLMSVariant type = (*it)->GetObjectType();
            CGXDLMSVariant version = (*it)->GetVersion();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, type);//ClassID
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, version);//Version
            CGXDLMSVariant ln((*it)->m_LN, 6, DLMS_DATA_TYPE_OCTET_STRING);
            GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln);//LN
            //Access rights.
            if ((ret = GetAccessRights(*it, e.GetServer(), data)) != 0)
            {
                return ret;
            };
            if (settings.IsServer())
            {
                settings.SetIndex(settings.GetIndex() + 1);
                //If PDU is full.
                if (!gbt && !e.GetSkipMaxPduSize() && data.GetSize() >= settings.GetMaxPduSize())
                {
                    break;
                }
            }
        }
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns user list
int CGXDLMSAssociationLogicalName::GetUsers(
    CGXDLMSSettings& settings,
    CGXDLMSValueEventArg& e,
    CGXByteBuffer& data)
{
    unsigned long pos = 0;
    //Add count only for first time.
    if (settings.GetIndex() == 0)
    {
        settings.SetCount((unsigned short)m_UserList.size());
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        GXHelpers::SetObjectCount((unsigned long)m_UserList.size(), data);
    }
    for (std::vector<std::pair<unsigned char, std::string> >::iterator it = m_UserList.begin(); it != m_UserList.end(); ++it)
    {
        ++pos;
        if (!(pos <= settings.GetIndex()))
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);//Count
            CGXDLMSVariant tmp = it->first;
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);//Id
            tmp = it->second;
            GXHelpers::SetData(data, DLMS_DATA_TYPE_STRING, tmp);//Name
            if (settings.IsServer())
            {
                settings.SetIndex(settings.GetIndex() + 1);
                //If PDU is full.
                if (!e.GetSkipMaxPduSize() && data.GetSize() >= settings.GetMaxPduSize())
                {
                    break;
                }
            }
        }
    }
    return DLMS_ERROR_CODE_OK;
}


CGXDLMSAssociationLogicalName::CGXDLMSAssociationLogicalName() :
    CGXDLMSAssociationLogicalName("0.0.40.0.0.255")
{
}

/**
 Constructor.
 @param ln Logical Name of the object.
*/
CGXDLMSAssociationLogicalName::CGXDLMSAssociationLogicalName(std::string ln) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, ln, 0)
{
    m_AssociationStatus = DLMS_ASSOCIATION_STATUS_NON_ASSOCIATED;
    m_Secret.AddString("Gurux");
    m_Version = 2;
    m_ClientSAP = 0;
    m_ServerSAP = 0;
}


CGXDLMSAssociationLogicalName::~CGXDLMSAssociationLogicalName()
{
    m_ObjectList.clear();
}

CGXDLMSObjectCollection& CGXDLMSAssociationLogicalName::GetObjectList()
{
    return m_ObjectList;
}

unsigned char CGXDLMSAssociationLogicalName::GetClientSAP()
{
    return m_ClientSAP;
}
void CGXDLMSAssociationLogicalName::SetClientSAP(unsigned char value)
{
    m_ClientSAP = value;
}

unsigned short CGXDLMSAssociationLogicalName::GetServerSAP()
{
    return m_ServerSAP;
}

void CGXDLMSAssociationLogicalName::SetServerSAP(unsigned short value)
{
    m_ServerSAP = value;
}

CGXApplicationContextName& CGXDLMSAssociationLogicalName::GetApplicationContextName()
{
    return m_ApplicationContextName;
}

CGXDLMSContextType& CGXDLMSAssociationLogicalName::GetXDLMSContextInfo()
{
    return m_XDLMSContextInfo;
}

CGXAuthenticationMechanismName& CGXDLMSAssociationLogicalName::GetAuthenticationMechanismName()
{
    return m_AuthenticationMechanismName;
}

CGXByteBuffer& CGXDLMSAssociationLogicalName::GetSecret()
{
    return m_Secret;
}
void CGXDLMSAssociationLogicalName::SetSecret(CGXByteBuffer& value)
{
    m_Secret = value;
}

std::vector<std::pair<unsigned char, std::string> >& CGXDLMSAssociationLogicalName::GetUserList()
{
    return m_UserList;
}

void CGXDLMSAssociationLogicalName::SetUserList(std::vector<std::pair<unsigned char, std::string> >& value)
{
    m_UserList = value;
}

std::pair<unsigned char, std::string>& CGXDLMSAssociationLogicalName::GetCurrentUser()
{
    return m_CurrentUser;
}

void CGXDLMSAssociationLogicalName::SetCurrentUser(std::pair<unsigned char, std::string>& value)
{
    m_CurrentUser = value;
}

DLMS_ASSOCIATION_STATUS CGXDLMSAssociationLogicalName::GetAssociationStatus()
{
    return m_AssociationStatus;
}

void CGXDLMSAssociationLogicalName::SetAssociationStatus(DLMS_ASSOCIATION_STATUS value)
{
    m_AssociationStatus = value;
}

std::string CGXDLMSAssociationLogicalName::GetSecuritySetupReference()
{
    return m_SecuritySetupReference;
}
void CGXDLMSAssociationLogicalName::SetSecuritySetupReference(std::string value)
{
    m_SecuritySetupReference = value;
}

// Returns amount of attributes.
int CGXDLMSAssociationLogicalName::GetAttributeCount()
{
    if (m_Version > 1)
        return 11;
    //Security Setup Reference is from version 1.
    if (GetVersion() > 0)
        return 9;
    return 8;
}

// Returns amount of methods.
int CGXDLMSAssociationLogicalName::GetMethodCount()
{
    if (m_Version > 1)
    {
        return 6;
    }
    return 4;
}

void CGXDLMSAssociationLogicalName::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_ObjectList.ToString());
    std::string str = CGXDLMSVariant(m_ClientSAP).ToString();
    str += "/";
    str += CGXDLMSVariant(m_ServerSAP).ToString();
    values.push_back(str);
    values.push_back(m_ApplicationContextName.ToString());
    values.push_back(m_XDLMSContextInfo.ToString());
    values.push_back(m_AuthenticationMechanismName.ToString());
    values.push_back(m_Secret.ToHexString());
    values.push_back(CGXDLMSVariant(m_AssociationStatus).ToString());
    //Security Setup Reference is from version 1.
    if (GetVersion() > 0)
    {
        values.push_back(m_SecuritySetupReference);
    }
    if (GetVersion() > 1)
    {
        std::stringstream sb;
        bool empty = true;
        for (std::vector<std::pair<unsigned char, std::string> >::iterator it = m_UserList.begin(); it != m_UserList.end(); ++it)
        {
            if (empty)
            {
                empty = false;
            }
            else
            {
                sb << ',';
            }
            sb << (int)it->first;
            sb << ':';
            sb << it->second;
        }
        values.push_back(sb.str());
        sb.clear();
        sb << (int) m_CurrentUser.first;
        sb << ':';
        sb << m_CurrentUser.second;
        values.push_back(sb.str());
    }
}

void CGXDLMSAssociationLogicalName::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ObjectList is static and read only once.
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //associated_partners_id is static and read only once.
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //Application Context Name is static and read only once.
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //xDLMS Context Info
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
    // Authentication Mechanism Name
    if (all || !IsRead(6))
    {
        attributes.push_back(6);
    }
    // Secret
    if (all || !IsRead(7))
    {
        attributes.push_back(7);
    }
    // Association Status
    if (all || !IsRead(8))
    {
        attributes.push_back(8);
    }
    //Security Setup Reference is from version 1.
    if (GetVersion() > 0 && !IsRead(9))
    {
        attributes.push_back(9);
    }
    //User list and current user are in version 2.
    if (m_Version > 1)
    {
        if (all || !IsRead(10))
        {
            attributes.push_back(10);
        }
        if (all || !IsRead(11))
        {
            attributes.push_back(11);
        }
    }
}

int CGXDLMSAssociationLogicalName::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 6)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 7)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 8)
    {
        type = DLMS_DATA_TYPE_ENUM;
        return DLMS_ERROR_CODE_OK;
    }
    if (m_Version > 0 && index == 9)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (m_Version > 1)
    {
        if (index == 10)
        {
            type = DLMS_DATA_TYPE_ARRAY;
            return DLMS_ERROR_CODE_OK;
        }
        if (index == 11)
        {
            type = DLMS_DATA_TYPE_STRUCTURE;
            return DLMS_ERROR_CODE_OK;
        }
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int CGXDLMSAssociationLogicalName::Invoke(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    // Check reply_to_HLS_authentication
    if (e.GetIndex() == 1)
    {
        int ret;
        unsigned long ic = 0;
        CGXByteBuffer secret;
        CGXByteBuffer* readSecret;
        if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
        {
            unsigned char ch;
            readSecret = &settings.GetSourceSystemTitle();
            CGXByteBuffer bb;
            bb.Set(e.GetParameters().byteArr, e.GetParameters().GetSize());
            if ((ret = bb.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            if ((ret = bb.GetUInt32(&ic)) != 0)
            {
                return ret;
            }
        }
        else if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_SHA256)
        {
            secret.Set(&m_Secret);
            secret.Set(&settings.GetSourceSystemTitle());
            secret.Set(&settings.GetCipher()->GetSystemTitle());
            secret.Set(&settings.GetStoCChallenge());
            secret.Set(&settings.GetCtoSChallenge());
            readSecret = &secret;
        }
        else
        {
            readSecret = &m_Secret;
        }
        CGXByteBuffer serverChallenge;
        if ((ret = CGXSecure::Secure(settings, settings.GetCipher(), ic,
            settings.GetStoCChallenge(), *readSecret, serverChallenge)) != 0)
        {
            return ret;
        }
        if (serverChallenge.Compare(e.GetParameters().byteArr, e.GetParameters().GetSize()))
        {
            if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
            {
                readSecret = &settings.GetCipher()->GetSystemTitle();
                ic = settings.GetCipher()->GetFrameCounter();
            }
            else if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_SHA256)
            {
                secret.Clear();
                secret.Set(&m_Secret);
                secret.Set(&settings.GetCipher()->GetSystemTitle());
                secret.Set(&settings.GetSourceSystemTitle());
                secret.Set(&settings.GetCtoSChallenge());
                secret.Set(&settings.GetStoCChallenge());
                readSecret = &secret;
            }
            else
            {
                readSecret = &m_Secret;
            }
            if ((ret = CGXSecure::Secure(settings, settings.GetCipher(), ic,
                settings.GetCtoSChallenge(), *readSecret, serverChallenge)) != 0)
            {
                return ret;
            }
            e.SetValue(serverChallenge);
            settings.SetConnected((DLMS_CONNECTION_STATE)(settings.GetConnected() | DLMS_CONNECTION_STATE_HDLC));
            m_AssociationStatus = DLMS_ASSOCIATION_STATUS_ASSOCIATED;
        }
        else
        {
            settings.SetConnected((DLMS_CONNECTION_STATE)(settings.GetConnected() & ~DLMS_CONNECTION_STATE_HDLC));
            m_AssociationStatus = DLMS_ASSOCIATION_STATUS_NON_ASSOCIATED;
        }
    }
    else if (e.GetIndex() == 2)
    {
        if (e.GetParameters().GetSize() == 0)
        {
            e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
        }
        else
        {
            m_Secret.Clear();
            m_Secret.Set(e.GetParameters().byteArr, e.GetParameters().GetSize());
        }
    }
    else if (e.GetIndex() == 5)
    {
        if (e.GetParameters().Arr.size() != 2)
        {
            e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
        }
        else
        {
            m_UserList.push_back(std::pair<unsigned char, std::string>(e.GetParameters().Arr[0].bVal, e.GetParameters().Arr[1].strVal));
        }
    }
    else if (e.GetIndex() == 6)
    {
        if (e.GetParameters().Arr.size() != 2)
        {
            e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
        }
        else
        {
            for (std::vector<std::pair<unsigned char, std::string> >::iterator it = m_UserList.begin(); it != m_UserList.end(); ++it)
            {
                if (it->first == e.GetParameters().Arr[0].bVal)
                {
                    m_UserList.erase(it);
                    break;
                }
            }
        }
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return 0;
}

int CGXDLMSAssociationLogicalName::GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    int ret;
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
    if (e.GetIndex() == 2)
    {
        e.SetByteArray(true);
        CGXByteBuffer buff;
        ret = GetObjects(settings, e, buff);
        ret = 0;
        e.SetValue(buff);
        return ret;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        //Add count
        data.SetUInt8(2);
        data.SetUInt8(DLMS_DATA_TYPE_INT8);
        data.SetUInt8(m_ClientSAP);
        data.SetUInt8(DLMS_DATA_TYPE_UINT16);
        data.SetUInt16(m_ServerSAP);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        //Add count
        data.SetUInt8(0x7);
        CGXDLMSVariant ctt = m_ApplicationContextName.GetJointIsoCtt();
        CGXDLMSVariant country = m_ApplicationContextName.GetCountry();
        CGXDLMSVariant name = m_ApplicationContextName.GetCountryName();
        CGXDLMSVariant organization = m_ApplicationContextName.GetIdentifiedOrganization();
        CGXDLMSVariant ua = m_ApplicationContextName.GetDlmsUA();
        CGXDLMSVariant context = m_ApplicationContextName.GetApplicationContext();
        CGXDLMSVariant id = m_ApplicationContextName.GetContextId();
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, ctt)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, country)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, name)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, organization)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, ua)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, context)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, id)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        e.SetByteArray(true);
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(6);
        if ((ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_BIT_STRING, CGXBitString::ToBitString(m_XDLMSContextInfo.GetConformance(), 24))) != 0 ||
            (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT16, m_XDLMSContextInfo.GetMaxReceivePduSize())) != 0 ||
            (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT16, m_XDLMSContextInfo.GetMaxSendPduSize())) != 0 ||
            (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_UINT8, m_XDLMSContextInfo.GetDlmsVersionNumber())) != 0 ||
            (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_INT8, m_XDLMSContextInfo.GetQualityOfService())) != 0 ||
            (ret = GXHelpers::SetData2(data, DLMS_DATA_TYPE_OCTET_STRING, m_XDLMSContextInfo.GetCypheringInfo())) != 0)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        e.SetByteArray(true);
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        //Add count
        data.SetUInt8(0x7);
        CGXDLMSVariant ctt = m_AuthenticationMechanismName.GetJointIsoCtt();
        CGXDLMSVariant country = m_AuthenticationMechanismName.GetCountry();
        CGXDLMSVariant name = m_AuthenticationMechanismName.GetCountryName();
        CGXDLMSVariant organization = m_AuthenticationMechanismName.GetIdentifiedOrganization();
        CGXDLMSVariant ua = m_AuthenticationMechanismName.GetDlmsUA();
        CGXDLMSVariant context = m_AuthenticationMechanismName.GetAuthenticationMechanismName();
        CGXDLMSVariant id = m_AuthenticationMechanismName.GetMechanismId();
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, ctt)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, country)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, name)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, organization)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, ua)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, context)) != 0 ||
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, id)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 7)
    {
        e.SetValue(m_Secret);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 8)
    {
        e.SetValue((unsigned char)m_AssociationStatus);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 9)
    {
        CGXDLMSVariant tmp;
        GXHelpers::SetLogicalName(m_SecuritySetupReference.c_str(), tmp);
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 10)
    {
        e.SetByteArray(true);
        CGXByteBuffer buff;
        ret = GetUsers(settings, e, buff);
        e.SetValue(buff);
        return ret;
    }
    if (e.GetIndex() == 11)
    {
        e.SetByteArray(true);
        CGXByteBuffer data;
        data.SetUInt8((unsigned char)DLMS_DATA_TYPE_STRUCTURE);
        //Add structure size.
        data.SetUInt8(2);
        CGXDLMSVariant tmp = m_CurrentUser.first;
        GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = m_CurrentUser.second;
        GXHelpers::SetData(data, DLMS_DATA_TYPE_STRING, tmp);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int CGXDLMSAssociationLogicalName::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_ObjectList.clear();
        if (e.GetValue().vt != DLMS_DATA_TYPE_NONE)
        {
            for (std::vector<CGXDLMSVariant >::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                DLMS_OBJECT_TYPE type = (DLMS_OBJECT_TYPE)(*it).Arr[0].ToInteger();
                int version = (*it).Arr[1].ToInteger();
                std::string ln;
                GXHelpers::GetLogicalName((*it).Arr[2].byteArr, ln);
                CGXDLMSObject* pObj = settings.GetObjects().FindByLN(type, ln);
                if (pObj == NULL)
                {
                    pObj = CGXDLMSObjectFactory::CreateObject(type, ln);
                    if (pObj != NULL)
                    {
                        settings.AddAllocateObject(pObj);
                        pObj->SetVersion(version);
                    }
                }
                if (pObj != NULL)
                {
                    UpdateAccessRights(pObj, (*it).Arr[3]);
                    m_ObjectList.push_back(pObj);
                }
            }
        }
    }
    else if (e.GetIndex() == 3)
    {
        if (e.GetValue().Arr.size() == 2)
        {
            m_ClientSAP = e.GetValue().Arr[0].ToInteger();
            m_ServerSAP = e.GetValue().Arr[1].ToInteger();
        }
        else
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
    }
    else if (e.GetIndex() == 4)
    {
        //Value of the object identifier encoded in BER
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            CGXByteBuffer tmp;
            tmp.Set(e.GetValue().byteArr, e.GetValue().GetSize());
            int ret;
            unsigned char val;
            if ((ret = tmp.GetUInt8(0, &val)) != 0)
            {
                return ret;
            }
            if (val == 0x60)
            {
                m_ApplicationContextName.SetJointIsoCtt(0);
                m_ApplicationContextName.SetCountry(0);
                m_ApplicationContextName.SetCountryName(0);
                tmp.SetPosition(3);
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetIdentifiedOrganization(val);
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetDlmsUA(val);
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetApplicationContext(val);
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetContextId((DLMS_APPLICATION_CONTEXT_NAME)val);
            }
            else
            {
                //Get Tag and Len.
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 2)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 7)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x11)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetJointIsoCtt(val);
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x11)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetCountry(val);
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x12)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetCountryName(val);
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x11)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetIdentifiedOrganization(val);
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x11)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetDlmsUA(val);
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x11)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetApplicationContext(val);
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x11)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetContextId((DLMS_APPLICATION_CONTEXT_NAME)val);
            }
        }
        else
        {
            m_ApplicationContextName.SetJointIsoCtt(e.GetValue().Arr[0].ToInteger());
            m_ApplicationContextName.SetCountry(e.GetValue().Arr[1].ToInteger());
            m_ApplicationContextName.SetCountryName(e.GetValue().Arr[2].ToInteger());
            m_ApplicationContextName.SetIdentifiedOrganization(e.GetValue().Arr[3].ToInteger());
            m_ApplicationContextName.SetDlmsUA(e.GetValue().Arr[4].ToInteger());
            m_ApplicationContextName.SetApplicationContext(e.GetValue().Arr[5].ToInteger());
            m_ApplicationContextName.SetContextId((DLMS_APPLICATION_CONTEXT_NAME)e.GetValue().Arr[6].ToInteger());
        }
    }
    else if (e.GetIndex() == 5)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRUCTURE)
        {
            CGXByteBuffer tmp;
            m_XDLMSContextInfo.SetConformance((DLMS_CONFORMANCE)e.GetValue().Arr[0].ToInteger());
            m_XDLMSContextInfo.SetMaxReceivePduSize(e.GetValue().Arr[1].ToInteger());
            m_XDLMSContextInfo.SetMaxSendPduSize(e.GetValue().Arr[2].ToInteger());
            m_XDLMSContextInfo.SetDlmsVersionNumber(e.GetValue().Arr[3].ToInteger());
            m_XDLMSContextInfo.SetQualityOfService(e.GetValue().Arr[4].ToInteger());
            tmp.Set(e.GetValue().Arr[5].byteArr, e.GetValue().Arr[5].GetSize());
            m_XDLMSContextInfo.SetCypheringInfo(tmp);
        }
    }
    else if (e.GetIndex() == 6)
    {
        if (e.GetValue().vt != DLMS_DATA_TYPE_NONE)
        {
            unsigned char val;
            int ret;
            //Value of the object identifier encoded in BER
            if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                CGXByteBuffer tmp;
                tmp.Set(e.GetValue().byteArr, e.GetValue().GetSize());
                if ((ret = tmp.GetUInt8(0, &val)) != 0)
                {
                    return ret;
                }
                if (val == 0x60)
                {
                    m_AuthenticationMechanismName.SetJointIsoCtt(0);
                    m_AuthenticationMechanismName.SetCountry(0);
                    m_AuthenticationMechanismName.SetCountryName(0);
                    tmp.SetPosition(3);
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetIdentifiedOrganization(val);
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetDlmsUA(val);
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetAuthenticationMechanismName(val);
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetMechanismId((DLMS_AUTHENTICATION)val);
                }
                else
                {
                    //Get Tag and Len.
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 2)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 7)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x11)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetJointIsoCtt(val);
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x11)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetCountry(val);
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x12)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    unsigned short tmp2;
                    if ((ret = tmp.GetUInt16(&tmp2)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetCountryName(tmp2);
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x11)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetIdentifiedOrganization(val);
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x11)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetDlmsUA(val);
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x11)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetAuthenticationMechanismName(val);
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x11)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetMechanismId((DLMS_AUTHENTICATION)val);
                }
            }
            else
            {
                m_AuthenticationMechanismName.SetJointIsoCtt(e.GetValue().Arr[0].ToInteger());
                m_AuthenticationMechanismName.SetCountry(e.GetValue().Arr[1].ToInteger());
                m_AuthenticationMechanismName.SetCountryName(e.GetValue().Arr[2].ToInteger());
                m_AuthenticationMechanismName.SetIdentifiedOrganization(e.GetValue().Arr[3].ToInteger());
                m_AuthenticationMechanismName.SetDlmsUA(e.GetValue().Arr[4].ToInteger());
                m_AuthenticationMechanismName.SetAuthenticationMechanismName(e.GetValue().Arr[5].ToInteger());
                m_AuthenticationMechanismName.SetMechanismId((DLMS_AUTHENTICATION)e.GetValue().Arr[6].ToInteger());
            }
        }
    }
    else if (e.GetIndex() == 7)
    {
        m_Secret.Clear();
        m_Secret.Set(e.GetValue().byteArr, e.GetValue().size);
    }
    else if (e.GetIndex() == 8)
    {
        m_AssociationStatus = (DLMS_ASSOCIATION_STATUS)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 9)
    {
        GXHelpers::GetLogicalName(e.GetValue().byteArr, m_SecuritySetupReference);
    }
    else if (e.GetIndex() == 10)
    {
        m_UserList.clear();
        for (std::vector<CGXDLMSVariant >::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            m_UserList.push_back(std::pair<unsigned char, std::string>(it->Arr[0].bVal, it->Arr[1].strVal));
        }
    }
    else if (e.GetIndex() == 11)
    {
        if (e.GetValue().Arr.size() == 2)
        {
            m_CurrentUser = std::pair<unsigned char, std::string>(e.GetValue().Arr[0].bVal, e.GetValue().Arr[1].strVal);
        }
        else
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
