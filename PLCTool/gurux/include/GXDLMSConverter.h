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

#ifndef GXDLMSCONVERTER_H
#define GXDLMSCONVERTER_H

#include "enums.h"

//This class is used to convert enum and error values to std::string.
class CGXDLMSConverter
{
public:
    //Get Unit as String.
    static const char* GetUnitAsString(int unit);
    //Get baudrate enum balue as std::string.
    static const char* ToString(DLMS_BAUD_RATE value);
    //Get clock base enum value as std::string.
    static const char* ToString(DLMS_CLOCK_BASE value);
    //Get clock status enum value as std::string.
    static const char* ToString(DLMS_CLOCK_STATUS value);

    //Get error message as std::string.
    static const char* GetErrorMessage(int error);

    //Get address state enum value as std::string.
    static const char* ToString(DLMS_ADDRESS_STATE value);

    //Get Disconnect control control state as a std::string.
    static const char* ToString(DLMS_CONTROL_STATE value);

    //Get Disconnect control control mode as a std::string.
    static const char* ToString(DLMS_CONTROL_MODE value);

    //Get Security Setup security policy as a std::string.
    static const char* ToString(DLMS_SECURITY_POLICY value);

    //Get Security Setup security policy v1 as a std::string.
    static int ToString(DLMS_SECURITY_POLICY1 value, std::string& str);

    //Get Security Setup security suite as a std::string.
    static const char* ToString(DLMS_SECURITY_SUITE value);

    //Get Logical Name association status as a std::string.
    static const char* ToString(DLMS_ASSOCIATION_STATUS value);

    //Get optical port protocol mode as a std::string.
    static const char* ToString(DLMS_OPTICAL_PROTOCOL_MODE value);

    //Get optical port response time as a std::string.
    static const char* ToString(DLMS_LOCAL_PORT_RESPONSE_TIME value);

    //GSM status to string.
    static const char* ToString(DLMS_GSM_STATUS value);

    //GSM circuit switced status to string.
    static const char* ToString(DLMS_GSM_CIRCUIT_SWITCH_STATUS value);

    // Packet switched status to the string.
    static const char* ToString(DLMS_GSM_PACKET_SWITCH_STATUS value);

    //Convert authentication enum value to string.
    static const char* ToString(DLMS_AUTHENTICATION value);

    //Convert authentication string to enum value.
    static DLMS_AUTHENTICATION ValueOfAuthentication(const char* value);

    //Convert association enum value to string.
    static const char* ToString(DLMS_ASSOCIATION_RESULT value);

    //Convert association string to enum value.
    static DLMS_ASSOCIATION_RESULT ValueOfAssociation(const char* value);

    //Convert source diagnostic enum value to string.
    static const char* ToString(DLMS_SOURCE_DIAGNOSTIC value);

    //Convert source diagnostic string to enum value.
    static DLMS_SOURCE_DIAGNOSTIC ValueOfSourceDiagnostic(const char* value);

    //Convert security enum value to string.
    static const char* ToString(DLMS_SECURITY value);

    //Convert object type enum value to string.
    static const char* ToString(DLMS_OBJECT_TYPE type);

    static DLMS_OBJECT_TYPE ValueOfObjectType(const char* value);
};
#endif //GXDLMSCONVERTER_H