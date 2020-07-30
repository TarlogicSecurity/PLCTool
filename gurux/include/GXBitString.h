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

#ifndef GXBIT_STRING_H
#define GXBIT_STRING_H

#include <string>
/**
* Date object. Time part is ignored.
*/
class CGXBitString
{
private:
    std::string m_Value;
    //Convert bit string to Integer.
    int ToInteger(int& value, unsigned char maxSize);
public:
    //Constructor.
    CGXBitString();

    //Constructor.
    CGXBitString(std::string value);

    //Convert bit string to string.
    std::string& ToString();

    //Convert bit string to Integer.
    int ToInteger(int& value);

    //Convert bit string to byte.
    int ToByte(unsigned char& value);

    // Convert integer value to BitString.
    // value : Value to convert.
    // count: Amount of bits.
    // returns: Bitstring.
    static std::string ToBitString(unsigned int value, unsigned int count);
};
#endif //GXBIT_STRING_H