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

#ifndef GXAUTHENTICATION_H
#define GXAUTHENTICATION_H

#include "GXDLMSVariant.h"

class GXAuthentication
{
    CGXDLMSVariant m_ClientID;
    DLMS_AUTHENTICATION m_Type;
    std::string m_Password;
public:
    //Constructor.
    GXAuthentication(void)
    {
    }

    //Constructor.
    GXAuthentication(DLMS_AUTHENTICATION type, std::string password, CGXDLMSVariant clientID)
    {
        m_Type = type;
        m_ClientID = clientID;
    }


    /// <summary>
    /// Authentication type
    /// </summary>
    DLMS_AUTHENTICATION GetType()
    {
        return m_Type;
    }

    /// <summary>
    /// Authentication type
    /// </summary>
    void SetType(DLMS_AUTHENTICATION value)
    {
        m_Type = value;
    }

    /// <summary>
    /// Client address.
    /// </summary>
    CGXDLMSVariant GetClientID()
    {
        return m_ClientID;
    }

    /// <summary>
    /// Client address.
    /// </summary>
    void SetClientID(CGXDLMSVariant value)
    {
        m_ClientID = value;
    }

    /// <summary>
    /// Get Used password.
    /// </summary>
    std::string GetPassword()
    {
        return m_Password;
    }

    /// <summary>
    /// Set Used password.
    /// </summary>
    void SetPassword(std::string value)
    {
        m_Password = value;
    }
};
#endif //GXAUTHENTICATION_H