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

#include "../include/GXDLMSConverter.h"
#include "../include/errorcodes.h"

const char* CGXDLMSConverter::GetErrorMessage(int error)
{
    const char* str;
    switch (error)
    {
    case DLMS_ERROR_CODE_OK:
        str = "OK";
        break;
    case DLMS_ERROR_CODE_INVALID_PARAMETER:
        str = "Invalid parameter.";
        break;
    case DLMS_ERROR_CODE_NOT_INITIALIZED:
        str = "Server is not initialized.";
        break;
    case DLMS_ERROR_CODE_OUTOFMEMORY:
        str = "Not enough memory available.";
        break;
    case DLMS_ERROR_CODE_NOT_REPLY:
        str = "Packet is not a reply for a send packet.";
        break;
    case DLMS_ERROR_CODE_REJECTED:
        str = "Meter rejects send packet.";
        break;
    case DLMS_ERROR_CODE_INVALID_LOGICAL_NAME:
        str = "Invalid Logical Name.";
        break;
    case DLMS_ERROR_CODE_INVALID_CLIENT_ADDRESS:
        str = "Client HDLC Address is not set.";
        break;
    case DLMS_ERROR_CODE_INVALID_SERVER_ADDRESS:
        str = "Server HDLC Address is not set.";
        break;
    case DLMS_ERROR_CODE_INVALID_DATA_FORMAT:
        str = "Not a HDLC frame.";
        break;
    case DLMS_ERROR_CODE_INVALID_VERSION_NUMBER:
        str = "Invalid DLMS version number.";
        break;
    case DLMS_ERROR_CODE_CLIENT_ADDRESS_NO_NOT_MATCH:
        str = "Client addresses do not match.";
        break;
    case DLMS_ERROR_CODE_SERVER_ADDRESS_NO_NOT_MATCH:
        str = "Server addresses do not match.";
        break;
    case DLMS_ERROR_CODE_WRONG_CRC:
        str = "CRC do not match.";
        break;
    case DLMS_ERROR_CODE_INVALID_RESPONSE:
        str = "Invalid response";
        break;
    case DLMS_ERROR_CODE_INVALID_TAG:
        str = "Invalid Tag.";
        break;
    case DLMS_ERROR_CODE_ENCODING_FAILED:
        str = "Encoding failed. Not enough data.";
        break;
    case DLMS_ERROR_CODE_REJECTED_PERMAMENT:
        str = "Rejected permament.";
        break;
    case DLMS_ERROR_CODE_REJECTED_TRANSIENT:
        str = "Rejected transient.";
        break;
    case DLMS_ERROR_CODE_NO_REASON_GIVEN:
        str = "No reason given.";
        break;
    case DLMS_ERROR_CODE_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED:
        str = "Application context name not supported.";
        break;
    case DLMS_ERROR_CODE_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED:
        str = "Authentication mechanism name not recognised.";
        break;
    case DLMS_ERROR_CODE_AUTHENTICATION_MECHANISM_NAME_REQUIRED:
        str = "Authentication mechanism name required.";
        break;
    case DLMS_ERROR_CODE_AUTHENTICATION_FAILURE:
        str = "Authentication failure.";
        break;
    case DLMS_ERROR_CODE_AUTHENTICATION_REQUIRED:
        str = "Authentication required.";
        break;
    case DLMS_ERROR_CODE_HARDWARE_FAULT:
        str = "Access Error : Device reports a hardware fault.";
        break;
    case DLMS_ERROR_CODE_TEMPORARY_FAILURE:
        str = "Access Error : Device reports a temporary failure.";
        break;
    case DLMS_ERROR_CODE_READ_WRITE_DENIED:
        str = "Access Error : Device reports Read-Write denied.";
        break;
    case DLMS_ERROR_CODE_UNDEFINED_OBJECT:
        str = "Access Error : Device reports a undefined object.";
        break;
    case DLMS_ERROR_CODE_INCONSISTENT_CLASS:
        str = "Access Error : Device reports a inconsistent Class or Object.";
        break;
    case DLMS_ERROR_CODE_UNAVAILABLE_OBJECT:
        str = "Access Error : Device reports a unavailable object.";
        break;
    case DLMS_ERROR_CODE_UNMATCH_TYPE:
        str = "Access Error : Device reports a unmatched type.";
        break;
    case DLMS_ERROR_CODE_ACCESS_VIOLATED:
        str = "Access Error : Device reports scope of access violated.";
        break;
    case DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE:
        str = "Access Error : Data Block Unavailable.";
        break;
    case DLMS_ERROR_CODE_LONG_GET_OR_READ_ABORTED:
        str = "Access Error : Long Get Or Read Aborted.";
        break;
    case DLMS_ERROR_CODE_NO_LONG_GET_OR_READ_IN_PROGRESS:
        str = "Access Error : No Long Get Or Read In Progress.";
        break;
    case DLMS_ERROR_CODE_LONG_SET_OR_WRITE_ABORTED:
        str = "Access Error : Long Set Or Write Aborted.";
        break;
    case DLMS_ERROR_CODE_NO_LONG_SET_OR_WRITE_IN_PROGRESS:
        str = "Access Error : No Long Set Or Write In Progress.";
        break;
    case DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID:
        str = "Access Error : Data Block Number Invalid.";
        break;
    case DLMS_ERROR_CODE_OTHER_REASON:
        str = "Access Error : Other Reason.";
        break;
    case DLMS_ERROR_CODE_UNKNOWN:
        str = "Unknown error.";
        break;
    case DLMS_ERROR_CODE_SEND_FAILED:
        str = "Data send failed.";
        break;
    case DLMS_ERROR_CODE_RECEIVE_FAILED:
        str = "Data receive failed.";
        break;
    case DLMS_ERROR_CODE_NOT_IMPLEMENTED:
        str = "Not implemeted.";
        break;
    case DLMS_ERROR_CODE_INVALID_INVOKE_ID:
        str = "Invalid Invoke ID.";
        break;
    default:
        str = "Unknown error.";
        break;
    }
    return str;
}

