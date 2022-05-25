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

#include "../include/GXDLMSObject.h"
#include "../include/GXHelpers.h"

//SN Constructor.
CGXDLMSObject::CGXDLMSObject(DLMS_OBJECT_TYPE type, std::string& ln, unsigned short sn)
{
    Initialize(sn, type, 0, NULL);
    GXHelpers::SetLogicalName(ln.c_str(), m_LN);
}

//LN Constructor.
CGXDLMSObject::CGXDLMSObject(DLMS_OBJECT_TYPE type, std::string& ln)
{
    Initialize(0, type, 0, NULL);
    GXHelpers::SetLogicalName(ln.c_str(), m_LN);
}

CGXDLMSObject::CGXDLMSObject()
{
    Initialize(0, DLMS_OBJECT_TYPE_NONE, 0, NULL);
}

CGXDLMSObject::CGXDLMSObject(short sn, unsigned short class_id, unsigned char version, CGXByteBuffer& ln)
{
    Initialize(sn, class_id, version, &ln);
}

CGXDLMSObject::CGXDLMSObject(DLMS_OBJECT_TYPE type)
{
    Initialize(0, type, 0, NULL);
}

int CGXDLMSObject::GetLogicalName(CGXDLMSObject* target, CGXDLMSVariant& value)
{
    if (target == NULL)
    {
        unsigned char ln[6] = {0};
        value.Add(ln, 6);
    }
    else
    {
        value.Add(target->m_LN, 6);
    }
    value.vt = DLMS_DATA_TYPE_OCTET_STRING;
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMSObject::SetLogicalName(CGXDLMSObject * target, CGXDLMSVariant& value)
{
    if (value.vt == DLMS_DATA_TYPE_STRING)
    {
        return GXHelpers::SetLogicalName(value.strVal.c_str(), target->m_LN);
    }
    if (value.vt != DLMS_DATA_TYPE_OCTET_STRING || value.GetSize() != 6)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    memcpy(target->m_LN, value.byteArr, 6);
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMSObject::SetLogicalName(CGXDLMSObject* target, std::string& value)
{
    return GXHelpers::SetLogicalName(value.c_str(), target->m_LN);
}

void CGXDLMSObject::Initialize(short sn, unsigned short class_id, unsigned char version, CGXByteBuffer* ln)
{
    m_SN = sn;
    m_ObjectType = (DLMS_OBJECT_TYPE)class_id;
    m_Version = version;
    if (ln == NULL)
    {
        memset(m_LN, 0, 6);
    }
    else
    {
        int cnt = ln->GetSize();
        if (cnt == 6)
        {
            ln->Get(m_LN, 6);
        }
        else
        {
            assert(false);
        }
    }
    //Attribute 1 is always Logical name.
    m_Attributes.push_back(CGXDLMSAttribute(1, DLMS_DATA_TYPE_OCTET_STRING, DLMS_DATA_TYPE_OCTET_STRING));
}

CGXDLMSObject::~CGXDLMSObject(void)
{
    m_Attributes.clear();
    m_MethodAttributes.clear();
}

CGXDLMSVariant CGXDLMSObject::GetName()
{
    if (m_SN != 0)
    {
        return CGXDLMSVariant(m_SN);
    }
    CGXDLMSVariant ln;
    GXHelpers::GetLogicalName(m_LN, ln.strVal);
    ln.vt = DLMS_DATA_TYPE_STRING;
    return ln;
}

int CGXDLMSObject::SetName(CGXDLMSVariant& value)
{
    if (value.vt == DLMS_DATA_TYPE_UINT16)
    {
        m_SN = value.uiVal;
        return DLMS_ERROR_CODE_OK;
    }
    if (value.vt == DLMS_DATA_TYPE_STRING)
    {
        GXHelpers::SetLogicalName(value.strVal.c_str(), m_LN);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

DLMS_OBJECT_TYPE CGXDLMSObject::GetObjectType()
{
    return m_ObjectType;
}

int CGXDLMSObject::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index < 1)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    for (std::vector<CGXDLMSAttribute>::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            type = (*it).GetDataType();
            return DLMS_ERROR_CODE_OK;
        }
    }
    type = DLMS_DATA_TYPE_NONE;
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMSObject::SetDataType(int index, DLMS_DATA_TYPE type)
{
    for (CGXAttributeCollection::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            (*it).SetDataType(type);
            return DLMS_ERROR_CODE_OK;
        }
    }
    CGXDLMSAttribute att(index);
    att.SetDataType(type);
    m_Attributes.push_back(att);
    return DLMS_ERROR_CODE_OK;
}

DLMS_ACCESS_MODE CGXDLMSObject::GetAccess(int index)
{
    //LN is read only.
    if (index == 1)
    {
        return DLMS_ACCESS_MODE_READ;
    }
    for (CGXAttributeCollection::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            return (*it).GetAccess();
        }
    }
    return DLMS_ACCESS_MODE_READ_WRITE;
}

