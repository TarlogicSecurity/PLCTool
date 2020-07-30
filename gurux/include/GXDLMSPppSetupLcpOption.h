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

#ifndef GXDLMSPPPSETUPLCPOPTION_H
#define GXDLMSPPPSETUPLCPOPTION_H

#include "IGXDLMSBase.h"
#include "GXDLMSObject.h"
#include "GXHelpers.h"

typedef enum
{
    PPP_SETUP_LCP_OPTION_TYPE_MAX_REC_UNIT = 1,
    PPP_SETUP_LCP_OPTION_TYPE_ASYNC_CONTROL_CHAR_MAP = 2,
    PPP_SETUP_LCP_OPTION_TYPE_AUTH_PROTOCOL = 3,
    PPP_SETUP_LCP_OPTION_TYPE_MAGIC_NUMBER = 5,
    PPP_SETUP_LCP_OPTION_TYPE_PROTOCOL_FIELD_COMPRESSION = 7,
    PPP_SETUP_LCP_OPTION_TYPE_ADDRESS_AND_CTR_COMPRESSION = 8,
    PPP_SETUP_LCP_OPTION_TYPE_FCS_ALTERNATIVES = 9,
    PPP_SETUP_LCP_OPTION_TYPE_CALLBACK = 13
} PPP_SETUP_LCP_OPTION_TYPE;

class CGXDLMSPppSetupLcpOption
{
    PPP_SETUP_LCP_OPTION_TYPE m_Type;
    CGXDLMSVariant m_Data;
    int m_Length;
public:

    PPP_SETUP_LCP_OPTION_TYPE GetType();
    void SetType(PPP_SETUP_LCP_OPTION_TYPE value);

    int GetLength();
    void SetLength(int value);

    CGXDLMSVariant& GetData();
    void SetData(CGXDLMSVariant& value);

    std::string ToString();
};
#endif //GXDLMSPPPSETUPLCPOPTION_H