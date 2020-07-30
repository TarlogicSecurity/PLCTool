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

#ifndef GXDLMSVARIANT_H
#define GXDLMSVARIANT_H

#include "GXDate.h"
#include "GXTime.h"
#include "GXDateTime.h"
#include "GXBytebuffer.h"

#define __tagVARIANT
#define __VARIANT_NAME_1
#define __VARIANT_NAME_2
#define __VARIANT_NAME_3

class CGXDLMSVariant;

struct dlmsVARIANT
{
    union
    {
        struct __tagVARIANT
        {
            DLMS_DATA_TYPE vt;
            union
            {
                unsigned char bVal;
                char cVal;
                short iVal;
                long lVal;
                long long llVal;
                float fltVal;
                double dblVal;
                bool boolVal;
                unsigned short uiVal;
                unsigned long ulVal;
                unsigned long long ullVal;
                unsigned char *byteArr;
            } 	__VARIANT_NAME_3;
        } 	__VARIANT_NAME_2;
    } 	__VARIANT_NAME_1;
    CGXDateTime dateTime;
    //Size of byte array.
    unsigned short size;
    std::string strVal;
    std::vector<CGXDLMSVariant> Arr;
};

class CGXDLMSVariant : public dlmsVARIANT
{
    static int Convert(CGXDLMSVariant* item, DLMS_DATA_TYPE type);
public:
    void Clear();
    CGXDLMSVariant();

    //Copy constructor.
    CGXDLMSVariant(const CGXDLMSVariant& value);

    CGXDLMSVariant(float value);
    CGXDLMSVariant(double value);

    CGXDLMSVariant(unsigned long long value);
    CGXDLMSVariant(long long value);
    CGXDLMSVariant(bool value);
    CGXDLMSVariant(char value);
    CGXDLMSVariant(short value);
    CGXDLMSVariant(int value);
    CGXDLMSVariant(long value);
    CGXDLMSVariant(struct tm value);
    CGXDLMSVariant(unsigned char* value, int count);
    CGXDLMSVariant(CGXDate& value);
    CGXDLMSVariant(CGXTime& value);
    CGXDLMSVariant(CGXDateTime& value);
    CGXDLMSVariant(CGXDLMSVariant* value);
    CGXDLMSVariant(unsigned char* pValue, int count, DLMS_DATA_TYPE type);
    CGXDLMSVariant(unsigned char value);
    CGXDLMSVariant(unsigned short value);
    CGXDLMSVariant(unsigned long value);
    CGXDLMSVariant(std::string value);
    CGXDLMSVariant(CGXByteBuffer& value);
    CGXDLMSVariant(const char* value);
    CGXDLMSVariant(unsigned int value);
    //Destructor.
    ~CGXDLMSVariant();


    CGXDLMSVariant& operator=(const CGXDLMSVariant& value);

    CGXDLMSVariant& operator=(std::string value);
    CGXDLMSVariant& operator=(const char* value);
    CGXDLMSVariant& operator=(CGXByteBuffer& value);
    CGXDLMSVariant& operator=(float value);
    CGXDLMSVariant& operator=(double value);
    CGXDLMSVariant& operator=(unsigned long long value);
    CGXDLMSVariant& operator=(long long value);
    CGXDLMSVariant& operator=(const bool value);
    CGXDLMSVariant& operator=(char value);
    CGXDLMSVariant& operator=(short value);
    CGXDLMSVariant& operator=(int value);
    CGXDLMSVariant& operator=(long value);
    CGXDLMSVariant& operator=(unsigned char value);
    CGXDLMSVariant& operator=(unsigned short value);
    CGXDLMSVariant& operator=(unsigned int value);
    CGXDLMSVariant& operator=(unsigned long value);
    CGXDLMSVariant& operator=(struct tm value);
    CGXDLMSVariant& operator=(CGXDate& value);
    CGXDLMSVariant& operator=(CGXTime& value);
    CGXDLMSVariant& operator=(CGXDateTime& value);
    void Add(const unsigned char*, int count);
    void Add(const char*, int count);
    void Add(std::string value);
    bool Equals(CGXDLMSVariant& item);
    int ChangeType(DLMS_DATA_TYPE newType);
    //Get size in bytes.
    int GetSize();
    //Get size in bytes.
    static int GetSize(DLMS_DATA_TYPE vt);
    std::string ToString();
    int ToInteger();
    double ToDouble();
    int GetBytes(CGXByteBuffer& value);

    //Returns true if value is number.
    bool IsNumber();
};
#endif //GXDLMSVARIANT_H
