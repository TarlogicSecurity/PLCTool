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

#include "../include/GXDLMSParameterMonitor.h"
#include "../include/GXDLMSObjectFactory.h"
#include "../include/GXDLMSSettings.h"

//Constructor.
CGXDLMSParameterMonitor::CGXDLMSParameterMonitor() :
    CGXDLMSParameterMonitor("0.0.16.2.0.255", 0)
{
}

//SN Constructor.
CGXDLMSParameterMonitor::CGXDLMSParameterMonitor(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_PARAMETER_MONITOR, ln, sn)
{

}

//LN Constructor.
CGXDLMSParameterMonitor::CGXDLMSParameterMonitor(std::string ln) :
    CGXDLMSParameterMonitor(ln, 0)
{

}

CGXDLMSParameterMonitor::~CGXDLMSParameterMonitor()
{
    for (std::vector<CGXDLMSTarget*>::iterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
    {
        delete (*it);
    }
    m_Parameters.clear();
}

CGXDLMSTarget& CGXDLMSParameterMonitor::GetChangedParameter()
{
    return m_ChangedParameter;
}
void CGXDLMSParameterMonitor::SetChangedParameter(CGXDLMSTarget& value)
{
    m_ChangedParameter = value;
}

/**
 * Capture time.
 */
CGXDateTime& CGXDLMSParameterMonitor::GetCaptureTime()
{
    return m_CaptureTime;
}

void CGXDLMSParameterMonitor::SetCaptureTime(CGXDateTime& value)
{
    m_CaptureTime = value;
}

/**
 * Changed Parameter
 */
std::vector<CGXDLMSTarget*>& CGXDLMSParameterMonitor::GetParameters()
{
    return m_Parameters;
}
void CGXDLMSParameterMonitor::SetParameters(std::vector<CGXDLMSTarget*>& value)
{
    m_Parameters = value;
}

// Returns amount of attributes.
int CGXDLMSParameterMonitor::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSParameterMonitor::GetMethodCount()
{
    return 2;
}

void CGXDLMSParameterMonitor::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string tmp;
    GetLogicalName(tmp);
    values.push_back(tmp);
    values.push_back(m_ChangedParameter.ToString());
    values.push_back(m_CaptureTime.ToString());
    tmp = "";
    for (std::vector<CGXDLMSTarget*>::iterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
    {
        tmp += (*it)->ToString();
    }
    values.push_back(tmp);
}

void CGXDLMSParameterMonitor::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // ChangedParameter
    if (all || CanRead(2)) {
        attributes.push_back(2);
    }
    // CaptureTime
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // Parameters
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
}

int CGXDLMSParameterMonitor::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index)
    {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_STRUCTURE;
        break;
    case 3:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 4:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSParameterMonitor::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    int ret;
    CGXDLMSVariant tmp;
    CGXByteBuffer data;
    switch (e.GetIndex())
    {
    case 1:
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        break;
    case 2:
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(4);
        if (m_ChangedParameter.GetTarget() == NULL)
        {
            unsigned char ln[6] = { 0 };
            tmp = 0;
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, tmp);
            tmp.Clear();
            tmp.Add(ln, 6);
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
            tmp = 1;
            GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, tmp);
            tmp.Clear();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_NONE, tmp);
        }
        else
        {
            tmp = m_ChangedParameter.GetTarget()->GetObjectType();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, tmp);
            tmp.Clear();
            if ((ret = GetLogicalName(m_ChangedParameter.GetTarget(), tmp)) != 0)
            {
                return ret;
            }
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
            tmp = m_ChangedParameter.GetAttributeIndex();
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, tmp)) != 0)
            {
                return ret;
            }
            tmp = m_ChangedParameter.GetValue();
            if ((ret = GXHelpers::SetData(data, tmp.vt, tmp)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
    }
    break;
    case 3:
        e.SetValue(m_CaptureTime);
        break;
    case 4:
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        data.SetUInt8((unsigned char)m_Parameters.size());
        for (std::vector<CGXDLMSTarget*>::iterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3);
            tmp = (*it)->GetTarget()->GetObjectType();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, tmp);
            tmp.Clear();
            if ((ret = GetLogicalName((*it)->GetTarget(), tmp)) != 0)
            {
                return ret;
            }
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
            tmp = (*it)->GetAttributeIndex();
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, tmp)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
    }
    break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int CGXDLMSParameterMonitor::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    int ret;
    CGXByteBuffer bb;
    CGXDLMSVariant tmp;
    switch (e.GetIndex())
    {
    case 1:
        return SetLogicalName(this, e.GetValue());
    case 2:
    {
        if (e.GetValue().Arr.size() != 4)
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        m_ChangedParameter.Clear();
        DLMS_OBJECT_TYPE type = (DLMS_OBJECT_TYPE)e.GetValue().Arr[0].ToInteger();
        unsigned char* ln = e.GetValue().Arr[1].byteArr;
        m_ChangedParameter.SetTarget(settings.GetObjects().FindByLN(type, ln));
        if (m_ChangedParameter.GetTarget() == NULL)
        {
            CGXDLMSObject* tmp = CGXDLMSObjectFactory::CreateObject(type);
            settings.AddAllocateObject(tmp);
            m_ChangedParameter.SetTarget(tmp);
            CGXDLMSObject::SetLogicalName(m_ChangedParameter.GetTarget(), e.GetValue().Arr[1]);
        }
        m_ChangedParameter.SetAttributeIndex(e.GetValue().Arr[2].ToInteger());
        m_ChangedParameter.SetValue(e.GetValue().Arr[3]);
        break;
    }
    case 3:
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            e.GetValue().GetBytes(bb);
            CGXDataInfo info;
            info.SetType(DLMS_DATA_TYPE_DATETIME);
            if ((ret = GXHelpers::GetData(bb, info, tmp)) != 0)
            {
                return ret;
            }
            m_CaptureTime = tmp.dateTime;
        }
        else
        {
            m_CaptureTime = e.GetValue().dateTime;
        }
        break;
    case 4:
    {
        for (std::vector<CGXDLMSTarget*>::iterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
        {
            delete (*it);
        }
        m_Parameters.clear();
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            if (it->Arr.size() != 3)
            {
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            DLMS_OBJECT_TYPE type = (DLMS_OBJECT_TYPE)it->Arr[0].ToInteger();
            unsigned char* ln = it->Arr[1].byteArr;
            CGXDLMSTarget* pObj = new CGXDLMSTarget();
            pObj->SetTarget(settings.GetObjects().FindByLN(type, ln));
            if (pObj->GetTarget() == NULL)
            {
                CGXDLMSObject* tmp = CGXDLMSObjectFactory::CreateObject(type);
                settings.AddAllocateObject(tmp);
                pObj->SetTarget(tmp);
                CGXDLMSObject::SetLogicalName(pObj->GetTarget(), it->Arr[1]);
            }
            pObj->SetAttributeIndex(it->Arr[2].ToInteger());
            m_Parameters.push_back(pObj);
        }
        break;
    }
    default:
        return DLMS_ERROR_CODE_READ_WRITE_DENIED;
    }
    return DLMS_ERROR_CODE_OK;
}