const char* CGXDLMSConverter::ToString(DLMS_BAUD_RATE value)
{
    const char* str;
    switch (value)
    {
    case 0:
        str = "Baudrate300";
        break;
    case 1:
        str = "Baudrate600";
        break;
    case 2:
        str = "Baudrate1200";
        break;
    case 3:
        str = "Baudrate2400";
        break;
    case 4:
        str = "Baudrate4000";
        break;
    case 5:
        str = "Baudrate9600";
        break;
    case 6:
        str = "Baudrate19200";
        break;
    case 7:
        str = "Baudrate38400";
        break;
    case 8:
        str = "Baudrate57600";
        break;
    case 9:
        str = "Baudrate115200";
        break;
    default:
        str = "Unknown baudrate";
        break;
    }
    return str;
}

const char* CGXDLMSConverter::ToString(DLMS_CLOCK_STATUS value)
{
    const char* str;
    switch (value)
    {
    case 0:
        str = "None";
        break;
    case 0x1:
        str = "InvalidValue";
        break;
    case 0x2:
        str = "DoubtfulValue";
        break;
    case 0x4:
        str = "DifferentClockBase";
        break;
    case 0x8:
        str = "Reserved1";
        break;
    case 0x10:
        str = "Reserved2";
        break;
    case 0x20:
        str = "Reserved3";
        break;
    case 0x40:
        str = "Reserved4";
        break;
    case 0x80:
        str = "DaylightSaveActive";
        break;
    default:
        str = "Unknown Clock status";
        break;
    }
    return str;
}

const char* CGXDLMSConverter::ToString(DLMS_CLOCK_BASE value)
{
    const char* str;
    switch (value)
    {
    case 0:
        str = "None";
        break;
    case 1:
        str = "Crystal";
        break;
    case 2:
        str = "Frequency50";
        break;
    case 3:
        str = "Frequency60";
        break;
    case 4:
        str = "GPS";
        break;
    case 5:
        str = "Radio";
        break;
    default:
        str = "Unknown Clock base";
        break;
    }
    return str;
}

const char* CGXDLMSConverter::GetUnitAsString(int unit)
{
    const char* str;
    switch (unit)
    {
    case 0:
        str = "None";
        break;
    case 1:
        str = "Year";
        break;
    case 2:
        str = "Month";
        break;
    case 3:
        str = "Week";
        break;
    case 4:
        str = "Day";
        break;
    case 5:
        str = "Hour";
        break;
    case 6:
        str = "Minute";
        break;
    case 7:
        str = "Second";
        break;
    case 8:
        str = "PhaseAngle";
        break;
    case 9:
        str = "Temperature";
        break;
    case 10:
        str = "LocalCurrency";
        break;
    case 11:
        str = "Length";
        break;
    case 12:
        str = "Speed";
        break;
    case 13:
        str = "Volume";
        break;
    case 14:
        str = "CorrectedVolume";
        break;
    case 15:
        str = "VolumeFlux";
        break;
    case 16:
        str = "CorrectedVolumeFlux";
        break;
    case 17:
        str = "VolumeFlux";
        break;
    case 18:
        str = "CorrectedVolumeFlux";
        break;
    case 19:
        str = "Volume";
        break;
    case 20:
        str = "MassKg";
        break;
    case 21:
        str = "Force";
        break;
    case 22:
        str = "Energy";
        break;
    case 23:
        str = "PressurePascal";
        break;
    case 24:
        str = "PressureBar";
        break;
    case 25:
        str = "Energy";
        break;
    case 26:
        str = "ThermalPower";
        break;
    case 27:
        str = "ActivePower";
        break;
    case 28:
        str = "ApparentPower";
        break;
    case 29:
        str = "ReactivePower";
        break;
    case 30:
        str = "ActiveEnergy";
        break;
    case 31:
        str = "ApparentEnergy";
        break;
    case 32:
        str = "ReactiveEnergy";
        break;
    case 33:
        str = "Current";
        break;
    case 34:
        str = "ElectricalCharge";
        break;
    case 35:
        str = "Voltage";
        break;
    case 36:
        str = "Electrical field strength E V/m";
        break;
    case 37:
        str = "Capacity C farad C/V = As/V";
        break;
    case 38:
        str = "Resistance";
        break;
    case 39:
        str = "Resistivity";
        break;
    case 40:
        str = "Magnetic flux F weber Wb = Vs";
        break;
    case 41:
        str = "Induction T tesla Wb/m2";
        break;
    case 42:
        str = "Magnetic field strength H A/m";
        break;
    case 43:
        str = "Inductivity L henry H = Wb/A";
        break;
    case 44:
        str = "Frequency";
        break;
    case 45:
        str = "ActiveEnergy";
        break;
    case 46:
        str = "ReactiveEnergy";
        break;
    case 47:
        str = "ApparentEnergy";
        break;
    case 48:
        str = "V260*60s";
        break;
    case 49:
        str = "A260*60s";
        break;
    case 50:
        str = "Mass";
        break;
    case 51:
        str = "ConductanceSiemens";
        break;
    case 254:
        str = "OtherUnit";
        break;
    case 255:
        str = "NoUnit";
        break;
    default:
        str = "";
        break;
    }
    return str;
}


