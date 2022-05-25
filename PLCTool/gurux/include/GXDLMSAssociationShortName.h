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

#ifndef GXDLMSASSOCIATIONSHORTNAME_H
#define GXDLMSASSOCIATIONSHORTNAME_H

#include "GXDLMSObject.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAssociationShortName
*/
class CGXDLMSAssociationShortName : public CGXDLMSObject
{
    CGXDLMSObjectCollection m_ObjectList;
    std::string m_SecuritySetupReference;
    CGXByteBuffer m_Secret;

    int GetAccessRights(CGXDLMSObject* pObj, CGXDLMSServer* server, CGXByteBuffer& data);
    void UpdateAccessRights(CGXDLMSVariant& buff);

public:
    //Constructor.
    CGXDLMSAssociationShortName();

    CGXDLMSObjectCollection& GetObjectList();

    CGXByteBuffer& GetSecret();

    void SetSecret(CGXByteBuffer& value);

    std::string& GetSecuritySetupReference();

    void SetSecuritySetupReference(std::string& value);

    //Get attribute values of object.
    void GetValues(
        std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    int GetDataType(
        int index,
        DLMS_DATA_TYPE& type);

    // Returns SN Association View.
    int GetObjects(
        CGXDLMSSettings& settings,
        CGXDLMSValueEventArg& e,
        CGXByteBuffer& data);

    int Invoke(
        CGXDLMSSettings& settings,
        CGXDLMSValueEventArg& e);

    int GetValue(
        CGXDLMSSettings& settings,
        CGXDLMSValueEventArg& e);

    int SetValue(
        CGXDLMSSettings& settings,
        CGXDLMSValueEventArg& e);
};
#endif //GXDLMSASSOCIATIONSHORTNAME_H