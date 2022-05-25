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

#include "../include/GXDLMSMd5.h"
unsigned int gxmd5_F(unsigned int x, unsigned int y, unsigned int z)
{
    return (x & y) | (~x & z);
}

unsigned int gxmd5_G(unsigned int x, unsigned int y, unsigned int z)
{
    return (x & z) | (y & ~z);
}

unsigned int gxmd5_H(unsigned int x, unsigned int y, unsigned int z)
{
    return x ^ y ^ z;
}

unsigned int gxmd5_I(unsigned int x, unsigned int y, unsigned int z)
{
    return y ^ (x | ~z);
}

unsigned int gxmd5_rotate_left(unsigned int x, int n)
{
    return (x << n) | (x >> (32 - n));
}

void gxmd5_FF(unsigned int *a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac)
{
    *a = gxmd5_rotate_left(*a + gxmd5_F(b, c, d) + x + ac, s) + b;
}

void gxmd5_GG(unsigned int *a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac) {
    *a = gxmd5_rotate_left(*a + gxmd5_G(b, c, d) + x + ac, s) + b;
}

void gxmd5_HH(unsigned int *a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac) {
    *a = gxmd5_rotate_left(*a + gxmd5_H(b, c, d) + x + ac, s) + b;
}

void gxmd5_II(unsigned int *a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac) {
    *a = gxmd5_rotate_left(*a + gxmd5_I(b, c, d) + x + ac, s) + b;
}

void gxmd5_decode(unsigned int * output, unsigned char *input, unsigned int len)
{
    for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
    {
        output[i] = (input[j]) | (((unsigned int)input[j + 1]) << 8) |
            (((unsigned int)input[j + 2]) << 16) | (((unsigned int)input[j + 3]) << 24);
    }
}

void CGXDLMSMD5::Encode(unsigned char * output, unsigned int *input, unsigned int len)
{
    unsigned int pos = 0;
    for (unsigned int i = 0; i != len; ++i)
    {
        output[pos] = input[i] & 0xff;
        ++pos;
        output[pos] = (input[i] >> 8) & 0xff;
        ++pos;
        output[pos] = (input[i] >> 16) & 0xff;
        ++pos;
        output[pos] = (input[i] >> 24) & 0xff;
        ++pos;
    }
}