const char* CGXDLMSConverter::ToString(DLMS_ADDRESS_STATE value)
{
    const char* str;
    switch (value)
    {
    case 0:
        str = "None";
        break;
    case 1:
        str = "Assigned";
        break;
    default:
        str = "Unknown Clock base";
        break;
    }
    return str;
}

//Get Disconnect control control state as a std::string.
const char* CGXDLMSConverter::ToString(DLMS_CONTROL_STATE value)
{
    const char* str;
    switch (value)
    {
    case 0:
        str = "Disconnected";
        break;
    case 1:
        str = "Connected";
        break;
    case 2:
        str = "ReadyForReconnection";
        break;
    default:
        str = "Unknown control state";
        break;
    }
    return str;
}

//Get Disconnect control control mode as a std::string.
const char* CGXDLMSConverter::ToString(DLMS_CONTROL_MODE value)
{
    const char* str;
    switch (value)
    {
    case 0:
        str = "None";
        break;
    case 1:
        str = "Mode1";
        break;
    case 2:
        str = "Mode2";
        break;
    case 3:
        str = "Mode3";
        break;
    case 4:
        str = "Mode4";
        break;
    case 5:
        str = "Mode5";
        break;
    case 6:
        str = "Mode6";
        break;
    default:
        str = "Unknown control mode";
        break;
    }
    return str;
}

//Get Security Setup security policy as a std::string.
const char* CGXDLMSConverter::ToString(DLMS_SECURITY_POLICY value)
{
    const char* str;
    switch (value)
    {
    case DLMS_SECURITY_POLICY_NOTHING:
        str = "Nothing";
        break;
    case DLMS_SECURITY_POLICY_AUTHENTICATED:
        str = "Authenticated";
        break;
    case DLMS_SECURITY_POLICY_ENCRYPTED:
        str = "Encrypted";
        break;
    case DLMS_SECURITY_POLICY_AUTHENTICATED_ENCRYPTED:
        str = "AuthenticatedEncrypted";
        break;
    default:
        str = "Unknown security policy";
        break;
    }
    return str;
}

//Get Security Setup security policy v1 as a std::string.
int CGXDLMSConverter::ToString(DLMS_SECURITY_POLICY1 value, std::string& str)
{
    str.clear();
    if (value == 0)
    {
        str = "Nothing";
    }
    else
    {
        if ((value & DLMS_SECURITY_POLICY1_AUTHENTICATED_REQUEST) != 0)
        {
            str.append("RequestAuthenticated");
        }
        if ((value & DLMS_SECURITY_POLICY1_ENCRYPTED_REQUEST) != 0)
        {
            if (!str.empty())
            {
                str.append(", ");
            }
            str.append("RequestEncrypted");
        }
        if ((value & DLMS_SECURITY_POLICY1_DIGITALLY_SIGNED_REQUEST) != 0)
        {
            if (!str.empty())
            {
                str.append(", ");
            }
            str.append("RequestDigitallySigned");
        }
        if ((value & DLMS_SECURITY_POLICY1_AUTHENTICATED_RESPONSE) != 0)
        {
            if (!str.empty())
            {
                str.append(", ");
            }
            str.append("ResponseAuthenticated");
        }
        if ((value & DLMS_SECURITY_POLICY1_AUTHENTICATED_RESPONSE) != 0)
        {
            if (!str.empty())
            {
                str.append(", ");
            }
            str.append("ResponseEncrypted");
        }
        if ((value & DLMS_SECURITY_POLICY1_ENCRYPTED_RESPONSE) != 0)
        {
            if (!str.empty())
            {
                str.append(", ");
            }
            str.append("ResponseDigitallySigned");
        }
    }
    return 0;
}

//Get Security Setup security suite as a std::string.
const char* CGXDLMSConverter::ToString(DLMS_SECURITY_SUITE value)
{
    switch (value)
    {
    case 0://DLMS_SECURITY_SUITE_AES_GCM_128
        return "AES_GCM_128";
        break;
    default:
        break;
    }
    return "Unknown security suite";
}

const char* CGXDLMSConverter::ToString(DLMS_ASSOCIATION_STATUS value)
{
    switch (value)
    {
    case DLMS_ASSOCIATION_STATUS_NON_ASSOCIATED:
        return "NonAssociated";
        break;
    case DLMS_ASSOCIATION_STATUS_ASSOCIATION_PENDING:
        return "AssociationPending";
        break;
    case DLMS_ASSOCIATION_STATUS_ASSOCIATED:
        return "Associated";
        break;
    default:
        break;
    }
    return "Unknown association status";
}

