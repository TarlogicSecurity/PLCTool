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

#ifndef GXDLMSSHA1_H
#define GXDLMSSHA1_H

#include "GXBytebuffer.h"

//This class is used to handle SHA-1.
class CGXDLMSSha1
{
private:
    static void Transform(unsigned long *block, unsigned int *digest, unsigned int *transforms);
    static void Update(CGXByteBuffer& data, unsigned int *digest, unsigned int *transforms);
    static int Final(CGXByteBuffer& data, unsigned int *digest, unsigned int *transforms, CGXByteBuffer& reply);

public:
    static int Encrypt(CGXByteBuffer& data, CGXByteBuffer& crypted);
};
#endif //GXDLMSSHA1_H