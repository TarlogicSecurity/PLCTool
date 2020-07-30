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

#ifndef GXSNINFO_H
#define GXSNINFO_H

class CGXDLMSObject;

class CGXSNInfo
{
private:
    /**
     * Is attribute index or action index
     */
    bool m_Action;

    /**
     * Attribute index.
     */
    int m_Index;

    /**
     * COSEM object.
     */
    CGXDLMSObject* m_Item;

public:
    /**
     * Constructor.
     */
    CGXSNInfo();
    /**
     * @return The index
     */
    int GetIndex();

    /**
     * @param value
     *            The index to set
     */
    void SetIndex(int value);

    /**
     * @return Is action.
     */
    bool IsAction();

    /**
     * @param value
     *            Is action.
     */
    void SetAction(bool value);

    /**
     * @return The item
     */
    CGXDLMSObject* GetItem();

    /**
     * @param value
     *            The item to set
     */
    void SetItem(CGXDLMSObject* value);
};
#endif //GXSNINFO_H