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
// and/or modify it under the terms of the GNU General License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#ifndef GXDLMSCONNECTIONEVENTARGS_H
#define GXDLMSCONNECTIONEVENTARGS_H

class CGXDLMSConnectionEventArgs
{
private:
    // Server ID that client try to use to make connection.
    int m_ServerAddress;
public:
    /**
    * Constructor.
    */
    CGXDLMSConnectionEventArgs()
    {
        m_ServerAddress = 0;
    }

    int GetServerAddress()
    {
        return m_ServerAddress;
    }
    void SetServerAddress(int value)
    {
        m_ServerAddress = value;
    }
};

#endif //GXDLMSCONNECTIONEVENTARGS_H
