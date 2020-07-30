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

#ifndef GXDLMSAUTOCONNECT_H
#define GXDLMSAUTOCONNECT_H

#include "GXDLMSObject.h"

typedef enum
{
    /*
     * The device never connects.
     */
    DLMS_AUTO_CONNECT_MODE_NO_AUTO_CONNECT = 0,
    /**
     * Auto dialling allowed anytime,
    */
    DLMS_AUTO_CONNECT_MODE_AUTO_DIALLING_ALLOWED_ANYTIME = 1,
    /**
     * Auto dialling allowed within the validity time of the calling window.
    */
    DLMS_AUTO_CONNECT_MODE_AUTO_DIALLING_ALLOWED_CALLING_WINDOW = 2,
    /**
     * Regular auto dialling allowed within the validity time
     * of the calling window; alarm initiated auto dialling allowed anytime,
    */
    DLMS_AUTO_CONNECT_MODE_REGULAR_AUTO_DIALLING_ALLOWED_CALLING_WINDOW = 3,
    /**
     * SMS sending via Public Land Mobile Network (PLMN),
    */
    DLMS_AUTO_CONNECT_MODE_SMS_SENDING_PLMN = 4,
    /*
     * SMS sending via PSTN.
    */
    DLMS_AUTO_CONNECT_MODE_SMS_SENDING_PSTN = 5,
    /*
     * Email sending.
    */
    DLMS_AUTO_CONNECT_MODE_EMAIL_SENDING = 6,
    /*
   * The device is permanently connected to the communication network.
   */
   DLMS_AUTO_CONNECT_MODE_PERMANENTLY_CONNECT = 101,
   /*
   * The device is permanently connected to the communication network. No
   * connection possible outside the calling window.
   */
   DLMS_AUTO_CONNECT_MODE_CONNECT_WITH_CALLING_WINDOW = 102,
   /*
   * The device is permanently connected to the communication network.
   * Connection is possible as soon as the connect method is invoked.
   */
   DLMS_AUTO_CONNECT_MODE_CONNECT_INVOKED = 103,
   /*
   * The device is usually disconnected. It connects to the communication
   * network as soon as the connect method is invoked
   */
   DLMS_AUTO_CONNECT_MODE_DISCONNECT_CONNECT_INVOKED = 104,
   /*
   * (200..255) manufacturer specific modes
   */
   DLMS_AUTO_CONNECT_MODE_MANUFACTURE_SPESIFIC = 200
} DLMS_AUTO_CONNECT_MODE;

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAutoConnect
*/
class CGXDLMSAutoConnect : public CGXDLMSObject
{
    DLMS_AUTO_CONNECT_MODE m_Mode;
    std::vector<std::pair< CGXDateTime, CGXDateTime> > m_CallingWindow;
    std::vector< std::string > m_Destinations;
    int m_RepetitionDelay;
    int m_Repetitions;

public:
    //Constructor.
    CGXDLMSAutoConnect();

    //SN Constructor.
    CGXDLMSAutoConnect(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSAutoConnect(std::string ln);

    DLMS_AUTO_CONNECT_MODE GetMode();
    void SetMode(DLMS_AUTO_CONNECT_MODE value);

    int GetRepetitions();

    void SetRepetitions(int value);

    int GetRepetitionDelay();
    void SetRepetitionDelay(int value);

    std::vector<std::pair< CGXDateTime, CGXDateTime> >& GetCallingWindow();
    void SetCallingWindow(std::vector<std::pair< CGXDateTime, CGXDateTime> > value);

    std::vector< std::string >& GetDestinations();

    void SetDestinations(std::vector< std::string >& value);

    //Initiates the connection process.
    int Connect(CGXDLMSClient* client,
        std::vector<CGXByteBuffer>& reply);


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
#endif //GXDLMSAUTOCONNECT_H