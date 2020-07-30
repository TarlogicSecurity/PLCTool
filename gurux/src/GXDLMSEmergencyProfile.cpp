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

#include "../include/GXDLMSVariant.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXDLMSEmergencyProfile.h"
#include <sstream>

int CGXDLMSEmergencyProfile::GetID()
{
    return m_ID;
}
void CGXDLMSEmergencyProfile::SetID(int value)
{
    m_ID = value;
}
CGXDateTime& CGXDLMSEmergencyProfile::GetActivationTime()
{
    return m_ActivationTime;
}
void CGXDLMSEmergencyProfile::SetActivationTime(CGXDateTime value)
{
    m_ActivationTime = value;
}
int CGXDLMSEmergencyProfile::GetDuration()
{
    return m_Duration;
}
void CGXDLMSEmergencyProfile::SetDuration(int value)
{
    m_Duration = value;
}

std::string CGXDLMSEmergencyProfile::ToString()
{
    std::stringstream sb;
    sb << m_ID;
    sb << " ";
    sb << m_ActivationTime.ToString().c_str();
    sb << " ";
    sb << m_Duration;
    return sb.str();
}