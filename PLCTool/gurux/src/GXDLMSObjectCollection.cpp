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

#include <stdio.h>
#include "../include/GXDLMSObjectCollection.h"
#include "../include/GXDLMSConverter.h"

#include "../include/GXXmlWriter.h"
#include "../include/GXXmlReader.h"
#include "../include/GXDLMSObjectFactory.h"

CGXDLMSObjectCollection::~CGXDLMSObjectCollection()
{
}

CGXDLMSObject* CGXDLMSObjectCollection::FindByLN(DLMS_OBJECT_TYPE type, std::string& ln)
{
    unsigned char tmp[6];
    GXHelpers::SetLogicalName(ln.c_str(), tmp);
    for (CGXDLMSObjectCollection::iterator it = this->begin(); it != end(); ++it)
    {
        if ((type == DLMS_OBJECT_TYPE_ALL || (*it)->GetObjectType() == type))
        {
            if (memcmp(tmp, (*it)->m_LN, 6) == 0)
            {
                return *it;
            }
        }
    }
    return NULL;
}

CGXDLMSObject* CGXDLMSObjectCollection::FindByLN(DLMS_OBJECT_TYPE type, unsigned char ln[6])
{
    for (CGXDLMSObjectCollection::iterator it = this->begin(); it != end(); ++it)
    {
        if (type == DLMS_OBJECT_TYPE_ALL || (*it)->GetObjectType() == type)
        {
            if (memcmp(ln, (*it)->m_LN, 6) == 0)
            {
                return *it;
            }
        }
    }
    return NULL;
}

CGXDLMSObject* CGXDLMSObjectCollection::FindBySN(unsigned short sn)
{
    for (CGXDLMSObjectCollection::iterator it = begin(); it != end(); ++it)
    {
        if ((*it)->GetShortName() == sn)
        {
            return *it;
        }
    }
    return NULL;
}

void CGXDLMSObjectCollection::GetObjects(DLMS_OBJECT_TYPE type, CGXDLMSObjectCollection& items)
{
    for (CGXDLMSObjectCollection::iterator it = begin(); it != end(); ++it)
    {
        if ((*it)->GetObjectType() == type)
        {
            items.push_back(*it);
        }
    }
}

void CGXDLMSObjectCollection::push_back(CGXDLMSObject* item)
{
    std::vector<CGXDLMSObject*>::push_back(item);
}

void CGXDLMSObjectCollection::Free()
{
    for (CGXDLMSObjectCollection::iterator it = begin(); it != end(); ++it)
    {
        delete (*it);
    }
    std::vector<CGXDLMSObject*>::clear();
}

std::string CGXDLMSObjectCollection::ToString()
{
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (CGXDLMSObjectCollection::iterator it = begin(); it != end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = (*it)->GetName().ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    return sb.str();
}

int CGXDLMSObjectCollection::Save(const char* fileName)
{
    CGXXmlWriterSettings settings;
    return Save(fileName, settings);
}

int CGXDLMSObjectCollection::Save(const char* fileName, CGXXmlWriterSettings& settings)
{
    int ret;
    std::string ln;
#if defined(_WIN32) || defined(_WIN64)//Windows
    FILE* f = NULL;
    if (fopen_s(&f, fileName, "w") != 0)
    {
        return errno;
    }
#else
    FILE* f = fopen(fileName, "w");
    if (f == NULL)
    {
        return errno;
    }
#endif
    CGXXmlWriter writer(f, settings.GetSkipDefaults());
    if ((ret = writer.WriteStartDocument()) == 0 &&
        (ret = writer.WriteStartElement("Objects")) == 0)
    {
        for (CGXDLMSObjectCollection::iterator it = begin(); it != end(); ++it)
        {
            if ((ret = writer.WriteStartElement(CGXDLMSConverter::ToString((*it)->GetObjectType()))) != 0)
            {
                break;
            }
            // Add SN
            if ((*it)->GetShortName() != 0)
            {
                if ((ret = writer.WriteElementString("SN", (*it)->GetShortName())) != 0)
                {
                    break;
                }
            }
            // Add LN
            (*it)->GetLogicalName(ln);
            if ((ret = writer.WriteElementString("LN", ln)) != 0)
            {
                break;
            }
            // Add Version
            if ((*it)->GetVersion() != 0)
            {
                if ((ret = writer.WriteElementString("Version", (*it)->GetVersion())) != 0)
                {
                    break;
                }
            }
            // Add description if given.
            std::string& d = (*it)->GetDescription();
            if (!d.empty())
            {
                if ((ret = writer.WriteElementString("Description", d)) != 0)
                {
                    break;
                }
            }
            if (settings.GetValues())
            {
                if ((ret = writer.Save(*it)) != 0)
                {
                    break;
                }
            }
            // Close object.
            if ((ret = writer.WriteEndElement()) != 0)
            {
                break;
            }
        }
        if (ret == 0)
        {
            if ((ret = writer.WriteEndElement()) != 0 ||
                (ret = writer.WriteEndDocument()) != 0)
            {

            }
        }
    }
    if (f != NULL)
    {
        fclose(f);
    }
    return ret;
}

int CGXDLMSObjectCollection::Load(const char* fileName)
{
    int ret = 0;
    CGXDLMSObject* obj = NULL;
    std::string  target;
    DLMS_OBJECT_TYPE type = DLMS_OBJECT_TYPE_NONE;
#if defined(_WIN32) || defined(_WIN64)//Windows
    FILE* f = NULL;
    if (fopen_s(&f, fileName, "r") != 0)
    {
        return errno;
    }
#else
    FILE* f = fopen(fileName, "r");
    if (f == NULL)
    {
        return errno;
    }
#endif
    CGXXmlReader reader(f);
    while (!reader.IsEOF())
    {
        if (reader.IsStartElement())
        {
            target = reader.GetName();
            if (target == "Objects")
            {
                // Skip.
                reader.Read();
            }
            else if (target.find("GXDLMS", 0, 5) == 0)
            {
                type = CGXDLMSConverter::ValueOfObjectType(target.c_str());
                if (type == DLMS_OBJECT_TYPE_NONE)
                {
                    //Invalid object type.
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                reader.Read();
                obj = CGXDLMSObjectFactory::CreateObject(type);
                this->push_back(obj);
            }
            else if (target == "SN")
            {
                if (obj != NULL)
                {
                    obj->SetShortName(reader.ReadElementContentAsInt("SN"));
                }
            }
            else if (target == "LN")
            {
                if (obj != NULL)
                {
                    if ((ret = CGXDLMSObject::SetLogicalName(obj, reader.ReadElementContentAsString("LN"))) != 0)
                    {
                        break;
                    }
                    obj = NULL;
                }
            }
            else if (target == "Description")
            {
                if (obj != NULL)
                {
                    obj->SetDescription(reader.ReadElementContentAsString("Description"));
                }
            }
            else if (target == "Version")
            {
                if (obj != NULL)
                {
                    obj->SetVersion(reader.ReadElementContentAsInt("Version"));
                }
            }
            else
            {
                obj = NULL;
            }
        }
        else
        {
            reader.Read();
        }
    }
    if (f != NULL)
    {
        fclose(f);
    }
    return ret;
}
