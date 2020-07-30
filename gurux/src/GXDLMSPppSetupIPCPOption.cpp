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

#include "../include/GXDLMSPppSetupIPCPOption.h"
#include <sstream>

PPP_SETUP_IPCP_OPTION_TYPE CGXDLMSPppSetupIPCPOption::GetType()
{
    return m_Type;
}
void CGXDLMSPppSetupIPCPOption::SetType(PPP_SETUP_IPCP_OPTION_TYPE value)
{
    m_Type = value;
}

int CGXDLMSPppSetupIPCPOption::GetLength()
{
    return m_Length;
}
void CGXDLMSPppSetupIPCPOption::SetLength(int value)
{
    m_Length = value;
}

CGXDLMSVariant& CGXDLMSPppSetupIPCPOption::GetData()
{
    return m_Data;
}
void CGXDLMSPppSetupIPCPOption::SetData(CGXDLMSVariant& value)
{
    m_Data = value;
}

std::string CGXDLMSPppSetupIPCPOption::ToString()
{
    std::stringstream sb;
    sb << m_Type;
    sb << " ";
    sb << m_Length;
    return sb.str();
}