const char* CGXDLMSConverter::ToString(DLMS_OPTICAL_PROTOCOL_MODE value)
{
    switch (value)
    {
    case DLMS_OPTICAL_PROTOCOL_MODE_DEFAULT:
        return "Default";
        break;
    case DLMS_OPTICAL_PROTOCOL_MODE_NET:
        return "Net";
        break;
    case DLMS_OPTICAL_PROTOCOL_MODE_UNKNOWN:
        return "Unknown";
        break;
    default:
        break;
    }
    return "Unknown protocol mode";
}

const char* CGXDLMSConverter::ToString(DLMS_LOCAL_PORT_RESPONSE_TIME value)
{
    switch (value)
    {
    case DLMS_LOCAL_PORT_RESPONSE_TIME_20_MS:
        return "ms20";
        break;
    case DLMS_LOCAL_PORT_RESPONSE_TIME_200_MS:
        return "ms200";
        break;
    default:
        break;
    }
    return "Unknown response time";
}

const char* CGXDLMSConverter::ToString(DLMS_GSM_STATUS value)
{
    const char* str;
    switch (value)
    {
    case DLMS_GSM_STATUS_NONE:
        str = "None";
        break;
    case DLMS_GSM_STATUS_HOME_NETWORK:
        str = "HomeNetwork";
        break;
    case DLMS_GSM_STATUS_SEARCHING:
        str = "Searching";
        break;
    case DLMS_GSM_STATUS_DENIED:
        str = "Denied";
        break;
    case DLMS_GSM_STATUS_UNKNOWN:
        str = "Unknown";
        break;
    case DLMS_GSM_STATUS_ROAMING:
        str = "Roaming";
        break;
    default:
        str = NULL;
    }
    return str;
};

const char* CGXDLMSConverter::ToString(DLMS_GSM_CIRCUIT_SWITCH_STATUS value)
{
    const char* str;
    switch (value)
    {
    case DLMS_GSM_CIRCUIT_SWITCH_STATUS_INACTIVE:
        str = "Inactive";
        break;
    case DLMS_GSM_CIRCUIT_SWITCH_STATUS_INCOMING_CALL:
        str = "Incoming call";
        break;
    case DLMS_GSM_CIRCUIT_SWITCH_STATUS_ACTIVE:
        str = "Active";
        break;
    default:
        str = NULL;
    }
    return str;
};


const char* CGXDLMSConverter::ToString(DLMS_GSM_PACKET_SWITCH_STATUS value)
{
    const char* str;
    switch (value)
    {
    case DLMS_GSM_PACKET_SWITCH_STATUS_INACTIVE:
        str = "Inactive";
        break;
    case DLMS_GSM_PACKET_SWITCH_STATUS_GPRS:
        str = "GPRS";
        break;
    case DLMS_GSM_PACKET_SWITCH_STATUS_EDGE:
        str = "EDGE";
        break;
    case DLMS_GSM_PACKET_SWITCH_STATUS_UMTS:
        str = "UMTS";
        break;
    case DLMS_GSM_PACKET_SWITCH_STATUS_HSDPA:
        str = "HSDPA";
        break;
    default:
        str = NULL;
    }
    return str;
};

const char* CGXDLMSConverter::ToString(DLMS_AUTHENTICATION value)
{
    const char* str;
    switch (value)
    {
    case DLMS_AUTHENTICATION_HIGH:
        str = "High";
        break;
    case DLMS_AUTHENTICATION_HIGH_GMAC:
        str = "HighGMac";
        break;
    case DLMS_AUTHENTICATION_HIGH_MD5:
        str = "HighMd5";
        break;
    case DLMS_AUTHENTICATION_HIGH_SHA1:
        str = "HighSha1";
        break;
    case DLMS_AUTHENTICATION_HIGH_SHA256:
        str = "HighSha256";
        break;
    case DLMS_AUTHENTICATION_LOW:
        str = "Low";
        break;
    case DLMS_AUTHENTICATION_NONE:
        str = "None";
        break;
    default:
        str = "UNKNOWN";
        break;
    }
    return str;
}

DLMS_AUTHENTICATION CGXDLMSConverter::ValueOfAuthentication(const char* value) {
    DLMS_AUTHENTICATION v;
    if (strcmp(value, "None") == 0)
    {
        v = DLMS_AUTHENTICATION_NONE;
    }
    else if (strcmp(value, "Low") == 0)
    {
        v = DLMS_AUTHENTICATION_LOW;
    }
    else if (strcmp(value, "High") == 0)
    {
        v = DLMS_AUTHENTICATION_HIGH;
    }
    else if (strcmp(value, "HighMd5") == 0)
    {
        v = DLMS_AUTHENTICATION_HIGH_MD5;
    }
    else if (strcmp(value, "HighSha1") == 0)
    {
        v = DLMS_AUTHENTICATION_HIGH_SHA1;
    }
    else if (strcmp(value, "HighSha256") == 0)
    {
        v = DLMS_AUTHENTICATION_HIGH_SHA256;
    }
    else if (strcmp(value, "HighGMac") == 0)
    {
        v = DLMS_AUTHENTICATION_HIGH_GMAC;
    }
    else {
        v = (DLMS_AUTHENTICATION)-1;
    }
    return v;
}

//Convert association enum value to string.
const char* CGXDLMSConverter::ToString(DLMS_ASSOCIATION_RESULT value)
{
    const char* str;
    switch (value)
    {
    case DLMS_ASSOCIATION_RESULT_ACCEPTED:
        str = "ACCEPTED";
        break;
    case DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED:
        str = "PERMANENT_REJECTED";
        break;
    case DLMS_ASSOCIATION_RESULT_TRANSIENT_REJECTED:
        str = "TRANSIENT_REJECTED";
        break;
    default:
        str = "UNKNOWN";
        break;
    }
    return str;
}