void CGXDLMSMD5::Transform(unsigned char* block, unsigned int *state)
{
    unsigned int a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    gxmd5_decode(x, block, 64);

    /* Round 1 */
    gxmd5_FF(&a, b, c, d, x[0], S11, 0xd76aa478);
    gxmd5_FF(&d, a, b, c, x[1], S12, 0xe8c7b756);
    gxmd5_FF(&c, d, a, b, x[2], S13, 0x242070db);
    gxmd5_FF(&b, c, d, a, x[3], S14, 0xc1bdceee);
    gxmd5_FF(&a, b, c, d, x[4], S11, 0xf57c0faf);
    gxmd5_FF(&d, a, b, c, x[5], S12, 0x4787c62a);
    gxmd5_FF(&c, d, a, b, x[6], S13, 0xa8304613);
    gxmd5_FF(&b, c, d, a, x[7], S14, 0xfd469501);
    gxmd5_FF(&a, b, c, d, x[8], S11, 0x698098d8);
    gxmd5_FF(&d, a, b, c, x[9], S12, 0x8b44f7af);
    gxmd5_FF(&c, d, a, b, x[10], S13, 0xffff5bb1);
    gxmd5_FF(&b, c, d, a, x[11], S14, 0x895cd7be);
    gxmd5_FF(&a, b, c, d, x[12], S11, 0x6b901122);
    gxmd5_FF(&d, a, b, c, x[13], S12, 0xfd987193);
    gxmd5_FF(&c, d, a, b, x[14], S13, 0xa679438e);
    gxmd5_FF(&b, c, d, a, x[15], S14, 0x49b40821);

    /* Round 2 */
    gxmd5_GG(&a, b, c, d, x[1], S21, 0xf61e2562);
    gxmd5_GG(&d, a, b, c, x[6], S22, 0xc040b340);
    gxmd5_GG(&c, d, a, b, x[11], S23, 0x265e5a51);
    gxmd5_GG(&b, c, d, a, x[0], S24, 0xe9b6c7aa);
    gxmd5_GG(&a, b, c, d, x[5], S21, 0xd62f105d);
    gxmd5_GG(&d, a, b, c, x[10], S22, 0x2441453);
    gxmd5_GG(&c, d, a, b, x[15], S23, 0xd8a1e681);
    gxmd5_GG(&b, c, d, a, x[4], S24, 0xe7d3fbc8);
    gxmd5_GG(&a, b, c, d, x[9], S21, 0x21e1cde6);
    gxmd5_GG(&d, a, b, c, x[14], S22, 0xc33707d6);
    gxmd5_GG(&c, d, a, b, x[3], S23, 0xf4d50d87);
    gxmd5_GG(&b, c, d, a, x[8], S24, 0x455a14ed);
    gxmd5_GG(&a, b, c, d, x[13], S21, 0xa9e3e905);
    gxmd5_GG(&d, a, b, c, x[2], S22, 0xfcefa3f8);
    gxmd5_GG(&c, d, a, b, x[7], S23, 0x676f02d9);
    gxmd5_GG(&b, c, d, a, x[12], S24, 0x8d2a4c8a);

    /* Round 3 */
    gxmd5_HH(&a, b, c, d, x[5], S31, 0xfffa3942);
    gxmd5_HH(&d, a, b, c, x[8], S32, 0x8771f681);
    gxmd5_HH(&c, d, a, b, x[11], S33, 0x6d9d6122);
    gxmd5_HH(&b, c, d, a, x[14], S34, 0xfde5380c);
    gxmd5_HH(&a, b, c, d, x[1], S31, 0xa4beea44);
    gxmd5_HH(&d, a, b, c, x[4], S32, 0x4bdecfa9);
    gxmd5_HH(&c, d, a, b, x[7], S33, 0xf6bb4b60);
    gxmd5_HH(&b, c, d, a, x[10], S34, 0xbebfbc70);
    gxmd5_HH(&a, b, c, d, x[13], S31, 0x289b7ec6);
    gxmd5_HH(&d, a, b, c, x[0], S32, 0xeaa127fa);
    gxmd5_HH(&c, d, a, b, x[3], S33, 0xd4ef3085);
    gxmd5_HH(&b, c, d, a, x[6], S34, 0x4881d05);
    gxmd5_HH(&a, b, c, d, x[9], S31, 0xd9d4d039);
    gxmd5_HH(&d, a, b, c, x[12], S32, 0xe6db99e5);
    gxmd5_HH(&c, d, a, b, x[15], S33, 0x1fa27cf8);
    gxmd5_HH(&b, c, d, a, x[2], S34, 0xc4ac5665);

    /* Round 4 */
    gxmd5_II(&a, b, c, d, x[0], S41, 0xf4292244);
    gxmd5_II(&d, a, b, c, x[7], S42, 0x432aff97);
    gxmd5_II(&c, d, a, b, x[14], S43, 0xab9423a7);
    gxmd5_II(&b, c, d, a, x[5], S44, 0xfc93a039);
    gxmd5_II(&a, b, c, d, x[12], S41, 0x655b59c3);
    gxmd5_II(&d, a, b, c, x[3], S42, 0x8f0ccc92);
    gxmd5_II(&c, d, a, b, x[10], S43, 0xffeff47d);
    gxmd5_II(&b, c, d, a, x[1], S44, 0x85845dd1);
    gxmd5_II(&a, b, c, d, x[8], S41, 0x6fa87e4f);
    gxmd5_II(&d, a, b, c, x[15], S42, 0xfe2ce6e0);
    gxmd5_II(&c, d, a, b, x[6], S43, 0xa3014314);
    gxmd5_II(&b, c, d, a, x[13], S44, 0x4e0811a1);
    gxmd5_II(&a, b, c, d, x[4], S41, 0xf7537e82);
    gxmd5_II(&d, a, b, c, x[11], S42, 0xbd3af235);
    gxmd5_II(&c, d, a, b, x[2], S43, 0x2ad7d2bb);
    gxmd5_II(&b, c, d, a, x[9], S44, 0xeb86d391);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

int CGXDLMSMD5::Update(unsigned char* data, unsigned int len, unsigned char* buffer, unsigned int* count, unsigned int* state)
{
    unsigned int i;
    // Number of bytes.
    unsigned int index = count[0] / 8 % 64;

    // Update number of bits
    if ((count[0] += (len << 3)) < (len << 3))
    {
        count[1]++;
    }
    count[1] += (len >> 29);

    // number of bytes we need to fill in buffer
    unsigned int firstpart = 64 - index;


    // transform as many times as possible.
    if (len >= firstpart)
    {
        memcpy(&buffer[index], data, firstpart);
        Transform(buffer, state);

        // Transform block
        for (i = firstpart; i + 64 <= len; i += 64)
        {
            Transform(data + i, state);
        }
        index = 0;
    }
    else
    {
        i = 0;
    }

    memcpy(&buffer[index], &data[i], len - i);
    return 0;
}

int CGXDLMSMD5::Encrypt(CGXByteBuffer& data, CGXByteBuffer& digest)
{
    // Bytes that didn't fit in last 64 byte chunk
    unsigned char buffer[64];
    // Number of bits (lo, hi)
    unsigned long count[2] = { 0, 0 };
    // Digest
    unsigned int state[4] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };
    digest.Capacity(16);

    Update(data.GetData(), data.GetSize(), buffer, (unsigned int*)&count, (unsigned int*)&state);

    static unsigned char padding[64] = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    // Save number of bits
    unsigned char bits[8];
    Encode(bits, (unsigned int*)&count, 2);

    // Pad out to 56 mod 64.
    unsigned int index = count[0] / 8 % 64;
    unsigned int padLen = (index < 56) ? (56 - index) : (120 - index);
    Update(padding, padLen, buffer, (unsigned int*)&count, (unsigned int*)&state);

    // Append length (before padding)
    Update(bits, 8, buffer, (unsigned int*)&count, (unsigned int*)&state);

    // Store state in digest
    Encode(digest.GetData(), state, 4);
    digest.SetSize(16);
    return 0;
}

