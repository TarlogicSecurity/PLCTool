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

#include "../include/GXDLMSProfileGeneric.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXDLMSObjectFactory.h"
#include "../include/GXDLMSDemandRegister.h"
#include "../include/GXDLMSServer.h"

CGXDLMSProfileGeneric::~CGXDLMSProfileGeneric()
{
    for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator it = m_CaptureObjects.begin();
        it != m_CaptureObjects.end(); ++it)
    {
        delete it->second;
    }
    m_CaptureObjects.clear();
    m_Buffer.clear();
}

int CGXDLMSProfileGeneric::GetSelectedColumns(
    int selector,
    CGXDLMSVariant& parameters,
    std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >& columns)
{
    columns.clear();
    if (selector == 0)
    {
        // Return all rows.
        columns.insert(columns.end(), m_CaptureObjects.begin(), m_CaptureObjects.end());
        return 0;
    }
    else if (selector == 1)
    {
        return CGXDLMSProfileGeneric::GetSelectedColumns(parameters.Arr.at(3).Arr, columns);
    }
    else if (selector == 2)
    {
        int colStart = 1;
        int colCount = 0;
        if (parameters.Arr.size() > 2)
        {
            colStart = parameters.Arr[2].ToInteger();
        }
        if (parameters.Arr.size() > 3)
        {
            colCount = parameters.Arr[3].ToInteger();
        }
        else if (colStart != 1)
        {
            colCount = (int)m_CaptureObjects.size();
        }
        if (colStart != 1 || colCount != 0)
        {
            // Return all rows.
            columns.insert(columns.end(), m_CaptureObjects.begin() + colStart - 1,
                m_CaptureObjects.begin() + colStart + colCount - 1);

        }
        else
        {
            // Return all rows.
            columns.insert(columns.end(), m_CaptureObjects.begin(), m_CaptureObjects.end());
        }
        return 0;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return 0;
}
int CGXDLMSProfileGeneric::GetColumns(CGXByteBuffer& data)
{
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
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, tmp)) != 0) //ClassID
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
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0 || //LN
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, ai)) != 0 || //Attribute Index
            (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, di)) != 0) //Data Index
        {
            return ret;
        }
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMSProfileGeneric::GetData(
    CGXDLMSSettings& settings,
    CGXDLMSValueEventArg& e,
    std::vector< std::vector<CGXDLMSVariant> >& table,
    std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >& columns,
    CGXByteBuffer& data)
{
    if (settings.GetIndex() == 0) {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        if (e.GetRowEndIndex() != 0) {
            GXHelpers::SetObjectCount(e.GetRowEndIndex() - e.GetRowBeginIndex(), data);
        }
        else {
            GXHelpers::SetObjectCount((unsigned long)table.size(), data);
        }
    }

    std::vector<DLMS_DATA_TYPE> types;
    DLMS_DATA_TYPE type;
    int ret;
    for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator it = m_CaptureObjects.begin();
        it != m_CaptureObjects.end(); ++it)
    {
        if ((ret = (*it).first->GetDataType((*it).second->GetAttributeIndex(), type)) != 0)
        {
            return ret;
        }
        types.push_back(type);
    }
    for (std::vector< std::vector<CGXDLMSVariant> >::iterator row = table.begin(); row != table.end(); ++row)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        if (columns.size() == 0)
        {
            GXHelpers::SetObjectCount((unsigned long)(*row).size(), data);
        }
        else
        {
            GXHelpers::SetObjectCount((unsigned long)columns.size(), data);
        }
        int pos = -1;
        for (std::vector<CGXDLMSVariant>::iterator value = (*row).begin(); value != (*row).end(); ++value)
        {
            DLMS_DATA_TYPE tp = types[++pos];
            if (tp == DLMS_DATA_TYPE_NONE)
            {
                tp = value->vt;
                types[pos] = tp;
            }

            if (columns.size() == 0 ||
                std::find(columns.begin(), columns.end(), m_CaptureObjects.at(pos)) != columns.end())
            {
                if ((ret = GXHelpers::SetData(data, tp, *value)) != 0)
                {
                    return ret;
                }
            }
        }
        settings.SetIndex(settings.GetIndex() + 1);
    }
    if (e.GetRowEndIndex() != 0)
    {
        e.SetRowBeginIndex(e.GetRowBeginIndex() + (unsigned short)table.size());
    }
    return DLMS_ERROR_CODE_OK;
}

