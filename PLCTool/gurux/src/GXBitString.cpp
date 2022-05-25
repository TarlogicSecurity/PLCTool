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

#include "../include/errorcodes.h"
#include "../include/GXBitString.h"
#include "../include/GXHelpers.h"

CGXBitString::CGXBitString()
{
}

CGXBitString::CGXBitString(std::string value)
{
    m_Value = value;
}

std::string& CGXBitString::ToString()
{
    return m_Value;
}

int CGXBitString::ToInteger(int& value, unsigned char)
{
    value = 0;
    int pos = 0;
    for (std::string::iterator it = m_Value.begin(); it != m_Value.end(); ++it)
    {
        if (*it == '1')
        {
            value |= (1 << pos);
        }
        else if (*it != '0')
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        ++pos;
    }
    return 0;
}

int CGXBitString::ToInteger(int& value)
{
    return ToInteger(value, 32);
}

int CGXBitString::ToByte(unsigned char& value)
{
    int ret, tmp = 0;
    ret = ToInteger(tmp, 8);
    if (ret == 0)
    {
        value = tmp;
    }
    else
    {
        value = 0;
    }
    return ret;
}

void ToBitString2(std::stringstream& sb, unsigned char value, unsigned int count)
{
    unsigned char pos;
    if (count > 8)
    {
        count = 8;
    }
    for (pos = 0; pos != count; ++pos)
    {
        if ((value & (1 << pos)) != 0)
        {
            sb << "1";
        }
        else
        {
            sb << "0";
        }
    }
}
std::string CGXBitString::ToBitString(unsigned int value, unsigned int count)
{
    std::stringstream sb;
    ToBitString2(sb, (unsigned char)(value & 0xFF), count);
    if (count > 8)
    {
        ToBitString2(sb, (unsigned char)((value >> 8) & 0xFF), count - 8);
        if (count > 16)
        {
            ToBitString2(sb, (unsigned char)((value >> 16) & 0xFF), count - 16);
            if (count > 24)
            {
                ToBitString2(sb, (unsigned char)((value >> 24) & 0xFF), count - 24);
            }
        }
    }
    if (sb.width() > count)
    {
        return sb.str().substr(0, (unsigned int)(sb.width() - count));
    }
    return sb.str();
}
