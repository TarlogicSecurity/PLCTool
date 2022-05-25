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

#include "../include/GXXmlReader.h"
#include <string.h>
CGXXmlReader::CGXXmlReader(FILE* f)
{
    m_f = f;
    m_Buffer[0] = 0;
    m_EventType = XML_EVENT_TYPE_NONE;
    m_Index = 0;
    m_Size = 0;
}

bool CGXXmlReader::IsEOF()
{
    if (m_Size == m_Index)
    {
        m_Index = m_Size = 0;
        m_Buffer[0] = 0;
        char* s = fgets(m_Buffer, sizeof(m_Buffer), m_f);
        if (s == NULL)
        {
            return true;
        }
        m_Size = (int) strlen(m_Buffer);
    }
    return false;
}

bool CGXXmlReader::Read()
{
    if (m_Index < m_Size)
    {
        if (m_Buffer[m_Index] == ' ')
        {
            ++m_Index;
            return true;
        }
        if (m_Buffer[m_Index] == '\n')
        {
            ++m_Index;
            return true;
        }
    }
    m_Index = m_Size = 0;
    return false;
}

bool CGXXmlReader::IsStartElement()
{
    if (m_Buffer[m_Index] == '<' && m_Buffer[m_Index + 1] != '?')
    {
        char* s = strstr(m_Buffer + m_Index, ">");
        if (s != NULL)
        {
            m_Name.clear();
            m_Name.append(m_Buffer + m_Index + 1, s - m_Buffer - m_Index - 1);
            m_Index = (int)(s - m_Buffer + 1);
            return true;
        }
    }
    return false;
}

std::string& CGXXmlReader::GetText()
{
    char tag[256];
    strcpy(tag, "</");
    strcat(tag, m_Name.c_str());
    strcat(tag, ">");
    char* s = strstr(m_Buffer + m_Index, tag);
    m_Value.append(m_Buffer + m_Index, s);
    m_Index = (int)(s - m_Buffer);
    Read();
    GetNext();
    return m_Value;
}

void CGXXmlReader::GetNext()
{
    while (m_EventType == XML_EVENT_TYPE_COMMENT ||
        m_EventType == XML_EVENT_TYPE_SPACE ||
        m_EventType == XML_EVENT_TYPE_CHARACTERS)
    {
        Read();
    }
}

std::string& CGXXmlReader::ReadElementContentAsString(const char* name)
{
    return ReadElementContentAsString(name, NULL);
}

std::string& CGXXmlReader::ReadElementContentAsString(const char* name, const char* defaultValue)
{
    GetNext();
    m_Value.clear();
    if (m_Name == name)
    {
        return GetText();
    }
    if (defaultValue != NULL)
    {
        m_Value.append(defaultValue);
    }
    return m_Value;
}

int CGXXmlReader::ReadElementContentAsInt(const char* name)
{
    return ReadElementContentAsInt(name, 0);
}

int CGXXmlReader::ReadElementContentAsInt(const char* name, int defaultValue)
{
    GetNext();
    if (m_Name == name)
    {
        std::string str = GetText();
        int ret = atol(str.c_str());
        return ret;
    }
    return defaultValue;
}

std::string& CGXXmlReader::GetName()
{
    return m_Name;
}
