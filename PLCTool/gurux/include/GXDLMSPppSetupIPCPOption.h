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

#ifndef GXDLMSPPPSETUPIPCPOPTION_H
#define GXDLMSPPPSETUPIPCPOPTION_H

#include "IGXDLMSBase.h"
#include "GXDLMSObject.h"
#include "GXHelpers.h"

enum PPP_SETUP_IPCP_OPTION_TYPE
{
    PPP_SETUP_IPCP_OPTION_TYPE_IPCOMPRESSIONPROTOCOL = 2,
    PPP_SETUP_IPCP_OPTION_TYPE_PREFLOCALIP = 3,
    PPP_SETUP_IPCP_OPTION_TYPE_PREFPEERIP = 20,
    PPP_SETUP_IPCP_OPTION_TYPE_GAO = 21,
    PPP_SETUP_IPCP_OPTION_TYPE_USIP = 22
};

class CGXDLMSPppSetupIPCPOption
{
	friend class CGXDLMSPppSetup;
    PPP_SETUP_IPCP_OPTION_TYPE m_Type;
    int m_Length;
    CGXDLMSVariant m_Data;
public:

    PPP_SETUP_IPCP_OPTION_TYPE GetType();
    void SetType(PPP_SETUP_IPCP_OPTION_TYPE value);

    int GetLength();
    void SetLength(int value);

    CGXDLMSVariant& GetData();
    void SetData(CGXDLMSVariant& value);

    std::string ToString();
};
#endif //GXDLMSPPPSETUPIPCPOPTION_H
