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

#ifndef GXDLMSSECURECLIENT_H
#define GXDLMSSECURECLIENT_H

#include "GXDLMSClient.h"

class CGXDLMSSecureClient : public CGXDLMSClient
{
private:
    CGXCipher m_Cipher;
public:
    /////////////////////////////////////////////////////////////////////////////
    //Constructor
    /////////////////////////////////////////////////////////////////////////////
    CGXDLMSSecureClient(
        bool UseLogicalNameReferencing = true,
        int ClientAddress = 16,
        int ServerAddress = 1,
        //Authentication type.
        DLMS_AUTHENTICATION authentication = DLMS_AUTHENTICATION_NONE,
        //Password if authentication is used.
        const char* password = NULL,
        DLMS_INTERFACE_TYPE intefaceType = DLMS_INTERFACE_TYPE_HDLC);

    /////////////////////////////////////////////////////////////////////////////
    //Destructor.
    /////////////////////////////////////////////////////////////////////////////
    ~CGXDLMSSecureClient();

    /**
     * @return Ciphering settings.
     */
    CGXCipher* GetCiphering();

    /**
    * Encrypt data using Key Encrypting Key.
    *
    * @param kek
    *            Key Encrypting Key, also known as Master key.
    * @param data
    *            Data to encrypt.
    * @return Encrypt data.
    */
    static int Encrypt(
        CGXByteBuffer& kek,
        CGXByteBuffer& data,
        CGXByteBuffer& reply);

    /**
    * Decrypt data using Key Encrypting Key.
    *
    * @param kek
    *            Key Encrypting Key, also known as Master key.
    * @param data
    *            Data to decrypt.
    * @return Decrypted data.
    */
    static int Decrypt(
        CGXByteBuffer& kek,
        CGXByteBuffer& data,
        CGXByteBuffer& reply);
};
#endif //GXDLMSCLIENT_H
