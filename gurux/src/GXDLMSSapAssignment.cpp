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
#include "../include/GXDLMSSapAssignment.h"
#include <sstream>

CGXDLMSSapAssignment::CGXDLMSSapAssignment() :
    CGXDLMSSapAssignment("0.0.41.0.0.255", 0)
{
}


CGXDLMSSapAssignment::CGXDLMSSapAssignment(std::string ln) :
    CGXDLMSSapAssignment(ln, 0)
{
}


CGXDLMSSapAssignment::CGXDLMSSapAssignment(std::string ln, unsigned short sn) :
    CGXDLMSObject(DLMS_OBJECT_TYPE_SAP_ASSIGNMENT, ln, sn)
{

}

std::map<int, std::string >& CGXDLMSSapAssignment::GetSapAssignmentList()
{
    return m_SapAssignmentList;
}
void CGXDLMSSapAssignment::SetSapAssignmentList(std::map<int, std::string >& value)
{
    m_SapAssignmentList = value;
}

// Returns amount of attributes.
int CGXDLMSSapAssignment::GetAttributeCount()
{
    return 2;
}

// Returns amount of methods.
int CGXDLMSSapAssignment::GetMethodCount()
{
    return 1;
}

void CGXDLMSSapAssignment::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::map<int, std::string >::iterator it = m_SapAssignmentList.begin(); it != m_SapAssignmentList.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = CGXDLMSVariant((it->first)).ToString();
        sb.write(str.c_str(), str.size());
        sb << ", ";
        sb.write(it->second.c_str(), it->second.size());
    }
    sb << ']';
    values.push_back(sb.str());
}

void CGXDLMSSapAssignment::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //SapAssignmentList
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
}

int CGXDLMSSapAssignment::GetDataType(int index, DLMS_DATA_TYPE& type)
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
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}


// Returns value of given attribute.
int CGXDLMSSapAssignment::GetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
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
        unsigned long cnt = (unsigned long)m_SapAssignmentList.size();
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        GXHelpers::SetObjectCount(cnt, data);
        int ret;
        if (cnt != 0)
        {
            CGXDLMSVariant f, s;
            CGXByteBuffer bb;
            for (std::map<int, std::string >::iterator it = m_SapAssignmentList.begin();
                it != m_SapAssignmentList.end(); ++it)
            {
                data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
                data.SetUInt8(2); //Count
                f = it->first;
                bb.Clear();
                bb.AddString(it->second);
                s = bb;
                if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, f)) != 0 ||
                    (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, s)) != 0)
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

/*
 * Set value of given attribute.
 */
int CGXDLMSSapAssignment::SetValue(CGXDLMSSettings&, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    if (e.GetIndex() == 2)
    {
        m_SapAssignmentList.clear();
        for (std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin();
            item != e.GetValue().Arr.end(); ++item)
        {
            std::string str;
            if ((*item).Arr[1].vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                CGXDLMSVariant tmp;
                CGXDLMSClient::ChangeType((*item).Arr[1], DLMS_DATA_TYPE_STRING, tmp);
                str = tmp.strVal;
            }
            else
            {
                str = (*item).Arr[1].ToString();
            }
            m_SapAssignmentList[(*item).Arr[0].ToInteger()] = str;
        }
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}
