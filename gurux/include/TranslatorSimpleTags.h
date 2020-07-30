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

#ifndef TRANSLATOR_SIMPLE_TAGS_H
#define TRANSLATOR_SIMPLE_TAGS_H

#include "enums.h"
#include "errorcodes.h"
#include "TranslatorTags.h"
#include "TranslatorGeneralTags.h"

class CTranslatorSimpleTags
{
public:
    // Get general tags.
    static void GetGeneralTags(DLMS_TRANSLATOR_OUTPUT_TYPE, std::map<unsigned long, std::string> &list)
    {
        list[DLMS_COMMAND_SNRM] = "Snrm";
        list[DLMS_COMMAND_UNACCEPTABLE_FRAME] = "UnacceptableFrame";
        list[DLMS_COMMAND_DISCONNECT_MODE] = "DisconnectMode";
        list[DLMS_COMMAND_UA] = "Ua";
        list[DLMS_COMMAND_AARQ] = "AssociationRequest";
        list[DLMS_COMMAND_AARE] = "AssociationResponse";
        list[TRANSLATOR_GENERAL_TAGS_APPLICATION_CONTEXT_NAME] = "ApplicationContextName";
        list[DLMS_COMMAND_INITIATE_RESPONSE] = "InitiateResponse";
        list[DLMS_COMMAND_INITIATE_REQUEST] = "InitiateRequest";
        list[TRANSLATOR_GENERAL_TAGS_NEGOTIATED_QUALITY_OF_SERVICE] = "NegotiatedQualityOfService";
        list[TRANSLATOR_GENERAL_TAGS_PROPOSED_QUALITY_OF_SERVICE] = "ProposedQualityOfService";
        list[TRANSLATOR_GENERAL_TAGS_PROPOSED_DLMS_VERSION_NUMBER] = "ProposedDlmsVersionNumber";
        list[TRANSLATOR_GENERAL_TAGS_PROPOSED_MAX_PDU_SIZE] = "ProposedMaxPduSize";
        list[TRANSLATOR_GENERAL_TAGS_PROPOSED_CONFORMANCE] = "ProposedConformance";
        list[TRANSLATOR_GENERAL_TAGS_VAA_NAME] = "VaaName";
        list[TRANSLATOR_GENERAL_TAGS_NEGOTIATED_CONFORMANCE] = "NegotiatedConformance";
        list[TRANSLATOR_GENERAL_TAGS_NEGOTIATED_DLMS_VERSION_NUMBER] = "NegotiatedDlmsVersionNumber";
        list[TRANSLATOR_GENERAL_TAGS_NEGOTIATED_MAX_PDU_SIZE] = "NegotiatedMaxPduSize";
        list[TRANSLATOR_GENERAL_TAGS_CONFORMANCE_BIT] = "ConformanceBit";
        list[TRANSLATOR_GENERAL_TAGS_SENDER_ACSE_REQUIREMENTS] = "SenderACSERequirements";
        list[TRANSLATOR_GENERAL_TAGS_RESPONDER_ACSE_REQUIREMENT] = "ResponderACSERequirement";
        list[TRANSLATOR_GENERAL_TAGS_RESPONDING_MECHANISM_NAME] = "MechanismName";
        list[TRANSLATOR_GENERAL_TAGS_CALLING_MECHANISM_NAME] = "MechanismName";
        list[TRANSLATOR_GENERAL_TAGS_CALLING_AUTHENTICATION] = "CallingAuthentication";
        list[TRANSLATOR_GENERAL_TAGS_RESPONDING_AUTHENTICATION] = "RespondingAuthentication";
        list[DLMS_COMMAND_RELEASE_REQUEST] = "ReleaseRequest";
        list[DLMS_COMMAND_RELEASE_RESPONSE] = "ReleaseResponse";
        list[DLMS_COMMAND_DISCONNECT_REQUEST] = "DisconnectRequest";
        list[TRANSLATOR_GENERAL_TAGS_ASSOCIATION_RESULT] = "AssociationResult";
        list[TRANSLATOR_GENERAL_TAGS_RESULT_SOURCE_DIAGNOSTIC] = "ResultSourceDiagnostic";
        list[TRANSLATOR_GENERAL_TAGS_ACSE_SERVICE_USER] = "ACSEServiceUser";
        list[TRANSLATOR_GENERAL_TAGS_CALLING_AP_TITLE] = "CallingAPTitle";
        list[TRANSLATOR_GENERAL_TAGS_RESPONDING_AP_TITLE] = "RespondingAPTitle";
        list[TRANSLATOR_GENERAL_TAGS_DEDICATED_KEY] = "DedicatedKey";
        list[DLMS_COMMAND_CONFIRMED_SERVICE_ERROR] = "ConfirmedServiceError";
        list[DLMS_COMMAND_INFORMATION_REPORT] = "InformationReportRequest";
        list[DLMS_COMMAND_EVENT_NOTIFICATION] = "EventNotificationRequest";
        list[DLMS_COMMAND_GENERAL_BLOCK_TRANSFER] = "GeneralBlockTransfer";
        list[TRANSLATOR_GENERAL_TAGS_CALLING_AE_INVOCATION_ID] = "CallingAEInvocationId";
        list[TRANSLATOR_GENERAL_TAGS_CALLED_AE_INVOCATION_ID] = "CalledAEInvocationId";
        list[TRANSLATOR_GENERAL_TAGS_RESPONDING_AE_INVOCATION_ID] = "RespondingAEInvocationId";
    }

