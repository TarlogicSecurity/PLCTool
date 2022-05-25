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

#include "../include/GXDLMSSecureClient.h"

CGXDLMSSecureClient::CGXDLMSSecureClient(bool UseLogicalNameReferencing,
    int clientAddress,
    int serverAddress,
    //Authentication type.
    DLMS_AUTHENTICATION authentication,
    //Password if authentication is used.
    const char* password,
    DLMS_INTERFACE_TYPE intefaceType) :
    CGXDLMSClient(
        UseLogicalNameReferencing,
        clientAddress,
        serverAddress,
        authentication,
        password,
        intefaceType), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}

CGXDLMSSecureClient::~CGXDLMSSecureClient()
{
}

CGXCipher* CGXDLMSSecureClient::GetCiphering()
{
    return m_Settings.GetCipher();
}

int CGXDLMSSecureClient::Encrypt(CGXByteBuffer& kek, CGXByteBuffer& data, CGXByteBuffer& reply)
{
    reply.Clear();
    if (kek.GetSize() < 16)
    {
        //Key Encrypting Key.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (kek.GetSize() % 8 != 0)
    {
        //Key Encrypting Key.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return CGXSecure::EncryptAesKeyWrapping(data, kek, reply);
}

int CGXDLMSSecureClient::Decrypt(CGXByteBuffer& kek, CGXByteBuffer& data, CGXByteBuffer& reply)
{
    reply.Clear();
    if (kek.GetSize() < 16)
    {
        //Key Encrypting Key.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (kek.GetSize() % 8 != 0)
    {
        //Key Encrypting Key.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (data.GetSize() < 16)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (data.GetSize() % 8 != 0)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return CGXSecure::DecryptAesKeyWrapping(data, kek, reply);
}