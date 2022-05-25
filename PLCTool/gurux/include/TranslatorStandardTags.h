//
// --------------------------------------------------------------------------
//  Gurux Ltd
//
//
//
// Filename:        $HeadURL$
//
// Version:         $Revision$]=
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
// Gurux Device Framework is distributed in the hope that it will be useful]=
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#ifndef TRANSLATOR_STANDARD_TAGS_H
#define TRANSLATOR_STANDARD_TAGS_H

#include "enums.h"
#include "errorcodes.h"
#include "TranslatorTags.h"
#include "TranslatorGeneralTags.h"

class CTranslatorStandardTags
{
public:
    /// Get general tags.
    static void GetGeneralTags(DLMS_TRANSLATOR_OUTPUT_TYPE, std::map<unsigned long, std::string >& list)
    {
        list[DLMS_COMMAND_SNRM] = "Snrm";
        list[DLMS_COMMAND_UNACCEPTABLE_FRAME] = "UnacceptableFrame";
        list[DLMS_COMMAND_DISCONNECT_MODE] = "DisconnectMode";
        list[DLMS_COMMAND_UA] = "Ua";
        list[DLMS_COMMAND_AARQ] = "aarq";
        list[DLMS_COMMAND_AARE] = "aare";
        list[TRANSLATOR_GENERAL_TAGS_APPLICATION_CONTEXT_NAME] = "application-context-name";
        list[DLMS_COMMAND_INITIATE_RESPONSE] = "InitiateResponse";
        list[DLMS_COMMAND_INITIATE_REQUEST] = "user-information";
        list[TRANSLATOR_GENERAL_TAGS_NEGOTIATED_QUALITY_OF_SERVICE] = "negotiated-quality-of-service";
        list[TRANSLATOR_GENERAL_TAGS_PROPOSED_QUALITY_OF_SERVICE] = "proposed-quality-of-service";
        list[TRANSLATOR_GENERAL_TAGS_PROPOSED_DLMS_VERSION_NUMBER] = "proposed-dlms-version-number";
        list[TRANSLATOR_GENERAL_TAGS_PROPOSED_MAX_PDU_SIZE] = "client-max-receive-pdu-size";
        list[TRANSLATOR_GENERAL_TAGS_PROPOSED_CONFORMANCE] = "proposed-conformance";
        list[TRANSLATOR_GENERAL_TAGS_VAA_NAME] = "VaaName";
        list[TRANSLATOR_GENERAL_TAGS_NEGOTIATED_CONFORMANCE] = "NegotiatedConformance";
        list[TRANSLATOR_GENERAL_TAGS_NEGOTIATED_DLMS_VERSION_NUMBER] = "NegotiatedDlmsVersionNumber";
        list[TRANSLATOR_GENERAL_TAGS_NEGOTIATED_MAX_PDU_SIZE] = "NegotiatedMaxPduSize";
        list[TRANSLATOR_GENERAL_TAGS_CONFORMANCE_BIT] = "ConformanceBit";
        list[TRANSLATOR_GENERAL_TAGS_SENDER_ACSE_REQUIREMENTS] = "sender-acse-requirements";
        list[TRANSLATOR_GENERAL_TAGS_RESPONDER_ACSE_REQUIREMENT] = "responder-acse-requirements";
        list[TRANSLATOR_GENERAL_TAGS_RESPONDING_MECHANISM_NAME] = "mechanism-name";
        list[TRANSLATOR_GENERAL_TAGS_CALLING_MECHANISM_NAME] = "mechanism-name";
        list[TRANSLATOR_GENERAL_TAGS_CALLING_AUTHENTICATION] = "calling-authentication-value";
        list[TRANSLATOR_GENERAL_TAGS_RESPONDING_AUTHENTICATION] = "responding-authentication-value";
        list[DLMS_COMMAND_RELEASE_REQUEST] = "rlrq";
        list[DLMS_COMMAND_RELEASE_RESPONSE] = "rlre";
        list[DLMS_COMMAND_DISCONNECT_REQUEST] = "Disc";
        list[TRANSLATOR_GENERAL_TAGS_ASSOCIATION_RESULT] = "result";
        list[TRANSLATOR_GENERAL_TAGS_RESULT_SOURCE_DIAGNOSTIC] = "result-source-diagnostic";
        list[TRANSLATOR_GENERAL_TAGS_ACSE_SERVICE_USER] = "acse-service-user";
        list[TRANSLATOR_GENERAL_TAGS_CALLING_AP_TITLE] = "CallingAPTitle";
        list[TRANSLATOR_GENERAL_TAGS_RESPONDING_AP_TITLE] = "RespondingAPTitle";
        list[TRANSLATOR_GENERAL_TAGS_CHAR_STRING] = "charstring";
        list[TRANSLATOR_GENERAL_TAGS_DEDICATED_KEY] = "dedicated-key";
        list[TRANSLATOR_GENERAL_TAGS_RESPONSE_ALLOWED] = "response-allowed";
        list[TRANSLATOR_GENERAL_TAGS_USER_INFORMATION] = "user-information";
        list[DLMS_COMMAND_CONFIRMED_SERVICE_ERROR] = "confirmedServiceError";
        list[DLMS_COMMAND_INFORMATION_REPORT] = "informationReportRequest";
        list[DLMS_COMMAND_EVENT_NOTIFICATION] = "event-notification-request";
        list[TRANSLATOR_GENERAL_TAGS_CALLING_AE_INVOCATION_ID] = "calling-AE-invocation-id";
        list[TRANSLATOR_GENERAL_TAGS_CALLED_AE_INVOCATION_ID] = "called-AE-invocation-id";
        list[TRANSLATOR_GENERAL_TAGS_RESPONDING_AE_INVOCATION_ID] = "responding-AE-invocation-id";
    }

    // Get SN tags.
    static void GetSnTags(DLMS_TRANSLATOR_OUTPUT_TYPE, std::map<unsigned long, std::string >& list)
    {
        list[DLMS_COMMAND_READ_REQUEST] = "readRequest";
        list[DLMS_COMMAND_WRITE_REQUEST] = "writeRequest";
        list[DLMS_COMMAND_WRITE_RESPONSE] = "writeResponse";
        list[DLMS_COMMAND_WRITE_REQUEST << 8 | DLMS_SINGLE_READ_RESPONSE_DATA] = "Data";
        list[DLMS_COMMAND_READ_REQUEST << 8 | DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME] = "variable-name";
        list[DLMS_COMMAND_READ_REQUEST << 8 | DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS] = "parameterized-access";
        list[DLMS_COMMAND_READ_REQUEST << 8 | DLMS_VARIABLE_ACCESS_SPECIFICATION_BLOCK_NUMBER_ACCESS] = "BlockNumberAccess";
        list[DLMS_COMMAND_WRITE_REQUEST << 8 | DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME] = "variable-name";
        list[DLMS_COMMAND_READ_RESPONSE] = "readResponse";
        list[DLMS_COMMAND_READ_RESPONSE << 8 | DLMS_SINGLE_READ_RESPONSE_DATA_BLOCK_RESULT] = "DataBlockResult";
        list[DLMS_COMMAND_READ_RESPONSE << 8 | DLMS_SINGLE_READ_RESPONSE_DATA] = "data";
        list[DLMS_COMMAND_WRITE_RESPONSE << 8 | DLMS_SINGLE_READ_RESPONSE_DATA] = "data";
        list[DLMS_COMMAND_READ_RESPONSE << 8 | DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR] = "data-access-error";
    }

