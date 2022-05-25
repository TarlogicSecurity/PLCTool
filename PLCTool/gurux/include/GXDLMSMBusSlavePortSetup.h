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

#ifndef GXDLMSMBUSSLAVEPORTSETUP_H
#define GXDLMSMBUSSLAVEPORTSETUP_H

#include "GXDLMSObject.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSMBusSlavePortSetup
*/
class CGXDLMSMBusSlavePortSetup : public CGXDLMSObject
{
    DLMS_BAUD_RATE m_DefaultBaud;
    DLMS_BAUD_RATE m_AvailableBaud;
    DLMS_ADDRESS_STATE m_AddressState;
    int m_BusAddress;

public:
    //Constructor.
    CGXDLMSMBusSlavePortSetup();

    //SN Constructor.
    CGXDLMSMBusSlavePortSetup(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSMBusSlavePortSetup(std::string ln);

    /**
     Defines the baud rate for the opening sequence.
    */
    DLMS_BAUD_RATE GetDefaultBaud();

    void SetDefaultBaud(DLMS_BAUD_RATE value);

    /**
     Defines the baud rate for the opening sequence.
    */
    DLMS_BAUD_RATE GetAvailableBaud();

    void SetAvailableBaud(DLMS_BAUD_RATE value);

    /**
     Defines whether or not the device has been assigned an address
     * since last power up of the device.
    */
    DLMS_ADDRESS_STATE GetAddressState();

    void SetAddressState(DLMS_ADDRESS_STATE value);

    /**
     Defines the baud rate for the opening sequence.
    */
    int GetBusAddress();

    void SetBusAddress(int value);

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
#endif //GXDLMSMBUSSLAVEPORTSETUP_H