DLMS_ASSOCIATION_RESULT CGXDLMSConverter::ValueOfAssociation(const char* value)
{
    DLMS_ASSOCIATION_RESULT v;
    if (strcmp(value, "Accepted") == 0)
    {
        v = DLMS_ASSOCIATION_RESULT_ACCEPTED;
    }
    else if (strcmp(value, "PERMANENT_REJECTED") == 0)
    {
        v = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
    }
    else if (strcmp(value, "TRANSIENT_REJECTED") == 0)
    {
        v = DLMS_ASSOCIATION_RESULT_TRANSIENT_REJECTED;
    }
    else {
        v = (DLMS_ASSOCIATION_RESULT)-1;
    }
    return v;
}

const char* CGXDLMSConverter::ToString(DLMS_SOURCE_DIAGNOSTIC value)
{
    const char* str;
    switch (value)
    {
    case DLMS_SOURCE_DIAGNOSTIC_NONE:
        str = "NONE";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED:
        str = "APPLICATION_CONTEXT_NAME_NOT_SUPPORTED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AP_TITLE_NOT_RECOGNIZED:
        str = "APPLICATION_CALLING_AP_TITLE_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN:
        str = "NO_REASON_GIVEN";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED:
        str = "APPLICATION_CALLING_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AE_QUALIFIER_NOT_RECOGNIZED:
        str = "APPLICATION_CALLING_AE_QUALIFIER_NOT_RECOGNIZED ";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED:
        str = "APPLICATION_CALLING_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AP_TITLE_NOT_RECOGNIZED:
        str = "APPLICATION_CALLED_AP_TITLE_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED:
        str = "APPLICATION_CALLED_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AE_QUALIFIER_NOT_RECOGNIZED:
        str = "APPLICATION_CALLED_AE_QUALIFIER_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED:
        str = "APPLICATION_CALLED_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED:
        str = "AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_REQUIRED:
        str = "AUTHENTICATION_MECHANISM_NAME_REQUIRED";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_FAILURE:
        str = "AUTHENTICATION_FAILURE";
        break;
    case DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_REQUIRED:
        str = "AUTHENTICATION_REQUIRED";
        break;
    default:
        str = "UNKNOWN";
        break;
    }
    return str;
}

