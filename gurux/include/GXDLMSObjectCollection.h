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

#ifndef GXDLMSOBJECTCOLLECTION_H
#define GXDLMSOBJECTCOLLECTION_H

#include <vector>
#include "GXDLMSObject.h"
#include "GXXmlWriterSettings.h"

class CGXDLMSObjectCollection : public std::vector<CGXDLMSObject*>
{
public:
    ~CGXDLMSObjectCollection();

    CGXDLMSObject* FindByLN(DLMS_OBJECT_TYPE type, std::string& ln);

    CGXDLMSObject* FindByLN(DLMS_OBJECT_TYPE type, unsigned char ln[6]);

    CGXDLMSObject* FindBySN(unsigned short sn);

    void GetObjects(DLMS_OBJECT_TYPE type, CGXDLMSObjectCollection& items);

    void push_back(
        CGXDLMSObject* item);

    void Free();

    std::string ToString();

    /**
    * Save COSEM objects to the file.
    *
    * fileName: File name.
    */
    int Save(
        const char* fileName);

    /**
    * Save COSEM objects to the file.
    *
    * fileName: File name.
    * settings: XML write settings.
    */
    int Save(
        const char* fileName,
        CGXXmlWriterSettings& settings);

    /**
    * Load COSEM objects from the file.
    *
    * fileName XML file name.
    */
    int Load(
        const char* fileName);
};

#endif //GXDLMSOBJECTCOLLECTION_H
