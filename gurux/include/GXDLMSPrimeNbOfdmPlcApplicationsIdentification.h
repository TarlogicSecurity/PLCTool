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

#ifndef GXDLMSPRIMENBOFDMPLCAPPLICATIONSIDENTIFICATION_H
#define GXDLMSPRIMENBOFDMPLCAPPLICATIONSIDENTIFICATION_H

#include "GXDLMSObject.h"

/*
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSPrimeNbOfdmPlcApplicationsIdentification
*/
class CGXDLMSPrimeNbOfdmPlcApplicationsIdentification : public CGXDLMSObject
{
    /*
     * Textual description of the firmware version running on the device.
     */
    std::string m_FirmwareVersion;

    /*
     * Unique vendor identifier assigned by PRIME Alliance.
     */
    unsigned short m_VendorId;

    /*
     * Vendor assigned unique identifier for specific product.
     */
    unsigned short m_ProductId;
public:
    //Constructor.
    CGXDLMSPrimeNbOfdmPlcApplicationsIdentification();

    //SN Constructor.
    CGXDLMSPrimeNbOfdmPlcApplicationsIdentification(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSPrimeNbOfdmPlcApplicationsIdentification(std::string ln);

    /*
    * Returns Textual description of the firmware version running on the
    *         device.
    */
    std::string& GetFirmwareVersion();

    /*
     * value: Textual description of the firmware version running on the device.
     */
    void SetFirmwareVersion(std::string& value);

    /*
     * Returns Unique vendor identifier assigned by PRIME Alliance.
     */
    unsigned short GetVendorId();

    /*
     * value: Unique vendor identifier assigned by PRIME Alliance.
     */
    void SetVendorId(unsigned short value);

    /*
     * Returns Vendor assigned unique identifier for specific product.
     */
    unsigned short GetProductId();

    /*
     * value: Vendor assigned unique identifier for specific product.
     */
    void SetProductId(unsigned short value);

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

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

    // Returns value of given attribute.
    int GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);
};
#endif //GXDLMSPRIMENBOFDMPLCAPPLICATIONSIDENTIFICATION_H