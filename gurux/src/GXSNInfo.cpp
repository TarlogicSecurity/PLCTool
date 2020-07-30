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

#include "../include/GXSNInfo.h"

CGXSNInfo::CGXSNInfo()
{
    m_Action = false;
    m_Index = 0;
    m_Item = 0;
}

int CGXSNInfo::GetIndex()
{
    return m_Index;
}

void CGXSNInfo::SetIndex(int value)
{
    m_Index = value;
}

bool CGXSNInfo::IsAction()
{
    return m_Action;
}

void CGXSNInfo::SetAction(bool value)
{
    m_Action = value;
}

CGXDLMSObject* CGXSNInfo::GetItem()
{
    return m_Item;
}

void CGXSNInfo::SetItem(CGXDLMSObject* value)
{
    m_Item = value;
}