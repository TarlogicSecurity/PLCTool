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
#ifndef GXDLMSPROFILEGENERIC_H
#define GXDLMSPROFILEGENERIC_H

#include "GXDLMSCaptureObject.h"
#include "GXDLMSRegister.h"

enum GX_SORT_METHOD
{
    /**
     First in first out

     When circle buffer is full first item is removed.
    */
    DLMS_SORT_METHOD_FIFO = 1,
    /**
     Last in first out.

     When circle buffer is full last item is removed.
    */
    DLMS_SORT_METHOD_LIFO = 2,
    /**
     Largest is first.
    */
    DLMS_SORT_METHOD_LARGEST = 3,
    /**
     Smallest is first.
    */
    DLMS_SORT_METHOD_SMALLEST = 4,
    /**
     Nearst to zero is first.
    */
    DLMS_SORT_METHOD_NEAREST_TO_ZERO = 5,
    /**
     Farest from zero is first.
    */
    DLMS_SORT_METHOD_FAREST_FROM_ZERO = 6
};

class CGXDLMSServer;

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSProfileGeneric
*/
class CGXDLMSProfileGeneric : public CGXDLMSObject
{
private:
    std::vector< std::vector<CGXDLMSVariant> > m_Buffer;
    std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> > m_CaptureObjects;
    int m_CapturePeriod;
    GX_SORT_METHOD m_SortMethod;
    CGXDLMSObject* m_SortObject;
    unsigned long m_ProfileEntries;
    unsigned long m_EntriesInUse;

    int m_SortObjectAttributeIndex;
    int m_SortObjectDataIndex;

    int GetColumns(CGXByteBuffer& data);
    int GetData(
        CGXDLMSSettings& settings,
        CGXDLMSValueEventArg& e,
        std::vector< std::vector<CGXDLMSVariant> >& table,
        std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >& columns,
        CGXByteBuffer& data);

    int GetProfileGenericData(
        CGXDLMSSettings& settings,
        CGXDLMSValueEventArg& e,
        CGXByteBuffer& reply);

    /**
     * Get selected columns.
     *
     * @param parameters
     *            Received data.
     * @param cols
     *            Selected columns.
     */
    int GetSelectedColumns(
        std::vector<CGXDLMSVariant>& cols,
        std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >& columns);
public:
    /*
     * Add new capture object (column) to the profile generic.
     */
    int AddCaptureObject(
        CGXDLMSObject* pObj,
        int attributeIndex,
        int dataIndex);

    /**
    * Get selected columns from parameters.
    *
    * @param selector
    *            Is read by entry or range.
    * @param parameters
    *            Received parameters where columns information is found.
    * @return Selected columns.
    */
    int GetSelectedColumns(
        int selector,
        CGXDLMSVariant& parameters,
        std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >& columns);

    /**
     Constructor.
    */
    CGXDLMSProfileGeneric();

    //SN Constructor.
    CGXDLMSProfileGeneric(std::string ln, unsigned short sn);

    /**
     Constructor.

     @param ln Logical Name of the object.
    */
    CGXDLMSProfileGeneric(std::string ln);

    /**
     Destructor.
    */
    virtual ~CGXDLMSProfileGeneric();

    /**
     Data of profile generic.
    */
    std::vector< std::vector<CGXDLMSVariant> >& GetBuffer();

    /**
     Captured Objects.
    */
    std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >& GetCaptureObjects();

    /**
     How often values are captured.
    */
    int GetCapturePeriod();
    void SetCapturePeriod(int value);

    /**
     How columns are sorted.
    */
    GX_SORT_METHOD GetSortMethod();
    void SetSortMethod(GX_SORT_METHOD value);

    /**
     Column that is used for sorting.
    */
    CGXDLMSObject* GetSortObject();
    void SetSortObject(CGXDLMSObject* value);

    /**
     Entries (rows) in Use.
    */
    unsigned long GetEntriesInUse();
    void SetEntriesInUse(unsigned long value);

    /**
     Maximum Entries (rows) count.
    */
    unsigned long GetProfileEntries();
    void SetProfileEntries(unsigned long value);
    /**
    Attribute index of sort object.
    */
    int GetSortObjectAttributeIndex();
    void SetSortObjectAttributeIndex(int value);

    /**
     Data index of sort object.
    */
    int GetSortObjectDataIndex();
    void SetSortObjectDataIndex(int value);

    /**
     Clears the buffer.
    */
    void Reset();

    /*
    * Copies the values of the objects to capture into the buffer by reading
    * capture objects.
    */
    int Capture(CGXDLMSServer* server);

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

    //Returns amount of attributes.
    int GetAttributeCount();

    //Returns amount of methods.
    int GetMethodCount();

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    int Invoke(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    /*
    * Returns value of given attribute.
    */
    int GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    /*
     * Set value of given attribute.
     */
    int SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    /**
    * Clears the buffer.
    *
    * @param client
    *            DLMS client.
    * @param reply
    *            Action bytes.
    * @return Result code.
    */
    int Reset(CGXDLMSClient& client, std::vector<CGXByteBuffer>& reply);

    /**
    * Copies the values of the objects to capture into the buffer by reading
    * each capture object.
    *
    * @param client
    *            DLMS client.
    * @param reply
    *            Action bytes.
    * @return Result code.
    */
    int Capture(CGXDLMSClient& client, std::vector<CGXByteBuffer>& reply);
};
#endif //GXDLMSPROFILEGENERIC_H