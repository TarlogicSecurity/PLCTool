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
#ifndef GXDLMSREGISTER_H
#define GXDLMSREGISTER_H

#include "GXDLMSObject.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSRegister
*/
class CGXDLMSRegister : public CGXDLMSObject
{
protected:
    CGXDLMSVariant m_Value;
    signed char m_Scaler;
    unsigned char m_Unit;

    //SN Constructor.
    CGXDLMSRegister(DLMS_OBJECT_TYPE type, std::string ln, unsigned short sn);

    bool IsRead(int index);

public:
    //Constructor.
    CGXDLMSRegister(void);

    //SN Constructor.
    CGXDLMSRegister(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSRegister(std::string ln);

    /// <summary>
    /// Get value of COSEM Data object.
    /// </summary>
    CGXDLMSVariant& GetValue();

    /// <summary>
    /// Set value of COSEM Data object.
    /// </summary>
    void SetValue(CGXDLMSVariant& value);

    // Scaler of COSEM Register object.
    double GetScaler();

    virtual void SetScaler(double value);

    // Unit of COSEM Register object.
    int GetUnit();

    void SetUnit(unsigned char value);

    /*
     * Reset value.
     */
    void Reset();

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    /////////////////////////////////////////////////////////////////////////
    //Reset the register value.
    int Reset(
        CGXDLMSClient* client,
        std::vector<CGXByteBuffer>& reply);

    int Invoke(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    int GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    int SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);
};
#endif //GXDLMSREGISTER_H