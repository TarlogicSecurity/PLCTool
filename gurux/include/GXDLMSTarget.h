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

#ifndef GXDLMSTARGET_H
#define GXDLMSTARGET_H

#include "GXDLMSObject.h"

/**
*/
class CGXDLMSTarget
{
    friend class CGXDLMSParameterMonitor;
    /**
   * Target COSEM object.
   */
    CGXDLMSObject* m_Target;

    /**
     * Attribute Index of COSEM object.
     */
    unsigned char m_AttributeIndex;

    /**
     * Data index of COSEM object. All targets don't use this.
     */
    unsigned char m_DataIndex;

    /**
     * Data value of COSEM object. All targets don't use this.
     */
    CGXDLMSVariant m_Value;
public:
    // Constructor.
    CGXDLMSTarget();

    // Destructor.
    ~CGXDLMSTarget();

    void Clear();

    CGXDLMSObject* GetTarget();

    void SetTarget(CGXDLMSObject* value);

    /**
     * @return Attribute Index of COSEM object.
     */
    unsigned char GetAttributeIndex();

    /**
     * @param value
     *            Attribute Index of COSEM object.
     */
    void SetAttributeIndex(unsigned char value);

    /**
     * @return Data index of COSEM object. All targets don't use this.
     */
    unsigned char GetDataIndex();

    /**
     * @param value
     *            Data index of COSEM object. All targets don't use this.
     */
    void SetDataIndex(unsigned char value);

    /**
     * @return Data value of COSEM object. All targets don't use this.
     */
    CGXDLMSVariant& GetValue();

    /**
     * @param value
     *            Data value of COSEM object. All targets don't use this.
     */
    void SetValue(CGXDLMSVariant& value);

    std::string ToString();
};
#endif //GXDLMSTARGET_H