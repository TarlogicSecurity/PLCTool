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

#ifndef GX_XML_READER_H
#define GX_XML_READER_H

#include <vector>
#include <string>

class CGXXmlReader
{
    enum XML_EVENT_TYPE
    {
        XML_EVENT_TYPE_NONE,
        XML_EVENT_TYPE_START_ELEMENT,
        XML_EVENT_TYPE_END_ELEMENT,
        XML_EVENT_TYPE_COMMENT,
        XML_EVENT_TYPE_SPACE,
        XML_EVENT_TYPE_CHARACTERS
    };
    XML_EVENT_TYPE m_EventType;
    int m_Index;
    int m_Size;
    std::string m_Name;
    std::string m_Value;
    char m_Buffer[256];
    FILE* m_f;
public:
    //Constructor.
    CGXXmlReader(FILE* f);

    bool IsEOF();

    bool Read();

    bool IsStartElement();

    std::string& GetText();

    void GetNext();

    std::string& ReadElementContentAsString(const char* name);

    std::string& ReadElementContentAsString(const char* name, const char* defaultValue);

    int ReadElementContentAsInt(const char* name);

    int ReadElementContentAsInt(const char* name, int defaultValue);

    std::string& GetName();
};

#endif //GX_XML_READER_H