/*
* Add new capture object (column) to the profile generic.
*/
int CGXDLMSProfileGeneric::AddCaptureObject(CGXDLMSObject* pObj, int attributeIndex, int dataIndex)
{
    if (pObj == NULL)
    {
        //Invalid Object
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (attributeIndex < 1)
    {
        //Invalid attribute index
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (dataIndex < 0)
    {
        //Invalid data index
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    CGXDLMSCaptureObject* pCO = new CGXDLMSCaptureObject(attributeIndex, dataIndex);
    m_CaptureObjects.push_back(std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*>(pObj, pCO));
    return DLMS_ERROR_CODE_OK;
}

/**
     * Get selected columns.
     *
     * @param cols
     *            selected columns.
     * @return Selected columns.
     */
int CGXDLMSProfileGeneric::GetSelectedColumns(
    std::vector<CGXDLMSVariant>& cols,
    std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >& columns)
{
    int dataIndex;
    unsigned char attributeIndex;
    std::string ln, ln2;
    for (std::vector<CGXDLMSVariant>::iterator it = cols.begin(); it != cols.end(); ++it)
    {
        DLMS_OBJECT_TYPE ot = (DLMS_OBJECT_TYPE)it->Arr[0].ToInteger();
        GXHelpers::GetLogicalName(it->Arr[1].byteArr, ln);
        attributeIndex = it->Arr[2].ToInteger();
        dataIndex = it->Arr[3].ToInteger();
        // Find columns and update only them.
        for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator c = m_CaptureObjects.begin(); c != m_CaptureObjects.end(); ++c)
        {
            c->first->GetLogicalName(ln2);
            if (c->first->GetObjectType() == ot &&
                c->second->GetAttributeIndex() == attributeIndex &&
                c->second->GetDataIndex() == dataIndex &&
                ln2.compare(ln) == 0)
            {
                columns.push_back(*c);
                break;
            }
        }
    }
    return 0;
}

int CGXDLMSProfileGeneric::GetProfileGenericData(
    CGXDLMSSettings& settings, CGXDLMSValueEventArg& e, CGXByteBuffer& reply)
{
    std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> > columns;
    //If all data is read.
    if (e.GetSelector() == 0 || e.GetParameters().vt == DLMS_DATA_TYPE_NONE || e.GetRowEndIndex() != 0)
    {
        return GetData(settings, e, GetBuffer(), columns, reply);
    }
    std::vector< std::vector<CGXDLMSVariant> >& table = GetBuffer();
    std::vector< std::vector<CGXDLMSVariant> > items;
    if (e.GetSelector() == 1) //Read by range
    {
        int ret;
        CGXDLMSVariant value;
        if ((ret = CGXDLMSClient::ChangeType(e.GetParameters().Arr[1], DLMS_DATA_TYPE_DATETIME, value)) != 0)
        {
            return ret;
        }
        struct tm tmp = value.dateTime.GetValue();
        time_t start = mktime(&tmp);
        value.Clear();
        if ((ret = CGXDLMSClient::ChangeType(e.GetParameters().Arr[2], DLMS_DATA_TYPE_DATETIME, value)) != 0)
        {
            return ret;
        }
        tmp = value.dateTime.GetValue();
        time_t end = mktime(&tmp);
        value.Clear();

        if (e.GetParameters().Arr.size() > 3)
        {
            ret = GetSelectedColumns(e.GetParameters().Arr[3].Arr, columns);
        }
        for (std::vector< std::vector<CGXDLMSVariant> >::iterator row = table.begin(); row != table.end(); ++row)
        {
            tmp = (*row)[0].dateTime.GetValue();
            time_t tm = mktime(&tmp);
            if (tm >= start && tm <= end)
            {
                items.push_back(*row);
            }
        }
    }
    else if (e.GetSelector() == 2) //Read by entry.
    {
        int start = e.GetParameters().Arr[0].ToInteger();
        int count = e.GetParameters().Arr[1].ToInteger();
        if (start == 0)
        {
            start = 1;
        }
        if (count == 0)
        {
            count = (int)table.size();
        }
        if ((unsigned long)(start + count - 1) > table.size())
        {
            count = (int)table.size();
        }

        int colStart = 1;
        int colCount = 0;
        if (e.GetParameters().Arr.size() > 2)
        {
            colStart = e.GetParameters().Arr[2].ToInteger();
        }
        if (e.GetParameters().Arr.size() > 3)
        {
            colCount = e.GetParameters().Arr[3].ToInteger();
        }
        else if (colStart != 1)
        {
            colCount = (int)m_CaptureObjects.size();
        }
        if (colStart != 1 || colCount != 0)
        {
            columns.insert(columns.end(), m_CaptureObjects.begin() + colStart - 1,
                m_CaptureObjects.begin() + colStart + colCount - 1);
        }
        //Get rows.
        // Starting index is 1.
        for (int pos = 0; pos < count; ++pos)
        {
            if ((unsigned int)(pos + start - 1) == table.size())
            {
                break;
            }
            items.push_back(table[start + pos - 1]);
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return GetData(settings, e, items, columns, reply);
}

/**
 Constructor.
*/
CGXDLMSProfileGeneric::CGXDLMSProfileGeneric() :
    CGXDLMSProfileGeneric("", 0)
{
}

//SN Constructor.
CGXDLMSProfileGeneric::CGXDLMSProfileGeneric(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_PROFILE_GENERIC, ln, sn)
{
    m_SortObjectAttributeIndex = 0;
    m_SortObjectDataIndex = 0;
    m_SortObject = NULL;
    m_CapturePeriod = 3600;
    m_EntriesInUse = m_ProfileEntries = 0;
    m_SortMethod = DLMS_SORT_METHOD_FIFO;
}

/**
 Constructor.

 @param ln Logical Name of the object.
*/
CGXDLMSProfileGeneric::CGXDLMSProfileGeneric(std::string ln) :
    CGXDLMSProfileGeneric(ln, 0)
{
}

/**
 Data of profile generic.
*/
std::vector< std::vector<CGXDLMSVariant> >& CGXDLMSProfileGeneric::GetBuffer()
{
    return m_Buffer;
}

/**
 Captured Objects.
*/
std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >& CGXDLMSProfileGeneric::GetCaptureObjects()
{
    return m_CaptureObjects;
}

/**
 How often values are captured.
*/
int CGXDLMSProfileGeneric::GetCapturePeriod()
{
    return m_CapturePeriod;
}
void CGXDLMSProfileGeneric::SetCapturePeriod(int value)
{
    m_CapturePeriod = value;
}

/**
 How columns are sorted.
*/
GX_SORT_METHOD CGXDLMSProfileGeneric::GetSortMethod()
{
    return m_SortMethod;
}
void CGXDLMSProfileGeneric::SetSortMethod(GX_SORT_METHOD value)
{
    m_SortMethod = value;
}

CGXDLMSObject* CGXDLMSProfileGeneric::GetSortObject()
{
    return m_SortObject;
}
void CGXDLMSProfileGeneric::SetSortObject(CGXDLMSObject* value)
{
    m_SortObject = value;
}

unsigned long CGXDLMSProfileGeneric::GetEntriesInUse()
{
    return m_EntriesInUse;
}

void CGXDLMSProfileGeneric::SetEntriesInUse(unsigned long value)
{
    m_EntriesInUse = value;
}

unsigned long CGXDLMSProfileGeneric::GetProfileEntries()
{
    return m_ProfileEntries;
}

void CGXDLMSProfileGeneric::SetProfileEntries(unsigned long value)
{
    m_ProfileEntries = value;
}

int CGXDLMSProfileGeneric::GetSortObjectAttributeIndex()
{
    return m_SortObjectAttributeIndex;
}
void CGXDLMSProfileGeneric::SetSortObjectAttributeIndex(int value)
{
    m_SortObjectAttributeIndex = value;
}

int CGXDLMSProfileGeneric::GetSortObjectDataIndex()
{
    return m_SortObjectDataIndex;
}

void CGXDLMSProfileGeneric::SetSortObjectDataIndex(int value)
{
    m_SortObjectDataIndex = value;
}

void CGXDLMSProfileGeneric::Reset()
{
    m_Buffer.erase(m_Buffer.begin(), m_Buffer.end());
    m_EntriesInUse = 0;
}

/*
* Copies the values of the objects to capture into the buffer by reading
* capture objects.
*/
int CGXDLMSProfileGeneric::Capture(CGXDLMSServer* server)
{
    std::vector<CGXDLMSVariant> values;
    int ret;
    CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(server, this, 2);
    CGXDLMSValueEventCollection args;
    args.push_back(e);
    server->PreGet(args);
    if (!e->GetHandled())
    {
        for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator it = m_CaptureObjects.begin();
            it != m_CaptureObjects.end(); ++it)
        {
            CGXDLMSValueEventArg tmp(server, it->first, it->second->GetAttributeIndex());
            if ((ret = it->first->GetValue(server->GetSettings(), tmp)) != 0)
            {
                return ret;
            }
            values.push_back(tmp.GetValue());
        }
        // Remove first items if buffer is full.
        if (GetProfileEntries() == GetBuffer().size())
        {
            m_Buffer.pop_back();
        }
        m_Buffer.push_back(values);
        m_EntriesInUse = (unsigned long)m_Buffer.size();
    }
    server->PostGet(args);
    return 0;
}

int CGXDLMSProfileGeneric::Reset(CGXDLMSClient& client, std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant value = (unsigned char)0;
    return client.Method(this, 1, value, reply);
}

int CGXDLMSProfileGeneric::Capture(CGXDLMSClient& client, std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant value = (unsigned char)0;
    return client.Method(this, 2, value, reply);
}

void CGXDLMSProfileGeneric::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    std::stringstream sb;
    bool empty = true;
    for (std::vector< std::vector<CGXDLMSVariant> >::iterator row = m_Buffer.begin(); row != m_Buffer.end(); ++row)
    {
        for (std::vector<CGXDLMSVariant>::iterator cell = row->begin(); cell != row->end(); ++cell)
        {
            sb << cell->ToString();
            sb << " | ";
        }
        sb << "\r\n";
    }
    values.push_back(sb.str());
    //Clear str.
    sb.str(std::string());
    sb << '[';
    empty = true;
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

    values.push_back(CGXDLMSVariant(m_CapturePeriod).ToString());
    values.push_back(CGXDLMSVariant(m_SortMethod).ToString());
    if (m_SortObject == NULL)
    {
        values.push_back("");
    }
    else
    {
        values.push_back(m_SortObject->GetName().ToString());
    }
    values.push_back(CGXDLMSVariant(m_EntriesInUse).ToString());
    values.push_back(CGXDLMSVariant(m_ProfileEntries).ToString());
}

void CGXDLMSProfileGeneric::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Buffer
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //CaptureObjects
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //CapturePeriod
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //SortMethod
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
    //SortObject
    if (all || !IsRead(6))
    {
        attributes.push_back(6);
    }
    //EntriesInUse
    if (all || !IsRead(7))
    {
        attributes.push_back(7);
    }
    //ProfileEntries
    if (all || !IsRead(8))
    {
        attributes.push_back(8);
    }
}

//Returns amount of attributes.
int CGXDLMSProfileGeneric::GetAttributeCount()
{
    return 8;
}

//Returns amount of methods.
int CGXDLMSProfileGeneric::GetMethodCount()
{
    return 2;
}

int CGXDLMSProfileGeneric::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_UINT32;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_ENUM;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 6)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 7)
    {
        type = DLMS_DATA_TYPE_UINT32;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 8)
    {
        type = DLMS_DATA_TYPE_UINT32;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}


