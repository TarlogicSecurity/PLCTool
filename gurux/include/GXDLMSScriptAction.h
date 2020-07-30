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

#ifndef GXDLMSSCRIPTACTION_H
#define GXDLMSSCRIPTACTION_H

#include "enums.h"
#include "../include/GXDLMSVariant.h"

enum SCRIPT_ACTION_TYPE
{
    /*
     * Write attribute.
     */
    SCRIPT_ACTION_TYPE_WRITE = 1,
    /*
     * Execute specific method
     */
    SCRIPT_ACTION_TYPE_EXECUTE = 2
};

class CGXDLMSScriptAction
{
    SCRIPT_ACTION_TYPE m_Type;
    DLMS_OBJECT_TYPE m_ObjectType;
    std::string m_LogicalName;
    int m_Index;
    CGXDLMSVariant m_Parameter;
public:
    /**
     Defines which action to be applied to the referenced object.
    */
    SCRIPT_ACTION_TYPE GetType();
    void SetType(SCRIPT_ACTION_TYPE value);

    /**
     Executed object type.
    */
    DLMS_OBJECT_TYPE GetObjectType();
    void SetObjectType(DLMS_OBJECT_TYPE value);

    /**
     Logical name of executed object.
    */
    std::string GetLogicalName();
    void SetLogicalName(std::string value);

    /**
     defines which attribute of the selected object is affected; or
     * which specific method is to be executed.
    */
    int GetIndex();
    void SetIndex(int value);

    /**
     Parameter is service spesific.
    */
    CGXDLMSVariant GetParameter();
    void SetParameter(CGXDLMSVariant value);

    std::string ToString();
};
#endif //GXDLMSSCRIPTACTION_H