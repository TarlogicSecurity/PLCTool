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

#ifndef GX_XMLWRITER_H
#define GX_XMLWRITER_H
#include <vector>
#include <string>
#include "GXDLMSObject.h"

class CGXXmlWriter
{
    bool m_SkipDefaults;
    FILE* m_f;
    const char* NEW_LINE = "\n";
    std::vector<std::string> m_Elements;

    //
    // Append spaces to the buffer.
    // count: Amount of spaces.
    int AppendSpaces();

    int Write(const char* data);

    int WriteAttributeString(const char* name, const char* value);
    int WriteAttributeString(const char* name, const int value);
    int WriteArray(CGXDLMSVariant& data);
public:
    //Constructor.
    CGXXmlWriter(FILE* f, bool skipDefaults);
    int WriteStartDocument();

    int WriteStartElement(const char* name);

    int WriteElementString(const char* name, int value);
    int WriteElementString(const char* name, long value);
    int WriteElementString(const char* name, unsigned int value);
    int WriteElementString(const char* name, unsigned long value);
    int WriteElementString(const char* name, double value, double defaultValue);
    int WriteElementString(const char* name, const char* value);
    int WriteElementString(const char* name, CGXDateTime& value);
    int WriteElementString(const char* name, CGXDate& value);
    int WriteElementString(const char* name, CGXTime& value);

    int WriteElementString(const char* name, const std::string& value);

    // Write End Element tag.
    int WriteEndElement(bool addSpaces);

    // Write End Element tag.
    int WriteEndElement();

    // Write End document tag.
    int WriteEndDocument();

    // Write object value to file.
    //
    // name: Object name.
    // value: Object value.
    int WriteElementObject(
        const char* name,
        CGXDLMSVariant& value,
        DLMS_DATA_TYPE dt = DLMS_DATA_TYPE_NONE,
        DLMS_DATA_TYPE uiType = DLMS_DATA_TYPE_NONE);

    //Save DLMS object to the file.
    int Save(CGXDLMSObject* obj);
};

#endif //GX_XMLWRITER_H