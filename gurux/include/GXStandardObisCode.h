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
#ifndef GXSTANDARDOBISCODE_H
#define GXSTANDARDOBISCODE_H

#include <string>
#include <vector>

class CGXStandardObisCode
{
    std::vector< std::string > m_OBIS;
    std::string m_DataType;
    std::string m_Interfaces;
    std::string m_Description;
public:
    // Constructor.
    CGXStandardObisCode()
    {
    }

    // Constructor.
    CGXStandardObisCode(std::vector< std::string > obis, std::string& desc,
                        std::string& interfaces, std::string& dataType);
    // OBIS code.
    std::vector< std::string >& GetOBIS();

    void SetOBIS(std::vector< std::string >& value);

    // OBIS code description.
    std::string& GetDescription();

    void SetDescription(std::string value);

    // Interfaces that are using this m_OBIS code.
    std::string& GetInterfaces();
    void SetInterfaces(std::string value);

    // Standard data types.
    std::string& GetDataType();
    void SetDataType(std::string value);
};
#endif //GXSTANDARDOBISCODE_H