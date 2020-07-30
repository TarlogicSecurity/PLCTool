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

#ifndef GXDLMSDEMANDREGISTER_H
#define GXDLMSDEMANDREGISTER_H

#include "GXDLMSObject.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSDemandRegister
*/
class CGXDLMSDemandRegister : public CGXDLMSObject
{
    CGXDLMSVariant m_CurrentAverageValue;
    CGXDLMSVariant m_LastAverageValue;
    signed char m_Scaler;
    unsigned char m_Unit;
    CGXDLMSVariant m_Status;
    CGXDateTime m_CaptureTime;
    CGXDateTime m_StartTimeCurrent;
    int m_NumberOfPeriods;
    unsigned long m_Period;

protected:
    bool IsRead(int index);

public:
    /**
     Constructor.
    */
    CGXDLMSDemandRegister();

    /**
     Constructor.

     @param ln Logical Name of the object.
    */
    CGXDLMSDemandRegister(std::string ln);

    /**
     Constructor.

     @param ln Logical Name of the object.
     @param sn Short Name of the object.
    */
    CGXDLMSDemandRegister(std::string ln, unsigned short sn);

    /**
     Current Average value of COSEM Data object.
    */
    CGXDLMSVariant& GetCurrentAverageValue();
    void SetCurrentAverageValue(CGXDLMSVariant& value);
    void SetCurrentAverageValue(CGXDLMSVariant value);

    /**
     Last Average value of COSEM Data object.
    */
    CGXDLMSVariant& GetLastAverageValue();
    void SetLastAverageValue(CGXDLMSVariant& value);
    void SetLastAverageValue(CGXDLMSVariant value);

    /**
     Scaler of COSEM Register object.
    */
    double GetScaler();
    void SetScaler(double value);

    // Unit of COSEM Register object.
    int GetUnit();
    void SetUnit(unsigned char value);

    /**
     Scaler of COSEM Register object.
    */
    CGXDLMSVariant& GetStatus();
    void SetStatus(CGXDLMSVariant& value);
    void SetStatus(CGXDLMSVariant value);

    /**
     Capture time of COSEM Register object.
    */
    CGXDateTime& GetCaptureTime();
    void SetCaptureTime(CGXDateTime value);

    /**
     Current start time of COSEM Register object.
    */
    CGXDateTime& GetStartTimeCurrent();
    void SetStartTimeCurrent(CGXDateTime& value);

    unsigned long GetPeriod();
    void SetPeriod(unsigned long value);

    int GetNumberOfPeriods();
    void SetNumberOfPeriods(int value);

    /*
     * Reset value.
     */
    int Reset(CGXDLMSClient* client,
        std::vector<CGXByteBuffer>& reply);

    /*
     * Next period.
     */
    int NextPeriod(CGXDLMSClient* client,
        std::vector<CGXByteBuffer>& reply);

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

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    int Invoke(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    int GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    int SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);
};

#endif //GXDLMSDEMANDREGISTER_H