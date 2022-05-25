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

#include "../include/GXDLMSTranslatorStructure.h"
#include "../include/GXHelpers.h"

std::string CGXDLMSTranslatorStructure::GetTag(unsigned long tag)
{
    if (m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML || m_OmitNameSpace)
    {
        return m_Tags[tag];
    }
    return "x:" + m_Tags[tag];
}

DLMS_TRANSLATOR_OUTPUT_TYPE CGXDLMSTranslatorStructure::GetOutputType()
{
    return m_OutputType;
}

bool CGXDLMSTranslatorStructure::GetOmitNameSpace()
{
    return m_OmitNameSpace;
}

int CGXDLMSTranslatorStructure::GetOffset()
{
    return m_Offset;
}

void CGXDLMSTranslatorStructure::SetOffset(int value)
{
    if (value < 0)
    {
        assert(0);
    }
    m_Offset = value;
}

std::string CGXDLMSTranslatorStructure::GetDataType(DLMS_DATA_TYPE type)
{
    return GetTag(DATA_TYPE_OFFSET + (unsigned long)type);
}

bool CGXDLMSTranslatorStructure::GetShowStringAsHex()
{
    return m_ShowStringAsHex;
}

void CGXDLMSTranslatorStructure::SetShowStringAsHex(bool value)
{
    m_ShowStringAsHex = value;
}

bool CGXDLMSTranslatorStructure::GetComments()
{
    return m_Comments;
}

void CGXDLMSTranslatorStructure::SetComments(bool value)
{
    m_Comments = value;
}

bool CGXDLMSTranslatorStructure::GetIgnoreSpaces()
{
    return m_IgnoreSpaces;
}

void CGXDLMSTranslatorStructure::SetIgnoreSpaces(bool value)
{
    m_IgnoreSpaces = value;
}

CGXDLMSTranslatorStructure::CGXDLMSTranslatorStructure(
    DLMS_TRANSLATOR_OUTPUT_TYPE type,
    bool omitNameSpace,
    bool numericsAshex,
    bool hex,
    bool comments,
    std::map<unsigned long, std::string> list)
{
    m_OutputType = type;
    m_OmitNameSpace = omitNameSpace;
    m_ShowNumericsAsHex = numericsAshex;
    m_ShowStringAsHex = hex;
    m_Comments = comments;
    m_Tags = list;
    m_IgnoreSpaces = false;
    m_Offset = 0;
}

std::string CGXDLMSTranslatorStructure::ToString()
{
    return m_Sb.str();
}

void CGXDLMSTranslatorStructure::AppendSpaces()
{
    if (m_IgnoreSpaces)
    {
        m_Sb << ' ';
    }
    else
    {
        for (int pos = 0; pos != m_Offset; ++pos)
        {
            m_Sb << "  ";
        }
    }
}

void CGXDLMSTranslatorStructure::AppendLine(std::string str)
{
    if (m_IgnoreSpaces)
    {
        m_Sb << str;
    }
    else
    {
        AppendSpaces();
        m_Sb << str;
        m_Sb << "\r\n";
    }
}

void CGXDLMSTranslatorStructure::AppendLine(unsigned long tag, std::string name, CGXDLMSVariant& value)
{
    AppendLine(GetTag(tag), name, value);
}

void CGXDLMSTranslatorStructure::AppendLine(unsigned long tag, std::string name, std::string& value)
{
    AppendLine(GetTag(tag), name, value);
}

