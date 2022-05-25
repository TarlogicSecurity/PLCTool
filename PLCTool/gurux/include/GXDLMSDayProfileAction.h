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

#ifndef GXDLMSDAYPROFILEACTION_H
#define GXDLMSDAYPROFILEACTION_H

#include "GXTime.h"

//Activity Calendar's Day Profile Action is defined on the standard.
class CGXDLMSDayProfileAction
{
    CGXTime m_StartTime;
    std::string m_ScriptLogicalName;
    int m_ScriptSelector;
public:
    /**
     Constructor.
    */
    CGXDLMSDayProfileAction();

    /**
     Constructor.
    */
    CGXDLMSDayProfileAction(CGXTime& startTime, std::string scriptLogicalName, int scriptSelector);

    /**
     Defines the time when the script is to be executed.
    */
    CGXTime& GetStartTime();
    void SetStartTime(CGXTime& value);

    /**
     Defines the logical name of the "Script table" object;
    */
    std::string& GetScriptLogicalName();
    void SetScriptLogicalName(std::string& value);

    /**
     Defines the script_identifier of the script to be executed.
    */
    int GetScriptSelector();
    void SetScriptSelector(int value);

    std::string ToString();
};
#endif //GXDLMSDAYPROFILEACTION_H
