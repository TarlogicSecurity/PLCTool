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

#include "../include/GXDLMSIp4SetupIpOption.h"
#include <sstream>

IP_OPTION_TYPE CGXDLMSIp4SetupIpOption::GetType()
{
    return m_Type;
}
void CGXDLMSIp4SetupIpOption::SetType(IP_OPTION_TYPE value)
{
    m_Type = value;
}

short CGXDLMSIp4SetupIpOption::GetLength()
{
    return m_Length;
}
void CGXDLMSIp4SetupIpOption::SetLength(short value)
{
    m_Length = value;
}

CGXByteBuffer& CGXDLMSIp4SetupIpOption::GetData()
{
    return m_Data;
}

void CGXDLMSIp4SetupIpOption::SetData(CGXByteBuffer& value)
{
    m_Data = value;
}

std::string CGXDLMSIp4SetupIpOption::ToString()
{
    std::stringstream sb;
    sb << m_Type;
    return sb.str();
}
