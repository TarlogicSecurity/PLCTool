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

#ifndef GXDLMSSHA256_H
#define GXDLMSSHA256_H

#include "GXBytebuffer.h"

//This class is used to handle SHA-256.
class CGXDLMSSha256
{
private:
    static void Transform(unsigned int *h, const unsigned char *message, unsigned int block_nb);
    static int Final(unsigned int *h, unsigned char *block, unsigned char *digest, unsigned int len, unsigned int totalLen);
    static int Update(unsigned int *h, unsigned char *block, CGXByteBuffer&  data, unsigned int *len, unsigned int *totalLen);
public:
    static int Encrypt(CGXByteBuffer& data, CGXByteBuffer& crypted);
};
#endif //GXDLMSSHA256_H