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
#include "../include/GXDLMSQualityOfService.h"

int CGXDLMSQualityOfService::GetPrecedence()
{
    return m_Precedence;
}
void CGXDLMSQualityOfService::SetPrecedence(int value)
{
    m_Precedence = value;
}

int CGXDLMSQualityOfService::GetDelay()
{
    return m_Delay;
}
void CGXDLMSQualityOfService::SetDelay(int value)
{
    m_Delay = value;
}

int CGXDLMSQualityOfService::GetReliability()
{
    return m_Reliability;
}
void CGXDLMSQualityOfService::SetReliability(int value)
{
    m_Reliability = value;
}

int CGXDLMSQualityOfService::GetPeakThroughput()
{
    return m_PeakThroughput;
}
void CGXDLMSQualityOfService::SetPeakThroughput(int value)
{
    m_PeakThroughput = value;
}

int CGXDLMSQualityOfService::GetMeanThroughput()
{
    return m_MeanThroughput;
}
void CGXDLMSQualityOfService::SetMeanThroughput(int value)
{
    m_MeanThroughput = value;
}


std::string CGXDLMSQualityOfService::ToString()
{
    std::stringstream sb;
    sb << m_Precedence;
    sb << " ";
    sb << m_Delay;
    sb << " ";
    sb << m_Reliability;
    sb << " ";
    sb << m_PeakThroughput;
    sb << " ";
    sb << m_MeanThroughput;
    return sb.str();
}