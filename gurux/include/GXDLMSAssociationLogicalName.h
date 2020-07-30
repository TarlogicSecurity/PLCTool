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
#ifndef GXDLMSASSOCIATIONLOGICALNAME_H
#define GXDLMSASSOCIATIONLOGICALNAME_H

#include "GXDLMSObject.h"
#include "GXApplicationContextName.h"
#include "GXDLMSContextType.h"
#include "GXAuthenticationMechanismName.h"
#include "GXDLMSObjectCollection.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAssociationLogicalName
*/
class CGXDLMSAssociationLogicalName : public CGXDLMSObject
{
private:

    DLMS_ASSOCIATION_STATUS m_AssociationStatus;
    CGXDLMSObjectCollection m_ObjectList;
    unsigned char m_ClientSAP;
    unsigned short m_ServerSAP;
    CGXApplicationContextName m_ApplicationContextName;
    CGXDLMSContextType m_XDLMSContextInfo;
    CGXAuthenticationMechanismName m_AuthenticationMechanismName;

    /**
    * Secret.
    */
    CGXByteBuffer m_Secret;

    std::string m_SecuritySetupReference;

    std::vector<std::pair<unsigned char, std::string> > m_UserList;

    std::pair<unsigned char, std::string> m_CurrentUser;

    void UpdateAccessRights(
        CGXDLMSObject* pObj,
        CGXDLMSVariant data);

    int GetAccessRights(
        CGXDLMSObject* pItem,
        CGXDLMSServer* server,
        CGXByteBuffer& data);

    // Returns LN Association View.
    int GetObjects(
        CGXDLMSSettings& settings,
        CGXDLMSValueEventArg& e,
        CGXByteBuffer& data);

    int GetUsers(
        CGXDLMSSettings& settings,
        CGXDLMSValueEventArg& e,
        CGXByteBuffer& data);
public:
    /**
     Constructor.
    */
    CGXDLMSAssociationLogicalName();

    /**
     Constructor.
     ln: Logical Name of the object.
    */
    CGXDLMSAssociationLogicalName(std::string ln);

    /**
     Destructor.
    */
    ~CGXDLMSAssociationLogicalName();

    CGXDLMSObjectCollection& GetObjectList();


    // Contains the identifiers of the COSEM client APs within the physical devices hosting these APs,
    // which belong to the AA modelled by the “Association LN” object.
    unsigned char GetClientSAP();
    void SetClientSAP(unsigned char value);

    // Contains the identifiers of the COSEM server (logical device) APs within the physical
    // devices hosting these APs, which belong to the AA modelled by the “Association LN” object.
    unsigned short GetServerSAP();
    void SetServerSAP(unsigned short value);

    CGXApplicationContextName& GetApplicationContextName();

    CGXDLMSContextType& GetXDLMSContextInfo();

    CGXAuthenticationMechanismName& GetAuthenticationMechanismName();

    CGXByteBuffer& GetSecret();

    void SetSecret(CGXByteBuffer& value);

    std::vector<std::pair<unsigned char, std::string> >& GetUserList();

    void SetUserList(std::vector<std::pair<unsigned char, std::string> >& value);

    std::pair<unsigned char, std::string>& GetCurrentUser();

    void SetCurrentUser(std::pair<unsigned char, std::string>& value);


    // Updates secret.
    int UpdateSecret(
        CGXDLMSClient* client,
        std::vector<CGXByteBuffer>& reply);

    // Add user to user list.
    int AddUser(
        CGXDLMSClient* client,
        unsigned char id,
        std::string name,
        std::vector<CGXByteBuffer>& reply);

    // Remove user fro user list.
    int RemoveUser(
        CGXDLMSClient* client,
        unsigned char id,
        std::string name,
        std::vector<CGXByteBuffer>& reply);


    DLMS_ASSOCIATION_STATUS GetAssociationStatus();

    void SetAssociationStatus(DLMS_ASSOCIATION_STATUS value);

    std::string GetSecuritySetupReference();
    void SetSecuritySetupReference(std::string value);

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    int Invoke(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    int GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);
    int SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);
};
#endif //GXDLMSASSOCIATIONLOGICALNAME_H