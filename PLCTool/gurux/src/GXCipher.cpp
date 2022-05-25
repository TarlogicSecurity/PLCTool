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

#include "../include/GXCipher.h"
#include "../include/chipperingenums.h"
#include "../include/GXHelpers.h"

void CGXCipher::Init(
    unsigned char* systemTitle,
    unsigned char count)
{
    const unsigned char BLOCKCIPHERKEY[] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
    };
    const unsigned char AUTHENTICATIONKEY[] =
    {
        0xD0,  0xD1,  0xD2, 0xD3,  0xD4,  0xD5,  0xD6,  0xD7,
        0xD8,  0xD9,  0xDA,  0xDB,  0xDC, 0xDD,  0xDE,  0xDF
    };
    m_FrameCounter = 0;
    m_Security = DLMS_SECURITY_NONE;
    m_SystemTitle.Set(systemTitle, count);
    m_BlockCipherKey.Set(BLOCKCIPHERKEY, sizeof(BLOCKCIPHERKEY));
    m_AuthenticationKey.Set(AUTHENTICATIONKEY, sizeof(AUTHENTICATIONKEY));
    m_SecuritySuite = DLMS_SECURITY_SUITE_AES_GCM_128;
}

CGXCipher::CGXCipher(CGXByteBuffer& systemTitle)
{
    Init((unsigned char*)systemTitle.m_Data, (unsigned char)systemTitle.m_Size);
}

CGXCipher::CGXCipher(const char* systemTitle)
{
    Init((unsigned char*)systemTitle, (unsigned char)strlen(systemTitle));
}

CGXCipher::CGXCipher(unsigned char* systemTitle, unsigned char count)
{
    Init(systemTitle, count);
}

CGXCipher::~CGXCipher()
{
}

/**
* Get nonse from frame counter and system title.
*
* @param frameCounter
*            Frame counter.
* @param systemTitle
*            System title.
* @return
*/
static int GetNonse(
    unsigned long frameCounter,
    CGXByteBuffer& systemTitle,
    CGXByteBuffer& nonce)
{
    if (systemTitle.GetSize() != 8)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    nonce.SetSize(0);
    nonce.Capacity(12);
    nonce.Set(systemTitle.GetData(), systemTitle.GetSize());
    nonce.SetUInt32(frameCounter);
    return 0;
}

//Get UInt32.
#define GETU32(pt) (((unsigned long)(pt)[0] << 24) | \
                    ((unsigned long)(pt)[1] << 16) | \
                    ((unsigned long)(pt)[2] <<  8) | \
                    ((unsigned long)(pt)[3]))

//Set Int32 as Big Endian value.
#define PUT32(ct, st) { \
                    (ct)[0] = (unsigned char)((st) >> 24); \
                    (ct)[1] = (unsigned char)((st) >> 16); \
                    (ct)[2] = (unsigned char)((st) >>  8); \
                    (ct)[3] = (unsigned char)(st); }

//Note! Rcon value is different than in aes!
static const unsigned char Rcon[11] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };


//There is only one TE table. Counting is taking little bit longer, but memory is needed less.
const unsigned long Te0[256] = {
    0xc66363a5U, 0xf87c7c84U, 0xee777799U, 0xf67b7b8dU,
    0xfff2f20dU, 0xd66b6bbdU, 0xde6f6fb1U, 0x91c5c554U,
    0x60303050U, 0x02010103U, 0xce6767a9U, 0x562b2b7dU,
    0xe7fefe19U, 0xb5d7d762U, 0x4dababe6U, 0xec76769aU,
    0x8fcaca45U, 0x1f82829dU, 0x89c9c940U, 0xfa7d7d87U,
    0xeffafa15U, 0xb25959ebU, 0x8e4747c9U, 0xfbf0f00bU,
    0x41adadecU, 0xb3d4d467U, 0x5fa2a2fdU, 0x45afafeaU,
    0x239c9cbfU, 0x53a4a4f7U, 0xe4727296U, 0x9bc0c05bU,
    0x75b7b7c2U, 0xe1fdfd1cU, 0x3d9393aeU, 0x4c26266aU,
    0x6c36365aU, 0x7e3f3f41U, 0xf5f7f702U, 0x83cccc4fU,
    0x6834345cU, 0x51a5a5f4U, 0xd1e5e534U, 0xf9f1f108U,
    0xe2717193U, 0xabd8d873U, 0x62313153U, 0x2a15153fU,
    0x0804040cU, 0x95c7c752U, 0x46232365U, 0x9dc3c35eU,
    0x30181828U, 0x379696a1U, 0x0a05050fU, 0x2f9a9ab5U,
    0x0e070709U, 0x24121236U, 0x1b80809bU, 0xdfe2e23dU,
    0xcdebeb26U, 0x4e272769U, 0x7fb2b2cdU, 0xea75759fU,
    0x1209091bU, 0x1d83839eU, 0x582c2c74U, 0x341a1a2eU,
    0x361b1b2dU, 0xdc6e6eb2U, 0xb45a5aeeU, 0x5ba0a0fbU,
    0xa45252f6U, 0x763b3b4dU, 0xb7d6d661U, 0x7db3b3ceU,
    0x5229297bU, 0xdde3e33eU, 0x5e2f2f71U, 0x13848497U,
    0xa65353f5U, 0xb9d1d168U, 0x00000000U, 0xc1eded2cU,
    0x40202060U, 0xe3fcfc1fU, 0x79b1b1c8U, 0xb65b5bedU,
    0xd46a6abeU, 0x8dcbcb46U, 0x67bebed9U, 0x7239394bU,
    0x944a4adeU, 0x984c4cd4U, 0xb05858e8U, 0x85cfcf4aU,
    0xbbd0d06bU, 0xc5efef2aU, 0x4faaaae5U, 0xedfbfb16U,
    0x864343c5U, 0x9a4d4dd7U, 0x66333355U, 0x11858594U,
    0x8a4545cfU, 0xe9f9f910U, 0x04020206U, 0xfe7f7f81U,
    0xa05050f0U, 0x783c3c44U, 0x259f9fbaU, 0x4ba8a8e3U,
    0xa25151f3U, 0x5da3a3feU, 0x804040c0U, 0x058f8f8aU,
    0x3f9292adU, 0x219d9dbcU, 0x70383848U, 0xf1f5f504U,
    0x63bcbcdfU, 0x77b6b6c1U, 0xafdada75U, 0x42212163U,
    0x20101030U, 0xe5ffff1aU, 0xfdf3f30eU, 0xbfd2d26dU,
    0x81cdcd4cU, 0x180c0c14U, 0x26131335U, 0xc3ecec2fU,
    0xbe5f5fe1U, 0x359797a2U, 0x884444ccU, 0x2e171739U,
    0x93c4c457U, 0x55a7a7f2U, 0xfc7e7e82U, 0x7a3d3d47U,
    0xc86464acU, 0xba5d5de7U, 0x3219192bU, 0xe6737395U,
    0xc06060a0U, 0x19818198U, 0x9e4f4fd1U, 0xa3dcdc7fU,
    0x44222266U, 0x542a2a7eU, 0x3b9090abU, 0x0b888883U,
    0x8c4646caU, 0xc7eeee29U, 0x6bb8b8d3U, 0x2814143cU,
    0xa7dede79U, 0xbc5e5ee2U, 0x160b0b1dU, 0xaddbdb76U,
    0xdbe0e03bU, 0x64323256U, 0x743a3a4eU, 0x140a0a1eU,
    0x924949dbU, 0x0c06060aU, 0x4824246cU, 0xb85c5ce4U,
    0x9fc2c25dU, 0xbdd3d36eU, 0x43acacefU, 0xc46262a6U,
    0x399191a8U, 0x319595a4U, 0xd3e4e437U, 0xf279798bU,
    0xd5e7e732U, 0x8bc8c843U, 0x6e373759U, 0xda6d6db7U,
    0x018d8d8cU, 0xb1d5d564U, 0x9c4e4ed2U, 0x49a9a9e0U,
    0xd86c6cb4U, 0xac5656faU, 0xf3f4f407U, 0xcfeaea25U,
    0xca6565afU, 0xf47a7a8eU, 0x47aeaee9U, 0x10080818U,
    0x6fbabad5U, 0xf0787888U, 0x4a25256fU, 0x5c2e2e72U,
    0x381c1c24U, 0x57a6a6f1U, 0x73b4b4c7U, 0x97c6c651U,
    0xcbe8e823U, 0xa1dddd7cU, 0xe874749cU, 0x3e1f1f21U,
    0x964b4bddU, 0x61bdbddcU, 0x0d8b8b86U, 0x0f8a8a85U,
    0xe0707090U, 0x7c3e3e42U, 0x71b5b5c4U, 0xcc6666aaU,
    0x904848d8U, 0x06030305U, 0xf7f6f601U, 0x1c0e0e12U,
    0xc26161a3U, 0x6a35355fU, 0xae5757f9U, 0x69b9b9d0U,
    0x17868691U, 0x99c1c158U, 0x3a1d1d27U, 0x279e9eb9U,
    0xd9e1e138U, 0xebf8f813U, 0x2b9898b3U, 0x22111133U,
    0xd26969bbU, 0xa9d9d970U, 0x078e8e89U, 0x339494a7U,
    0x2d9b9bb6U, 0x3c1e1e22U, 0x15878792U, 0xc9e9e920U,
    0x87cece49U, 0xaa5555ffU, 0x50282878U, 0xa5dfdf7aU,
    0x038c8c8fU, 0x59a1a1f8U, 0x09898980U, 0x1a0d0d17U,
    0x65bfbfdaU, 0xd7e6e631U, 0x844242c6U, 0xd06868b8U,
    0x824141c3U, 0x299999b0U, 0x5a2d2d77U, 0x1e0f0f11U,
    0x7bb0b0cbU, 0xa85454fcU, 0x6dbbbbd6U, 0x2c16163aU,
};