int CGXDLMSProfileGeneric::Invoke(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1) {
        // Reset.
        Reset();
    }
    else if (e.GetIndex() == 2) {
        // Capture.
        Capture(e.GetServer());
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return 0;
}

/*
* Returns value of given attribute.
*/
int CGXDLMSProfileGeneric::GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
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
    }
    else if (e.GetIndex() == 2)
    {
        CGXByteBuffer tmp;
        int ret = GetProfileGenericData(settings, e, tmp);
        e.SetByteArray(true);
        e.SetValue(tmp);
        return ret;
    }
    else if (e.GetIndex() == 3)
    {
        CGXByteBuffer data;
        int ret = GetColumns(data);
        e.SetByteArray(true);
        e.SetValue(data);
        return ret;
    }
    else if (e.GetIndex() == 4)
    {
        e.SetValue(GetCapturePeriod());
    }
    else if (e.GetIndex() == 5)
    {
        e.SetValue(GetSortMethod());
    }
    else if (e.GetIndex() == 6)
    {
        e.SetByteArray(true);
        static char empty[6] = { 0 };
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(4);
        if (m_SortObject == NULL)
        {
            //ClassID
            data.SetUInt8(DLMS_DATA_TYPE_UINT16);
            data.SetUInt16(0);
            //LN
            data.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            data.SetUInt8(6);
            data.Set(empty, 6);
            //Selected Attribute Index
            data.SetUInt8(DLMS_DATA_TYPE_INT8);
            data.SetUInt8(0);
            //Selected Data Index
            data.SetUInt8(DLMS_DATA_TYPE_UINT16);
            data.SetUInt16(0);
        }
        else
        {
            int ret;
            CGXDLMSVariant ln;
            //ClassID
            data.SetUInt8(DLMS_DATA_TYPE_UINT16);
            data.SetUInt16(m_SortObject->GetObjectType());
            //LN
            data.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            data.SetUInt8(6);
            if ((ret = GetLogicalName(m_SortObject, ln)) != 0)
            {
                return ret;
            }
            data.Set(ln.byteArr, 6);
            //Selected Attribute Index
            data.SetUInt8(DLMS_DATA_TYPE_INT8);
            data.SetUInt8(m_SortObjectAttributeIndex);
            //Selected Data Index
            data.SetUInt8(DLMS_DATA_TYPE_UINT16);
            data.SetUInt16(m_SortObjectDataIndex);
        }
        e.SetValue(data);
    }
    else if (e.GetIndex() == 7)
    {
        e.SetValue(GetEntriesInUse());
    }
    else if (e.GetIndex() == 8)
    {
        e.SetValue(GetProfileEntries());
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

/*
 * Set value of given attribute.
 */
int CGXDLMSProfileGeneric::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    int ret;
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        if (m_CaptureObjects.size() == 0)
        {
            //Read capture objects first.
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        if (e.GetValue().vt != DLMS_DATA_TYPE_NONE)
        {
            std::vector<DLMS_DATA_TYPE> types;
            DLMS_DATA_TYPE type;
            for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator it = m_CaptureObjects.begin(); it != m_CaptureObjects.end(); ++it)
            {
                if ((ret = (*it).first->GetUIDataType((*it).second->GetAttributeIndex(), type)) != 0)
                {
                    return ret;
                }
                types.push_back(type);
            }

            for (std::vector<CGXDLMSVariant >::iterator row = e.GetValue().Arr.begin(); row != e.GetValue().Arr.end(); ++row)
            {
                if ((*row).Arr.size() != m_CaptureObjects.size())
                {
                    //Number of columns do not match.
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                CGXDLMSVariant data;
                for (unsigned int pos = 0; pos < (*row).Arr.size(); ++pos)
                {
                    DLMS_DATA_TYPE type = types[pos];
                    if (type != DLMS_DATA_TYPE_NONE && row->Arr[pos].vt == DLMS_DATA_TYPE_OCTET_STRING)
                    {
                        if ((ret = CGXDLMSClient::ChangeType(row->Arr[pos], type, data)) != 0)
                        {
                            return ret;
                        }
                        row->Arr[pos] = data;
                    }
                    std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> item = m_CaptureObjects[pos];
                    if ((item.first->GetObjectType() == DLMS_OBJECT_TYPE_REGISTER || item.first->GetObjectType() == DLMS_OBJECT_TYPE_EXTENDED_REGISTER)
                        && item.second->GetAttributeIndex() == 2)
                    {
                        double scaler = ((CGXDLMSRegister*)item.first)->GetScaler();
                        if (scaler != 1)
                        {
                            row->Arr[pos] = row->Arr[pos].ToDouble() * scaler;
                        }
                    }
                    else if (item.first->GetObjectType() == DLMS_OBJECT_TYPE_DEMAND_REGISTER &&
                        (item.second->GetAttributeIndex() == 2 || item.second->GetAttributeIndex() == 3))
                    {
                        double scaler = ((CGXDLMSDemandRegister*)item.first)->GetScaler();
                        if (scaler != 1)
                        {
                            row->Arr[pos] = row->Arr[pos].ToDouble() * scaler;
                        }
                    }
                }
                m_Buffer.push_back(row->Arr);
            }
        }
        m_EntriesInUse = (unsigned long)m_Buffer.size();
    }
    else if (e.GetIndex() == 3)
    {
        m_CaptureObjects.clear();
        m_Buffer.clear();
        m_EntriesInUse = 0;
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
                    pObj = CGXDLMSObjectFactory::CreateObject(type, ln);
                    settings.AddAllocateObject(pObj);
                }
                AddCaptureObject(pObj, (*it).Arr[2].ToInteger(), (*it).Arr[3].ToInteger());
            }
        }
    }
    else if (e.GetIndex() == 4)
    {
        m_CapturePeriod = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_SortMethod = (GX_SORT_METHOD)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 6)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_NONE)
        {
            m_SortObject = NULL;
        }
        else
        {
            if (e.GetValue().Arr.size() != 4)
            {
                //Invalid structure format.
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            DLMS_OBJECT_TYPE type = (DLMS_OBJECT_TYPE)e.GetValue().Arr[0].ToInteger();
            std::string ln;
            GXHelpers::GetLogicalName(e.GetValue().Arr[1].byteArr, ln);
            m_SortObjectAttributeIndex = e.GetValue().Arr[2].ToInteger();
            m_SortObjectDataIndex = e.GetValue().Arr[3].ToInteger();
            m_SortObject = settings.GetObjects().FindByLN(type, ln);
            if (m_SortObject == NULL)
            {
                m_SortObject = CGXDLMSObjectFactory::CreateObject(type, ln);
                settings.AddAllocateObject(m_SortObject);
            }
        }
    }
    else if (e.GetIndex() == 7)
    {
        m_EntriesInUse = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 8)
    {
        m_ProfileEntries = e.GetValue().ToInteger();
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
