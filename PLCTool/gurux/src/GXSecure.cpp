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

#include "../include/GXSecure.h"
#include "../include/GXDLMSMd5.h"
#include "../include/GXDLMSSha1.h"
#include "../include/GXDLMSSha256.h"

#include <stdlib.h> //rand

int CGXSecure::GenerateChallenge(DLMS_AUTHENTICATION, CGXByteBuffer& challenge)
{
    // Random challenge is 8 to 64 bytes.
    // Texas Instruments accepts only 16 byte long challenge.
    // For this reason challenge size is 16 bytes at the moment.
    int len = 16;
    //int len = rand() % 58 + 8;
    unsigned char val;
    for (int pos = 0; pos != len; ++pos)
    {
        val = rand();
        challenge.SetUInt8(val);
    }
    return 0;
}

static const unsigned char WRAP_IV[] = { 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6 };
int CGXSecure::EncryptAesKeyWrapping(CGXByteBuffer& data, CGXByteBuffer& kek, CGXByteBuffer& reply)
{
    CGXByteBuffer buf, buf2;
    //unsigned char buf[16] = { 0 };
    unsigned char n, j, i;

    if (kek.GetSize() != 16 || data.GetSize() != 16)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    reply.Capacity(24);
    reply.SetSize(24);
    reply.SetPosition(0);
    // Amount of 64-bit blocks.
    n = (unsigned char)(data.GetSize() >> 3);
    memcpy(reply.GetData(), WRAP_IV, 8);
    memcpy(reply.GetData() + 8, data.GetData(), data.GetSize());
    for (j = 0; j != 6; j++)
    {
        for (i = 1; i <= n; i++)
        {
            kek.SetPosition(0);
            buf2.SetSize(0);
            buf2.Set(&kek);
            buf.SetSize(0);
            buf.Set(reply.GetData(), 8);
            buf.Set(reply.GetData() + (8 * i), 8);
            CGXCipher::Aes1Encrypt(buf, 0, buf2);
            unsigned int t = n * j + i;
            for (int k = 1; t != 0; k++)
            {
                unsigned char v = (unsigned char)t;
                buf.GetData()[sizeof(WRAP_IV) - k] ^= v;
                t = (int)((unsigned int)t >> 8);
            }
            memcpy(reply.GetData(), buf.GetData(), 8);
            memcpy(reply.GetData() + (8 * i), buf.GetData() + 8, 8);
        }
    }
    return 0;
}

int CGXSecure::DecryptAesKeyWrapping(CGXByteBuffer& data, CGXByteBuffer& kek, CGXByteBuffer& reply)
{
    unsigned char a[8];
    CGXByteBuffer buf, buf2;
    signed char j, i;
    unsigned char k, v, n;
    unsigned short t;
    // Amount of 64-bit blocks.
    n = (unsigned char)(data.GetSize() >> 3);
    if (kek.GetSize() != 16 || data.GetSize() != n * 8)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    reply.Capacity(16);
    reply.SetSize(16);
    reply.SetPosition(0);
    memcpy(a, data.GetData(), sizeof(WRAP_IV));
    memcpy(reply.GetData(), data.GetData() + sizeof(WRAP_IV), data.GetSize() - sizeof(WRAP_IV));
    if (--n == 0)
    {
        n = 1;
    }
    for (j = 5; j >= 0; j--)
    {
        for (i = n; i >= 1; i--)
        {
            buf2.SetSize(0);
            buf2.Set(&kek);
            buf.SetSize(0);
            buf.Set(a, sizeof(WRAP_IV));
            buf.Set(reply.GetData() + 8 * (i - 1), 8);
            t = n * j + i;
            for (k = 1; t != 0; k++)
            {
                v = (unsigned char)t;
                buf.GetData()[sizeof(WRAP_IV) - k] ^= v;
                t = (unsigned short)(t >> 8);
            }
            CGXCipher::Aes1Decrypt(buf, buf2);
            memcpy(a, buf.GetData(), 8);
            memcpy(reply.GetData() + 8 * (i - 1), buf.GetData() + 8, 8);
        }
    }
    if (memcmp(a, WRAP_IV, sizeof(WRAP_IV)) != 0)
    {
        reply.SetSize(16);
        return DLMS_ERROR_CODE_FALSE;
    }
    return 0;
}

/**
    * Chipher text.
    *
    * @param auth
    *            Authentication level.
    * @param data
    *            Text to chipher.
    * @param secret
    *            Secret.
    * @return Chiphered text.
    */
int CGXSecure::Secure(
    CGXDLMSSettings& settings,
    CGXCipher* cipher,
    unsigned long ic,
    CGXByteBuffer& data,
    CGXByteBuffer& secret,
    CGXByteBuffer& reply)
{
    int ret = 0, pos;
    reply.Clear();
    if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH)
    {
        CGXByteBuffer s;
        int len = data.GetSize();
        if (len % 16 != 0)
        {
            len += (16 - (data.GetSize() % 16));
        }
        if (secret.GetSize() > data.GetSize())
        {
            len = secret.GetSize();
            if (len % 16 != 0)
            {
                len += (16 - (secret.GetSize() % 16));
            }
        }
        s.Set(&secret);
        s.Zero(s.GetSize(), len - s.GetSize());
        reply.Set(&data);
        reply.Zero(reply.GetSize(), len - reply.GetSize());
        for (pos = 0; pos < len / 16; ++pos)
        {
            CGXCipher::Aes1Encrypt(reply, pos * 16, s);
        }
        return 0;
    }
    // Get server Challenge.
    CGXByteBuffer challenge;
    // Get shared secret
    if (settings.GetAuthentication() != DLMS_AUTHENTICATION_HIGH_GMAC &&
        settings.GetAuthentication() != DLMS_AUTHENTICATION_HIGH_SHA256)
    {
        challenge.Set(&data);
        challenge.Set(&secret);
    }
    if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_MD5)
    {
        return CGXDLMSMD5::Encrypt(challenge, reply);
    }
    else if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_SHA1)
    {
        return CGXDLMSSha1::Encrypt(challenge, reply);
    }
    else if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_SHA256)
    {
        return CGXDLMSSha256::Encrypt(secret, reply);
    }
    else if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
    {
        CGXByteBuffer& key = settings.GetCipher()->GetBlockCipherKey();
        ret = cipher->Encrypt(DLMS_SECURITY_AUTHENTICATION,
            DLMS_COUNT_TYPE_TAG, ic, 0, secret, key, data, true);
        if (ret == 0)
        {
            reply.SetUInt8(DLMS_SECURITY_AUTHENTICATION);
            reply.SetUInt32(ic);
            reply.Set(&data);
        }
    }
    return ret;
}