#define RCON(i) ((unsigned long) Rcon[(i)] << 24)

#define ROTATE(val, bits) ((val >> bits) | (val << (32 - bits)))

#define TE0(i) Te0[((i) >> 24) & 0xff]

#define TE1(i) ROTATE(Te0[((i) >> 16) & 0xff], 8)

#define TE2(i) ROTATE(Te0[((i) >> 8) & 0xff], 16)

#define TE3(i) ROTATE(Te0[(i) & 0xff], 24)

#define TE41(i) ((Te0[((i) >> 24) & 0xff] << 8) & 0xff000000)

#define TE42(i) (Te0[((i) >> 16) & 0xff] & 0x00ff0000)

#define TE43(i) (Te0[((i) >> 8) & 0xff] & 0x0000ff00)

#define TE44(i) ((Te0[(i) & 0xff] >> 8) & 0x000000ff)

#define TE421(i) ((Te0[((i) >> 16) & 0xff] << 8) & 0xff000000)

#define TE432(i) (Te0[((i) >> 8) & 0xff] & 0x00ff0000)

#define TE443(i) (Te0[(i) & 0xff] & 0x0000ff00)

#define TE414(i) ((Te0[((i) >> 24) & 0xff] >> 8) & 0x000000ff)

#define TE411(i) ((Te0[((i) >> 24) & 0xff] << 8) & 0xff000000)

#define TE422(i) (Te0[((i) >> 16) & 0xff] & 0x00ff0000)

#define TE433(i) (Te0[((i) >> 8) & 0xff] & 0x0000ff00)

#define TE444(i) ((Te0[(i) & 0xff] >> 8) & 0x000000ff)

int CGXCipher::Int(uint32_t* rk,
    const unsigned char* cipherKey,
    unsigned short keyBits)
{
    unsigned long i;
    unsigned long temp;

    rk[0] = GETU32(cipherKey);
    rk[1] = GETU32(cipherKey + 4);
    rk[2] = GETU32(cipherKey + 8);
    rk[3] = GETU32(cipherKey + 12);

    if (keyBits == 128)
    {
        for (i = 0; i < 10; i++)
        {
            temp = rk[3];
            rk[4] = rk[0] ^ TE421(temp) ^ TE432(temp) ^
                TE443(temp) ^ TE414(temp);
            rk[4] = rk[0] ^ TE421(temp) ^ TE432(temp) ^
                TE443(temp) ^ TE414(temp) ^ RCON(i);
            rk[5] = rk[1] ^ rk[4];
            rk[6] = rk[2] ^ rk[5];
            rk[7] = rk[3] ^ rk[6];
            rk += 4;
        }
        return 0;
    }

    rk[4] = GETU32(cipherKey + 16);
    rk[5] = GETU32(cipherKey + 20);

    if (keyBits == 192)
    {
        for (i = 0; i < 8; i++)
        {
            temp = rk[5];
            rk[6] = rk[0] ^ TE421(temp) ^ TE432(temp) ^
                TE443(temp) ^ TE414(temp) ^ RCON(i);
            rk[7] = rk[1] ^ rk[6];
            rk[8] = rk[2] ^ rk[7];
            rk[9] = rk[3] ^ rk[8];
            if (i == 7)
                return 12;
            rk[10] = rk[4] ^ rk[9];
            rk[11] = rk[5] ^ rk[10];
            rk += 6;
        }
    }

    rk[6] = GETU32(cipherKey + 24);
    rk[7] = GETU32(cipherKey + 28);

    if (keyBits == 256)
    {
        for (i = 0; i < 7; i++)
        {
            temp = rk[7];
            rk[8] = rk[0] ^ TE421(temp) ^ TE432(temp) ^
                TE443(temp) ^ TE414(temp) ^ RCON(i);
            rk[9] = rk[1] ^ rk[8];
            rk[10] = rk[2] ^ rk[9];
            rk[11] = rk[3] ^ rk[10];
            if (i == 6)
                return 14;
            temp = rk[11];
            rk[12] = rk[4] ^ TE411(temp) ^ TE422(temp) ^
                TE433(temp) ^ TE444(temp);
            rk[13] = rk[5] ^ rk[12];
            rk[14] = rk[6] ^ rk[13];
            rk[15] = rk[7] ^ rk[14];
            rk += 8;
        }
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

void CGXCipher::AesEncrypt(
    const unsigned int* rk,
    unsigned int Nr,
    const unsigned char* pt,
    unsigned char* ct)
{
    unsigned long s0, s1, s2, s3, t0, t1, t2, t3;
    int r;
    s0 = GETU32(pt) ^ rk[0];
    s1 = GETU32(pt + 4) ^ rk[1];
    s2 = GETU32(pt + 8) ^ rk[2];
    s3 = GETU32(pt + 12) ^ rk[3];

#define ROUND(i,d,s) \
d##0 = TE0(s##0) ^ TE1(s##1) ^ TE2(s##2) ^ TE3(s##3) ^ rk[4 * i]; \
d##1 = TE0(s##1) ^ TE1(s##2) ^ TE2(s##3) ^ TE3(s##0) ^ rk[4 * i + 1]; \
d##2 = TE0(s##2) ^ TE1(s##3) ^ TE2(s##0) ^ TE3(s##1) ^ rk[4 * i + 2]; \
d##3 = TE0(s##3) ^ TE1(s##0) ^ TE2(s##1) ^ TE3(s##2) ^ rk[4 * i + 3]

    /* Nr - 1 full rounds: */
    r = Nr >> 1;
    for (;;)
    {
        ROUND(1, t, s);
        rk += 8;
        if (--r == 0)
            break;
        ROUND(0, s, t);
    }

#undef ROUND

    s0 = TE41(t0) ^ TE42(t1) ^ TE43(t2) ^ TE44(t3) ^ rk[0];
    PUT32(ct, s0);
    s1 = TE41(t1) ^ TE42(t2) ^ TE43(t3) ^ TE44(t0) ^ rk[1];
    PUT32(ct + 4, s1);
    s2 = TE41(t2) ^ TE42(t3) ^ TE43(t0) ^ TE44(t1) ^ rk[2];
    PUT32(ct + 8, s2);
    s3 = TE41(t3) ^ TE42(t0) ^ TE43(t1) ^ TE44(t2) ^ rk[3];
    PUT32(ct + 12, s3);
}

void CGXCipher::Xor(
    unsigned char* dst,
    const unsigned char* src)
{
    uint32_t* d = (uint32_t*)dst;
    uint32_t* s = (uint32_t*)src;
    *d++ ^= *s++;
    *d++ ^= *s++;
    *d++ ^= *s++;
    *d++ ^= *s++;
}

void CGXCipher::shift_right_block(unsigned char* v)
{
    unsigned long val = GETU32(v + 12);
    val >>= 1;
    if (v[11] & 0x01)
    {
        val |= 0x80000000;
    }
    PUT32(v + 12, val);

    val = GETU32(v + 8);
    val >>= 1;
    if (v[7] & 0x01)
        val |= 0x80000000;
    PUT32(v + 8, val);

    val = GETU32(v + 4);
    val >>= 1;
    if (v[3] & 0x01)
        val |= 0x80000000;
    PUT32(v + 4, val);

    val = GETU32(v);
    val >>= 1;
    PUT32(v, val);
}

void CGXCipher::MultiplyH(const unsigned char* x, const unsigned char* y, unsigned char* z)
{
    unsigned char tmp[16];
    int i, j;

    memset(z, 0, 16);
    memcpy(tmp, y, 16);

    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (x[i] & 1 << (7 - j))
            {
                Xor(z, tmp);
            }
            //If last bit.
            if (tmp[15] & 0x01)
            {
                shift_right_block(tmp);
                tmp[0] ^= 0xe1;
            }
            else
            {
                shift_right_block(tmp);
            }
        }
    }
}

