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

#ifndef GXDLMSLIMITS_H
#define GXDLMSLIMITS_H

#include "GXDLMSVariant.h"

class CGXDLMSLimits
{
    unsigned short m_MaxInfoTX, m_MaxInfoRX;
    unsigned char m_WindowSizeRX, m_WindowSizeTX;
public:
    static const unsigned short DEFAULT_MAX_INFO_RX = 128;
    static const unsigned short DEFAULT_MAX_INFO_TX = 128;
    static const unsigned char DEFAULT_WINDOWS_SIZE_RX = 1;
    static const unsigned char DEFAULT_WINDOWS_SIZE_TX = 1;
    //Constructor.
    CGXDLMSLimits();

    // The maximum information field length in transmit.
    // DefaultValue is 128.
    unsigned short GetMaxInfoTX();

    void SetMaxInfoTX(unsigned short value);

    /// The maximum information field length in receive.
    /// DefaultValue is 62.
    unsigned short GetMaxInfoRX();

    void SetMaxInfoRX(unsigned short value);

    /// The window size in transmit.
    /// DefaultValue is 1.
    unsigned char GetWindowSizeTX();

    void SetWindowSizeTX(unsigned char value);

    /// The window size in receive.
    /// DefaultValue is 1.
    unsigned char GetWindowSizeRX();

    void SetWindowSizeRX(unsigned char value);
};
#endif //GXDLMSLIMITS_H