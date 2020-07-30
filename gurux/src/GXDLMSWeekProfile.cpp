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
#include "../include/GXDLMSWeekProfile.h"
#include <sstream>

/**
 Constructor.
*/
CGXDLMSWeekProfile::CGXDLMSWeekProfile()
{
    m_Monday = m_Tuesday = m_Wednesday = m_Thursday = m_Friday = m_Saturday = m_Sunday = 0;
}

/**
 Constructor.
*/
CGXDLMSWeekProfile::CGXDLMSWeekProfile(std::string name, int monday, int tuesday, int wednesday, int thursday, int friday, int saturday, int sunday)
{
    SetName(name);
    SetMonday(monday);
    SetTuesday(tuesday);
    SetWednesday(wednesday);
    SetThursday(thursday);
    SetFriday(friday);
    SetSaturday(saturday);
    SetSunday(sunday);
}

void CGXDLMSWeekProfile::GetName(CGXByteBuffer& value)
{
    value.Clear();
    value.Set(&m_Name, 0, m_Name.GetSize());
}

void CGXDLMSWeekProfile::SetName(CGXByteBuffer& value)
{
    m_Name = value;
}

std::string CGXDLMSWeekProfile::GetName()
{
    return m_Name.ToString();
}
void CGXDLMSWeekProfile::SetName(std::string value)
{
    m_Name.AddString(value.c_str());
}

int CGXDLMSWeekProfile::GetMonday()
{
    return m_Monday;
}
void CGXDLMSWeekProfile::SetMonday(int value)
{
    m_Monday = value;
}

int CGXDLMSWeekProfile::GetTuesday()
{
    return m_Tuesday;
}
void CGXDLMSWeekProfile::SetTuesday(int value)
{
    m_Tuesday = value;
}

int CGXDLMSWeekProfile::GetWednesday()
{
    return m_Wednesday;
}
void CGXDLMSWeekProfile::SetWednesday(int value)
{
    m_Wednesday = value;
}

int CGXDLMSWeekProfile::GetThursday()
{
    return m_Thursday;
}
void CGXDLMSWeekProfile::SetThursday(int value)
{
    m_Thursday = value;
}

int CGXDLMSWeekProfile::GetFriday()
{
    return m_Friday;
}
void CGXDLMSWeekProfile::SetFriday(int value)
{
    m_Friday = value;
}

int CGXDLMSWeekProfile::GetSaturday()
{
    return m_Saturday;
}
void CGXDLMSWeekProfile::SetSaturday(int value)
{
    m_Saturday = value;
}

int CGXDLMSWeekProfile::GetSunday()
{
    return m_Sunday;
}
void CGXDLMSWeekProfile::SetSunday(int value)
{
    m_Sunday = value;
}

std::string CGXDLMSWeekProfile::ToString()
{
    std::stringstream sb;
    if (m_Name.IsAsciiString())
    {
        sb << m_Name.ToString();
    }
    else
    {
        sb << m_Name.ToHexString();
    }
    return sb.str();
}