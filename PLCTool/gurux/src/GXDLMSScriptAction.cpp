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

#include "../include/GXDLMSScriptAction.h"
#include <sstream>

/**
 Defines which action to be applied to the referenced object.
*/
SCRIPT_ACTION_TYPE CGXDLMSScriptAction::GetType()
{
    return m_Type;
}
void CGXDLMSScriptAction::SetType(SCRIPT_ACTION_TYPE value)
{
    m_Type = value;
}

/**
 Executed object type.
*/
DLMS_OBJECT_TYPE CGXDLMSScriptAction::GetObjectType()
{
    return m_ObjectType;
}
void CGXDLMSScriptAction::SetObjectType(DLMS_OBJECT_TYPE value)
{
    m_ObjectType = value;
}

/**
 Logical name of executed object.
*/
std::string CGXDLMSScriptAction::GetLogicalName()
{
    return m_LogicalName;
}
void CGXDLMSScriptAction::SetLogicalName(std::string value)
{
    m_LogicalName = value;
}

/**
 defines which attribute of the selected object is affected; or
 * which specific method is to be executed.
*/
int CGXDLMSScriptAction::GetIndex()
{
    return m_Index;
}
void CGXDLMSScriptAction::SetIndex(int value)
{
    m_Index = value;
}

/**
 Parameter is service spesific.
*/
CGXDLMSVariant CGXDLMSScriptAction::GetParameter()
{
    return m_Parameter;
}
void CGXDLMSScriptAction::SetParameter(CGXDLMSVariant value)
{
    m_Parameter = value;
}

std::string CGXDLMSScriptAction::ToString()
{
    std::stringstream sb;
    sb << m_Type;
    sb << " ";
    sb << m_ObjectType;
    sb << " ";
    sb << m_LogicalName.c_str();
    sb << " ";
    sb << m_Index;
    sb << " ";
    sb << m_Parameter.ToString().c_str();
    return sb.str();
}