void CGXCipher::GetGHash(
    const unsigned char* h,
    const unsigned char* x,
    int xlen,
    unsigned char* y)
{
    int m, i;
    const unsigned char* xpos = x;
    unsigned char tmp[16];
    m = xlen / 16;
    for (i = 0; i < m; i++)
    {
        Xor(y, xpos);
        xpos += 16;
        MultiplyH(y, h, tmp);
        memcpy(y, tmp, 16);
    }
    if (x + xlen > xpos)
    {
        size_t last = x + xlen - xpos;
        memcpy(tmp, xpos, last);
        memset(tmp + last, 0, sizeof(tmp) - last);

        Xor(y, tmp);

        MultiplyH(y, h, tmp);
        memcpy(y, tmp, 16);
    }
}

void CGXCipher::Init_j0(
    const unsigned char* iv,
    unsigned char len,
    const unsigned char* H,
    unsigned char* J0)
{
    unsigned char tmp[16];
    //IV length.
    if (len == 12)
    {
        memcpy(J0, iv, len);
        memset(J0 + len, 0, 16 - len);
        J0[15] = 0x01;
    }
    else
    {
        memset(J0, 0, 16);
        GetGHash(H, iv, len, J0);
        PUT32(tmp, (unsigned long)0);
        PUT32(tmp + 4, (unsigned long)0);
        //Here is expected that data is newer longger than 32 bit.
        //This is done because microcontrollers show warning here.
        PUT32(tmp + 8, (unsigned long)0);
        PUT32(tmp + 12, (unsigned long)(len * 8));
        GetGHash(H, tmp, sizeof(tmp), J0);
    }
}

void CGXCipher::Inc32(unsigned char* block)
{
    unsigned long val;
    val = GETU32(block + 16 - 4);
    val++;
    PUT32(block + 16 - 4, val);
}