void CGXDLMSTranslatorStructure::AppendLine(std::string tag, std::string name, CGXDLMSVariant& value)
{
    std::string str;
    if (value.vt == DLMS_DATA_TYPE_UINT8)
    {
        IntegerToHex((long)value.bVal, 2, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_INT8)
    {
        IntegerToHex((long)value.cVal, 2, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_UINT16)
    {
        IntegerToHex((unsigned long)value.uiVal, 4, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_INT16)
    {
        IntegerToHex((long)value.iVal, 4, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_UINT32)
    {
        IntegerToHex((unsigned long)value.ullVal, 8, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_INT32)
    {
        IntegerToHex(value.lVal, 8, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_UINT64)
    {
        IntegerToHex(value.ullVal, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_INT64)
    {
        IntegerToHex(value.llVal, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        str = GXHelpers::BytesToHex(value.byteArr, value.GetSize());
    }
    else
    {
        str = value.ToString();
    }
    AppendLine(tag, name, str);
}

void CGXDLMSTranslatorStructure::StartComment(std::string comment)
{
    if (m_Comments)
    {
        AppendSpaces();
        m_Sb << "<!--";
        m_Sb << comment;
        m_Sb << '\r';
        m_Sb << '\n';
        ++m_Offset;
    }
}

void CGXDLMSTranslatorStructure::AppendLine(std::string tag, std::string name, std::string& value)
{
    AppendSpaces();
    m_Sb << '<';
    m_Sb << tag;
    if (m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
        m_Sb << ' ';
        if (name == "")
        {
            m_Sb << "Value";
        }
        else
        {
            m_Sb << name;
        }
        m_Sb << "=\"";
    }
    else
    {
        m_Sb << '>';
    }
    m_Sb << value;
    if (m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
        m_Sb << "\" />";
    }
    else
    {
        m_Sb << "</";
        m_Sb << tag;
        m_Sb << '>';
    }
    m_Sb << '\r';
    m_Sb << '\n';
}

void CGXDLMSTranslatorStructure::EndComment()
{
    if (m_Comments)
    {
        --m_Offset;
        AppendSpaces();
        m_Sb << "-->";
        m_Sb << '\r';
        m_Sb << '\n';
    }
}

void CGXDLMSTranslatorStructure::AppendComment(std::string comment)
{
    if (m_Comments)
    {
        AppendSpaces();
        m_Sb << "<!--";
        m_Sb << comment;
        m_Sb << "-->";
        m_Sb << '\r';
        m_Sb << '\n';
    }
}

void CGXDLMSTranslatorStructure::Append(std::string& value)
{
    m_Sb << value;
}

void CGXDLMSTranslatorStructure::Append(unsigned long tag, bool start)
{
    if (start)
    {
        m_Sb << '<';
    }
    else
    {
        m_Sb << "</";
    }
    m_Sb << GetTag(tag);
    m_Sb << '>';
}

void CGXDLMSTranslatorStructure::AppendStartTag(unsigned long tag, std::string name, std::string& value)
{
    AppendStartTag(tag, name, value, false);
}

void CGXDLMSTranslatorStructure::AppendStartTag(unsigned long tag, std::string name, std::string& value, bool plain)
{
    AppendSpaces();
    m_Sb << '<';
    m_Sb << GetTag(tag);
    if (m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML && !name.empty())
    {
        m_Sb << ' ';
        m_Sb << name;
        m_Sb << "=\"";
        m_Sb << value;
        m_Sb << "\" >";
    }
    else
    {
        m_Sb << ">";
    }
    if (!plain)
    {
        m_Sb << "\r\n";
    }
    ++m_Offset;
}

void CGXDLMSTranslatorStructure::AppendStartTag(unsigned long cmd)
{
    AppendStartTag(GetTag(cmd), false);
}

void CGXDLMSTranslatorStructure::AppendStartTag2(unsigned long cmd, bool plain)
{
    AppendStartTag(GetTag(cmd), plain);
}

void CGXDLMSTranslatorStructure::AppendStartTag(unsigned long cmd, unsigned long type)
{
    AppendStartTag(GetTag(cmd << 8 | type));
}

void CGXDLMSTranslatorStructure::AppendStartTag(std::string tag, bool plain)
{
    AppendSpaces();
    m_Sb << "<";
    m_Sb << tag;
    m_Sb << ">";
    if (!plain)
    {
        m_Sb << "\r\n";
    }
    ++m_Offset;
}

void CGXDLMSTranslatorStructure::AppendStartTag(std::string tag)
{
    AppendStartTag(tag, false);
}

void CGXDLMSTranslatorStructure::AppendEndTag(unsigned long cmd, unsigned long type)
{
    AppendEndTag(cmd << 8 | type);
}

void CGXDLMSTranslatorStructure::AppendEndTag(unsigned long tag)
{
    AppendEndTag(tag, false);
}

void CGXDLMSTranslatorStructure::AppendEndTag(unsigned long tag, bool plain)
{
    --m_Offset;
    if (!plain)
    {
        AppendSpaces();
    }
    m_Sb << "</";
    m_Sb << GetTag(tag);
    m_Sb << ">\r\n";
}

void CGXDLMSTranslatorStructure::AppendEndTag(std::string& tag)
{
    --m_Offset;
    AppendSpaces();
    m_Sb << "</";
    m_Sb << tag;
    m_Sb << ">\r\n";
}

void CGXDLMSTranslatorStructure::AppendEmptyTag(unsigned long tag)
{
    AppendEmptyTag(m_Tags[tag]);
}

void CGXDLMSTranslatorStructure::AppendEmptyTag(std::string tag)
{
    AppendSpaces();
    m_Sb << "<";
    m_Sb << tag;
    m_Sb << "/>\r\n";
}

void CGXDLMSTranslatorStructure::Trim()
{
    SetXmlLength(GetXmlLength() - 2);
}

int CGXDLMSTranslatorStructure::GetXmlLength()
{
    return (int)m_Sb.tellp();
}

void CGXDLMSTranslatorStructure::SetXmlLength(int value)
{
    m_Sb.seekp(value);
}

int CGXDLMSTranslatorStructure::IntegerToHex(long value, int desimals, std::string& result)
{
    return IntegerToHex(value, desimals, false, result);
}

int CGXDLMSTranslatorStructure::IntegerToHex(unsigned long value, int desimals, std::string& result)
{
    return IntegerToHex(value, desimals, false, result);
}

int CGXDLMSTranslatorStructure::IntegerToHex(long value, int desimals, bool, std::string& result)
{
    if (m_ShowNumericsAsHex && m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
        std::string f = "%." + GXHelpers::IntToString(desimals);
        f.append("X");
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, f.c_str(), value);
#else
        sprintf(tmp, f.c_str(), value);
#endif
    }
    else
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%d", value);
#else
        sprintf(tmp, "%ld", value);
#endif
    }
    result = tmp;
    return 0;
}

int CGXDLMSTranslatorStructure::IntegerToHex(unsigned long value, int desimals, bool, std::string& result)
{
    if (m_ShowNumericsAsHex && m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
        std::string f = "%." + GXHelpers::IntToString(desimals);
        f.append("X");
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, f.c_str(), value);
#else
        sprintf(tmp, f.c_str(), value);
#endif
    }
    else
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%lu", value);
#else
        sprintf(tmp, "%lu", value);
#endif
    }
    result = tmp;
    return 0;
}

int CGXDLMSTranslatorStructure::IntegerToHex(unsigned long value, std::string& result)
{
    if (m_ShowNumericsAsHex && m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%.8X", value);
#else
        sprintf(tmp, "%.8lX", value);
#endif
    }
    else
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%lu", value);
#else
        sprintf(tmp, "%lu", value);
#endif
    }
    result = tmp;
    return 0;
}

int CGXDLMSTranslatorStructure::IntegerToHex(long long value, std::string& result)
{
    if (m_ShowNumericsAsHex && m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%.16llX", value);
#else
        sprintf(tmp, "%.16llX", value);
#endif
    }
    else
    {
#if defined(_WIN32) || defined(_WIN64)//Windows
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%lld", value);
#else
        sprintf(tmp, "%I64d", value);
#endif

#else
        sprintf(tmp, "%lld", value);
#endif
    }
    result = tmp;
    return 0;
}

int CGXDLMSTranslatorStructure::IntegerToHex(unsigned long long value, std::string& result)
{
    if (m_ShowNumericsAsHex && m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%.16I64X", value);
#else
        sprintf(tmp, "%.16llX", value);
#endif
    }
    else
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%I64u", value);
#else
        sprintf(tmp, "%llu", value);
#endif
    }
    result = tmp;
    return 0;
}
