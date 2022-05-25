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

#include "../include/GXDLMSSpecialDay.h"
#include <sstream>

int CGXDLMSSpecialDay::GetIndex()
{
    return m_Index;
}
void CGXDLMSSpecialDay::SetIndex(int value)
{
    m_Index = value;
}

CGXDate& CGXDLMSSpecialDay::GetDate()
{
    return m_Date;
}

void CGXDLMSSpecialDay::SetDate(CGXDate& value)
{
    m_Date = value;
}

void CGXDLMSSpecialDay::SetDate(CGXDateTime& value)
{
    m_Date = value;
}

int CGXDLMSSpecialDay::GetDayId()
{
    return m_DayId;
}
void CGXDLMSSpecialDay::SetDayId(int value)
{
    m_DayId = value;
}

std::string CGXDLMSSpecialDay::ToString()
{
    std::stringstream sb;
    sb << m_Index;
    sb << " ";
    sb << m_Date.ToString().c_str();
    sb << " ";
    sb << m_DayId;
    return sb.str();
}