void CGXCipher::Gctr(unsigned int* aes, const unsigned char* icb, unsigned char* in, int len, unsigned char* out)
{
    size_t i, n, last;
    unsigned char cb[16], tmp[16] = { 0 };
    unsigned char* pin = in;
    unsigned char* pout = out;
    if (len == 0)
    {
        return;
    }
    n = len / 16;
    memcpy(cb, icb, 16);

    //Full blocks.
    for (i = 0; i < n; i++)
    {

        if (out == NULL)
        {
            AesEncrypt(aes, aes[60], cb, tmp);
            Xor(pin, tmp);
        }
        else
        {
            AesEncrypt(aes, aes[60], cb, pout);
            Xor(pout, pin);
            pout += 16;
        }
        pin += 16;
        Inc32(cb);
    }
    last = in + len - pin;
    //Last, partial block.
    if (last)
    {
        AesEncrypt(aes, aes[60], cb, tmp);
        for (i = 0; i < last; i++)
        {
            if (out == NULL)
            {
                *pin ^= tmp[i];
                ++pin;
            }
            else
            {
                *pout = *pin++ ^ tmp[i];
                ++pout;
            }
        }
    }
}

void CGXCipher::AesGcmGctr(unsigned int* aes, const unsigned char* J0, unsigned char* in, int len, unsigned char* out)
{
    unsigned char J0inc[16];
    if (len == 0)
    {
        return;
    }

    memcpy(J0inc, J0, 16);
    Inc32(J0inc);
    Gctr(aes, J0inc, in, len, out);
}

void CGXCipher::AesGcmGhash(const unsigned char* H, const unsigned char* aad, int aad_len,
    const unsigned char* crypt, int crypt_len, unsigned char* S)
{
    unsigned char len_buf[16];
    GetGHash(H, aad, aad_len, S);
    GetGHash(H, crypt, crypt_len, S);
    //Here is expected that data is newer longger than 32 bit.
    //This is done because microcontrollers show warning here.
    PUT32(len_buf, (unsigned long)0);
    PUT32(len_buf + 4, (unsigned long)(aad_len * 8));
    PUT32(len_buf + 8, (unsigned long)0);
    PUT32(len_buf + 12, (unsigned long)(crypt_len * 8));
    GetGHash(H, len_buf, sizeof(len_buf), S);
}

int CGXCipher::Encrypt(
    DLMS_SECURITY security,
    DLMS_COUNT_TYPE type,
    unsigned long frameCounter,
    unsigned char tag,
    CGXByteBuffer& systemTitle,
    CGXByteBuffer& key,
    CGXByteBuffer& input,
    bool encrypt)
{
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)//If Windows or Linux
    //    printf("System title: %s\r\n", systemTitle.ToHexString().c_str());
    //    printf("key: %s\r\n", key.ToHexString().c_str());
    //    printf("Authentication Key: %s\r\n", m_AuthenticationKey.ToHexString().c_str());
#endif //defined(_WIN32) || defined(_WIN64) || defined(__linux__)//If Windows or Linux
    int ret;
    uint32_t aes[61] = { 0 };
    unsigned char H[16] = { 0 };
    unsigned char J0[16] = { 0 };
    unsigned char S[16] = { 0 };
    CGXByteBuffer nonse;
    CGXByteBuffer aad;
    if (systemTitle.GetSize() != 8)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((ret = GetNonse(frameCounter, systemTitle, nonse)) != 0)
    {
        return ret;
    }
    if ((ret = Int(aes, key.m_Data, 16 * 8)) != 0)
    {
        return ret;
    }
    aes[60] = 10;
    //Hash subkey.
    AesEncrypt(aes, aes[60], H, H);
    Init_j0(nonse.m_Data, (unsigned char)nonse.GetSize(), H, J0);

    //Allocate space for authentication tag.
    if (security != DLMS_SECURITY_ENCRYPTION && !encrypt)
    {
        //Save authentication key to nonse.
        nonse.Clear();
        nonse.Set(input.GetData() + input.GetSize() - 12, 12);
        input.SetSize(input.GetSize() - 12);
    }
    if (security == DLMS_SECURITY_AUTHENTICATION)
    {
        input.Move(input.m_Position, 17, input.Available());
        input.m_Position = 0;
        input.SetUInt8(0, security);
        memcpy(input.m_Data + 1, m_AuthenticationKey.m_Data, 16);
        AesGcmGhash(H, input.m_Data, input.m_Size, input.m_Data, 0, S);
        if (type == DLMS_COUNT_TYPE_TAG)
        {
            input.m_Size = 0;
        }
        else
        {
            input.Move(17, 0, input.m_Size - 17);
        }
        Gctr(aes, J0, S, sizeof(S), input.m_Data + input.m_Size);
        if (encrypt)
        {
            input.m_Size += 12;
        }
        else
        {
            if (memcmp(nonse.m_Data, input.m_Data + input.m_Size, 12) != 0)
            {
                ret = DLMS_ERROR_CODE_INVALID_TAG;
            }
        }
    }
    else if (security == DLMS_SECURITY_ENCRYPTION)
    {
        //Encrypt the data.
        AesGcmGctr(aes, J0, input.m_Data + input.GetPosition(), input.Available(), NULL);
    }
    else if (security == DLMS_SECURITY_AUTHENTICATION_ENCRYPTION)
    {
        if (encrypt)
        {
            //Encrypt the data.
            AesGcmGctr(aes, J0, input.m_Data + input.m_Position, input.Available(), NULL);
        }
        //Count authentication.
        input.Move(input.m_Position, 17, input.Available());
        input.m_Position = 0;
        input.SetUInt8(0, security);
        memcpy(input.m_Data + 1, m_AuthenticationKey.m_Data, 16);
        AesGcmGhash(H, input.m_Data, 17, input.m_Data + 17, input.m_Size - 17, S);
        input.Move(17, 0, input.m_Size - 17);
        Gctr(aes, J0, S, sizeof(S), input.m_Data + input.m_Size);
        if (!encrypt)
        {
            //Decrypt the data.
            AesGcmGctr(aes, J0, input.m_Data + input.m_Position, input.Available(), NULL);
        }
        Gctr(aes, J0, S, sizeof(S), input.m_Data + input.m_Size);
        if (encrypt)
        {
            input.m_Size += 12;
        }
        else
        {
            if (memcmp(nonse.m_Data, input.m_Data + input.m_Size, 12) != 0)
            {
                ret = DLMS_ERROR_CODE_INVALID_TAG;
            }
        }
    }
    if (encrypt)
    {
        ++m_FrameCounter;
    }
    if (encrypt && type == DLMS_COUNT_TYPE_PACKET)
    {
        nonse.Clear();
        if ((ret = nonse.SetUInt8(tag)) == 0)
        {
            if (tag == DLMS_COMMAND_GENERAL_GLO_CIPHERING ||
                tag == DLMS_COMMAND_GENERAL_DED_CIPHERING)
            {
                GXHelpers::SetObjectCount(8, nonse);
                nonse.Set(systemTitle.GetData(), 8);
            }
            GXHelpers::SetObjectCount(5 + input.GetSize(), nonse);
            if ((ret = nonse.SetUInt8(security)) == 0 &&
                (ret = nonse.SetUInt32(frameCounter)) == 0)
            {
                input.Move(0, nonse.GetSize(), input.GetSize());
                memcpy(input.m_Data, nonse.m_Data, nonse.GetSize());
            }
        }
    }
    return 0;
}