    // Get LN tags.
    static void GetLnTags(DLMS_TRANSLATOR_OUTPUT_TYPE, std::map<unsigned long, std::string >& list)
    {
        list[DLMS_COMMAND_GET_REQUEST] = "get-request";
        list[DLMS_COMMAND_GET_REQUEST << 8 | DLMS_GET_COMMAND_TYPE_NORMAL] = "get-request-normal";
        list[DLMS_COMMAND_GET_REQUEST << 8 | DLMS_GET_COMMAND_TYPE_NEXT_DATA_BLOCK] = "get-request-next";
        list[DLMS_COMMAND_GET_REQUEST << 8 | DLMS_GET_COMMAND_TYPE_WITH_LIST] = "get-request-with-list";
        list[DLMS_COMMAND_SET_REQUEST] = "set-request";
        list[DLMS_COMMAND_SET_REQUEST << 8 | DLMS_SET_REQUEST_TYPE_NORMAL] = "set-request-normal";
        list[DLMS_COMMAND_SET_REQUEST << 8 | DLMS_SET_REQUEST_TYPE_FIRST_DATA_BLOCK] = "set-request-first-data-block";
        list[DLMS_COMMAND_SET_REQUEST << 8 | DLMS_SET_REQUEST_TYPE_WITH_DATA_BLOCK] = "set-request-with-data-block";
        list[DLMS_COMMAND_SET_REQUEST << 8 | DLMS_SET_REQUEST_TYPE_WITH_LIST] = "set-request-with-list";
        list[DLMS_COMMAND_METHOD_REQUEST] = "action-request";
        list[DLMS_COMMAND_METHOD_REQUEST << 8 | DLMS_ACTION_REQUEST_TYPE_NORMAL] = "action-request-normal";
        list[DLMS_COMMAND_METHOD_REQUEST << 8 | DLMS_ACTION_REQUEST_TYPE_NEXT_BLOCK] = "ActionRequestForNextDataBlock";
        list[DLMS_COMMAND_METHOD_REQUEST << 8 | DLMS_ACTION_REQUEST_TYPE_WITH_LIST] = "action-request-with-list";
        list[DLMS_COMMAND_METHOD_RESPONSE] = "action-response";
        list[DLMS_COMMAND_METHOD_RESPONSE << 8 | DLMS_ACTION_REQUEST_TYPE_NORMAL] = "action-response-normal";
        list[DLMS_COMMAND_METHOD_RESPONSE << 8 | DLMS_ACTION_REQUEST_TYPE_NEXT_BLOCK] = "action-response-with-first-block";
        list[DLMS_COMMAND_METHOD_RESPONSE << 8 | DLMS_ACTION_REQUEST_TYPE_WITH_LIST] = "action-response-with-list";
        list[DLMS_TRANSLATOR_TAGS_SINGLE_RESPONSE] = "single-response";
        list[DLMS_COMMAND_DATA_NOTIFICATION] = "data-notification";
        list[DLMS_COMMAND_GET_RESPONSE] = "get-response";
        list[DLMS_COMMAND_GET_RESPONSE << 8 | DLMS_GET_COMMAND_TYPE_NORMAL] = "get-response-normal";
        list[DLMS_COMMAND_GET_RESPONSE << 8 | DLMS_GET_COMMAND_TYPE_NEXT_DATA_BLOCK] = "get-response-with-data-block";
        list[DLMS_COMMAND_GET_RESPONSE << 8 | DLMS_GET_COMMAND_TYPE_WITH_LIST] = "get-response-with-list";
        list[DLMS_COMMAND_SET_RESPONSE] = "set-response";
        list[DLMS_COMMAND_SET_RESPONSE << 8 | DLMS_SET_RESPONSE_TYPE_NORMAL] = "set-response-normal";
        list[DLMS_COMMAND_SET_RESPONSE << 8 | DLMS_SET_RESPONSE_TYPE_DATA_BLOCK] = "set-response-data-block";
        list[DLMS_COMMAND_SET_RESPONSE << 8 | DLMS_SET_RESPONSE_TYPE_LAST_DATA_BLOCK] = "set-response-with-last-data-block";
        list[DLMS_COMMAND_SET_RESPONSE << 8 | DLMS_SET_RESPONSE_TYPE_WITH_LIST] = "set-response-with-list";
        list[DLMS_COMMAND_ACCESS_REQUEST] = "access-request";
        list[DLMS_COMMAND_ACCESS_REQUEST << 8 | DLMS_ACCESS_SERVICE_COMMAND_TYPE_GET] = "access-request-get";
        list[DLMS_COMMAND_ACCESS_REQUEST << 8 | DLMS_ACCESS_SERVICE_COMMAND_TYPE_SET] = "access-request-set";
        list[DLMS_COMMAND_ACCESS_REQUEST << 8 | DLMS_ACCESS_SERVICE_COMMAND_TYPE_ACTION] = "access-request-action";
        list[DLMS_COMMAND_ACCESS_RESPONSE] = "access-response";
        list[DLMS_COMMAND_ACCESS_RESPONSE << 8 | DLMS_ACCESS_SERVICE_COMMAND_TYPE_GET] = "access-response-get";
        list[DLMS_COMMAND_ACCESS_RESPONSE << 8 | DLMS_ACCESS_SERVICE_COMMAND_TYPE_SET] = "access-response-set";
        list[DLMS_COMMAND_ACCESS_RESPONSE << 8 | DLMS_ACCESS_SERVICE_COMMAND_TYPE_ACTION] = "access-response-action";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_BODY] = "access-request-body";
        list[DLMS_TRANSLATOR_TAGS_LIST_OF_ACCESS_REQUEST_SPECIFICATION] = "access-request-specification";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_SPECIFICATION] = "Access-Request-Specification";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_LIST_OF_DATA] = "access-request-list-of-data";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_RESPONSE_BODY] = "access-response-body";
        list[DLMS_TRANSLATOR_TAGS_LIST_OF_ACCESS_RESPONSE_SPECIFICATION] = "access-response-specification";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_RESPONSE_SPECIFICATION] = "Access-Response-Specification";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_RESPONSE_LIST_OF_DATA] = "access-response-list-of-data";
        list[DLMS_TRANSLATOR_TAGS_SERVICE] = "service";
        list[DLMS_TRANSLATOR_TAGS_SERVICE_ERROR] = "service-error";
        list[DLMS_COMMAND_GENERAL_BLOCK_TRANSFER] = "general-block-transfer";
        list[DLMS_COMMAND_GATEWAY_REQUEST] = "gateway-request";
        list[DLMS_COMMAND_GATEWAY_RESPONSE] = "gateway-response";
    }

    // Get glo tags.
    static void GetGloTags(DLMS_TRANSLATOR_OUTPUT_TYPE, std::map<unsigned long, std::string >& list)
    {
        list[DLMS_COMMAND_GLO_INITIATE_REQUEST] = "glo-initiate-request";
        list[DLMS_COMMAND_GLO_INITIATE_RESPONSE] = "glo-initiate-response";
        list[DLMS_COMMAND_GLO_GET_REQUEST] = "glo-get-request";
        list[DLMS_COMMAND_GLO_GET_RESPONSE] = "glo-get-response";
        list[DLMS_COMMAND_GLO_SET_REQUEST] = "glo-set-request";
        list[DLMS_COMMAND_GLO_SET_RESPONSE] = "glo-set-response";
        list[DLMS_COMMAND_GLO_METHOD_REQUEST] = "glo-action-request";
        list[DLMS_COMMAND_GLO_METHOD_RESPONSE] = "glo-action-response";
        list[DLMS_COMMAND_GLO_READ_REQUEST] = "glo-read-request";
        list[DLMS_COMMAND_GLO_READ_RESPONSE] = "glo-read-response";
        list[DLMS_COMMAND_GLO_WRITE_REQUEST] = "glo-write-request";
        list[DLMS_COMMAND_GLO_WRITE_RESPONSE] = "glo-write-response";
        list[DLMS_COMMAND_GENERAL_GLO_CIPHERING] = "general-glo-ciphering";
    }

    // Get ded tags.
    static void GetDedTags(DLMS_TRANSLATOR_OUTPUT_TYPE, std::map<unsigned long, std::string >& list)
    {
        list[DLMS_COMMAND_DED_INITIATE_REQUEST] = "ded-initiate-request";
        list[DLMS_COMMAND_DED_INITIATE_RESPONSE] = "ded-initiate-response";
        list[DLMS_COMMAND_DED_GET_REQUEST] = "ded-get-request";
        list[DLMS_COMMAND_DED_GET_RESPONSE] = "ded-get-response";
        list[DLMS_COMMAND_DED_SET_REQUEST] = "ded-set-request";
        list[DLMS_COMMAND_DED_SET_RESPONSE] = "ded-set-response";
        list[DLMS_COMMAND_DED_METHOD_REQUEST] = "ded-action-request";
        list[DLMS_COMMAND_DED_METHOD_RESPONSE] = "ded-action-response";
        list[DLMS_COMMAND_GENERAL_DED_CIPHERING] = "general-ded-ciphering";
    }

    /// Get translator tags.
    static void GetTranslatorTags(DLMS_TRANSLATOR_OUTPUT_TYPE, std::map<unsigned long, std::string >& list)
    {
        list[DLMS_TRANSLATOR_TAGS_WRAPPER] = "Wrapper";
        list[DLMS_TRANSLATOR_TAGS_HDLC] = "Hdlc";
        list[DLMS_TRANSLATOR_TAGS_PDU_DLMS] = "xDLMS-APDU";
        list[DLMS_TRANSLATOR_TAGS_PDU_CSE] = "aCSE-APDU";
        list[DLMS_TRANSLATOR_TAGS_TARGET_ADDRESS] = "TargetAddress";
        list[DLMS_TRANSLATOR_TAGS_SOURCE_ADDRESS] = "SourceAddress";
        list[DLMS_TRANSLATOR_TAGS_LIST_OF_VARIABLE_ACCESS_SPECIFICATION] = "variable-access-specification";
        list[DLMS_TRANSLATOR_TAGS_LIST_OF_DATA] = "list-of-data";
        list[DLMS_TRANSLATOR_TAGS_SUCCESS] = "Success";
        list[DLMS_TRANSLATOR_TAGS_DATA_ACCESS_ERROR] = "data-access-result";
        list[DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR] = "cosem-attribute-descriptor";
        list[DLMS_TRANSLATOR_TAGS_CLASS_ID] = "class-id";
        list[DLMS_TRANSLATOR_TAGS_INSTANCE_ID] = "instance-id";
        list[DLMS_TRANSLATOR_TAGS_ATTRIBUTE_ID] = "attribute-id";
        list[DLMS_TRANSLATOR_TAGS_METHOD_INVOCATION_PARAMETERS] = "method-invocation-parameters";
        list[DLMS_TRANSLATOR_TAGS_SELECTOR] = "selector";
        list[DLMS_TRANSLATOR_TAGS_PARAMETER] = "parameter";
        list[DLMS_TRANSLATOR_TAGS_LAST_BLOCK] = "LastBlock";
        list[DLMS_TRANSLATOR_TAGS_BLOCK_NUMBER] = "block-number";
        list[DLMS_TRANSLATOR_TAGS_RAW_DATA] = "RawData";
        list[DLMS_TRANSLATOR_TAGS_METHOD_DESCRIPTOR] = "cosem-method-descriptor";
        list[DLMS_TRANSLATOR_TAGS_METHOD_ID] = "method-id";
        list[DLMS_TRANSLATOR_TAGS_RESULT] = "result";
        list[DLMS_TRANSLATOR_TAGS_RETURN_PARAMETERS] = "return-parameters";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_SELECTION] = "access-selection";
        list[DLMS_TRANSLATOR_TAGS_VALUE] = "value";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_SELECTOR] = "access-selector";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_PARAMETERS] = "access-parameters";
        list[DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_LIST] = "attribute-descriptor-list";
        list[DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_WITH_SELECTION] = "Cosem-Attribute-Descriptor-With-Selection";
        list[DLMS_TRANSLATOR_TAGS_READ_DATA_BLOCK_ACCESS] = "ReadDataBlockAccess";
        list[DLMS_TRANSLATOR_TAGS_WRITE_DATA_BLOCK_ACCESS] = "WriteDataBlockAccess";
        list[DLMS_TRANSLATOR_TAGS_DATA] = "data";
        list[DLMS_TRANSLATOR_TAGS_INVOKE_ID] = "invoke-id-and-priority";
        list[DLMS_TRANSLATOR_TAGS_LONG_INVOKE_ID] = "long-invoke-id-and-priority";
        list[DLMS_TRANSLATOR_TAGS_DATE_TIME] = "date-time";
        list[DLMS_TRANSLATOR_TAGS_CURRENT_TIME] = "current-time";
        list[DLMS_TRANSLATOR_TAGS_TIME] = "time";
        list[DLMS_TRANSLATOR_TAGS_REASON] = "Reason";
        list[DLMS_TRANSLATOR_TAGS_VARIABLE_ACCESS_SPECIFICATION] = "Variable-Access-Specification";
        list[DLMS_TRANSLATOR_TAGS_CHOICE] = "CHOICE";
        list[DLMS_TRANSLATOR_TAGS_NOTIFICATION_BODY] = "notification-body";
        list[DLMS_TRANSLATOR_TAGS_DATA_VALUE] = "data-value";
        list[DLMS_TRANSLATOR_TAGS_INITIATE_ERROR] = "initiateError";
        list[DLMS_TRANSLATOR_TAGS_CIPHERED_SERVICE] = "ciphered-content";
        list[DLMS_TRANSLATOR_TAGS_SYSTEM_TITLE] = "system-title";
        list[DLMS_TRANSLATOR_TAGS_DATA_BLOCK] = "DataBlock";
        list[DLMS_TRANSLATOR_TAGS_TRANSACTION_ID] = "TransactionId";
        list[DLMS_TRANSLATOR_TAGS_ORIGINATOR_SYSTEM_TITLE] = "OriginatorSystemTitle";
        list[DLMS_TRANSLATOR_TAGS_RECIPIENT_SYSTEM_TITLE] = "RecipientSystemTitle";
        list[DLMS_TRANSLATOR_TAGS_OTHER_INFORMATION] = "OtherInformation";
        list[DLMS_TRANSLATOR_TAGS_KEY_INFO] = "KeyInfo";
        list[DLMS_TRANSLATOR_TAGS_CIPHERED_CONTENT] = "CipheredContent";
        list[DLMS_TRANSLATOR_TAGS_AGREED_KEY] = "AgreedKey";
        list[DLMS_TRANSLATOR_TAGS_KEY_PARAMETERS] = "KeyParameters";
        list[DLMS_TRANSLATOR_TAGS_KEY_CIPHERED_DATA] = "KeyCipheredData";
        list[DLMS_TRANSLATOR_TAGS_ATTRIBUTE_VALUE] = "attribute-value";
        list[DLMS_TRANSLATOR_TAGS_MAX_INFO_RX] = "MaxInfoRX";
        list[DLMS_TRANSLATOR_TAGS_MAX_INFO_TX] = "MaxInfoTX";
        list[DLMS_TRANSLATOR_TAGS_WINDOW_SIZE_RX] = "WindowSizeRX";
        list[DLMS_TRANSLATOR_TAGS_WINDOW_SIZE_TX] = "WindowSizeTX";
        list[DLMS_TRANSLATOR_TAGS_VALUE_LIST] = "value-list";
        list[DLMS_TRANSLATOR_TAGS_DATA_ACCESS_RESULT] = "data-access-result";
        list[DLMS_TRANSLATOR_TAGS_BLOCK_CONTROL] = "block-control";
        list[DLMS_TRANSLATOR_TAGS_BLOCK_NUMBER_ACK] = "block-number-ack";
        list[DLMS_TRANSLATOR_TAGS_BLOCK_DATA] = "block-data";
        list[DLMS_TRANSLATOR_TAGS_CONTENTS_DESCRIPTION] = "contents-description";
        list[DLMS_TRANSLATOR_TAGS_ARRAY_CONTENTS] = "array-contents";
        list[DLMS_TRANSLATOR_TAGS_NETWORK_ID] = "network-id";
        list[DLMS_TRANSLATOR_TAGS_PHYSICAL_DEVICE_ADDRESS] = "physical-device-address";
        list[DLMS_TRANSLATOR_TAGS_PROTOCOL_VERSION] = "protocol-version";
        list[DLMS_TRANSLATOR_TAGS_CALLED_AP_TITLE] = "called-ap-title";
        list[DLMS_TRANSLATOR_TAGS_CALLED_AP_INVOCATION_ID] = "called-ap-invocation-id";
        list[DLMS_TRANSLATOR_TAGS_CALLED_AE_INVOCATION_ID] = "called-ae-invocation-id";
        list[DLMS_TRANSLATOR_TAGS_CALLING_AP_INVOCATION_ID] = "calling-ap-invocation-id";
        list[DLMS_TRANSLATOR_TAGS_CALLED_AE_QUALIFIER] = "called-ae-qualifier";
    }

    static void GetDataTypeTags(std::map<unsigned long, std::string >& list)
    {
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_NONE] = "null-data";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_ARRAY] = "array";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_BINARY_CODED_DESIMAL] = "bcd";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_BIT_STRING] = "bit-string";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_BOOLEAN] = "boolean";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_COMPACT_ARRAY] = "compact-array";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_DATE] = "date";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_DATETIME] = "date-time";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_ENUM] = "enum";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_FLOAT32] = "float32";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_FLOAT64] = "float64]=";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_INT16] = "long";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_INT32] = "double-long";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_INT64] = "long64";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_INT8] = "integer";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_OCTET_STRING] = "octet-string";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_STRING] = "visible-string";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_STRING_UTF8] = "utf8-string";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_STRUCTURE] = "structure";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_TIME] = "time";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_UINT16] = "long-unsigned";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_UINT32] = "double-long-unsigned";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_UINT64] = "long64-unsigned";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_UINT8] = "unsigned";
    }

    static int ErrorCodeToString(DLMS_ERROR_CODE value, std::string& str)
    {
        switch (value)
        {
        case DLMS_ERROR_CODE_ACCESS_VIOLATED:
            str = "scope-of-access-violated";
            break;
        case DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID:
            str = "data-block-number-invalid";
            break;
        case DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE:
            str = "data-block-unavailable";
            break;
        case DLMS_ERROR_CODE_HARDWARE_FAULT:
            str = "hardware-fault";
            break;
        case DLMS_ERROR_CODE_INCONSISTENT_CLASS:
            str = "object-class-inconsistent";
            break;
        case DLMS_ERROR_CODE_LONG_GET_OR_READ_ABORTED:
            str = "long-Get-aborted";
            break;
        case DLMS_ERROR_CODE_LONG_SET_OR_WRITE_ABORTED:
            str = "long-set-aborted";
            break;
        case DLMS_ERROR_CODE_NO_LONG_GET_OR_READ_IN_PROGRESS:
            str = "no-long-Get-in-progress";
            break;
        case DLMS_ERROR_CODE_NO_LONG_SET_OR_WRITE_IN_PROGRESS:
            str = "no-long-set-in-progress";
            break;
        case DLMS_ERROR_CODE_OK:
            str = "success";
            break;
        case DLMS_ERROR_CODE_OTHER_REASON:
            str = "other-reason";
            break;
        case DLMS_ERROR_CODE_READ_WRITE_DENIED:
            str = "read-write-denied";
            break;
        case DLMS_ERROR_CODE_TEMPORARY_FAILURE:
            str = "temporary-failure";
            break;
        case DLMS_ERROR_CODE_UNAVAILABLE_OBJECT:
            str = "object-unavailable";
            break;
        case DLMS_ERROR_CODE_UNDEFINED_OBJECT:
            str = "object-undefined";
            break;
        case DLMS_ERROR_CODE_UNMATCH_TYPE:
            str = "type-unmatched";
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        return 0;
    }

    static DLMS_ERROR_CODE ValueOfErrorCode(std::string& value)
    {
        DLMS_ERROR_CODE v;
        if (value.compare("scope-of-access-violated") == 0)
        {
            v = DLMS_ERROR_CODE_ACCESS_VIOLATED;
        }
        else if (value.compare("data-block-number-invalid") == 0)
        {
            v = DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID;
        }
        else if (value.compare("data-block-unavailable") == 0)
        {
            v = DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE;
        }
        else if (value.compare("hardware-fault") == 0)
        {
            v = DLMS_ERROR_CODE_HARDWARE_FAULT;
        }
        else if (value.compare("object-class-inconsistent") == 0)
        {
            v = DLMS_ERROR_CODE_INCONSISTENT_CLASS;
        }
        else if (value.compare("long-Get-aborted") == 0)
        {
            v = DLMS_ERROR_CODE_LONG_GET_OR_READ_ABORTED;
        }
        else if (value.compare("long-set-aborted") == 0)
        {
            v = DLMS_ERROR_CODE_LONG_SET_OR_WRITE_ABORTED;
        }
        else if (value.compare("no-long-Get-in-progress") == 0)
        {
            v = DLMS_ERROR_CODE_NO_LONG_GET_OR_READ_IN_PROGRESS;
        }
        else if (value.compare("no-long-set-in-progress") == 0)
        {
            v = DLMS_ERROR_CODE_NO_LONG_SET_OR_WRITE_IN_PROGRESS;
        }
        else if (value.compare("success") == 0)
        {
            v = DLMS_ERROR_CODE_OK;
        }
        else if (value.compare("other-reason") == 0)
        {
            v = DLMS_ERROR_CODE_OTHER_REASON;
        }
        else if (value.compare("read-write-denied") == 0)
        {
            v = DLMS_ERROR_CODE_READ_WRITE_DENIED;
        }
        else if (value.compare("temporary-failure") == 0)
        {
            v = DLMS_ERROR_CODE_TEMPORARY_FAILURE;
        }
        else if (value.compare("object-unavailable") == 0)
        {
            v = DLMS_ERROR_CODE_UNAVAILABLE_OBJECT;
        }
        else if (value.compare("object-undefined") == 0)
        {
            v = DLMS_ERROR_CODE_UNDEFINED_OBJECT;
        }
        else if (value.compare("type-unmatched") == 0)
        {
            v = DLMS_ERROR_CODE_UNMATCH_TYPE;
        }
        else
        {
            v = DLMS_ERROR_CODE_UNKNOWN;
        }
        return v;
    }

    static void GetServiceErrors(std::map <DLMS_SERVICE_ERROR, std::string > &list)
    {
        list[DLMS_SERVICE_ERROR_APPLICATION_REFERENCE] = "application-reference";
        list[DLMS_SERVICE_ERROR_HARDWARE_RESOURCE] = "hardware-resource";
        list[DLMS_SERVICE_ERROR_VDE_STATE_ERROR] = "vde-state-error";
        list[DLMS_SERVICE_ERROR_SERVICE] = "service";
        list[DLMS_SERVICE_ERROR_DEFINITION] = "definition";
        list[DLMS_SERVICE_ERROR_ACCESS] = "access";
        list[DLMS_SERVICE_ERROR_INITIATE] = "initiate";
        list[DLMS_SERVICE_ERROR_LOAD_DATASET] = "load-data-set";
        list[DLMS_SERVICE_ERROR_TASK] = "task";
    }

    static void GetApplicationReference(std::map < DLMS_APPLICATION_REFERENCE, std::string > &list)
    {
        list[DLMS_APPLICATION_REFERENCE_APPLICATION_CONTEXT_UNSUPPORTED] = "application-context-unsupported";
        list[DLMS_APPLICATION_REFERENCE_APPLICATION_REFERENCE_INVALID] = "application-reference-invalid";
        list[DLMS_APPLICATION_REFERENCE_APPLICATION_UNREACHABLE] = "application-unreachable";
        list[DLMS_APPLICATION_REFERENCE_DECIPHERING_ERROR] = "deciphering-error";
        list[DLMS_APPLICATION_REFERENCE_OTHER] = "other";
        list[DLMS_APPLICATION_REFERENCE_PROVIDER_COMMUNICATION_ERROR] = "provider-communication-error";
        list[DLMS_APPLICATION_REFERENCE_TIME_ELAPSED] = "time-elapsed";
    }

    static void GetHardwareResource(std::map < DLMS_HARDWARE_RESOURCE, std::string > &list)
    {
        list[DLMS_HARDWARE_RESOURCE_MASS_STORAGE_UNAVAILABLE] = "mass-storage-unavailable";
        list[DLMS_HARDWARE_RESOURCE_MEMORY_UNAVAILABLE] = "memory-unavailable";
        list[DLMS_HARDWARE_RESOURCE_OTHER] = "other";
        list[DLMS_HARDWARE_RESOURCE_OTHER_RESOURCE_UNAVAILABLE] = "other-resource-unavailable";
        list[DLMS_HARDWARE_RESOURCE_PROCESSOR_RESOURCE_UNAVAILABLE] = "processor-resource-unavailable";
    }

    static void GetVdeStateError(std::map < DLMS_VDE_STATE_ERROR, std::string > &list)
    {
        list[DLMS_VDE_STATE_ERROR_LOADING_DATASET] = "loading-data-set";
        list[DLMS_VDE_STATE_ERROR_NO_DLMS_CONTEXT] = "no-dlms-context";
        list[DLMS_VDE_STATE_ERROR_OTHER] = "other";
        list[DLMS_VDE_STATE_ERROR_STATUS_INOPERABLE] = "status-inoperable";
        list[DLMS_VDE_STATE_ERROR_STATUS_NO_CHANGE] = "status-nochange";
    }

    static void GetService(std::map < DLMS_SERVICE, std::string > &list)
    {
        list[DLMS_SERVICE_OTHER] = "other";
        list[DLMS_SERVICE_PDU_SIZE] = "pdu-size";
        list[DLMS_SERVICE_UNSUPPORTED] = "service-unsupported";
    }

    static void GetDefinition(std::map < DLMS_DEFINITION, std::string > &list)
    {
        list[DLMS_DEFINITION_OBJECT_ATTRIBUTE_INCONSISTENT] = "object-attribute-inconsistent";
        list[DLMS_DEFINITION_OBJECT_CLASS_INCONSISTENT] = "object-class-inconsistent";
        list[DLMS_DEFINITION_OBJECT_UNDEFINED] = "object-undefined";
        list[DLMS_DEFINITION_OTHER] = "other";
    }

    static void GetAccess(std::map < DLMS_ACCESS, std::string > &list)
    {
        list[DLMS_ACCESS_HARDWARE_FAULT] = "hardware-fault";
        list[DLMS_ACCESS_OBJECT_ACCESS_INVALID] = "object-access-violated";
        list[DLMS_ACCESS_OBJECT_UNAVAILABLE] = "object-unavailable";
        list[DLMS_ACCESS_OTHER] = "other";
        list[DLMS_ACCESS_SCOPE_OF_ACCESS_VIOLATED] = "scope-of-access-violated";
    }

    static void GetInitiate(std::map < DLMS_INITIATE, std::string > &list)
    {
        list[DLMS_INITIATE_DLMS_VERSION_TOO_LOW] = "dlms-version-too-low";
        list[DLMS_INITIATE_INCOMPATIBLE_CONFORMANCE] = "incompatible-conformance";
        list[DLMS_INITIATE_OTHER] = "other";
        list[DLMS_INITIATE_PDU_SIZE_TOOSHORT] = "pdu-size-too-short";
        list[DLMS_INITIATE_REFUSED_BY_THE_VDE_HANDLER] = "refused-by-the-VDE-Handler";
    }

    static void GetLoadDataSet(std::map < DLMS_LOAD_DATASET, std::string > &list)
    {
        list[DLMS_LOAD_DATASET_NOT_READY] = "data-set-not-ready";
        list[DLMS_LOAD_DATASET_SIZE_TOO_LARGE] = "dataset-size-too-large";
        list[DLMS_LOAD_DATASET_INTERPRETATION_FAILURE] = "interpretation-failure";
        list[DLMS_LOAD_DATASET_NOT_AWAITED_SEGMENT] = "not-awaited-segment";
        list[DLMS_LOAD_DATASET_NOT_LOADABLE] = "not-loadable";
        list[DLMS_LOAD_DATASET_OTHER] = "other";
        list[DLMS_LOAD_DATASET_PRIMITIVE_OUT_OF_SEQUENCE] = "primitive-out-of-sequence";
        list[DLMS_LOAD_DATASET_STORAGE_FAILURE] = "storage-failure";
    }

    static void GetTask(std::map < DLMS_TASK, std::string > &list)
    {
        list[DLMS_TASK_NO_REMOTE_CONTROL] = "no-remote-control";
        list[DLMS_TASK_OTHER] = "other";
        list[DLMS_TASK_TI_RUNNIN] = "ti-running";
        list[DLMS_TASK_TI_STOPPED] = "ti-stopped";
        list[DLMS_TASK_TI_UNUSABLE] = "ti-unusable";
    }

    static std::string GetServiceErrorValue(DLMS_SERVICE_ERROR error, unsigned char value)
    {
        std::string str;
        switch (error)
        {
        case DLMS_SERVICE_ERROR_APPLICATION_REFERENCE:
        {
            std::map< DLMS_APPLICATION_REFERENCE, std::string > list;
            GetApplicationReference(list);
            str = list[(DLMS_APPLICATION_REFERENCE)value];
        }
        break;
        case DLMS_SERVICE_ERROR_HARDWARE_RESOURCE:
        {
            std::map< DLMS_HARDWARE_RESOURCE, std::string > list;
            GetHardwareResource(list);
            str = list[(DLMS_HARDWARE_RESOURCE)value];
        }
        break;
        case DLMS_SERVICE_ERROR_VDE_STATE_ERROR:
        {
            std::map< DLMS_VDE_STATE_ERROR, std::string > list;
            GetVdeStateError(list);
            str = list[(DLMS_VDE_STATE_ERROR)value];
        }
        break;
        case DLMS_SERVICE_ERROR_SERVICE:
        {
            std::map< DLMS_SERVICE, std::string > list;
            GetService(list);
            str = list[(DLMS_SERVICE)value];
        }
        break;
        case DLMS_SERVICE_ERROR_DEFINITION:
        {
            std::map< DLMS_DEFINITION, std::string > list;
            GetDefinition(list);
            str = list[(DLMS_DEFINITION)value];
        }
        break;
        case DLMS_SERVICE_ERROR_ACCESS:
        {
            std::map< DLMS_ACCESS, std::string > list;
            GetAccess(list);
            str = list[(DLMS_ACCESS)value];
        }
        break;
        case DLMS_SERVICE_ERROR_INITIATE:
        {
            std::map< DLMS_INITIATE, std::string > list;
            GetInitiate(list);
            str = list[(DLMS_INITIATE)value];
        }
        break;
        case DLMS_SERVICE_ERROR_LOAD_DATASET:
        {
            std::map< DLMS_LOAD_DATASET, std::string > list;
            GetLoadDataSet(list);
            str = list[(DLMS_LOAD_DATASET)value];
        }
        break;
        case DLMS_SERVICE_ERROR_TASK:
        {
            std::map<DLMS_TASK, std::string > list;
            GetTask(list);
            str = list[(DLMS_TASK)value];
        }
        break;
        case DLMS_SERVICE_ERROR_OTHER_ERROR:
            str = value;
            break;
        default:
            str = "";
            break;
        }
        return str;
    }

    /// <summary>
    ///
    /// </summary>
    /// <param name="error">Service error enumeration value.</param>
    /// <returns>Service error simple XML tag.</returns>
    static std::string ServiceErrorToString(DLMS_SERVICE_ERROR error)
    {
        std::map <DLMS_SERVICE_ERROR, std::string > list;
        GetServiceErrors(list);
        return list[error];
    }

    /// <summary>
    ///
    /// </summary>
    /// <param name="value">Service error simple XML tag.</param>
    /// <returns>Service error enumeration value.</returns>
    static DLMS_SERVICE_ERROR GetServiceError(std::string value)
    {
        std::map<DLMS_SERVICE_ERROR, std::string > list;
        GetServiceErrors(list);
        for (std::map<DLMS_SERVICE_ERROR, std::string >::iterator it = list.begin(); it != list.end(); ++it)
        {
            if (value.compare(it->second) == 0)
            {
                return it->first;
            }
        }
        return (DLMS_SERVICE_ERROR)-1;
    }

    static DLMS_APPLICATION_REFERENCE GetApplicationReference(std::string value)
    {
        DLMS_APPLICATION_REFERENCE ret = (DLMS_APPLICATION_REFERENCE)-1;
        std::map < DLMS_APPLICATION_REFERENCE, std::string > list;
        GetApplicationReference(list);
        for (std::map < DLMS_APPLICATION_REFERENCE, std::string >::iterator it = list.begin(); it != list.end(); ++it)
        {
            if (value.compare(it->second) == 0)
            {
                ret = it->first;
                break;
            }
        }
        return ret;
    }

    static DLMS_HARDWARE_RESOURCE GetHardwareResource(std::string value)
    {
        DLMS_HARDWARE_RESOURCE ret = (DLMS_HARDWARE_RESOURCE)-1;
        std::map < DLMS_HARDWARE_RESOURCE, std::string > list;
        GetHardwareResource(list);
        for (std::map < DLMS_HARDWARE_RESOURCE, std::string >::iterator it = list.begin(); it != list.end(); ++it)
        {
            if (value.compare(it->second) == 0)
            {
                ret = it->first;
                break;
            }
        }
        return ret;
    }

    static DLMS_VDE_STATE_ERROR GetVdeStateError(std::string value)
    {
        DLMS_VDE_STATE_ERROR ret = (DLMS_VDE_STATE_ERROR)-1;
        std::map < DLMS_VDE_STATE_ERROR, std::string > list;
        GetVdeStateError(list);
        for (std::map < DLMS_VDE_STATE_ERROR, std::string >::iterator it = list.begin(); it != list.end(); ++it)
        {
            if (value.compare(it->second) == 0)
            {
                ret = it->first;
                break;
            }
        }
        return ret;
    }

    static DLMS_SERVICE GetService(std::string value)
    {
        DLMS_SERVICE ret = (DLMS_SERVICE)-1;
        std::map < DLMS_SERVICE, std::string > list;
        GetService(list);
        for (std::map < DLMS_SERVICE, std::string >::iterator it = list.begin(); it != list.end(); ++it)
        {
            if (value.compare(it->second) == 0)
            {
                ret = it->first;
                break;
            }
        }
        return ret;
    }

    static DLMS_DEFINITION GetDefinition(std::string value)
    {
        DLMS_DEFINITION ret = (DLMS_DEFINITION)-1;
        std::map < DLMS_DEFINITION, std::string > list;
        GetDefinition(list);
        for (std::map < DLMS_DEFINITION, std::string >::iterator it = list.begin(); it != list.end(); ++it)
        {
            if (value.compare(it->second) == 0)
            {
                ret = it->first;
                break;
            }
        }
        return ret;
    }

    static DLMS_ACCESS GetAccess(std::string value)
    {
        DLMS_ACCESS ret = (DLMS_ACCESS)-1;
        std::map < DLMS_ACCESS, std::string > list;
        GetAccess(list);
        for (std::map < DLMS_ACCESS, std::string >::iterator it = list.begin(); it != list.end(); ++it)
        {
            if (value.compare(it->second) == 0)
            {
                ret = it->first;
                break;
            }
        }
        return ret;
    }

    static DLMS_INITIATE GetInitiate(std::string value)
    {
        DLMS_INITIATE ret = (DLMS_INITIATE)-1;
        std::map < DLMS_INITIATE, std::string > list;
        GetInitiate(list);
        for (std::map < DLMS_INITIATE, std::string >::iterator it = list.begin(); it != list.end(); ++it)
        {
            if (value.compare(it->second) == 0)
            {
                ret = it->first;
                break;
            }
        }
        return ret;
    }

    static DLMS_LOAD_DATASET GetLoadDataSet(std::string value)
    {
        DLMS_LOAD_DATASET ret = (DLMS_LOAD_DATASET)-1;
        std::map < DLMS_LOAD_DATASET, std::string > list;
        GetLoadDataSet(list);
        for (std::map < DLMS_LOAD_DATASET, std::string >::iterator it = list.begin(); it != list.end(); ++it)
        {
            if (value.compare(it->second) == 0)
            {
                ret = it->first;
                break;
            }
        }
        return ret;
    }

    static DLMS_TASK GetTask(std::string value)
    {
        DLMS_TASK ret = (DLMS_TASK)-1;
        std::map < DLMS_TASK, std::string > list;
        GetTask(list);
        for (std::map < DLMS_TASK, std::string >::iterator it = list.begin(); it != list.end(); ++it)
        {
            if (value.compare(it->second) == 0)
            {
                ret = it->first;
                break;
            }
        }
        return ret;
    }

    static unsigned char GetError(DLMS_SERVICE_ERROR serviceError, std::string value)
    {
        int ret = 0;
        switch (serviceError)
        {
        case DLMS_SERVICE_ERROR_APPLICATION_REFERENCE:
            ret = GetApplicationReference(value);
            break;
        case DLMS_SERVICE_ERROR_HARDWARE_RESOURCE:
            ret = GetHardwareResource(value);
            break;
        case DLMS_SERVICE_ERROR_VDE_STATE_ERROR:
            ret = GetVdeStateError(value);
            break;
        case DLMS_SERVICE_ERROR_SERVICE:
            ret = GetService(value);
            break;
        case DLMS_SERVICE_ERROR_DEFINITION:
            ret = GetDefinition(value);
            break;
        case DLMS_SERVICE_ERROR_ACCESS:
            ret = GetAccess(value);
            break;
        case DLMS_SERVICE_ERROR_INITIATE:
            ret = GetInitiate(value);
            break;
        case DLMS_SERVICE_ERROR_LOAD_DATASET:
            ret = GetLoadDataSet(value);
            break;
        case DLMS_SERVICE_ERROR_TASK:
            ret = GetTask(value);
            break;
        case DLMS_SERVICE_ERROR_OTHER_ERROR:
            ret = atol(value.c_str());
            break;
        default:
            break;
        }
        return ret;
    }

    static DLMS_CONFORMANCE ValueOfConformance(std::string value)
    {
        DLMS_CONFORMANCE ret;
        if (value.compare("access") == 0)
        {
            ret = DLMS_CONFORMANCE_ACCESS;
        }
        else if (value.compare("action") == 0)
        {
            ret = DLMS_CONFORMANCE_ACTION;
        }
        else if (value.compare("attribute0-supported-with-get") == 0)
        {
            ret = DLMS_CONFORMANCE_ATTRIBUTE_0_SUPPORTED_WITH_GET;
        }
        else if (value.compare("attribute0-supported-with-set") == 0)
        {
            ret = DLMS_CONFORMANCE_ATTRIBUTE_0_SUPPORTED_WITH_SET;
        }
        else if (value.compare("block-transfer-with-action") == 0)
        {
            ret = DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_ACTION;
        }
        else if (value.compare("block-transfer-with-get-or-read") == 0)
        {
            ret = DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_GET_OR_READ;
        }
        else if (value.compare("block-transfer-with-set-or-write") == 0)
        {
            ret = DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_SET_OR_WRITE;
        }
        else if (value.compare("data-notification") == 0)
        {
            ret = DLMS_CONFORMANCE_DATA_NOTIFICATION;
        }
        else if (value.compare("event-notification") == 0)
        {
            ret = DLMS_CONFORMANCE_EVENT_NOTIFICATION;
        }
        else if (value.compare("general-block-transfer") == 0)
        {
            ret = DLMS_CONFORMANCE_GENERAL_BLOCK_TRANSFER;
        }
        else if (value.compare("general-protection") == 0)
        {
            ret = DLMS_CONFORMANCE_GENERAL_PROTECTION;
        }
        else if (value.compare("get") == 0)
        {
            ret = DLMS_CONFORMANCE_GET;
        }
        else if (value.compare("information-report") == 0)
        {
            ret = DLMS_CONFORMANCE_INFORMATION_REPORT;
        }
        else if (value.compare("multiple-references") == 0)
        {
            ret = DLMS_CONFORMANCE_MULTIPLE_REFERENCES;
        }
        else if (value.compare("parameterized-access") == 0)
        {
            ret = DLMS_CONFORMANCE_PARAMETERIZED_ACCESS;
        }
        else if (value.compare("priority-mgmt-supported") == 0)
        {
            ret = DLMS_CONFORMANCE_PRIORITY_MGMT_SUPPORTED;
        }
        else if (value.compare("read") == 0)
        {
            ret = DLMS_CONFORMANCE_READ;
        }
        else if (value.compare("reserved-seven") == 0)
        {
            ret = DLMS_CONFORMANCE_RESERVED_SEVEN;
        }
        else if (value.compare("reserved-six") == 0)
        {
            ret = DLMS_CONFORMANCE_RESERVED_SIX;
        }
        else if (value.compare("reserved-zero") == 0)
        {
            ret = DLMS_CONFORMANCE_RESERVED_ZERO;
        }
        else if (value.compare("selective-access") == 0)
        {
            ret = DLMS_CONFORMANCE_SELECTIVE_ACCESS;
        }
        else if (value.compare("set") == 0)
        {
            ret = DLMS_CONFORMANCE_SET;
        }
        else if (value.compare("unconfirmed-write") == 0)
        {
            ret = DLMS_CONFORMANCE_UN_CONFIRMED_WRITE;
        }
        else if (value.compare("write") == 0)
        {
            ret = DLMS_CONFORMANCE_WRITE;
        }
        else
        {
            ret = (DLMS_CONFORMANCE)-1;
        }
        return ret;
    }

    static int ConformanceToString(DLMS_CONFORMANCE value, std::string& str)
    {
        switch (value)
        {
        case DLMS_CONFORMANCE_ACCESS:
            str = "access";
            break;
        case DLMS_CONFORMANCE_ACTION:
            str = "action";
            break;
        case DLMS_CONFORMANCE_ATTRIBUTE_0_SUPPORTED_WITH_GET:
            str = "attribute0-supported-with-get";
            break;
        case DLMS_CONFORMANCE_ATTRIBUTE_0_SUPPORTED_WITH_SET:
            str = "attribute0-supported-with-set";
            break;
        case DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_ACTION:
            str = "block-transfer-with-action";
            break;
        case DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_GET_OR_READ:
            str = "block-transfer-with-get-or-read";
            break;
        case DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_SET_OR_WRITE:
            str = "block-transfer-with-set-or-write";
            break;
        case DLMS_CONFORMANCE_DATA_NOTIFICATION:
            str = "data-notification";
            break;
        case DLMS_CONFORMANCE_EVENT_NOTIFICATION:
            str = "event-notification";
            break;
        case DLMS_CONFORMANCE_GENERAL_BLOCK_TRANSFER:
            str = "general-block-transfer";
            break;
        case DLMS_CONFORMANCE_GENERAL_PROTECTION:
            str = "general-protection";
            break;
        case DLMS_CONFORMANCE_GET:
            str = "get";
            break;
        case DLMS_CONFORMANCE_INFORMATION_REPORT:
            str = "information-report";
            break;
        case DLMS_CONFORMANCE_MULTIPLE_REFERENCES:
            str = "multiple-references";
            break;
        case DLMS_CONFORMANCE_PARAMETERIZED_ACCESS:
            str = "parameterized-access";
            break;
        case DLMS_CONFORMANCE_PRIORITY_MGMT_SUPPORTED:
            str = "priority-mgmt-supported";
            break;
        case DLMS_CONFORMANCE_READ:
            str = "read";
            break;
        case DLMS_CONFORMANCE_RESERVED_SEVEN:
            str = "reserved-seven";
            break;
        case DLMS_CONFORMANCE_RESERVED_SIX:
            str = "reserved-six";
            break;
        case DLMS_CONFORMANCE_RESERVED_ZERO:
            str = "reserved-zero";
            break;
        case DLMS_CONFORMANCE_SELECTIVE_ACCESS:
            str = "selective-access";
            break;
        case DLMS_CONFORMANCE_SET:
            str = "set";
            break;
        case DLMS_CONFORMANCE_UN_CONFIRMED_WRITE:
            str = "unconfirmed-write";
            break;
        case DLMS_CONFORMANCE_WRITE:
            str = "write";
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        return 0;
    }


    static std::string ReleaseResponseReasonToString(DLMS_RELEASE_RESPONSE_REASON value)
    {
        std::string str;
        switch (value)
        {
        case DLMS_RELEASE_RESPONSE_REASON_NORMAL:
            str = "normal";
            break;
        case DLMS_RELEASE_RESPONSE_REASON_NOTFINISHED:
            str = "not-finished";
            break;
        case DLMS_RELEASE_RESPONSE_REASON_USERDEFINED:
            str = "user-defined";
            break;
        default:
            str = "Invalid value.";
        }
        return str;
    }

    static DLMS_RELEASE_RESPONSE_REASON ValueOfReleaseResponseReason(std::string value)
    {
        DLMS_RELEASE_RESPONSE_REASON ret;
        if (value.compare("normal") == 0)
        {
            ret = DLMS_RELEASE_RESPONSE_REASON_NORMAL;
        }
        else if (value.compare("not-finished") == 0)
        {
            ret = DLMS_RELEASE_RESPONSE_REASON_NOTFINISHED;
        }
        else if (value.compare("user-defined") == 0)
        {
            ret = DLMS_RELEASE_RESPONSE_REASON_USERDEFINED;
        }
        else
        {
            ret = (DLMS_RELEASE_RESPONSE_REASON)-1;
        }
        return ret;
    }

    static std::string ReleaseRequestReasonToString(DLMS_RELEASE_REQUEST_REASON value)
    {
        std::string str;
        switch (value)
        {
        case DLMS_RELEASE_REQUEST_REASON_NORMAL:
            str = "normal";
            break;
        case DLMS_RELEASE_REQUEST_REASON_URGENT:
            str = "urgent";
            break;
        case DLMS_RELEASE_REQUEST_REASON_USER_DEFINED:
            str = "user-defined";
            break;
        default:
            str = "Invalid value.";
        }
        return str;
    }

    static DLMS_RELEASE_REQUEST_REASON ValueOfReleaseRequestReason(std::string value)
    {
        DLMS_RELEASE_REQUEST_REASON ret;
        if (value.compare("normal") == 0)
        {
            ret = DLMS_RELEASE_REQUEST_REASON_NORMAL;
        }
        else if (value.compare("urgent") == 0)
        {
            ret = DLMS_RELEASE_REQUEST_REASON_URGENT;
        }
        else if (value.compare("user-defined") == 0)
        {
            ret = DLMS_RELEASE_REQUEST_REASON_USER_DEFINED;
        }
        else
        {
            ret = (DLMS_RELEASE_REQUEST_REASON)-1;
        }
        return ret;
    }
};
#endif //TRANSLATOR_STANDARD_TAGS_H
