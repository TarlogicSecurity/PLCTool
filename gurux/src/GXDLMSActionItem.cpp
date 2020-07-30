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

#include "../include/GXDLMSActionItem.h"

//Constructor.
CGXDLMSActionItem::CGXDLMSActionItem()
{
    m_ScriptSelector = 0;
}

std::string& CGXDLMSActionItem::GetLogicalName()
{
    return m_LogicalName;
}
void CGXDLMSActionItem::SetLogicalName(std::string& value)
{
    m_LogicalName = value;
}

int CGXDLMSActionItem::GetScriptSelector()
{
    return m_ScriptSelector;
}
void CGXDLMSActionItem::SetScriptSelector(int value)
{
    m_ScriptSelector = value;
}

std::string CGXDLMSActionItem::ToString()
{
    std::stringstream sb;
    sb << m_LogicalName.c_str();
    sb << " ";
    sb << m_ScriptSelector;
    return sb.str();
}