int CGXCipher::Decrypt(
    CGXByteBuffer& title,
    CGXByteBuffer& key,
    CGXByteBuffer& data,
    DLMS_SECURITY& security,
    DLMS_SECURITY_SUITE& suite)
{
    unsigned long length;
    int ret;
    unsigned char ch;
    unsigned long frameCounter;
    DLMS_COMMAND cmd;
    CGXByteBuffer* pTitle = &title;
    CGXByteBuffer systemTitle, countTag;
    if (data.GetSize() - data.GetPosition() < 2)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    cmd = (DLMS_COMMAND)ch;
    switch (cmd)
    {
    case DLMS_COMMAND_GENERAL_GLO_CIPHERING:
    case DLMS_COMMAND_GENERAL_DED_CIPHERING:
        if ((ret = GXHelpers::GetObjectCount(data, length)) != 0)
        {
            return ret;
        }
        if (length != 0)
        {
            systemTitle.Set(&data, data.m_Position, length);
            pTitle = &systemTitle;
        }
        break;
    case DLMS_COMMAND_GLO_INITIATE_REQUEST:
    case DLMS_COMMAND_GLO_INITIATE_RESPONSE:
    case DLMS_COMMAND_GLO_READ_REQUEST:
    case DLMS_COMMAND_GLO_READ_RESPONSE:
    case DLMS_COMMAND_GLO_WRITE_REQUEST:
    case DLMS_COMMAND_GLO_WRITE_RESPONSE:
    case DLMS_COMMAND_GLO_GET_REQUEST:
    case DLMS_COMMAND_GLO_GET_RESPONSE:
    case DLMS_COMMAND_GLO_SET_REQUEST:
    case DLMS_COMMAND_GLO_SET_RESPONSE:
    case DLMS_COMMAND_GLO_METHOD_REQUEST:
    case DLMS_COMMAND_GLO_METHOD_RESPONSE:
    case DLMS_COMMAND_GLO_EVENT_NOTIFICATION_REQUEST:
    case DLMS_COMMAND_DED_GET_REQUEST:
    case DLMS_COMMAND_DED_GET_RESPONSE:
    case DLMS_COMMAND_DED_SET_REQUEST:
    case DLMS_COMMAND_DED_SET_RESPONSE:
    case DLMS_COMMAND_DED_METHOD_REQUEST:
    case DLMS_COMMAND_DED_METHOD_RESPONSE:
    case DLMS_COMMAND_DED_EVENT_NOTIFICATION:
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((ret = GXHelpers::GetObjectCount(data, length)) != 0)
    {
        return ret;
    }
    if (data.Available() < length)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    security = (DLMS_SECURITY)(ch & 0x30);
    suite = (DLMS_SECURITY_SUITE)(ch & 0x3);
    if ((ret = data.GetUInt32(&frameCounter)) != 0)
    {
        return ret;
    }
    if ((ret = Encrypt(
        security,
        DLMS_COUNT_TYPE_DATA,
        frameCounter,
        0,
        *pTitle,
        key,
        data,
        false)) != 0)
    {
        return ret;
    }
    return 0;
}

/**
* multiply by 2 in the Galois field
*
* @param value
*            value to multiply.
* @return Value multiply by 2.
*/
static unsigned char GaloisMultiply(unsigned char value)
{
    if (value >> 7 != 0)
    {
        value = (value << 1);
        return (value ^ 0x1b);
    }
    else
    {
        return (value << 1);
    }
}

int CGXCipher::Aes1Encrypt(
    CGXByteBuffer& buff,
    unsigned short offset,
    CGXByteBuffer& secret)
{
    unsigned char buf1, buf2, buf3, buf4, round, i;
    unsigned char* key = secret.m_Data;
    unsigned char* data = buff.m_Data;
    for (round = 0; round < 10; ++round)
    {
        for (i = 0; i < 16; ++i)
        {
            data[i + offset] = S_BOX[(data[i + offset] ^ key[i]) & 0xFF];
        }
        // shift rows
        buf1 = data[1 + offset];
        data[1 + offset] = data[5 + offset];
        data[5 + offset] = data[9 + offset];
        data[9 + offset] = data[13 + offset];
        data[13 + offset] = buf1;

        buf1 = data[2 + offset];
        buf2 = data[6 + offset];
        data[2 + offset] = data[10 + offset];
        data[6 + offset] = data[14 + offset];
        data[10 + offset] = buf1;
        data[14 + offset] = buf2;

        buf1 = data[15 + offset];
        data[15 + offset] = data[11 + offset];
        data[11 + offset] = data[7 + offset];
        data[7 + offset] = data[3 + offset];
        data[3 + offset] = buf1;

        if (round < 9)
        {
            for (i = 0; i < 4; i++)
            {
                buf4 = (i << 2);
                buf1 = (data[buf4 + offset] ^ data[buf4 + 1 + offset]
                    ^ data[buf4 + 2 + offset]
                    ^ data[buf4 + 3 + offset]);
                buf2 = data[buf4 + offset];
                buf3 = (data[buf4 + offset]
                    ^ data[buf4 + 1 + offset]);
                buf3 = GaloisMultiply(buf3);
                data[buf4 + offset] =
                    (data[buf4 + offset] ^ buf3 ^ buf1);
                buf3 = (data[buf4 + 1 + offset]
                    ^ data[buf4 + 2 + offset]);
                buf3 = GaloisMultiply(buf3);
                data[buf4 + 1 + offset] =
                    (data[buf4 + 1 + offset] ^ buf3 ^ buf1);
                buf3 = (data[buf4 + 2 + offset]
                    ^ data[buf4 + 3 + offset]);
                buf3 = GaloisMultiply(buf3);
                data[buf4 + 2 + offset] = (data[buf4 + 2 + offset] ^ buf3 ^ buf1);
                buf3 = (data[buf4 + 3 + offset] ^ buf2);
                buf3 = GaloisMultiply(buf3);
                data[buf4 + 3 + offset] = (data[buf4 + 3 + offset] ^ buf3 ^ buf1);
            }
        }

        key[0] = (S_BOX[key[13] & 0xFF] ^ key[0] ^ R_CON[round]);
        key[1] = (S_BOX[key[14] & 0xFF] ^ key[1]);
        key[2] = (S_BOX[key[15] & 0xFF] ^ key[2]);
        key[3] = (S_BOX[key[12] & 0xFF] ^ key[3]);
        for (i = 4; i < 16; i++)
        {
            key[i] = (key[i] ^ key[i - 4]);
        }
    }

    for (i = 0; i < 16; i++)
    {
        data[i + offset] = (data[i + offset] ^ key[i]);
    }
    return 0;
}

int CGXCipher::Aes1Decrypt(
    CGXByteBuffer& buff,
    CGXByteBuffer& secret)
{
    unsigned char buf1, buf2, buf3, round, i;
    int buf4;
    CGXByteBuffer key2;
    key2.Set(&secret, 0, secret.GetSize());
    unsigned char* data = buff.GetData();
    unsigned char* key = key2.GetData();
    for (round = 0; round < 10; round++) {
        key[0] = (S_BOX[key[13] & 0xFF] ^ key[0] ^ R_CON[round]);
        key[1] = (S_BOX[key[14] & 0xFF] ^ key[1]);
        key[2] = (S_BOX[key[15] & 0xFF] ^ key[2]);
        key[3] = (S_BOX[key[12] & 0xFF] ^ key[3]);
        for (i = 4; i < 16; i++) {
            key[i] = (key[i] ^ key[i - 4]);
        }
    }

    for (i = 0; i < 16; i++) {
        data[i] = (data[i] ^ key[i]);
    }

    for (round = 0; round < 10; ++round) {
        for (i = 15; i > 3; --i) {
            key[i] = (key[i] ^ key[i - 4]);
        }
        key[0] = (S_BOX[key[13] & 0xFF] ^ key[0] ^ R_CON[9 - round]);
        key[1] = (S_BOX[key[14] & 0xFF] ^ key[1]);
        key[2] = (S_BOX[key[15] & 0xFF] ^ key[2]);
        key[3] = (S_BOX[key[12] & 0xFF] ^ key[3]);

        if (round > 0) {
            for (i = 0; i < 4; i++) {
                buf4 = (i << 2) & 0xFF;

                buf1 = GaloisMultiply(
                    GaloisMultiply(data[buf4] ^ data[buf4 + 2]));
                buf2 = GaloisMultiply(
                    GaloisMultiply(data[buf4 + 1] ^ data[buf4 + 3]));
                data[buf4] ^= buf1;
                data[buf4 + 1] ^= buf2;
                data[buf4 + 2] ^= buf1;
                data[buf4 + 3] ^= buf2;

                buf1 = (data[buf4] ^ data[buf4 + 1] ^ data[buf4 + 2] ^ data[buf4 + 3]);
                buf2 = data[buf4];
                buf3 = (data[buf4] ^ data[buf4 + 1]);
                buf3 = GaloisMultiply(buf3);
                data[buf4] = (data[buf4] ^ buf3 ^ buf1);
                buf3 = (data[buf4 + 1] ^ data[buf4 + 2]);
                buf3 = GaloisMultiply(buf3);
                data[buf4 + 1] = (data[buf4 + 1] ^ buf3 ^ buf1);
                buf3 = (data[buf4 + 2] ^ data[buf4 + 3]);
                buf3 = GaloisMultiply(buf3);
                data[buf4 + 2] = (data[buf4 + 2] ^ buf3 ^ buf1);
                buf3 = (data[buf4 + 3] ^ buf2);
                buf3 = GaloisMultiply(buf3);
                data[buf4 + 3] = (data[buf4 + 3] ^ buf3 ^ buf1);
            }
        }
        // Row 1
        buf1 = data[13];
        data[13] = data[9];
        data[9] = data[5];
        data[5] = data[1];
        data[1] = buf1;
        // Row 2
        buf1 = data[10];
        buf2 = data[14];
        data[10] = data[2];
        data[14] = data[6];
        data[2] = buf1;
        data[6] = buf2;
        // Row 3
        buf1 = data[3];
        data[3] = data[7];
        data[7] = data[11];
        data[11] = data[15];
        data[15] = buf1;

        for (i = 0; i < 16; i++) {
            data[i] = (S_BOX_REVERSED[data[i] & 0xFF] ^ key[i]);
        }
    }
    return 0;
}

bool CGXCipher::IsCiphered()
{
    return m_Security != DLMS_SECURITY_NONE;
}

DLMS_SECURITY CGXCipher::GetSecurity()
{
    return m_Security;
}

void CGXCipher::SetSecurity(DLMS_SECURITY value)
{
    m_Security = value;
}

/**
    * @return Used security suite.
    */
DLMS_SECURITY_SUITE CGXCipher::GetSecuritySuite()
{
    return m_SecuritySuite;
}

/**
 * @param value
 *            Used security suite.
 */
void CGXCipher::SetSecuritySuite(DLMS_SECURITY_SUITE value)
{
    m_SecuritySuite = value;
}

CGXByteBuffer& CGXCipher::GetSystemTitle()
{
    return m_SystemTitle;
}

void CGXCipher::SetSystemTitle(CGXByteBuffer& value)
{
    m_SystemTitle.Clear();
    m_SystemTitle.Set(value.m_Data, value.m_Size - value.m_Position);
}

CGXByteBuffer& CGXCipher::GetBlockCipherKey()
{
    return m_BlockCipherKey;
}

void CGXCipher::SetBlockCipherKey(CGXByteBuffer& value)
{
    m_BlockCipherKey.Clear();
    m_BlockCipherKey.Set(value.m_Data, value.m_Size - value.m_Position);
}

CGXByteBuffer& CGXCipher::GetAuthenticationKey()
{
    return m_AuthenticationKey;
}

void CGXCipher::SetAuthenticationKey(CGXByteBuffer& value)
{
    m_AuthenticationKey.Clear();
    m_AuthenticationKey.Set(value.m_Data, value.m_Size - value.m_Position);
}

unsigned long CGXCipher::GetFrameCounter()
{
    return m_FrameCounter;
}

void CGXCipher::SetFrameCounter(unsigned long value)
{
    m_FrameCounter = value;
}

unsigned long CGXCipher::GetInvocationCounter()
{
    return m_FrameCounter;
}

void CGXCipher::SetInvocationCounter(unsigned long value)
{
    m_FrameCounter = value;
}

void CGXCipher::Reset()
{

}

CGXByteBuffer& CGXCipher::GetDedicatedKey()
{
    return m_DedicatedKey;
}

void CGXCipher::SetDedicatedKey(CGXByteBuffer& value)
{
    m_DedicatedKey = value;
}
