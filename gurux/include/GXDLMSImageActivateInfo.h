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

#ifndef GXDLMSIMAGEACTIVATEINFO_H
#define GXDLMSIMAGEACTIVATEINFO_H

#include "GXBytebuffer.h"

class CGXDLMSImageActivateInfo
{
    long m_Size;
    CGXByteBuffer m_Identification;
    CGXByteBuffer m_Signature;
public:
    /**
     * Constructor.
     */
    CGXDLMSImageActivateInfo();

    /**
     * Constructor.
     */
    CGXDLMSImageActivateInfo(long size, CGXByteBuffer& identification, CGXByteBuffer& signature);

    /**
     *  Image_size is the size of the Image(s) to be activated.
     *  Expressed in octets;
    */
    long GetSize();
    void SetSize(long value);

    /**
     * Image identification is the identification of the Image(s)
     * to be activated, and may contain information like
     * manufacturer, device type, version information, etc.
    */
    CGXByteBuffer& GetIdentification();
    void SetIdentification(CGXByteBuffer& value);

    /**
     * Image signature is the signature of the Image(s) to be activated.
    */
    CGXByteBuffer& GetSignature();
    void SetSignature(CGXByteBuffer& value);

    std::string ToString();
};
#endif //GXDLMSIMAGEACTIVATEINFO_H