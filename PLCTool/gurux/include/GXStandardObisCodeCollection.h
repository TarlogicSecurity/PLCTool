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

#ifndef GXSTANDARDOBISCODECOLLECTION_H
#define GXSTANDARDOBISCODECOLLECTION_H

#include "errorcodes.h"
#include "GXStandardObisCode.h"
#include "GXHelpers.h"

/////////////////////////////////////////////////////////////////////////////
// Obis Code Collection is used to save all default OBIS Codes.
/////////////////////////////////////////////////////////////////////////////
class CGXStandardObisCodeCollection : public std::vector<CGXStandardObisCode>
{
    //Convert logican name std::string to bytes.
    static int GetUInt8(std::string ln, unsigned char* bytes);

    // Check is interface included to standard.
    bool EqualsInterface(CGXStandardObisCode item, int ic);

    // Check OBIS codes.
    static bool EqualsMask(std::string obis, int ic);

    // Check OBIS code.
    static bool EqualsObisCode(std::vector< std::string >& obisMask, unsigned char* ic);

    // Get description.
    std::string GetDescription(std::string& str);

public:
    bool Find(std::string ln, DLMS_OBJECT_TYPE objectType, CGXStandardObisCode& item);

    static bool EqualsMask(std::string obisMask, std::string ln);

    // Find Standard OBIS Code description.
    bool Find(unsigned char* pObisCode, int IC, CGXStandardObisCode& tmp);
};
#endif //GXSTANDARDOBISCODECOLLECTION_H