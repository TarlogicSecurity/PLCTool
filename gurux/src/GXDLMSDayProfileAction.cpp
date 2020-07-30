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

#include "../include/GXDLMSVariant.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXDLMSDayProfileAction.h"
#include <sstream>

/**
 Constructor.
*/
CGXDLMSDayProfileAction::CGXDLMSDayProfileAction()
{
    m_ScriptSelector = 0;
}

/**
 Constructor.
*/
CGXDLMSDayProfileAction::CGXDLMSDayProfileAction(CGXTime& startTime, std::string scriptLogicalName, int scriptSelector)
{
    SetStartTime(startTime);
    SetScriptLogicalName(scriptLogicalName);
    SetScriptSelector(scriptSelector);
}

/**
 Defines the time when the script is to be executed.
*/
CGXTime& CGXDLMSDayProfileAction::GetStartTime()
{
    return m_StartTime;
}
void CGXDLMSDayProfileAction::SetStartTime(CGXTime& value)
{
    m_StartTime = value;
}

/**
 Defines the logical name of the "Script table" object;
*/
std::string& CGXDLMSDayProfileAction::GetScriptLogicalName()
{
    return m_ScriptLogicalName;
}
void CGXDLMSDayProfileAction::SetScriptLogicalName(std::string& value)
{
    m_ScriptLogicalName = value;
}

/**
 Defines the script_identifier of the script to be executed.
*/
int CGXDLMSDayProfileAction::GetScriptSelector()
{
    return m_ScriptSelector;
}
void CGXDLMSDayProfileAction::SetScriptSelector(int value)
{
    m_ScriptSelector = value;
}

std::string CGXDLMSDayProfileAction::ToString()
{
    std::stringstream sb;
    sb << m_StartTime.ToString().c_str();
    sb << " ";
    sb << m_ScriptLogicalName.c_str();
    return sb.str();

}