DLMS_SOURCE_DIAGNOSTIC CGXDLMSConverter::ValueOfSourceDiagnostic(const char* value)
{
    DLMS_SOURCE_DIAGNOSTIC v;
    if (strcmp(value, "NONE") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_NONE;
    }
    else if (strcmp(value, "APPLICATION_CONTEXT_NAME_NOT_SUPPORTED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED;
    }
    else if (strcmp(value, "APPLICATION_CALLING_AP_TITLE_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AP_TITLE_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "NO_REASON_GIVEN") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN;
    }
    else if (strcmp(value, "APPLICATION_CALLING_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "APPLICATION_CALLING_AE_QUALIFIER_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AE_QUALIFIER_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "APPLICATION_CALLING_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLING_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "APPLICATION_CALLED_AP_TITLE_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AP_TITLE_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "APPLICATION_CALLED_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AP_INVOCATION_IDENTIFIER_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "APPLICATION_CALLED_AE_QUALIFIER_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AE_QUALIFIER_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "APPLICATION_CALLED_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CALLED_AE_INVOCATION_IDENTIFIER_NOT_RECOGNIZED;
    }
    else if (strcmp(value, "AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED;
    }
    else if (strcmp(value, "AUTHENTICATION_MECHANISM_NAME_REQUIRED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_REQUIRED;
    }
    else if (strcmp(value, "AUTHENTICATION_FAILURE") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_FAILURE;
    }
    else if (strcmp(value, "AUTHENTICATION_REQUIRED") == 0)
    {
        v = DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_REQUIRED;
    }
    else {
        v = (DLMS_SOURCE_DIAGNOSTIC)-1;
    }
    return v;
}

const char* CGXDLMSConverter::ToString(DLMS_SECURITY value)
{
    switch (value)
    {
    case DLMS_SECURITY_NONE:
        return "None";
    case DLMS_SECURITY_AUTHENTICATION:
        return "Authentication";
    case DLMS_SECURITY_ENCRYPTION:
        return "Encryption";
    case DLMS_SECURITY_AUTHENTICATION_ENCRYPTION:
        return "AuthenticationEncryption";
    default:
        break;
    }
    return "Invalid";
}

const char* CGXDLMSConverter::ToString(DLMS_OBJECT_TYPE type)
{
    const char* str;
    switch (type)
    {
    case DLMS_OBJECT_TYPE_ACTION_SCHEDULE:
        str = "GXDLMSActionSchedule";
        break;
    case DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR:
        str = "GXDLMSActivityCalendar";
        break;
    case DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME:
        str = "GXDLMSAssociationLogicalName";
        break;
    case DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME:
        str = "GXDLMSAssociationShortName";
        break;
    case DLMS_OBJECT_TYPE_AUTO_ANSWER:
        str = "GXDLMSAutoAnswer";
        break;
    case DLMS_OBJECT_TYPE_AUTO_CONNECT:
        str = "GXDLMSAutoConnect";
        break;
    case DLMS_OBJECT_TYPE_CLOCK:
        str = "GXDLMSClock";
        break;
    case DLMS_OBJECT_TYPE_DATA:
        str = "GXDLMSData";
        break;
    case DLMS_OBJECT_TYPE_DEMAND_REGISTER:
        str = "GXDLMSDemandRegister";
        break;
    case DLMS_OBJECT_TYPE_MAC_ADDRESS_SETUP:
        str = "GXDLMSMacAddressSetup";
        break;
    case DLMS_OBJECT_TYPE_EXTENDED_REGISTER:
        str = "GXDLMSExtendedRegister";
        break;
    case DLMS_OBJECT_TYPE_GPRS_SETUP:
        str = "GXDLMSGprsSetup";
        break;
    case DLMS_OBJECT_TYPE_SECURITY_SETUP:
        str = "GXDLMSSecuritySetup";
        break;
    case DLMS_OBJECT_TYPE_IEC_HDLC_SETUP:
        str = "GXDLMSIecHdlcSetup";
        break;
    case DLMS_OBJECT_TYPE_IEC_LOCAL_PORT_SETUP:
        str = "GXDLMSIecLocalPortSetup";
        break;
    case DLMS_OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP:
        str = "GXDLMSIEC_TWISTED_PAIR_SETUP";
        break;
    case DLMS_OBJECT_TYPE_IP4_SETUP:
        str = "GXDLMSIp4Setup";
        break;
    case DLMS_OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP:
        str = "GXDLMSMBusSlavePortSetup";
        break;
    case DLMS_OBJECT_TYPE_IMAGE_TRANSFER:
        str = "GXDLMSImageTransfer";
        break;
    case DLMS_OBJECT_TYPE_DISCONNECT_CONTROL:
        str = "GXDLMSDisconnectControl";
        break;
    case DLMS_OBJECT_TYPE_LIMITER:
        str = "GXDLMSLimiter";
        break;
    case DLMS_OBJECT_TYPE_MBUS_CLIENT:
        str = "GXDLMSMBusClient";
        break;
    case DLMS_OBJECT_TYPE_MODEM_CONFIGURATION:
        str = "GXDLMSModemConfiguration";
        break;
    case DLMS_OBJECT_TYPE_PPP_SETUP:
        str = "GXDLMSPppSetup";
        break;
    case DLMS_OBJECT_TYPE_PROFILE_GENERIC:
        str = "GXDLMSProfileGeneric";
        break;
    case DLMS_OBJECT_TYPE_REGISTER:
        str = "GXDLMSRegister";
        break;
    case DLMS_OBJECT_TYPE_REGISTER_ACTIVATION:
        str = "GXDLMSRegisterActivation";
        break;
    case DLMS_OBJECT_TYPE_REGISTER_MONITOR:
        str = "GXDLMSRegisterMonitor";
        break;
    case DLMS_OBJECT_TYPE_REGISTER_TABLE:
        str = "GXDLMSRegisterTable";
        break;
    case DLMS_OBJECT_TYPE_ZIG_BEE_SAS_STARTUP:
        str = "GXDLMSZigBeeSasStartup";
        break;
    case DLMS_OBJECT_TYPE_ZIG_BEE_SAS_JOIN:
        str = "GXDLMSZigBeeSasJoin";
        break;
    case DLMS_OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION:
        str = "GXDLMSZigBeeSasApsFragmentation";
        break;
    case DLMS_OBJECT_TYPE_ZIG_BEE_NETWORK_CONTROL:
        str = "GXDLMSZigBeeNetworkControl";
        break;
    case DLMS_OBJECT_TYPE_SAP_ASSIGNMENT:
        str = "GXDLMSSapAssignment";
        break;
    case DLMS_OBJECT_TYPE_SCHEDULE:
        str = "GXDLMSSchedule";
        break;
    case DLMS_OBJECT_TYPE_SCRIPT_TABLE:
        str = "GXDLMSScriptTable";
        break;
    case DLMS_OBJECT_TYPE_SMTP_SETUP:
        str = "GXDLMSSMTPSetup";
        break;
    case DLMS_OBJECT_TYPE_SPECIAL_DAYS_TABLE:
        str = "GXDLMSSpecialDaysTable";
        break;
    case DLMS_OBJECT_TYPE_STATUS_MAPPING:
        str = "GXDLMSStatusMapping";
        break;
    case DLMS_OBJECT_TYPE_TCP_UDP_SETUP:
        str = "GXDLMSTcpUdpSetup";
        break;
    case DLMS_OBJECT_TYPE_UTILITY_TABLES:
        str = "GXDLMSUtilityTables";
        break;
    case DLMS_OBJECT_TYPE_MBUS_MASTER_PORT_SETUP:
        str = "GXDLMSMBusMasterPortSetup";
        break;
    case DLMS_OBJECT_TYPE_MESSAGE_HANDLER:
        str = "GXDLMSMessageHandler";
        break;
    case DLMS_OBJECT_TYPE_PUSH_SETUP:
        str = "GXDLMSPushSetup";
        break;
    case DLMS_OBJECT_TYPE_ACCOUNT:
        str = "GXDLMSAccount";
        break;
    case DLMS_OBJECT_TYPE_CREDIT:
        str = "GXDLMSCredit";
        break;
    case DLMS_OBJECT_TYPE_CHARGE:
        str = "GXDLMSCharge";
        break;
    case DLMS_OBJECT_TYPE_PARAMETER_MONITOR:
        str = "GXDLMSParameterMonitor";
        break;
    case DLMS_OBJECT_TYPE_TOKEN_GATEWAY:
        str = "GXDLMSTokenGateway";
        break;
    case DLMS_OBJECT_TYPE_GSM_DIAGNOSTIC:
        str = "GXDLMSGSMDiagnostic";
        break;
    case DLMS_OBJECT_TYPE_COMPACT_DATA:
        str = "GXDLMSCompactData";
        break;
    case DLMS_OBJECT_TYPE_LLC_SSCS_SETUP:
        str = "GXDLMSLlcSscsSetup";
        break;
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_PHYSICAL_LAYER_COUNTERS:
        str = "GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters";
        break;
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_SETUP:
        str = "GXDLMSPrimeNbOfdmPlcMacSetup";
        break;
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_FUNCTIONAL_PARAMETERS:
        str = "GXDLMSPrimeNbOfdmPlcMacFunctionalParameters";
        break;
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_COUNTERS:
        str = "GXDLMSPrimeNbOfdmPlcMacCounters";
        break;
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_NETWORK_ADMINISTRATION_DATA:
        str = "GXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData";
        break;
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_APPLICATIONS_IDENTIFICATION:
        str = "GXDLMSPrimeNbOfdmPlcApplicationsIdentification";
        break;
    default:
        str = "Manufacture spesific.";
        break;
    }
    return str;
}

DLMS_OBJECT_TYPE CGXDLMSConverter::ValueOfObjectType(const char* value)
{
    DLMS_OBJECT_TYPE type;
    if (strcmp(value, "GXDLMSActionSchedule") == 0)
        type = DLMS_OBJECT_TYPE_ACTION_SCHEDULE;
    else if (strcmp(value, "GXDLMSActivityCalendar") == 0)
        type = DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR;
    else if (strcmp(value, "GXDLMSAssociationLogicalName") == 0)
        type = DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME;
    else if (strcmp(value, "GXDLMSAssociationShortName") == 0)
        type = DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME;
    else if (strcmp(value, "GXDLMSAutoAnswer") == 0)
        type = DLMS_OBJECT_TYPE_AUTO_ANSWER;
    else if (strcmp(value, "GXDLMSAutoConnect") == 0)
        type = DLMS_OBJECT_TYPE_AUTO_CONNECT;
    else if (strcmp(value, "GXDLMSClock") == 0)
        type = DLMS_OBJECT_TYPE_CLOCK;
    else if (strcmp(value, "GXDLMSData") == 0)
        type = DLMS_OBJECT_TYPE_DATA;
    else if (strcmp(value, "GXDLMSDemandRegister") == 0)
        type = DLMS_OBJECT_TYPE_DEMAND_REGISTER;
    else if (strcmp(value, "GXDLMSMacAddressSetup") == 0)
        type = DLMS_OBJECT_TYPE_MAC_ADDRESS_SETUP;
    else if (strcmp(value, "GXDLMSExtendedRegister") == 0)
        type = DLMS_OBJECT_TYPE_EXTENDED_REGISTER;
    else if (strcmp(value, "GXDLMSGprsSetup") == 0)
        type = DLMS_OBJECT_TYPE_GPRS_SETUP;
    else if (strcmp(value, "GXDLMSSecuritySetup") == 0)
        type = DLMS_OBJECT_TYPE_SECURITY_SETUP;
    else if (strcmp(value, "GXDLMSIecHdlcSetup") == 0 || strcmp(value, "GXDLMSHdlcSetup") == 0)
        type = DLMS_OBJECT_TYPE_IEC_HDLC_SETUP;
    else if (strcmp(value, "GXDLMSIecLocalPortSetup") == 0 || strcmp(value, "GXDLMSIECOpticalPortSetup") == 0)
        type = DLMS_OBJECT_TYPE_IEC_LOCAL_PORT_SETUP;
    else if (strcmp(value, "GXDLMSIEC_TWISTED_PAIR_SETUP") == 0)
        type = DLMS_OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP;
    else if (strcmp(value, "GXDLMSIp4Setup") == 0)
        type = DLMS_OBJECT_TYPE_IP4_SETUP;
    else if (strcmp(value, "GXDLMSMBusSlavePortSetup") == 0)
        type = DLMS_OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP;
    else if (strcmp(value, "GXDLMSImageTransfer") == 0)
        type = DLMS_OBJECT_TYPE_IMAGE_TRANSFER;
    else if (strcmp(value, "GXDLMSDisconnectControl") == 0)
        type = DLMS_OBJECT_TYPE_DISCONNECT_CONTROL;
    else if (strcmp(value, "GXDLMSLimiter") == 0)
        type = DLMS_OBJECT_TYPE_LIMITER;
    else if (strcmp(value, "GXDLMSMBusClient") == 0)
        type = DLMS_OBJECT_TYPE_MBUS_CLIENT;
    else if (strcmp(value, "GXDLMSModemConfiguration") == 0)
        type = DLMS_OBJECT_TYPE_MODEM_CONFIGURATION;
    else if (strcmp(value, "GXDLMSPppSetup") == 0)
        type = DLMS_OBJECT_TYPE_PPP_SETUP;
    else if (strcmp(value, "GXDLMSProfileGeneric") == 0)
        type = DLMS_OBJECT_TYPE_PROFILE_GENERIC;
    else if (strcmp(value, "GXDLMSRegister") == 0)
        type = DLMS_OBJECT_TYPE_REGISTER;
    else if (strcmp(value, "GXDLMSRegisterActivation") == 0)
        type = DLMS_OBJECT_TYPE_REGISTER_ACTIVATION;
    else if (strcmp(value, "GXDLMSRegisterMonitor") == 0)
        type = DLMS_OBJECT_TYPE_REGISTER_MONITOR;
    else if (strcmp(value, "GXDLMSRegisterTable") == 0)
        type = DLMS_OBJECT_TYPE_REGISTER_TABLE;
    else if (strcmp(value, "GXDLMSZigBeeSasStartup") == 0)
        type = DLMS_OBJECT_TYPE_ZIG_BEE_SAS_STARTUP;
    else if (strcmp(value, "GXDLMSZigBeeSasJoin") == 0)
        type = DLMS_OBJECT_TYPE_ZIG_BEE_SAS_JOIN;
    else if (strcmp(value, "GXDLMSZigBeeSasApsFragmentation") == 0)
        type = DLMS_OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION;
    else if (strcmp(value, "GXDLMSZigBeeNetworkControl") == 0)
        type = DLMS_OBJECT_TYPE_ZIG_BEE_NETWORK_CONTROL;
    else if (strcmp(value, "GXDLMSSapAssignment") == 0)
        type = DLMS_OBJECT_TYPE_SAP_ASSIGNMENT;
    else if (strcmp(value, "GXDLMSSchedule") == 0)
        type = DLMS_OBJECT_TYPE_SCHEDULE;
    else if (strcmp(value, "GXDLMSScriptTable") == 0)
        type = DLMS_OBJECT_TYPE_SCRIPT_TABLE;
    else if (strcmp(value, "GXDLMSSMTPSetup") == 0)
        type = DLMS_OBJECT_TYPE_SMTP_SETUP;
    else if (strcmp(value, "GXDLMSSpecialDaysTable") == 0)
        type = DLMS_OBJECT_TYPE_SPECIAL_DAYS_TABLE;
    else if (strcmp(value, "GXDLMSStatusMapping") == 0)
        type = DLMS_OBJECT_TYPE_STATUS_MAPPING;
    else if (strcmp(value, "GXDLMSTcpUdpSetup") == 0)
        type = DLMS_OBJECT_TYPE_TCP_UDP_SETUP;
    else if (strcmp(value, "GXDLMSUtilityTables") == 0)
        type = DLMS_OBJECT_TYPE_UTILITY_TABLES;
    else if (strcmp(value, "GXDLMSMBusMasterPortSetup") == 0)
        type = DLMS_OBJECT_TYPE_MBUS_MASTER_PORT_SETUP;
    else if (strcmp(value, "GXDLMSMessageHandler") == 0)
        type = DLMS_OBJECT_TYPE_MESSAGE_HANDLER;
    else if (strcmp(value, "GXDLMSPushSetup") == 0)
        type = DLMS_OBJECT_TYPE_PUSH_SETUP;
    else if (strcmp(value, "GXDLMSAccount") == 0)
        type = DLMS_OBJECT_TYPE_ACCOUNT;
    else if (strcmp(value, "GXDLMSCredit") == 0)
        type = DLMS_OBJECT_TYPE_CREDIT;
    else if (strcmp(value, "GXDLMSCharge") == 0)
        type = DLMS_OBJECT_TYPE_CHARGE;
    else if (strcmp(value, "GXDLMSParameterMonitor") == 0)
        type = DLMS_OBJECT_TYPE_PARAMETER_MONITOR;
    else if (strcmp(value, "GXDLMSTokenGateway") == 0)
        type = DLMS_OBJECT_TYPE_TOKEN_GATEWAY;
    else if (strcmp(value, "GXDLMSGSMDiagnostic") == 0)
        type = DLMS_OBJECT_TYPE_GSM_DIAGNOSTIC;
    else if (strcmp(value, "GXDLMSCompactData") == 0)
        type = DLMS_OBJECT_TYPE_COMPACT_DATA;
    else if (strcmp(value, "GXDLMSLlcSscsSetup") == 0)
        type = DLMS_OBJECT_TYPE_LLC_SSCS_SETUP;
    else if (strcmp(value, "GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters") == 0)
        type = DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_PHYSICAL_LAYER_COUNTERS;
    else if (strcmp(value, "GXDLMSPrimeNbOfdmPlcMacSetup") == 0)
        type = DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_SETUP;
    else if (strcmp(value, "GXDLMSPrimeNbOfdmPlcMacFunctionalParameters") == 0)
        type = DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_FUNCTIONAL_PARAMETERS;
    else if (strcmp(value, "GXDLMSPrimeNbOfdmPlcMacCounters") == 0)
        type = DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_COUNTERS;
    else if (strcmp(value, "GXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData") == 0)
        type = DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_NETWORK_ADMINISTRATION_DATA;
    else if (strcmp(value, "GXDLMSPrimeNbOfdmPlcApplicationsIdentification") == 0)
        type = DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_APPLICATIONS_IDENTIFICATION;
    else
        type = DLMS_OBJECT_TYPE_NONE;
    return type;
}