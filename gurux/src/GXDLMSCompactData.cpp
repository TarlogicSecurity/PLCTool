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

#include "../include/GXDLMSCompactData.h"
#include "../include/GXDLMSObjectFactory.h"
#include "../include/GXDLMSServer.h"

//Constructor.
CGXDLMSCompactData::CGXDLMSCompactData() :
    CGXDLMSCompactData("", 0)
{
}

//SN Constructor.
CGXDLMSCompactData::CGXDLMSCompactData(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_COMPACT_DATA, ln, sn)
{

}

//LN Constructor.
CGXDLMSCompactData::CGXDLMSCompactData(std::string ln) :
    CGXDLMSCompactData(ln, 0)
{

}

CGXDLMSCompactData::~CGXDLMSCompactData()
{
    for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator it = m_CaptureObjects.begin(); it != m_CaptureObjects.end(); ++it)
    {
        delete it->second;
    }
    m_CaptureObjects.clear();
}

CGXByteBuffer& CGXDLMSCompactData::GetBuffer() {
    return m_Buffer;
}

void CGXDLMSCompactData::SetBuffer(CGXByteBuffer& value) {
    m_Buffer = value;
}

std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >& CGXDLMSCompactData::GetCaptureObjects() {
    return m_CaptureObjects;
}

unsigned char CGXDLMSCompactData::GetTemplateId() {
    return m_TemplateId;
}

void CGXDLMSCompactData::SetTemplateId(unsigned char value) {
    m_TemplateId = value;
}

CGXByteBuffer& CGXDLMSCompactData::GetTemplateDescription() {
    return m_TemplateDescription;
}

void CGXDLMSCompactData::SetTemplateDescription(CGXByteBuffer& value) {
    m_TemplateDescription = value;
}

DLMS_CAPTURE_METHOD CGXDLMSCompactData::GetCaptureMethod() {
    return m_CaptureMethod;
}

void CGXDLMSCompactData::SetCaptureMethod(DLMS_CAPTURE_METHOD value) {
    m_CaptureMethod = value;
}

// Returns amount of attributes.
int CGXDLMSCompactData::GetAttributeCount()
{
    return 6;
}

// Returns amount of methods.
int CGXDLMSCompactData::GetMethodCount()
{
    return 2;
}

void CGXDLMSCompactData::GetValues(std::vector<std::string>& values)
{
    std::stringstream sb;
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_Buffer.ToString());
    //Clear str.
    sb.str(std::string());
    sb << '[';
    bool empty = true;
    for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator it = m_CaptureObjects.begin(); it != m_CaptureObjects.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = it->first->GetName().ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
    values.push_back(ln);
    values.push_back(GXHelpers::IntToString(m_TemplateId));
    values.push_back(m_TemplateDescription.ToString());
    values.push_back(GXHelpers::IntToString(m_CaptureMethod));
}

void CGXDLMSCompactData::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // Buffer
    if (all || CanRead(2)) {
        attributes.push_back(2);
    }
    // CaptureObjects
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // TemplateId
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
    // TemplateDescription
    if (all || CanRead(5)) {
        attributes.push_back(5);
    }
    // CaptureMethod
    if (all || CanRead(6)) {
        attributes.push_back(6);
    }
}

int CGXDLMSCompactData::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index) {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 3:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 4:
        type = DLMS_DATA_TYPE_UINT8;
        break;
    case 5:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 6:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}


int CGXDLMSCompactData::GetCaptureObjects(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    CGXByteBuffer data;
    unsigned long cnt = (unsigned long)m_CaptureObjects.size();
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    //Add count
    GXHelpers::SetObjectCount(cnt, data);
    std::string ln;
    int ret;
    CGXDLMSVariant tmp, ai, di;
    for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator it = m_CaptureObjects.begin(); it != m_CaptureObjects.end(); ++it)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(4); //Count
        tmp = it->first->GetObjectType();
        //ClassID
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, tmp)) != 0)
        {
            return ret;
        }
        tmp.Clear();
        if ((ret = GetLogicalName((*it).first, tmp)) != 0)
        {
            return ret;
        }
        ai = (*it).second->GetAttributeIndex();
        di = (*it).second->GetDataIndex();
        //LN
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0 ||
            //Attribute Index
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, ai)) != 0 ||
            //Data Index
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, di)) != 0)
        {
            return ret;
        }
    }
    e.SetValue(data);
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSCompactData::GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    switch (e.GetIndex())
    {
    case 1:
    {
        int ret;
        CGXDLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
    }
    break;
    case 2:
        e.SetValue(m_Buffer);
        break;
    case 3:
        return GetCaptureObjects(settings, e);
    case 4:
        e.SetValue(m_TemplateId);
        break;
    case 5:
        e.SetValue(m_TemplateDescription);
        break;
    case 6:
        e.SetValue(m_CaptureMethod);
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int CGXDLMSCompactData::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    switch (e.GetIndex()) {
    case 1:
        return SetLogicalName(this, e.GetValue());
    case 2:
        m_Buffer.Clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            m_Buffer.Set(e.GetValue().byteArr, e.GetValue().GetSize());
        }
        else if (e.GetValue().vt == DLMS_DATA_TYPE_STRING)
        {
            m_Buffer.AddString(e.GetValue().strVal);
        }
        break;
    case 3:
        for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator it = m_CaptureObjects.begin(); it != m_CaptureObjects.end(); ++it)
        {
            delete it->second;
        }
        m_CaptureObjects.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<CGXDLMSVariant >::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                if ((*it).Arr.size() != 4)
                {
                    //Invalid structure format.
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                DLMS_OBJECT_TYPE type = (DLMS_OBJECT_TYPE)(*it).Arr[0].ToInteger();
                std::string ln;
                GXHelpers::GetLogicalName((*it).Arr[1].byteArr, ln);
                CGXDLMSObject* pObj = settings.GetObjects().FindByLN(type, ln);
                if (pObj == NULL)
                {
                    continue;
                    pObj = CGXDLMSObjectFactory::CreateObject(type, ln);
                    if (pObj != NULL)
                    {
                        settings.AddAllocateObject(pObj);
                    }
                }
                CGXDLMSCaptureObject* pCO = new CGXDLMSCaptureObject(it->Arr[2].ToInteger(), it->Arr[3].ToInteger());
                m_CaptureObjects.push_back(std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*>(pObj, pCO));
            }
        }
        break;
    case 4:
        m_TemplateId = e.GetValue().ToInteger();
        break;
    case 5:
        m_TemplateDescription.Clear();
        m_TemplateDescription.Set(e.GetValue().byteArr, e.GetValue().GetSize());
        break;
    case 6:
        m_CaptureMethod = (DLMS_CAPTURE_METHOD)e.GetValue().ToInteger();
        break;
    default:
        return DLMS_ERROR_CODE_READ_WRITE_DENIED;
    }
    return DLMS_ERROR_CODE_OK;
}