// Set attribute access.
void CGXDLMSObject::SetAccess(int index, DLMS_ACCESS_MODE access)
{
    for (CGXAttributeCollection::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            (*it).SetAccess(access);
            return;
        }
    }
    CGXDLMSAttribute att(index);
    att.SetAccess(access);
    m_Attributes.push_back(att);
}

DLMS_METHOD_ACCESS_MODE CGXDLMSObject::GetMethodAccess(int index)
{
    for (CGXAttributeCollection::iterator it = m_MethodAttributes.begin(); it != m_MethodAttributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            return (*it).GetMethodAccess();
        }
    }
    return DLMS_METHOD_ACCESS_MODE_ACCESS;
}

void CGXDLMSObject::SetMethodAccess(int index, DLMS_METHOD_ACCESS_MODE access)
{
    for (CGXAttributeCollection::iterator it = m_MethodAttributes.begin(); it != m_MethodAttributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            (*it).SetMethodAccess(access);
            return;
        }
    }
    CGXDLMSAttribute att(index);
    att.SetMethodAccess(access);
    m_MethodAttributes.push_back(att);
}

int CGXDLMSObject::GetUIDataType(int index, DLMS_DATA_TYPE& type)
{
    for (CGXAttributeCollection::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            type = (*it).GetUIDataType();
            return DLMS_ERROR_CODE_OK;
        }
    }
    type = DLMS_DATA_TYPE_NONE;
    return DLMS_ERROR_CODE_OK;
}

void CGXDLMSObject::SetUIDataType(int index, DLMS_DATA_TYPE type)
{
    for (CGXAttributeCollection::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            return (*it).SetUIDataType(type);
        }
    }
    CGXDLMSAttribute att(index);
    att.SetUIDataType(type);
    m_Attributes.push_back(att);
}

unsigned short CGXDLMSObject::GetShortName()
{
    return m_SN;
}

void CGXDLMSObject::SetShortName(unsigned short value)
{
    m_SN = value;
}

void CGXDLMSObject::GetLogicalName(std::string& ln)
{
    GXHelpers::GetLogicalName(m_LN, ln);
}

void CGXDLMSObject::SetVersion(unsigned short value)
{
    m_Version = value;
}

unsigned short CGXDLMSObject::GetVersion()
{
    return m_Version;
}

CGXAttributeCollection& CGXDLMSObject::GetAttributes()
{
    return m_Attributes;
}

CGXAttributeCollection& CGXDLMSObject::GetMethodAttributes()
{
    return m_MethodAttributes;
}

//Get Object's Logical Name.
std::string& CGXDLMSObject::GetDescription()
{
    return m_Description;
}

//Set Object's Logical Name.
void CGXDLMSObject::SetDescription(std::string& value)
{
    m_Description = value;
}

bool CGXDLMSObject::IsRead(int index)
{
    if (!CanRead(index))
    {
        return true;
    }
    return m_ReadTimes.find(index) != m_ReadTimes.end();
}

bool CGXDLMSObject::CanRead(int index)
{
    return GetAccess(index) != DLMS_ACCESS_MODE_NONE;
}