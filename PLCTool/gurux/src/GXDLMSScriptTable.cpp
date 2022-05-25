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

#include "../include/GXDLMSScriptTable.h"
#include "../include/GXDLMSClient.h"
#include <sstream>

//Constructor.
CGXDLMSScriptTable::CGXDLMSScriptTable() : CGXDLMSScriptTable("0.0.10.0.100.255", 0)
{
}

//SN Constructor.
CGXDLMSScriptTable::CGXDLMSScriptTable(std::string ln, unsigned short sn) : CGXDLMSObject(DLMS_OBJECT_TYPE_SCRIPT_TABLE, ln, sn)
{

}

//LN Constructor.
CGXDLMSScriptTable::CGXDLMSScriptTable(std::string ln) : CGXDLMSScriptTable(ln, 0)
{

}

//Destructor.
CGXDLMSScriptTable::~CGXDLMSScriptTable()
{
    for (std::vector<CGXDLMSScript*>::iterator it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
    {
        delete *it;
    }
    m_Scripts.clear();
}


std::vector<CGXDLMSScript*>& CGXDLMSScriptTable::GetScripts()
{
    return m_Scripts;
}

int CGXDLMSScriptTable::Execute(CGXDLMSClient* client, CGXDLMSScript* script, std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data(script->GetID());
    data.vt = DLMS_DATA_TYPE_UINT16;
    return client->Method(this, 1, data, reply);
}

int CGXDLMSScriptTable::Execute(CGXDLMSClient* client, unsigned short scriptId, std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant data(scriptId);
    data.vt = DLMS_DATA_TYPE_UINT16;
    return client->Method(this, 1, data, reply);
}


// Returns amount of attributes.
int CGXDLMSScriptTable::GetAttributeCount()
{
    return 2;
}

// Returns amount of methods.
int CGXDLMSScriptTable::GetMethodCount()
{
    return 1;
}

void CGXDLMSScriptTable::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);

    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<CGXDLMSScript*>::iterator it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        sb << (*it)->GetID();
        for (std::vector<CGXDLMSScriptAction*>::iterator a = (*it)->GetActions().begin(); a != (*it)->GetActions().end(); ++a)
        {
            sb << " ";
            std::string str = (*a)->ToString();
            sb.write(str.c_str(), str.size());
        }
    }
    sb << ']';
    values.push_back(sb.str());
}

void CGXDLMSScriptTable::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Scripts
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
}

int CGXDLMSScriptTable::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    //Scripts
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Returns value of given attribute.
int CGXDLMSScriptTable::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        int ret;
        CGXDLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 2)
    {
        e.SetByteArray(true);
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        GXHelpers::SetObjectCount((unsigned long)m_Scripts.size(), data);
        int ret;
        CGXDLMSVariant id, type, oType, ln, index;
        for (std::vector<CGXDLMSScript*>::iterator it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            //Count
            data.SetUInt8(2);
            //Script_identifier:
            id = (*it)->GetID();
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, id)) != 0)
            {
                return ret;
            }
            data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
            //Count
            GXHelpers::SetObjectCount((unsigned long)(*it)->GetActions().size(), data);
            for (std::vector<CGXDLMSScriptAction*>::iterator a = (*it)->GetActions().begin(); a != (*it)->GetActions().end(); ++a)
            {
                GXHelpers::SetLogicalName((*a)->GetLogicalName().c_str(), ln);
                data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
                data.SetUInt8(5);
                type = (*a)->GetType();
                oType = (*a)->GetObjectType();
                index = (*a)->GetIndex();
                CGXDLMSVariant param = (*a)->GetParameter();
                if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_ENUM, type)) != 0 || //service_id
                    (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, oType)) != 0 || //class_id
                    (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0 || //logical_name
                    (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, index)) != 0 || //index
                    (ret = GXHelpers::SetData(data, (*a)->GetParameter().vt, param)) != 0) //parameter
                {
                    return ret;
                }
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSScriptTable::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        for (std::vector<CGXDLMSScript*>::iterator it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
        {
            delete *it;
        }
        m_Scripts.clear();
        //Fix Xemex bug here.
        //Xemex meters do not return array as they shoul be according standard.
        if (e.GetValue().Arr.size() != 0)
        {
            if (e.GetValue().Arr[0].vt == DLMS_DATA_TYPE_STRUCTURE)
            {
                std::string ln;
                for (std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
                {
                    CGXDLMSScript* script = new CGXDLMSScript();
                    script->SetID((*item).Arr[0].ToInteger());
                    m_Scripts.push_back(script);
                    for (std::vector<CGXDLMSVariant>::iterator arr = (*item).Arr[1].Arr.begin(); arr != (*item).Arr[1].Arr.end(); ++arr)
                    {
                        CGXDLMSScriptAction* it = new CGXDLMSScriptAction();
                        SCRIPT_ACTION_TYPE type = (SCRIPT_ACTION_TYPE)(*arr).Arr[0].ToInteger();
                        it->SetType(type);
                        DLMS_OBJECT_TYPE ot = (DLMS_OBJECT_TYPE)(*arr).Arr[1].ToInteger();
                        it->SetObjectType(ot);
                        ln.clear();
                        GXHelpers::GetLogicalName((*arr).Arr[2].byteArr, ln);
                        it->SetLogicalName(ln);
                        it->SetIndex((*arr).Arr[3].ToInteger());
                        it->SetParameter((*arr).Arr[4]);
                        script->GetActions().push_back(it);
                    }
                }
            }
            else //Read Xemex meter here.
            {
                CGXDLMSScript* script = new CGXDLMSScript();
                script->SetID(e.GetValue().Arr[0].ToInteger());
                m_Scripts.push_back(script);
                CGXDLMSScriptAction *it = new CGXDLMSScriptAction();
                SCRIPT_ACTION_TYPE type = (SCRIPT_ACTION_TYPE)e.GetValue().Arr[1].Arr[0].ToInteger();
                it->SetType(type);
                DLMS_OBJECT_TYPE ot = (DLMS_OBJECT_TYPE)e.GetValue().Arr[1].Arr[1].ToInteger();
                it->SetObjectType(ot);
                std::string ln;
                GXHelpers::GetLogicalName(e.GetValue().Arr[1].Arr[2].byteArr, ln);
                it->SetLogicalName(ln);
                it->SetIndex(e.GetValue().Arr[1].Arr[3].ToInteger());
                it->SetParameter(e.GetValue().Arr[1].Arr[4]);
                script->GetActions().push_back(it);
            }
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
