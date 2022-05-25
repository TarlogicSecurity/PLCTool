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

#include "../include/GXStandardObisCode.h"

CGXStandardObisCode::CGXStandardObisCode(std::vector< std::string > obis,
    std::string& desc,
    std::string& interfaces, std::string& dataType)
{
    m_OBIS.clear();
    m_OBIS.insert(m_OBIS.end(), obis.begin(), obis.end());
    SetDescription(desc);
    SetInterfaces(interfaces);
    SetDataType(dataType);
}

std::vector< std::string >& CGXStandardObisCode::GetOBIS()
{
    return m_OBIS;
}

void CGXStandardObisCode::SetOBIS(std::vector< std::string >& value)
{
    m_OBIS.clear();
    m_OBIS.insert(m_OBIS.end(), value.begin(), value.end());
}

/**
 m_OBIS code description.
*/
std::string& CGXStandardObisCode::GetDescription()
{
    return m_Description;
}
void CGXStandardObisCode::SetDescription(std::string value)
{
    m_Description = value;
}

/**
 m_Interfaces that are using this m_OBIS code.
*/
std::string& CGXStandardObisCode::GetInterfaces()
{
    return m_Interfaces;
}
void CGXStandardObisCode::SetInterfaces(std::string value)
{
    m_Interfaces = value;
}

/**
 Standard data types.
*/
std::string& CGXStandardObisCode::GetDataType()
{
    return m_DataType;
}

void CGXStandardObisCode::SetDataType(std::string value)
{
    m_DataType = value;
}