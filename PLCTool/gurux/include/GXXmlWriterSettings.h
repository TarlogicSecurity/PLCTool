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

#ifndef GX_XML_WRITER_SETTINGS
#define GX_XML_WRITER_SETTINGS

class CGXXmlWriterSettings
{
    bool m_Values;
    bool m_SkipDefaults;
public:
    //Constructor
    CGXXmlWriterSettings();

    //Are attribute values also serialized.
    bool GetValues();

    //Are attribute values also serialized.
    void SetValues(bool value);

    //Are default values serialized.
    bool GetSkipDefaults();

    //Are default values serialized.
    void SetSkipDefaults(bool value);
};

#endif //GX_XML_WRITER_SETTINGS