    // Get SN tags.
    static void GetSnTags(DLMS_TRANSLATOR_OUTPUT_TYPE, std::map<unsigned long, std::string> &list)
    {
        list[DLMS_COMMAND_READ_REQUEST] = "ReadRequest";
        list[DLMS_COMMAND_WRITE_REQUEST] = "WriteRequest";
        list[DLMS_COMMAND_WRITE_RESPONSE] = "WriteResponse";
        list[DLMS_COMMAND_READ_REQUEST << 8 | DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME] = "VariableName";
        list[DLMS_COMMAND_READ_REQUEST << 8 | DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS] = "ParameterisedAccess";
        list[DLMS_COMMAND_READ_REQUEST << 8 | DLMS_VARIABLE_ACCESS_SPECIFICATION_BLOCK_NUMBER_ACCESS] = "BlockNumberAccess";
        list[DLMS_COMMAND_WRITE_REQUEST << 8 | DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME] = "VariableName";
        list[DLMS_COMMAND_READ_RESPONSE] = "ReadResponse";
        list[DLMS_COMMAND_READ_RESPONSE << 8 | DLMS_SINGLE_READ_RESPONSE_DATA_BLOCK_RESULT] = "DataBlockResult";
        list[DLMS_COMMAND_READ_RESPONSE << 8 | DLMS_SINGLE_READ_RESPONSE_DATA] = "Data";
        list[DLMS_COMMAND_READ_RESPONSE << 8 | DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR] = "DataAccessError";
    }

