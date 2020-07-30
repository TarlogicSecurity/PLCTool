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

#include "../include/GXDLMSSeasonProfile.h"
#include <sstream>

/**
 Constructor.
*/
CGXDLMSSeasonProfile::CGXDLMSSeasonProfile()
{

}

/**
 Constructor.
*/
CGXDLMSSeasonProfile::CGXDLMSSeasonProfile(std::string name, CGXDateTime& start, std::string weekName)
{
    m_Name.AddString(name);
    SetStart(start);
    m_WeekName.AddString(weekName);
}

/**
 Constructor.
*/
CGXDLMSSeasonProfile::CGXDLMSSeasonProfile(CGXByteBuffer& name, CGXDateTime& start, CGXByteBuffer weekName)
{
    m_Name.Set(&name);
    SetStart(start);
    m_WeekName.Set(&weekName);
}

/**
 Name of season profile.
*/
CGXByteBuffer& CGXDLMSSeasonProfile::GetName()
{
    return m_Name;
}
void CGXDLMSSeasonProfile::SetName(CGXByteBuffer& value)
{
    m_Name = value;
}

/**
 Season Profile start time.
*/
CGXDateTime& CGXDLMSSeasonProfile::GetStart()
{
    return m_Start;
}
void CGXDLMSSeasonProfile::SetStart(CGXDateTime value)
{
    m_Start = value;
}

/**
 Week name of season profile.
*/
CGXByteBuffer& CGXDLMSSeasonProfile::GetWeekName()
{
    return m_WeekName;
}
void CGXDLMSSeasonProfile::SetWeekName(CGXByteBuffer& value)
{
    m_WeekName = value;
}

std::string CGXDLMSSeasonProfile::ToString()
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
    sb << " ";
    sb << m_Start.ToString().c_str();
    return sb.str();
}
