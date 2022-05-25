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
#ifndef ERRORCODES_H
#define ERRORCODES_H

/////////////////////////////////////////////////////////////////////////////
// Enumerates all DLMS error codes.
// https://www.gurux.fi/Gurux.DLMS.ErrorCodes
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    //Meter is not accept frame.
    DLMS_ERROR_CODE_UNACCEPTABLE_FRAME = -3,
    //Meter rejects send packet.
    DLMS_ERROR_CODE_REJECTED = -2,
    DLMS_ERROR_CODE_FALSE = -1,
    //////////////////////////////////////////
    //DLMS Standard error codes start here.
    DLMS_ERROR_CODE_OK = 0,
    //Access Error : Device reports a hardware fault
    DLMS_ERROR_CODE_HARDWARE_FAULT = 1,
    //Access Error : Device reports a temporary failure
    DLMS_ERROR_CODE_TEMPORARY_FAILURE = 2,
    // Access Error : Device reports Read-Write denied
    DLMS_ERROR_CODE_READ_WRITE_DENIED = 3,
    // Access Error : Device reports a undefined object
    DLMS_ERROR_CODE_UNDEFINED_OBJECT = 4,
    // Access Error : Device reports a inconsistent Class or Object
    DLMS_ERROR_CODE_INCONSISTENT_CLASS = 9,
    // Access Error : Device reports a unavailable object
    DLMS_ERROR_CODE_UNAVAILABLE_OBJECT = 11,
    // Access Error : Device reports a unmatched type
    DLMS_ERROR_CODE_UNMATCH_TYPE = 12,
    // Access Error : Device reports scope of access violated
    DLMS_ERROR_CODE_ACCESS_VIOLATED = 13,
    // Access Error : Data Block Unavailable.
    DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE = 14,
    // Access Error : Long Get Or Read Aborted.
    DLMS_ERROR_CODE_LONG_GET_OR_READ_ABORTED = 15,
    // Access Error : No Long Get Or Read In Progress.
    DLMS_ERROR_CODE_NO_LONG_GET_OR_READ_IN_PROGRESS = 16,
    // Access Error : Long Set Or Write Aborted.
    DLMS_ERROR_CODE_LONG_SET_OR_WRITE_ABORTED = 17,
    // Access Error : No Long Set Or Write In Progress.
    DLMS_ERROR_CODE_NO_LONG_SET_OR_WRITE_IN_PROGRESS = 18,
    // Access Error : Data Block Number Invalid.
    DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID = 19,
    // Access Error : Other Reason.
    DLMS_ERROR_CODE_OTHER_REASON = 250,
    //DLMS Standard error codes end here.
    //////////////////////////////////////////

    //Unknown error.
    DLMS_ERROR_CODE_UNKNOWN,
    //Data send failed.
    DLMS_ERROR_CODE_SEND_FAILED,
    //Data receive failed.
    DLMS_ERROR_CODE_RECEIVE_FAILED,
    DLMS_ERROR_CODE_NOT_IMPLEMENTED,
    //Secure connection is not supported.
    DLMS_ERROR_CODE_DLMS_SECURITY_NOT_IMPLEMENTED,
    //Invalid DLMS command.
    DLMS_ERROR_CODE_INVALID_COMMAND,
    //Invalid Block number.
    DLMS_ERROR_CODE_INVALID_BLOCK_NUMBER,
    //Invalid parameter.
    DLMS_ERROR_CODE_INVALID_PARAMETER,
    //Server is not initialized.
    DLMS_ERROR_CODE_NOT_INITIALIZED,
    //Not enough memory available.
    DLMS_ERROR_CODE_OUTOFMEMORY,
    //Packet is not a reply for a send packet.
    DLMS_ERROR_CODE_NOT_REPLY,
    //Invalid Logical Name
    DLMS_ERROR_CODE_INVALID_LOGICAL_NAME,
    //Client HDLC Address is not set.
    DLMS_ERROR_CODE_INVALID_CLIENT_ADDRESS,
    //Server HDLC Address is not set.
    DLMS_ERROR_CODE_INVALID_SERVER_ADDRESS,
    //Not a HDLC frame.
    DLMS_ERROR_CODE_INVALID_DATA_FORMAT,
    //Invalid DLMS version number.
    DLMS_ERROR_CODE_INVALID_VERSION_NUMBER,
    //Client addresses do not match
    DLMS_ERROR_CODE_CLIENT_ADDRESS_NO_NOT_MATCH,
    //Server addresses do not match
    DLMS_ERROR_CODE_SERVER_ADDRESS_NO_NOT_MATCH,
    //CRC do not match.
    DLMS_ERROR_CODE_WRONG_CRC,
    //Invalid response
    DLMS_ERROR_CODE_INVALID_RESPONSE,
    //Invalid Tag.
    DLMS_ERROR_CODE_INVALID_TAG,
    //Encoding failed. Not enough data.
    DLMS_ERROR_CODE_ENCODING_FAILED,
    DLMS_ERROR_CODE_REJECTED_PERMAMENT,
    DLMS_ERROR_CODE_REJECTED_TRANSIENT,
    DLMS_ERROR_CODE_NO_REASON_GIVEN,
    DLMS_ERROR_CODE_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED,
    DLMS_ERROR_CODE_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED,
    DLMS_ERROR_CODE_AUTHENTICATION_MECHANISM_NAME_REQUIRED,
    DLMS_ERROR_CODE_AUTHENTICATION_FAILURE,
    DLMS_ERROR_CODE_AUTHENTICATION_REQUIRED,
    DLMS_ERROR_CODE_INVALID_DATE_TIME,
    DLMS_ERROR_CODE_INVALID_INVOKE_ID
}DLMS_ERROR_CODE;

#endif //ERRORCODES_H