    // Get LN tags.
    static void GetLnTags(DLMS_TRANSLATOR_OUTPUT_TYPE, std::map<unsigned long, std::string> &list)
    {
        list[DLMS_COMMAND_GET_REQUEST] = "GetRequest";
        list[(DLMS_COMMAND_GET_REQUEST) << 8 | DLMS_GET_COMMAND_TYPE_NORMAL] = "GetRequestNormal";
        list[(DLMS_COMMAND_GET_REQUEST) << 8 | DLMS_GET_COMMAND_TYPE_NEXT_DATA_BLOCK] = "GetRequestForNextDataBlock";
        list[(DLMS_COMMAND_GET_REQUEST) << 8 | DLMS_GET_COMMAND_TYPE_WITH_LIST] = "GetRequestWithList";
        list[DLMS_COMMAND_SET_REQUEST] = "SetRequest";
        list[(DLMS_COMMAND_SET_REQUEST << 8) | DLMS_SET_REQUEST_TYPE_NORMAL] = "SetRequestNormal";
        list[(DLMS_COMMAND_SET_REQUEST << 8) | DLMS_SET_REQUEST_TYPE_FIRST_DATA_BLOCK] = "SetRequestFirstDataBlock";
        list[(DLMS_COMMAND_SET_REQUEST << 8) | DLMS_SET_REQUEST_TYPE_WITH_DATA_BLOCK] = "SetRequestWithDataBlock";
        list[(DLMS_COMMAND_SET_REQUEST << 8) | DLMS_SET_REQUEST_TYPE_WITH_LIST] = "SetRequestWithList";
        list[DLMS_COMMAND_METHOD_REQUEST] = "ActionRequest";
        list[(DLMS_COMMAND_METHOD_REQUEST << 8) | DLMS_ACTION_REQUEST_TYPE_NORMAL] = "ActionRequestNormal";
        list[(DLMS_COMMAND_METHOD_REQUEST << 8) | DLMS_ACTION_REQUEST_TYPE_NEXT_BLOCK] = "ActionRequestForNextDataBlock";
        list[(DLMS_COMMAND_METHOD_REQUEST << 8) | DLMS_ACTION_REQUEST_TYPE_WITH_LIST] = "ActionRequestWithList";
        list[DLMS_COMMAND_METHOD_RESPONSE] = "ActionResponse";
        list[(DLMS_COMMAND_METHOD_RESPONSE << 8) | DLMS_ACTION_REQUEST_TYPE_NORMAL] = "ActionResponseNormal";
        list[(DLMS_COMMAND_METHOD_RESPONSE << 8) | DLMS_ACTION_REQUEST_TYPE_NEXT_BLOCK] = "ActionResponseWithFirstBlock";
        list[(DLMS_COMMAND_METHOD_RESPONSE << 8) | DLMS_ACTION_REQUEST_TYPE_WITH_LIST] = "ActionResponseWithList";
        list[DLMS_COMMAND_DATA_NOTIFICATION] = "DataNotification";
        list[DLMS_COMMAND_GET_RESPONSE] = "GetResponse";
        list[DLMS_COMMAND_GET_RESPONSE << 8 | DLMS_GET_COMMAND_TYPE_NORMAL] = "GetResponseNormal";
        list[DLMS_COMMAND_GET_RESPONSE << 8 | DLMS_GET_COMMAND_TYPE_NEXT_DATA_BLOCK] = "GetResponsewithDataBlock";
        list[DLMS_COMMAND_GET_RESPONSE << 8 | DLMS_GET_COMMAND_TYPE_WITH_LIST] = "GetResponseWithList";
        list[DLMS_COMMAND_SET_RESPONSE] = "SetResponse";
        list[DLMS_COMMAND_SET_RESPONSE << 8 | DLMS_SET_RESPONSE_TYPE_NORMAL] = "SetResponseNormal";
        list[DLMS_COMMAND_SET_RESPONSE << 8 | DLMS_SET_RESPONSE_TYPE_DATA_BLOCK] = "SetResponseDataBlock";
        list[DLMS_COMMAND_SET_RESPONSE << 8 | DLMS_SET_RESPONSE_TYPE_LAST_DATA_BLOCK] = "SetResponseWithLastDataBlock";
        list[DLMS_COMMAND_SET_RESPONSE << 8 | DLMS_SET_RESPONSE_TYPE_WITH_LIST] = "SetResponseWithList";
        list[DLMS_COMMAND_ACCESS_REQUEST] = "AccessRequest";
        list[(DLMS_COMMAND_ACCESS_REQUEST << 8) | DLMS_ACCESS_SERVICE_COMMAND_TYPE_GET] = "AccessRequestGet";
        list[(DLMS_COMMAND_ACCESS_REQUEST << 8) | DLMS_ACCESS_SERVICE_COMMAND_TYPE_SET] = "AccessRequestSet";
        list[(DLMS_COMMAND_ACCESS_REQUEST << 8) | DLMS_ACCESS_SERVICE_COMMAND_TYPE_ACTION] = "AccessRequestAction";
        list[DLMS_COMMAND_ACCESS_RESPONSE] = "AccessResponse";
        list[(DLMS_COMMAND_ACCESS_RESPONSE << 8) | DLMS_ACCESS_SERVICE_COMMAND_TYPE_GET] = "AccessResponseGet";
        list[(DLMS_COMMAND_ACCESS_RESPONSE << 8) | DLMS_ACCESS_SERVICE_COMMAND_TYPE_SET] = "AccessResponseSet";
        list[(DLMS_COMMAND_ACCESS_RESPONSE << 8) | DLMS_ACCESS_SERVICE_COMMAND_TYPE_ACTION] = "AccessResponseAction";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_BODY] = "AccessRequestBody";
        list[DLMS_TRANSLATOR_TAGS_LIST_OF_ACCESS_REQUEST_SPECIFICATION] = "AccessRequestSpecification";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_SPECIFICATION] = "_AccessRequestSpecification";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_LIST_OF_DATA] = "AccessRequestListOfData";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_RESPONSE_BODY] = "AccessResponseBody";
        list[DLMS_TRANSLATOR_TAGS_LIST_OF_ACCESS_RESPONSE_SPECIFICATION] = "AccessResponseSpecification";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_RESPONSE_SPECIFICATION] = "_AccessResponseSpecification";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_RESPONSE_LIST_OF_DATA] = "AccessResponseListOfData";
        list[DLMS_TRANSLATOR_TAGS_SERVICE] = "Service";
        list[DLMS_TRANSLATOR_TAGS_SERVICE_ERROR] = "ServiceError";
        list[DLMS_COMMAND_GATEWAY_REQUEST] = "GatewayRequest";
        list[DLMS_COMMAND_GATEWAY_RESPONSE] = "GatewayResponse";
    }

    // Get glo tags.
    static void GetGloTags(DLMS_TRANSLATOR_OUTPUT_TYPE, std::map<unsigned long, std::string> &list)
    {
        list[DLMS_COMMAND_GLO_INITIATE_REQUEST] = "glo_InitiateRequest";
        list[DLMS_COMMAND_GLO_INITIATE_RESPONSE] = "glo_InitiateResponse";
        list[DLMS_COMMAND_GLO_GET_REQUEST] = "glo_GetRequest";
        list[DLMS_COMMAND_GLO_GET_RESPONSE] = "glo_GetResponse";
        list[DLMS_COMMAND_GLO_SET_REQUEST] = "glo_SetRequest";
        list[DLMS_COMMAND_GLO_SET_RESPONSE] = "glo_SetResponse";
        list[DLMS_COMMAND_GLO_METHOD_REQUEST] = "glo_ActionRequest";
        list[DLMS_COMMAND_GLO_METHOD_RESPONSE] = "glo_ActionResponse";
        list[DLMS_COMMAND_GLO_READ_REQUEST] = "glo_ReadRequest";
        list[DLMS_COMMAND_GLO_READ_RESPONSE] = "glo_ReadResponse";
        list[DLMS_COMMAND_GLO_WRITE_REQUEST] = "glo_WriteRequest";
        list[DLMS_COMMAND_GLO_WRITE_RESPONSE] = "glo_WriteResponse";
        list[DLMS_COMMAND_GENERAL_GLO_CIPHERING] = "GeneralGloCiphering";
    }

    /// <summary>
    /// Get ded tags.
    /// </summary>
    /// <param name="type"></param>
    /// <param name="list"></param>
    static void GetDedTags(DLMS_TRANSLATOR_OUTPUT_TYPE, std::map<unsigned long, std::string> & list)
    {
        list[DLMS_COMMAND_DED_INITIATE_REQUEST] = "ded_InitiateRequest";
        list[DLMS_COMMAND_DED_INITIATE_RESPONSE] = "ded_InitiateResponse";
        list[DLMS_COMMAND_DED_GET_REQUEST] = "ded_GetRequest";
        list[DLMS_COMMAND_DED_GET_RESPONSE] = "ded_GetResponse";
        list[DLMS_COMMAND_DED_SET_REQUEST] = "ded_SetRequest";
        list[DLMS_COMMAND_DED_SET_RESPONSE] = "ded_SetResponse";
        list[DLMS_COMMAND_DED_METHOD_REQUEST] = "ded_ActionRequest";
        list[DLMS_COMMAND_DED_METHOD_RESPONSE] = "ded_ActionResponse";
        list[DLMS_COMMAND_GENERAL_DED_CIPHERING] = "generalDedCiphering";
    }

    /// <summary>
    /// Get translator tags.
    /// </summary>
    /// <param name="type"></param>
    /// <param name="list"></param>
    static void GetTranslatorTags(DLMS_TRANSLATOR_OUTPUT_TYPE, std::map<unsigned long, std::string> & list)
    {
        list[DLMS_TRANSLATOR_TAGS_WRAPPER] = "Wrapper";
        list[DLMS_TRANSLATOR_TAGS_HDLC] = "Hdlc";
        list[DLMS_TRANSLATOR_TAGS_PDU_DLMS] = "Pdu";
        list[DLMS_TRANSLATOR_TAGS_TARGET_ADDRESS] = "TargetAddress";
        list[DLMS_TRANSLATOR_TAGS_SOURCE_ADDRESS] = "SourceAddress";
        list[DLMS_TRANSLATOR_TAGS_LIST_OF_VARIABLE_ACCESS_SPECIFICATION] = "ListOfVariableAccessSpecification";
        list[DLMS_TRANSLATOR_TAGS_LIST_OF_DATA] = "ListOfData";
        list[DLMS_TRANSLATOR_TAGS_SUCCESS] = "Ok";
        list[DLMS_TRANSLATOR_TAGS_DATA_ACCESS_ERROR] = "DataAccessError";
        list[DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR] = "AttributeDescriptor";
        list[DLMS_TRANSLATOR_TAGS_CLASS_ID] = "ClassId";
        list[DLMS_TRANSLATOR_TAGS_INSTANCE_ID] = "InstanceId";
        list[DLMS_TRANSLATOR_TAGS_ATTRIBUTE_ID] = "AttributeId";
        list[DLMS_TRANSLATOR_TAGS_METHOD_INVOCATION_PARAMETERS] = "MethodInvocationParameters";
        list[DLMS_TRANSLATOR_TAGS_SELECTOR] = "Selector";
        list[DLMS_TRANSLATOR_TAGS_PARAMETER] = "Parameter";
        list[DLMS_TRANSLATOR_TAGS_LAST_BLOCK] = "LastBlock";
        list[DLMS_TRANSLATOR_TAGS_BLOCK_NUMBER] = "BlockNumber";
        list[DLMS_TRANSLATOR_TAGS_RAW_DATA] = "RawData";
        list[DLMS_TRANSLATOR_TAGS_METHOD_DESCRIPTOR] = "MethodDescriptor";
        list[DLMS_TRANSLATOR_TAGS_METHOD_ID] = "MethodId";
        list[DLMS_TRANSLATOR_TAGS_RESULT] = "Result";
        list[DLMS_TRANSLATOR_TAGS_RETURN_PARAMETERS] = "ReturnParameters";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_SELECTION] = "AccessSelection";
        list[DLMS_TRANSLATOR_TAGS_VALUE] = "Value";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_SELECTOR] = "AccessSelector";
        list[DLMS_TRANSLATOR_TAGS_ACCESS_PARAMETERS] = "AccessParameters";
        list[DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_LIST] = "AttributeDescriptorList";
        list[DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_WITH_SELECTION] = "AttributeDescriptorWithSelection";
        list[DLMS_TRANSLATOR_TAGS_READ_DATA_BLOCK_ACCESS] = "ReadDataBlockAccess";
        list[DLMS_TRANSLATOR_TAGS_WRITE_DATA_BLOCK_ACCESS] = "WriteDataBlockAccess";
        list[DLMS_TRANSLATOR_TAGS_DATA] = "Data";
        list[DLMS_TRANSLATOR_TAGS_INVOKE_ID] = "InvokeIdAndPriority";
        list[DLMS_TRANSLATOR_TAGS_LONG_INVOKE_ID] = "LongInvokeIdAndPriority";
        list[DLMS_TRANSLATOR_TAGS_DATE_TIME] = "DateTime";
        list[DLMS_TRANSLATOR_TAGS_CURRENT_TIME] = "CurrentTime";
        list[DLMS_TRANSLATOR_TAGS_TIME] = "DateTime";
        list[DLMS_TRANSLATOR_TAGS_REASON] = "Reason";
        list[DLMS_TRANSLATOR_TAGS_NOTIFICATION_BODY] = "NotificationBody";
        list[DLMS_TRANSLATOR_TAGS_DATA_VALUE] = "DataValue";
        list[DLMS_TRANSLATOR_TAGS_CIPHERED_SERVICE] = "CipheredService";
        list[DLMS_TRANSLATOR_TAGS_SYSTEM_TITLE] = "SystemTitle";
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
        list[DLMS_TRANSLATOR_TAGS_ATTRIBUTE_VALUE] = "AttributeValue";
        list[DLMS_TRANSLATOR_TAGS_MAX_INFO_RX] = "MaxInfoRX";
        list[DLMS_TRANSLATOR_TAGS_MAX_INFO_TX] = "MaxInfoTX";
        list[DLMS_TRANSLATOR_TAGS_WINDOW_SIZE_RX] = "WindowSizeRX";
        list[DLMS_TRANSLATOR_TAGS_WINDOW_SIZE_TX] = "WindowSizeTX";
        list[DLMS_TRANSLATOR_TAGS_VALUE_LIST] = "ValueList";
        list[DLMS_TRANSLATOR_TAGS_DATA_ACCESS_RESULT] = "DataAccessResult";
        list[DLMS_TRANSLATOR_TAGS_BLOCK_CONTROL] = "BlockControl";
        list[DLMS_TRANSLATOR_TAGS_BLOCK_NUMBER_ACK] = "BlockNumberAck";
        list[DLMS_TRANSLATOR_TAGS_BLOCK_DATA] = "BlockData";
        list[DLMS_TRANSLATOR_TAGS_CONTENTS_DESCRIPTION] = "ContentsDescription";
        list[DLMS_TRANSLATOR_TAGS_ARRAY_CONTENTS] = "ArrayContents";
        list[DLMS_TRANSLATOR_TAGS_NETWORK_ID] = "NetworkId";
        list[DLMS_TRANSLATOR_TAGS_PHYSICAL_DEVICE_ADDRESS] = "PhysicalDeviceAddress";
        list[DLMS_TRANSLATOR_TAGS_PROTOCOL_VERSION] = "ProtocolVersion";
        list[DLMS_TRANSLATOR_TAGS_CALLED_AP_TITLE] = "CalledAPTitle";
        list[DLMS_TRANSLATOR_TAGS_CALLED_AP_INVOCATION_ID] = "CalledAPInvocationId";
        list[DLMS_TRANSLATOR_TAGS_CALLED_AE_INVOCATION_ID] = "CalledAEInvocationId";
        list[DLMS_TRANSLATOR_TAGS_CALLING_AP_INVOCATION_ID] = "CallingApInvocationId";
        list[DLMS_TRANSLATOR_TAGS_CALLED_AE_QUALIFIER] = "CalledAEQualifier";
    }

    static void GetDataTypeTags(std::map<unsigned long, std::string> & list)
    {
        list[(DATA_TYPE_OFFSET + DLMS_DATA_TYPE_NONE)] = "None";
        list[(DATA_TYPE_OFFSET + DLMS_DATA_TYPE_ARRAY)] = "Array";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_BINARY_CODED_DESIMAL] = "Bcd";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_BIT_STRING] = "BitString";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_BOOLEAN] = "Boolean";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_COMPACT_ARRAY] = "CompactArray";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_DATE] = "Date";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_DATETIME] = "DateTime";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_ENUM] = "Enum";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_FLOAT32] = "Float32";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_FLOAT64] = "Float64";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_INT16] = "Int16";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_INT32] = "Int32";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_INT64] = "Int64";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_INT8] = "Int8";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_OCTET_STRING] = "OctetString";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_STRING] = "String";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_STRING_UTF8] = "StringUTF8";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_STRUCTURE] = "Structure";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_TIME] = "Time";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_UINT16] = "UInt16";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_UINT32] = "UInt32";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_UINT64] = "UInt64";
        list[DATA_TYPE_OFFSET + DLMS_DATA_TYPE_UINT8] = "UInt8";
    }

    static int ErrorCodeToString(DLMS_ERROR_CODE value, std::string& str)
    {
        switch (value)
        {
        case DLMS_ERROR_CODE_ACCESS_VIOLATED:
            str = "AccessViolated";
            break;
        case DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID:
            str = "DataBlockNumberInvalid";
            break;
        case DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE:
            str = "DataBlockUnavailable";
            break;
        case DLMS_ERROR_CODE_HARDWARE_FAULT:
            str = "HardwareFault";
            break;
        case DLMS_ERROR_CODE_INCONSISTENT_CLASS:
            str = "InconsistentClass";
            break;
        case DLMS_ERROR_CODE_LONG_GET_OR_READ_ABORTED:
            str = "LongGetOrReadAborted";
            break;
        case DLMS_ERROR_CODE_LONG_SET_OR_WRITE_ABORTED:
            str = "LongSetOrWriteAborted";
            break;
        case DLMS_ERROR_CODE_NO_LONG_GET_OR_READ_IN_PROGRESS:
            str = "NoLongGetOrReadInProgress";
            break;
        case DLMS_ERROR_CODE_NO_LONG_SET_OR_WRITE_IN_PROGRESS:
            str = "NoLongSetOrWriteInProgress";
            break;
        case DLMS_ERROR_CODE_OK:
            str = "Success";
            break;
        case DLMS_ERROR_CODE_OTHER_REASON:
            str = "OtherReason";
            break;
        case DLMS_ERROR_CODE_READ_WRITE_DENIED:
            str = "ReadWriteDenied";
            break;
        case DLMS_ERROR_CODE_TEMPORARY_FAILURE:
            str = "TemporaryFailure";
            break;
        case DLMS_ERROR_CODE_UNAVAILABLE_OBJECT:
            str = "UnavailableObject";
            break;
        case DLMS_ERROR_CODE_UNDEFINED_OBJECT:
            str = "UndefinedObject";
            break;
        case DLMS_ERROR_CODE_UNMATCH_TYPE:
            str = "UnmatchedType";
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        return 0;
    }

    static DLMS_ERROR_CODE ValueOfErrorCode(std::string value)
    {
        DLMS_ERROR_CODE v;
        if (value.compare("AccessViolated") == 0)
        {
            v = DLMS_ERROR_CODE_ACCESS_VIOLATED;
        }
        else if (value.compare("DataBlockNumberInvalid") == 0)
        {
            v = DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID;
        }
        else if (value.compare("DataBlockUnavailable") == 0)
        {
            v = DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE;
        }
        else if (value.compare("HardwareFault") == 0)
        {
            v = DLMS_ERROR_CODE_HARDWARE_FAULT;
        }
        else if (value.compare("InconsistentClass") == 0)
        {
            v = DLMS_ERROR_CODE_INCONSISTENT_CLASS;
        }
        else if (value.compare("LongGetOrReadAborted") == 0)
        {
            v = DLMS_ERROR_CODE_LONG_GET_OR_READ_ABORTED;
        }
        else if (value.compare("LongSetOrWriteAborted") == 0)
        {
            v = DLMS_ERROR_CODE_LONG_SET_OR_WRITE_ABORTED;
        }
        else if (value.compare("NoLongGetOrReadInProgress") == 0)
        {
            v = DLMS_ERROR_CODE_NO_LONG_GET_OR_READ_IN_PROGRESS;
        }
        else if (value.compare("NoLongSetOrWriteInProgress") == 0)
        {
            v = DLMS_ERROR_CODE_NO_LONG_SET_OR_WRITE_IN_PROGRESS;
        }
        else if (value.compare("Ok") == 0 ||
            value.compare("Success") == 0)
        {
            v = DLMS_ERROR_CODE_OK;
        }
        else if (value.compare("OtherReason") == 0)
        {
            v = DLMS_ERROR_CODE_OTHER_REASON;
        }
        else if (value.compare("ReadWriteDenied") == 0)
        {
            v = DLMS_ERROR_CODE_READ_WRITE_DENIED;
        }
        else if (value.compare("TemporaryFailure") == 0)
        {
            v = DLMS_ERROR_CODE_TEMPORARY_FAILURE;
        }
        else if (value.compare("UnavailableObject") == 0)
        {
            v = DLMS_ERROR_CODE_UNAVAILABLE_OBJECT;
        }
        else if (value.compare("UndefinedObject") == 0)
        {
            v = DLMS_ERROR_CODE_UNDEFINED_OBJECT;
        }
        else if (value.compare("UnmatchedType") == 0)
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
        list[DLMS_SERVICE_ERROR_APPLICATION_REFERENCE] = "ApplicationReference";
        list[DLMS_SERVICE_ERROR_HARDWARE_RESOURCE] = "HardwareResource";
        list[DLMS_SERVICE_ERROR_VDE_STATE_ERROR] = "VdeStateError";
        list[DLMS_SERVICE_ERROR_SERVICE] = "Service";
        list[DLMS_SERVICE_ERROR_DEFINITION] = "Definition";
        list[DLMS_SERVICE_ERROR_ACCESS] = "Access";
        list[DLMS_SERVICE_ERROR_INITIATE] = "Initiate";
        list[DLMS_SERVICE_ERROR_LOAD_DATASET] = "LoadDataSet";
        list[DLMS_SERVICE_ERROR_TASK] = "Task";
    }

    static void GetApplicationReference(std::map < DLMS_APPLICATION_REFERENCE, std::string > &list)
    {
        list[DLMS_APPLICATION_REFERENCE_APPLICATION_CONTEXT_UNSUPPORTED] = "ApplicationContextUnsupported";
        list[DLMS_APPLICATION_REFERENCE_APPLICATION_REFERENCE_INVALID] = "ApplicationReferenceInvalid";
        list[DLMS_APPLICATION_REFERENCE_APPLICATION_UNREACHABLE] = "ApplicationUnreachable";
        list[DLMS_APPLICATION_REFERENCE_DECIPHERING_ERROR] = "DecipheringError";
        list[DLMS_APPLICATION_REFERENCE_OTHER] = "Other";
        list[DLMS_APPLICATION_REFERENCE_PROVIDER_COMMUNICATION_ERROR] = "ProviderCommunicationError";
        list[DLMS_APPLICATION_REFERENCE_TIME_ELAPSED] = "TimeElapsed";
    }

    static void GetHardwareResource(std::map < DLMS_HARDWARE_RESOURCE, std::string > &list)
    {
        list[DLMS_HARDWARE_RESOURCE_MASS_STORAGE_UNAVAILABLE] = "MassStorageUnavailable";
        list[DLMS_HARDWARE_RESOURCE_MEMORY_UNAVAILABLE] = "MemoryUnavailable";
        list[DLMS_HARDWARE_RESOURCE_OTHER] = "Other";
        list[DLMS_HARDWARE_RESOURCE_OTHER_RESOURCE_UNAVAILABLE] = "OtherResourceUnavailable";
        list[DLMS_HARDWARE_RESOURCE_PROCESSOR_RESOURCE_UNAVAILABLE] = "ProcessorResourceUnavailable";
    }

    static void GetVdeStateError(std::map < DLMS_VDE_STATE_ERROR, std::string > &list)
    {
        list[DLMS_VDE_STATE_ERROR_LOADING_DATASET] = "LoadingDataSet";
        list[DLMS_VDE_STATE_ERROR_NO_DLMS_CONTEXT] = "NoDlmsContext";
        list[DLMS_VDE_STATE_ERROR_OTHER] = "Other";
        list[DLMS_VDE_STATE_ERROR_STATUS_INOPERABLE] = "StatusInoperable";
        list[DLMS_VDE_STATE_ERROR_STATUS_NO_CHANGE] = "StatusNochange";
    }

    static void GetService(std::map < DLMS_SERVICE, std::string > &list)
    {
        list[DLMS_SERVICE_OTHER] = "Other";
        list[DLMS_SERVICE_PDU_SIZE] = "PduSize";
        list[DLMS_SERVICE_UNSUPPORTED] = "ServiceUnsupported";
    }

    static void GetDefinition(std::map < DLMS_DEFINITION, std::string > &list)
    {
        list[DLMS_DEFINITION_OBJECT_ATTRIBUTE_INCONSISTENT] = "ObjectAttributeInconsistent";
        list[DLMS_DEFINITION_OBJECT_CLASS_INCONSISTENT] = "ObjectClassInconsistent";
        list[DLMS_DEFINITION_OBJECT_UNDEFINED] = "ObjectUndefined";
        list[DLMS_DEFINITION_OTHER] = "Other";
    }

    static void GetAccess(std::map < DLMS_ACCESS, std::string > &list)
    {
        list[DLMS_ACCESS_HARDWARE_FAULT] = "HardwareFault";
        list[DLMS_ACCESS_OBJECT_ACCESS_INVALID] = "ObjectAccessInvalid";
        list[DLMS_ACCESS_OBJECT_UNAVAILABLE] = "ObjectUnavailable";
        list[DLMS_ACCESS_OTHER] = "Other";
        list[DLMS_ACCESS_SCOPE_OF_ACCESS_VIOLATED] = "ScopeOfAccessViolated";
    }

    static void GetInitiate(std::map < DLMS_INITIATE, std::string > &list)
    {
        list[DLMS_INITIATE_DLMS_VERSION_TOO_LOW] = "DlmsVersionTooLow";
        list[DLMS_INITIATE_INCOMPATIBLE_CONFORMANCE] = "IncompatibleConformance";
        list[DLMS_INITIATE_OTHER] = "Other";
        list[DLMS_INITIATE_PDU_SIZE_TOOSHORT] = "PduSizeTooShort";
        list[DLMS_INITIATE_REFUSED_BY_THE_VDE_HANDLER] = "RefusedByTheVDEHandler";
    }

    static void GetLoadDataSet(std::map < DLMS_LOAD_DATASET, std::string > &list)
    {
        list[DLMS_LOAD_DATASET_NOT_READY] = "DataSetNotReady";
        list[DLMS_LOAD_DATASET_SIZE_TOO_LARGE] = "DatasetSizeTooLarge";
        list[DLMS_LOAD_DATASET_INTERPRETATION_FAILURE] = "InterpretationFailure";
        list[DLMS_LOAD_DATASET_NOT_AWAITED_SEGMENT] = "NotAwaitedSegment";
        list[DLMS_LOAD_DATASET_NOT_LOADABLE] = "NotLoadable";
        list[DLMS_LOAD_DATASET_OTHER] = "Other";
        list[DLMS_LOAD_DATASET_PRIMITIVE_OUT_OF_SEQUENCE] = "PrimitiveOutOfSequence";
        list[DLMS_LOAD_DATASET_STORAGE_FAILURE] = "StorageFailure";
    }

    static void GetTask(std::map < DLMS_TASK, std::string > &list)
    {
        list[DLMS_TASK_NO_REMOTE_CONTROL] = "NoRemoteControl";
        list[DLMS_TASK_OTHER] = "Other";
        list[DLMS_TASK_TI_RUNNIN] = "tiRunning";
        list[DLMS_TASK_TI_STOPPED] = "tiStopped";
        list[DLMS_TASK_TI_UNUSABLE] = "tiUnusable";
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

    static std::string ReleaseResponseReasonToString(DLMS_RELEASE_RESPONSE_REASON value)
    {
        std::string str;
        switch (value)
        {
        case DLMS_RELEASE_RESPONSE_REASON_NORMAL:
            str = "Normal";
            break;
        case DLMS_RELEASE_RESPONSE_REASON_NOTFINISHED:
            str = "NotFinished";
            break;
        case DLMS_RELEASE_RESPONSE_REASON_USERDEFINED:
            str = "UserDefined";
            break;
        default:
            str = "Invalid value.";
        }
        return str;
    }

    static DLMS_RELEASE_RESPONSE_REASON ValueOfReleaseResponseReason(std::string value)
    {
        DLMS_RELEASE_RESPONSE_REASON ret;
        if (value.compare("Normal") == 0)
        {
            ret = DLMS_RELEASE_RESPONSE_REASON_NORMAL;
        }
        else if (value.compare("NotFinished") == 0)
        {
            ret = DLMS_RELEASE_RESPONSE_REASON_NOTFINISHED;
        }
        else if (value.compare("UserDefined") == 0)
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
            str = "Normal";
            break;
        case DLMS_RELEASE_REQUEST_REASON_URGENT:
            str = "Urgent";
            break;
        case DLMS_RELEASE_REQUEST_REASON_USER_DEFINED:
            str = "UserDefined";
            break;
        default:
            str = "Invalid value.";
        }
        return str;
    }

    static DLMS_RELEASE_REQUEST_REASON ValueOfReleaseRequestReason(std::string value)
    {
        DLMS_RELEASE_REQUEST_REASON ret;
        if (value.compare("Normal") == 0)
        {
            ret = DLMS_RELEASE_REQUEST_REASON_NORMAL;
        }
        else if (value.compare("Urgent") == 0)
        {
            ret = DLMS_RELEASE_REQUEST_REASON_URGENT;
        }
        else if (value.compare("UserDefined") == 0)
        {
            ret = DLMS_RELEASE_REQUEST_REASON_USER_DEFINED;
        }
        else
        {
            ret = (DLMS_RELEASE_REQUEST_REASON)-1;
        }
        return ret;
    }

    static DLMS_CONFORMANCE ValueOfConformance(std::string value)
    {
        DLMS_CONFORMANCE ret;
        if (value.compare("Access") == 0)
        {
            ret = DLMS_CONFORMANCE_ACCESS;
        }
        else if (value.compare("Action") == 0)
        {
            ret = DLMS_CONFORMANCE_ACTION;
        }
        else if (value.compare("Attribute0SupportedWithGet") == 0)
        {
            ret = DLMS_CONFORMANCE_ATTRIBUTE_0_SUPPORTED_WITH_GET;
        }
        else if (value.compare("Attribute0SupportedWithSet") == 0)
        {
            ret = DLMS_CONFORMANCE_ATTRIBUTE_0_SUPPORTED_WITH_SET;
        }
        else if (value.compare("BlockTransferWithAction") == 0)
        {
            ret = DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_ACTION;
        }
        else if (value.compare("BlockTransferWithGetOrRead") == 0)
        {
            ret = DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_GET_OR_READ;
        }
        else if (value.compare("BlockTransferWithSetOrWrite") == 0)
        {
            ret = DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_SET_OR_WRITE;
        }
        else if (value.compare("DataNotification") == 0)
        {
            ret = DLMS_CONFORMANCE_DATA_NOTIFICATION;
        }
        else if (value.compare("EventNotification") == 0)
        {
            ret = DLMS_CONFORMANCE_EVENT_NOTIFICATION;
        }
        else if (value.compare("GeneralBlockTransfer") == 0)
        {
            ret = DLMS_CONFORMANCE_GENERAL_BLOCK_TRANSFER;
        }
        else if (value.compare("GeneralProtection") == 0)
        {
            ret = DLMS_CONFORMANCE_GENERAL_PROTECTION;
        }
        else if (value.compare("Get") == 0)
        {
            ret = DLMS_CONFORMANCE_GET;
        }
        else if (value.compare("InformationReport") == 0)
        {
            ret = DLMS_CONFORMANCE_INFORMATION_REPORT;
        }
        else if (value.compare("MultipleReferences") == 0)
        {
            ret = DLMS_CONFORMANCE_MULTIPLE_REFERENCES;
        }
        else if (value.compare("ParameterizedAccess") == 0)
        {
            ret = DLMS_CONFORMANCE_PARAMETERIZED_ACCESS;
        }
        else if (value.compare("PriorityMgmtSupported") == 0)
        {
            ret = DLMS_CONFORMANCE_PRIORITY_MGMT_SUPPORTED;
        }
        else if (value.compare("Read") == 0)
        {
            ret = DLMS_CONFORMANCE_READ;
        }
        else if (value.compare("ReservedSeven") == 0)
        {
            ret = DLMS_CONFORMANCE_RESERVED_SEVEN;
        }
        else if (value.compare("ReservedSix") == 0)
        {
            ret = DLMS_CONFORMANCE_RESERVED_SIX;
        }
        else if (value.compare("ReservedZero") == 0)
        {
            ret = DLMS_CONFORMANCE_RESERVED_ZERO;
        }
        else if (value.compare("SelectiveAccess") == 0)
        {
            ret = DLMS_CONFORMANCE_SELECTIVE_ACCESS;
        }
        else if (value.compare("Aet") == 0)
        {
            ret = DLMS_CONFORMANCE_SET;
        }
        else if (value.compare("Unconfirmed-write") == 0)
        {
            ret = DLMS_CONFORMANCE_UN_CONFIRMED_WRITE;
        }
        else if (value.compare("Write") == 0)
        {
            ret = DLMS_CONFORMANCE_WRITE;
        }
        else
        {
            ret = (DLMS_CONFORMANCE)-1;
        }
        return ret;
    }

    static int ConformanceToString(DLMS_CONFORMANCE value, std::string& str) {
        switch (value) {
        case DLMS_CONFORMANCE_ACCESS:
            str = "Access";
            break;
        case DLMS_CONFORMANCE_ACTION:
            str = "Action";
            break;
        case DLMS_CONFORMANCE_ATTRIBUTE_0_SUPPORTED_WITH_GET:
            str = "Attribute0SupportedWithGet";
            break;
        case DLMS_CONFORMANCE_ATTRIBUTE_0_SUPPORTED_WITH_SET:
            str = "Attribute0SupportedWithSet";
            break;
        case DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_ACTION:
            str = "BlockTransferWithAction";
            break;
        case DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_GET_OR_READ:
            str = "BlockTransferWithGetOrRead";
            break;
        case DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_SET_OR_WRITE:
            str = "BlockTransferWithSetOrWrite";
            break;
        case DLMS_CONFORMANCE_DATA_NOTIFICATION:
            str = "DataNotification";
            break;
        case DLMS_CONFORMANCE_EVENT_NOTIFICATION:
            str = "EventNotification";
            break;
        case DLMS_CONFORMANCE_GENERAL_BLOCK_TRANSFER:
            str = "GeneralBlockTransfer";
            break;
        case DLMS_CONFORMANCE_GENERAL_PROTECTION:
            str = "GeneralProtection";
            break;
        case DLMS_CONFORMANCE_GET:
            str = "Get";
            break;
        case DLMS_CONFORMANCE_INFORMATION_REPORT:
            str = "InformationReport";
            break;
        case DLMS_CONFORMANCE_MULTIPLE_REFERENCES:
            str = "MultipleReferences";
            break;
        case DLMS_CONFORMANCE_PARAMETERIZED_ACCESS:
            str = "ParameterizedAccess";
            break;
        case DLMS_CONFORMANCE_PRIORITY_MGMT_SUPPORTED:
            str = "PriorityMgmtSupported";
            break;
        case DLMS_CONFORMANCE_READ:
            str = "Read";
            break;
        case DLMS_CONFORMANCE_RESERVED_SEVEN:
            str = "ReservedSeven";
            break;
        case DLMS_CONFORMANCE_RESERVED_SIX:
            str = "ReservedSix";
            break;
        case DLMS_CONFORMANCE_RESERVED_ZERO:
            str = "ReservedZero";
            break;
        case DLMS_CONFORMANCE_SELECTIVE_ACCESS:
            str = "SelectiveAccess";
            break;
        case DLMS_CONFORMANCE_SET:
            str = "Set";
            break;
        case DLMS_CONFORMANCE_UN_CONFIRMED_WRITE:
            str = "UnconfirmedWrite";
            break;
        case DLMS_CONFORMANCE_WRITE:
            str = "Write";
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        return 0;
    }
};
#endif //TRANSLATOR_SIMPLE_TAGS_H
