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
#ifndef GXDLMSWEEKPROFILE_H
#define GXDLMSWEEKPROFILE_H

#include <string>
#include "GXBytebuffer.h"

class CGXDLMSWeekProfile
{
    CGXByteBuffer m_Name;
    int m_Monday;
    int m_Tuesday;
    int m_Wednesday;
    int m_Thursday;
    int m_Friday;
    int m_Saturday;
    int m_Sunday;
public:
    /**
     Constructor.
    */
    CGXDLMSWeekProfile();

    /**
     Constructor.
    */
    CGXDLMSWeekProfile(std::string name, int monday, int tuesday, int wednesday, int thursday, int friday, int saturday, int sunday);

    std::string GetName();

    void SetName(std::string value);

    void GetName(CGXByteBuffer& value);

    void SetName(CGXByteBuffer& value);

    int GetMonday();

    void SetMonday(int value);

    int GetTuesday();

    void SetTuesday(int value);

    int GetWednesday();

    void SetWednesday(int value);

    int GetThursday();

    void SetThursday(int value);

    int GetFriday();

    void SetFriday(int value);

    int GetSaturday();

    void SetSaturday(int value);

    int GetSunday();

    void SetSunday(int value);

    std::string ToString();
};

#endif //GXDLMSWEEKPROFILE_H