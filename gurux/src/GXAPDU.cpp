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

#include "../include/GXAPDU.h"
#include "../include/TranslatorSimpleTags.h"
#include "../include/TranslatorStandardTags.h"
#include "../include/GXDLMSConverter.h"

/**
 * Retrieves the string that indicates the level of authentication, if any.
 */
int GetAuthenticationString(
    CGXDLMSSettings& settings,
    CGXByteBuffer& data,
    bool ignoreAcse)
{
    if (settings.GetAuthentication() != DLMS_AUTHENTICATION_NONE ||
        (!ignoreAcse && settings.GetCipher() != NULL && settings.GetCipher()->GetSecurity() != DLMS_SECURITY_NONE))
    {
        // Add sender ACSE-requirements field component.
        data.SetUInt8(BER_TYPE_CONTEXT
            | PDU_TYPE_SENDER_ACSE_REQUIREMENTS);
        data.SetUInt8(2);
        data.SetUInt8(BER_TYPE_BIT_STRING
            | BER_TYPE_OCTET_STRING);
        data.SetUInt8(0x80);
        data.SetUInt8(BER_TYPE_CONTEXT | PDU_TYPE_MECHANISM_NAME);
        // Len
        data.SetUInt8(7);
        // OBJECT IDENTIFIER
        unsigned char p[] = { 0x60, 0x85, 0x74, 0x05, 0x08, 0x02, (unsigned char)settings.GetAuthentication() };
        data.Set(p, 7);
    }
    // If authentication is used.
    if (settings.GetAuthentication() != DLMS_AUTHENTICATION_NONE)
    {
        // Add Calling authentication information.
        CGXByteBuffer* callingAuthenticationValue;
        if (settings.GetAuthentication() == DLMS_AUTHENTICATION_LOW)
        {
            callingAuthenticationValue = &settings.GetPassword();
        }
        else
        {
            callingAuthenticationValue = &settings.GetCtoSChallenge();
        }
        // 0xAC
        data.SetUInt8(BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_CALLING_AUTHENTICATION_VALUE);
        // Len
        GXHelpers::SetObjectCount(2 + callingAuthenticationValue->GetSize(), data);
        // Add authentication information.
        data.SetUInt8(BER_TYPE_CONTEXT);
        // Len.
        GXHelpers::SetObjectCount(callingAuthenticationValue->GetSize(), data);
        if (callingAuthenticationValue->GetSize() != 0)
        {
            data.Set(callingAuthenticationValue->GetData(), callingAuthenticationValue->GetSize());
        }
    }
    return 0;
}

/**
* Code application context name.
*
* @param settings
*            DLMS settings.
* @param data
*            Byte buffer where data is saved.
* @param cipher
*            Is ciphering settings.
*/
int GenerateApplicationContextName(
    CGXDLMSSettings& settings,
    CGXByteBuffer& data,
    CGXCipher* cipher)
{
    //ProtocolVersion
    if (settings.GetProtocolVersion() != NULL)
    {
        data.SetUInt8(BER_TYPE_CONTEXT | PDU_TYPE_PROTOCOL_VERSION);
        data.SetUInt8(2);
        data.SetUInt8((unsigned char)(8 - strlen(settings.GetProtocolVersion())));
        CGXDLMSVariant tmp = settings.GetProtocolVersion();
        GXHelpers::SetBitString(data, tmp, false);
    }
    // Application context name tag
    data.SetUInt8((BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_APPLICATION_CONTEXT_NAME));
    // Len
    data.SetUInt8(0x09);
    data.SetUInt8(BER_TYPE_OBJECT_IDENTIFIER);
    // Len
    data.SetUInt8(0x07);
    bool ciphered = cipher != NULL && cipher->IsCiphered();

    data.SetUInt8(0x60);
    data.SetUInt8(0x85);
    data.SetUInt8(0x74);
    data.SetUInt8(0x5);
    data.SetUInt8(0x8);
    data.SetUInt8(0x1);
    if (settings.GetUseLogicalNameReferencing())
    {
        data.SetUInt8(ciphered ? 3 : 1);
    }
    else
    {
        data.SetUInt8(ciphered ? 4 : 2);
    }
    // Add system title.
    if (!settings.IsServer() &&
        (ciphered || settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC))
    {
        if (cipher->GetSystemTitle().GetSize() == 0)
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        // Add calling-AP-title
        data.SetUInt8((BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | 6));
        // LEN
        GXHelpers::SetObjectCount(2 + cipher->GetSystemTitle().GetSize(), data);
        data.SetUInt8(BER_TYPE_OCTET_STRING);
        // LEN
        GXHelpers::SetObjectCount(cipher->GetSystemTitle().GetSize(), data);
        data.Set(cipher->GetSystemTitle().GetData(), cipher->GetSystemTitle().GetSize());
    }
    //Add CallingAEInvocationId.
    if (!settings.IsServer() && settings.GetUserID() != 0 && settings.GetCipher()->GetSecurity() != DLMS_SECURITY_NONE)
    {
        data.SetUInt8(BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_CALLING_AE_INVOCATION_ID);
        //LEN
        data.SetUInt8(3);
        data.SetUInt8(BER_TYPE_INTEGER);
        //LEN
        data.SetUInt8(1);
        data.SetUInt8(settings.GetUserID());
    }
    return 0;
}


// Reserved for internal use.
int GetConformanceFromArray(CGXByteBuffer& data, unsigned int& value)
{
    int ret;
    unsigned char v1, v2, v3;
    if ((ret = data.GetUInt8(&v1)) == 0 &&
        (ret = data.GetUInt8(&v2)) == 0 &&
        (ret = data.GetUInt8(&v3)) == 0)
    {
        value = GXHelpers::SwapBits(v1);
        value |= GXHelpers::SwapBits(v2) << 8;
        value |= GXHelpers::SwapBits(v3) << 16;
    }
    return ret;
}

// Reserved for internal use.
int SetConformanceToArray(unsigned int value, CGXByteBuffer& data)
{
    int ret;
    if ((ret = data.SetUInt8(GXHelpers::SwapBits((unsigned char)(value & 0xFF)))) != 0 ||
        (ret = data.SetUInt8(GXHelpers::SwapBits((unsigned char)((value >> 8) & 0xFF)))) != 0 ||
        (ret = data.SetUInt8(GXHelpers::SwapBits((unsigned char)((value >> 16) & 0xFF)))) != 0)
    {

    }
    return ret;
}

/**
 * Generate User information initiate request.
 *
 * @param settings
 *            DLMS settings.
 * @param cipher
 * @param data
 */
int GetInitiateRequest(
    CGXDLMSSettings& settings,
    CGXCipher*, CGXByteBuffer& data)
{
    // Tag for xDLMS-Initiate request
    data.SetUInt8(DLMS_COMMAND_INITIATE_REQUEST);
    // Usage field for dedicated-key component.
    if (settings.GetCipher() == NULL || settings.GetCipher()->GetDedicatedKey().GetSize() == 0 ||
        settings.GetCipher()->GetSecurity() == DLMS_SECURITY_NONE)
    {
        data.SetUInt8(0x00);
    }
    else
    {
        data.SetUInt8(0x1);
        CGXByteBuffer& dedKey = settings.GetCipher()->GetDedicatedKey();
        GXHelpers::SetObjectCount(dedKey.GetSize(), data);
        data.Set(&dedKey, 0, dedKey.GetSize());
    }
    // encoding of the response-allowed component (bool DEFAULT TRUE)
    // usage flag (FALSE, default value TRUE conveyed)
    data.SetUInt8(0);

    // Usage field of the proposed-quality-of-service component. Not used
    if (settings.GetQualityOfService() == 0)
    {
        data.SetUInt8(0x00);
    }
    else
    {
        data.SetUInt8(0x01);
        data.SetUInt8(settings.GetQualityOfService());
    }
    data.SetUInt8(settings.GetDLMSVersion());
    // Tag for conformance block
    data.SetUInt8(0x5F);
    data.SetUInt8(0x1F);
    // length of the conformance block
    data.SetUInt8(0x04);
    // encoding the number of unused bits in the bit string
    data.SetUInt8(0x00);
    //Add conformance block.
    SetConformanceToArray((unsigned int)settings.GetProposedConformance(), data);
    data.SetUInt16(settings.GetMaxPduSize());
    return 0;
}

int CGXAPDU::GenerateUserInformation(
    CGXDLMSSettings& settings,
    CGXCipher* cipher,
    CGXByteBuffer* encryptedData,
    CGXByteBuffer& data)
{
    int ret;
    data.SetUInt8(BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_USER_INFORMATION);
    if (cipher == NULL || !cipher->IsCiphered())
    {
        // Length for AARQ user field
        data.SetUInt8(0x10);
        // Coding the choice for user-information (Octet STRING, universal)
        data.SetUInt8(BER_TYPE_OCTET_STRING);
        // Length
        data.SetUInt8(0x0E);
        if ((ret = GetInitiateRequest(settings, cipher, data)) != 0)
        {
            return ret;
        }
    }
    else
    {
        if (encryptedData != NULL && encryptedData->GetSize() != 0)
        {
            //Length for AARQ user field
            data.SetUInt8((unsigned char)(4 + encryptedData->GetSize()));
            //Tag
            data.SetUInt8(BER_TYPE_OCTET_STRING);
            data.SetUInt8((unsigned char)(2 + encryptedData->GetSize()));
            //Coding the choice for user-information (Octet STRING, universal)
            data.SetUInt8(DLMS_COMMAND_GLO_INITIATE_REQUEST);
            data.SetUInt8((unsigned char)encryptedData->GetSize());
            data.Set(encryptedData);
        }
        else
        {
            unsigned char cmd = DLMS_COMMAND_GLO_INITIATE_REQUEST;
            CGXByteBuffer crypted;
            CGXByteBuffer& key = cipher->GetBlockCipherKey();
            if ((ret = GetInitiateRequest(settings, cipher, crypted)) != 0)
            {
                return ret;
            }
            if ((ret = cipher->Encrypt(cipher->GetSecurity(),
                DLMS_COUNT_TYPE_PACKET,
                settings.GetCipher()->GetFrameCounter(),
                cmd,
                cipher->GetSystemTitle(),
                key,
                crypted,
                true)) != 0)
            {
                return ret;
            }
            // Length for AARQ user field
            GXHelpers::SetObjectCount(2 + crypted.GetSize(), data);
            // Coding the choice for user-information (Octet string, universal)
            data.SetUInt8(BER_TYPE_OCTET_STRING);
            GXHelpers::SetObjectCount(crypted.GetSize(), data);
            data.Set(&crypted);
        }
    }
    return 0;
}

void GetConformance(unsigned long value, CGXDLMSTranslatorStructure* xml)
{
    std::string str;
    unsigned long tmp = 1;
    if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
        for (char it = 0; it != 24; ++it)
        {
            if ((tmp & value) != 0)
            {
                CTranslatorSimpleTags::ConformanceToString((DLMS_CONFORMANCE)tmp, str);
                xml->AppendLine(TRANSLATOR_GENERAL_TAGS_CONFORMANCE_BIT, "Name", str);
            }
            tmp = tmp << 1;
        }
    }
    else
    {
        for (char it = 0; it != 24; ++it)
        {
            if ((tmp & value) != 0)
            {
                CTranslatorStandardTags::ConformanceToString((DLMS_CONFORMANCE)tmp, str);
                str.append(" ");
                xml->Append(str);
            }
            tmp = tmp << 1;
        }
    }
}

int CGXAPDU::Parse(bool initiateRequest,
    CGXDLMSSettings& settings,
    CGXCipher*,
    CGXByteBuffer& data,
    CGXDLMSTranslatorStructure* xml,
    unsigned char tag)
{
    int ret;
    unsigned char ch, len;
    std::string str;
    bool response = tag == DLMS_COMMAND_INITIATE_RESPONSE;
    if (response)
    {
        if (xml != NULL)
        {
            xml->AppendStartTag(DLMS_COMMAND_INITIATE_RESPONSE);
        }
        // Optional usage field of the negotiated quality of service
        // component
        if ((ret = data.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        if (tag != 0)
        {
            if ((ret = data.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            settings.SetQualityOfService(tag);
            if (xml != NULL)
            {
                //NegotiatedQualityOfService
                std::string str;
                xml->IntegerToHex((long)tag, 2, str);
                xml->AppendLine(TRANSLATOR_GENERAL_TAGS_NEGOTIATED_QUALITY_OF_SERVICE, "", str);
            }
        }
    }
    else if (tag == DLMS_COMMAND_INITIATE_REQUEST)
    {
        if (xml != NULL)
        {
            xml->AppendStartTag(DLMS_COMMAND_INITIATE_REQUEST);
        }
        //Optional usage field of dedicated key.
        if ((ret = data.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        if (settings.GetCipher() != NULL)
        {
            settings.GetCipher()->GetDedicatedKey().Clear();
        }
        if (tag != 0)
        {
            if ((ret = data.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            CGXByteBuffer tmp;
            tmp.Set(&data, data.GetPosition(), len);
            if (settings.GetCipher() != NULL)
            {
                settings.GetCipher()->SetDedicatedKey(tmp);
            }
            if (xml != NULL)
            {
                str = tmp.ToHexString(false);
                xml->AppendLine(TRANSLATOR_GENERAL_TAGS_DEDICATED_KEY, "", str);
            }
        }
        // Optional usage field of the negotiated quality of service
        // component
        if ((ret = data.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        if (tag != 0)
        {
            if ((ret = data.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            settings.SetQualityOfService(tag);
            if (xml != NULL && (initiateRequest || xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML))
            {
                xml->IntegerToHex((long)tag, 2, str);
                xml->AppendLine(TRANSLATOR_GENERAL_TAGS_PROPOSED_QUALITY_OF_SERVICE, "", str);
            }
        }
        else
        {
            if (xml != NULL && xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
            {
                str = "true";
                xml->AppendLine(TRANSLATOR_GENERAL_TAGS_RESPONSE_ALLOWED, "", str);
            }
        }
        // Optional usage field of the proposed quality of service component
        if ((ret = data.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        // Skip if used.
        if (tag != 0)
        {
            if ((ret = data.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            settings.SetQualityOfService(len);
            if (xml != NULL && xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
            {
                xml->IntegerToHex((long)len, 2, str);
                xml->AppendLine(TRANSLATOR_GENERAL_TAGS_PROPOSED_QUALITY_OF_SERVICE, "", str);
            }

        }
    }
    else if (tag == DLMS_COMMAND_CONFIRMED_SERVICE_ERROR)
    {
        if (xml != NULL)
        {
            xml->AppendStartTag(DLMS_COMMAND_CONFIRMED_SERVICE_ERROR);
            if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
            {
                if ((ret = data.GetUInt8(&ch)) != 0)
                {
                    return ret;
                }
                xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_INITIATE_ERROR);
                if ((ret = data.GetUInt8(&ch)) != 0)
                {
                    return ret;
                }
                DLMS_SERVICE_ERROR type = (DLMS_SERVICE_ERROR)ch;
                std::string str = CTranslatorStandardTags::ServiceErrorToString(type);
                if ((ret = data.GetUInt8(&ch)) != 0)
                {
                    return ret;
                }
                std::string value = CTranslatorStandardTags::GetServiceErrorValue(type, ch);
                xml->AppendLine("x:" + str, "", value);
                xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_INITIATE_ERROR);
            }
            else
            {
                if ((ret = data.GetUInt8(&ch)) != 0)
                {
                    return ret;
                }
                xml->IntegerToHex((long)ch, 2, str);
                xml->AppendLine(DLMS_TRANSLATOR_TAGS_SERVICE, "", str);
                if ((ret = data.GetUInt8(&ch)) != 0)
                {
                    return ret;
                }
                DLMS_SERVICE_ERROR type = (DLMS_SERVICE_ERROR)ch;
                xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_SERVICE_ERROR);
                if ((ret = data.GetUInt8(&ch)) != 0)
                {
                    return ret;
                }
                std::string str1 = CTranslatorSimpleTags::ServiceErrorToString(type);
                str = CTranslatorSimpleTags::GetServiceErrorValue(type, ch);
                xml->AppendLine(str1, "", str);
                xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_SERVICE_ERROR);
            }
            xml->AppendEndTag(DLMS_COMMAND_CONFIRMED_SERVICE_ERROR);
            return 0;
        }

        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        DLMS_CONFIRMED_SERVICE_ERROR service = (DLMS_CONFIRMED_SERVICE_ERROR)ch;
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        DLMS_SERVICE_ERROR type = (DLMS_SERVICE_ERROR)ch;
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        return service << 16 | type << 8 | ch;
    }
    else
    {
        if (xml != NULL)
        {
            xml->AppendComment("Error: Failed to decrypt data.");
            data.SetPosition(data.GetSize());
            return 0;
        }
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    // Get DLMS version number.
    if (!response)
    {
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        settings.SetDLMSVersion(ch);
        //ProposedDlmsVersionNumber
        if (xml != NULL && (initiateRequest || xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML))
        {
            xml->IntegerToHex((long)ch, 2, str);
            xml->AppendLine(TRANSLATOR_GENERAL_TAGS_PROPOSED_DLMS_VERSION_NUMBER, "", str);
        }
    }
    else
    {
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (xml != NULL && (initiateRequest || xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML))
        {
            xml->IntegerToHex((long)ch, 2, str);
            xml->AppendLine(TRANSLATOR_GENERAL_TAGS_NEGOTIATED_DLMS_VERSION_NUMBER, "", str);
        }
        else if (ch != 6)
        {
            //Invalid DLMS version number.
            return DLMS_ERROR_CODE_INVALID_VERSION_NUMBER;
        }
    }

    // Tag for conformance block
    if ((ret = data.GetUInt8(&tag)) != 0)
    {
        return ret;
    }
    if (tag != 0x5F)
    {
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    // Old Way...
    if ((ret = data.GetUInt8(data.GetPosition(), &tag)) != 0)
    {
        return ret;
    }
    if (tag == 0x1F)
    {
        data.SetPosition(data.GetPosition() + 1);
    }
    if ((ret = data.GetUInt8(&len)) != 0)
    {
        return ret;
    }
    // The number of unused bits in the bit string.
    if ((ret = data.GetUInt8(&tag)) != 0)
    {
        return ret;
    }
    unsigned short pduSize;
    unsigned int v;
    if ((ret = GetConformanceFromArray(data, v)) != 0)
    {
        return ret;
    }
    if (settings.IsServer())
    {
        if (xml != NULL)
        {
            xml->AppendStartTag(TRANSLATOR_GENERAL_TAGS_PROPOSED_CONFORMANCE);
            GetConformance(v, xml);
        }
        else
        {
            v &= settings.GetProposedConformance();
            settings.SetNegotiatedConformance((DLMS_CONFORMANCE)v);
        }
    }
    else
    {
        if (xml != NULL)
        {
            xml->AppendStartTag(TRANSLATOR_GENERAL_TAGS_NEGOTIATED_CONFORMANCE);
            GetConformance(v, xml);
        }
        settings.SetNegotiatedConformance((DLMS_CONFORMANCE)v);
    }

    if (!response)
    {
        if ((ret = data.GetUInt16(&pduSize)) != 0)
        {
            return ret;
        }
        settings.SetMaxReceivePDUSize(pduSize);
        if (xml != NULL)
        {
            // ProposedConformance closing
            if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
            {
                xml->AppendEndTag(TRANSLATOR_GENERAL_TAGS_PROPOSED_CONFORMANCE);
            }
            else if (initiateRequest)
            {
                xml->Append(TRANSLATOR_GENERAL_TAGS_PROPOSED_CONFORMANCE, false);
            }
            // ProposedMaxPduSize
            xml->IntegerToHex((long)pduSize, 4, str);
            xml->AppendLine(TRANSLATOR_GENERAL_TAGS_PROPOSED_MAX_PDU_SIZE, "", str);
        }
        //If client asks too high PDU.
        if (pduSize > settings.GetMaxServerPDUSize())
        {
            settings.SetMaxReceivePDUSize(settings.GetMaxServerPDUSize());
        }
    }
    else
    {
        if ((ret = data.GetUInt16(&pduSize)) != 0)
        {
            return ret;
        }
        settings.SetMaxReceivePDUSize(pduSize);
        if (xml != NULL)
        {
            // NegotiatedConformance closing
            if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
            {
                xml->AppendEndTag(TRANSLATOR_GENERAL_TAGS_NEGOTIATED_CONFORMANCE);
            }
            else if (initiateRequest)
            {
                xml->Append(TRANSLATOR_GENERAL_TAGS_NEGOTIATED_CONFORMANCE, false);
            }
            // NegotiatedMaxPduSize
            xml->IntegerToHex((long)pduSize, 4, str);
            xml->AppendLine(TRANSLATOR_GENERAL_TAGS_NEGOTIATED_MAX_PDU_SIZE, "", str);
        }
    }
    if (response)
    {
        // VAA Name
        unsigned short vaa;
        if ((ret = data.GetUInt16(&vaa)) != 0)
        {
            return ret;
        }
        if (xml != NULL)
        {
            if (initiateRequest || xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
            {
                xml->IntegerToHex((long)vaa, 4, str);
                xml->AppendLine(TRANSLATOR_GENERAL_TAGS_VAA_NAME, "", str);
            }
        }
        if (vaa == 0x0007)
        {
            // If LN
            if (!settings.GetUseLogicalNameReferencing())
            {
                //Invalid VAA.
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
        }
        else if (vaa == 0xFA00)
        {
            // If SN
            if (settings.GetUseLogicalNameReferencing())
            {
                //Invalid VAA.
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
        }
        else
        {
            // Unknown VAA.
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        if (xml != NULL)
        {
            xml->AppendEndTag(DLMS_COMMAND_INITIATE_RESPONSE);
        }
    }
    else if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_COMMAND_INITIATE_REQUEST);
    }
    return 0;
}

int CGXAPDU::ParseInitiate(
    bool initiateRequest,
    CGXDLMSSettings& settings,
    CGXCipher* cipher,
    CGXByteBuffer& data,
    CGXDLMSTranslatorStructure* xml)
{
    int ret;
    int originalPos;
    unsigned char tag, tag1;
    unsigned long cnt;
    CGXByteBuffer encrypted;
    // Tag for xDLMS-Initate.response
    if ((ret = data.GetUInt8(&tag)) != 0)
    {
        return ret;
    }
    if (tag == DLMS_COMMAND_GLO_INITIATE_RESPONSE ||
        tag == DLMS_COMMAND_GLO_INITIATE_REQUEST ||
        tag == DLMS_COMMAND_GENERAL_GLO_CIPHERING ||
        tag == DLMS_COMMAND_GENERAL_DED_CIPHERING)
    {
        if (xml != NULL)
        {
            std::string str;
            originalPos = data.GetPosition();
            CGXByteBuffer st;
            if (tag == DLMS_COMMAND_GENERAL_GLO_CIPHERING ||
                tag == DLMS_COMMAND_GENERAL_DED_CIPHERING)
            {
                if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
                {
                    return ret;
                }
                st.Set(&data, data.GetPosition(), cnt);
            }
            else
            {
                st = settings.GetSourceSystemTitle();
            }
            if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
            {
                return ret;
            }
            encrypted.Set(&data, data.GetPosition(), data.Available());
            if (cipher != NULL && xml->GetComments())
            {
                int pos = xml->GetXmlLength();
                data.SetPosition(originalPos - 1);
                DLMS_SECURITY security = DLMS_SECURITY_NONE;
                DLMS_SECURITY_SUITE suite;
                if ((ret = cipher->Decrypt(st,
                    settings.GetCipher()->GetBlockCipherKey(),
                    data, security, suite)) != 0)
                {
                    return ret;
                }
                cipher->SetSecurity(security);
                if ((ret = data.GetUInt8(&tag1)) != 0)
                {
                    return ret;
                }
                xml->StartComment("Decrypted data:");
                str = "Security: ";
                str.append(CGXDLMSConverter::ToString(security));
                xml->AppendLine(str);
                if (Parse(initiateRequest, settings, cipher, data, xml, tag1) == 0)
                {
                    xml->EndComment();
                }
                else
                {
                    // It's OK if this fails.
                    xml->SetXmlLength(pos);
                }
            }
            str = encrypted.ToHexString(false);
            xml->AppendLine(tag, "", str);
            return 0;
        }
        data.SetPosition(data.GetPosition() - 1);
        DLMS_SECURITY security = DLMS_SECURITY_NONE;
        DLMS_SECURITY_SUITE suite;
        if ((ret = cipher->Decrypt(settings.GetSourceSystemTitle(),
            settings.GetCipher()->GetBlockCipherKey(), data, security, suite)) != 0)
        {
            return ret;
        }
        cipher->SetSecurity(security);
        cipher->SetSecuritySuite(suite);
        if ((ret = data.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
    }
    return Parse(initiateRequest, settings, cipher, data, xml, tag);
}

/**
 * Parse User Information from PDU.
 */
int CGXAPDU::ParseUserInformation(
    CGXDLMSSettings& settings,
    CGXCipher* cipher,
    CGXByteBuffer& data,
    CGXDLMSTranslatorStructure* xml)
{
    int ret;
    unsigned char len, tag;
    if ((ret = data.GetUInt8(&len)) != 0)
    {
        if (xml == NULL)
        {
            return ret;
        }
        xml->AppendComment("Error: Invalid data size.");
    }
    if (data.GetSize() - data.GetPosition() < len)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    // Encoding the choice for user information
    if ((ret = data.GetUInt8(&tag)) != 0)
    {
        return ret;
    }
    if (tag != 0x4)
    {
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    if ((ret = data.GetUInt8(&len)) != 0)
    {
        return ret;
    }
    if (xml != NULL && xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
    {
        std::string str = data.ToHexString(data.GetPosition(), len, false);
        xml->AppendLine(DLMS_COMMAND_INITIATE_REQUEST, "", str);
        data.SetPosition(data.GetPosition() + len);
        return 0;
    }
    return ParseInitiate(false, settings, cipher, data, xml);
}

/**
 * Parse application context name.
 *
 * @param settings
 *            DLMS settings.
 * @param buff
 *            Received data.
 */
int ParseApplicationContextName(
    CGXDLMSSettings& settings,
    CGXByteBuffer& buff,
    CGXDLMSTranslatorStructure* xml)
{
    int ret;
    unsigned char len, ch, name;
    // Get length.
    if ((ret = buff.GetUInt8(&len)) != 0)
    {
        return ret;
    }
    if (buff.GetSize() - buff.GetPosition() < len)
    {
        //Encoding failed. Not enough data.
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x6)
    {
        //Encoding failed. Not an Object ID.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (settings.IsServer() && settings.GetCipher() != NULL)
    {
        settings.GetCipher()->SetSecurity(DLMS_SECURITY_NONE);
    }
    // Object ID length.
    if ((ret = buff.GetUInt8(&len)) != 0)
    {
        return ret;
    }

    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x60)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x85)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x74)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x5)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x8)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x1)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }

    if ((ret = buff.GetUInt8(&name)) != 0)
    {
        return ret;
    }
    if (xml != NULL)
    {
        std::string str;
        settings.SetUseLogicalNameReferencing(name == 1 || name == 3);
        if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
        {
            if (name == 1)
            {
                str = "LN";
            }
            else if (name == 2)
            {
                str = "SN";
            }
            else if (name == 3)
            {
                str = "LN_WITH_CIPHERING";
            }
            else if (name == 4)
            {
                str = "SN_WITH_CIPHERING";
            }
        }
        else
        {
            str = GXHelpers::IntToString(name);
        }
        xml->AppendLine(TRANSLATOR_GENERAL_TAGS_APPLICATION_CONTEXT_NAME, "", str);
        return 0;
    }

    if (settings.GetUseLogicalNameReferencing())
    {
        if (name == 1 || name == 3)
        {
            return 0;
        }
        return DLMS_ERROR_CODE_FALSE;
    }
    if (name == 2 || name == 4)
    {
        return 0;
    }
    return DLMS_ERROR_CODE_FALSE;
}

int ValidateAare(
    CGXDLMSSettings& settings,
    CGXByteBuffer& buff)
{
    int ret;
    unsigned char tag;
    if ((ret = buff.GetUInt8(&tag)) != 0)
    {
        return ret;
    }
    if (settings.IsServer())
    {
        if (tag != (BER_TYPE_APPLICATION
            | BER_TYPE_CONSTRUCTED
            | PDU_TYPE_PROTOCOL_VERSION))
        {
            return DLMS_ERROR_CODE_INVALID_TAG;
        }
    }
    else
    {
        if (tag != (BER_TYPE_APPLICATION
            | BER_TYPE_CONSTRUCTED
            | PDU_TYPE_APPLICATION_CONTEXT_NAME))
        {
            return DLMS_ERROR_CODE_INVALID_TAG;
        }
    }
    return 0;
}

int UpdatePassword(
    CGXDLMSSettings& settings,
    CGXByteBuffer& buff,
    CGXDLMSTranslatorStructure* xml)
{
    CGXByteBuffer tmp;
    int ret;
    unsigned char ch, len;
    if ((ret = buff.GetUInt8(&len)) != 0)
    {
        return ret;
    }

    // Get authentication information.
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x80)
    {
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    if ((ret = buff.GetUInt8(&len)) != 0)
    {
        return ret;
    }
    tmp.Set(&buff, buff.GetPosition(), len);
    if (settings.GetAuthentication() == DLMS_AUTHENTICATION_LOW)
    {
        settings.SetPassword(tmp);
    }
    else
    {
        settings.SetCtoSChallenge(tmp);
    }
    if (xml != NULL)
    {
        if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
        {
            std::string str;
            if (settings.GetAuthentication() == DLMS_AUTHENTICATION_LOW)
            {
                if (settings.GetPassword().IsAsciiString())
                {
                    xml->AppendComment(settings.GetPassword().ToString());
                }
                str = settings.GetPassword().ToHexString(false);
                xml->AppendLine(TRANSLATOR_GENERAL_TAGS_CALLING_AUTHENTICATION,
                    "", str);
            }
            else
            {
                str = settings.GetCtoSChallenge().ToHexString(false);
                xml->AppendLine(TRANSLATOR_GENERAL_TAGS_CALLING_AUTHENTICATION,
                    "", str);
            }
        }
        else
        {
            xml->AppendStartTag(TRANSLATOR_GENERAL_TAGS_CALLING_AUTHENTICATION);
            xml->AppendStartTag(TRANSLATOR_GENERAL_TAGS_CHAR_STRING);
            std::string str;
            if (settings.GetAuthentication() == DLMS_AUTHENTICATION_LOW)
            {
                str = settings.GetPassword().ToHexString(false);
                xml->Append(str);
            }
            else
            {
                str = settings.GetCtoSChallenge().ToHexString(false);
                xml->Append(str);
            }
            xml->AppendEndTag(TRANSLATOR_GENERAL_TAGS_CHAR_STRING);
            xml->AppendEndTag(TRANSLATOR_GENERAL_TAGS_CALLING_AUTHENTICATION);
        }
    }
    return 0;
}

int UpdateAuthentication(
    CGXDLMSSettings& settings,
    CGXByteBuffer& buff)
{
    int ret;
    unsigned char ch;
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }

    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x60)
    {
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x85)
    {
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x74)
    {
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x05)
    {
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x08)
    {
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0x02)
    {
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch > DLMS_AUTHENTICATION_HIGH_SHA256)
    {
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    settings.SetAuthentication((DLMS_AUTHENTICATION)ch);
    return 0;
}

int CGXAPDU::GetUserInformation(
    CGXDLMSSettings& settings,
    CGXCipher* cipher,
    CGXByteBuffer& data)
{
    data.Clear();
    data.SetUInt8(DLMS_COMMAND_INITIATE_RESPONSE); // Tag for xDLMS-Initiate
    // response
    // Usage field for the response allowed component.// (not used)
    data.SetUInt8(0x00);
    // DLMS Version Number
    data.SetUInt8(06);
    data.SetUInt8(0x5F);
    data.SetUInt8(0x1F);
    // length of the conformance block
    data.SetUInt8(0x04);
    // encoding the number of unused bits in the bit string
    data.SetUInt8(0x00);
    SetConformanceToArray((unsigned int)settings.GetNegotiatedConformance(), data);
    data.SetUInt16(settings.GetMaxPduSize());
    // VAA Name VAA name (0x0007 for LN referencing and 0xFA00 for SN)
    if (settings.GetUseLogicalNameReferencing())
    {
        data.SetUInt16(0x0007);
    }
    else
    {
        data.SetUInt16(0xFA00);
    }
    if (cipher != NULL && cipher->IsCiphered())
    {
        return cipher->Encrypt(cipher->GetSecurity(),
            DLMS_COUNT_TYPE_PACKET,
            settings.GetCipher()->GetFrameCounter(),
            DLMS_COMMAND_GLO_INITIATE_RESPONSE,
            cipher->GetSystemTitle(),
            cipher->GetBlockCipherKey(),
            data,
            true);
    }
    return 0;
}

int CGXAPDU::GenerateAarq(
    CGXDLMSSettings& settings,
    CGXCipher* cipher,
    CGXByteBuffer* encryptedData,
    CGXByteBuffer& data)
{
    int ret;
    // AARQ APDU Tag
    data.SetUInt8(BER_TYPE_APPLICATION | BER_TYPE_CONSTRUCTED);
    // Length is updated later.
    unsigned long offset = data.GetSize();
    data.SetUInt8(0);
    ///////////////////////////////////////////
    // Add Application context name.
    if ((ret = GenerateApplicationContextName(settings, data, cipher)) != 0)
    {
        return ret;
    }
    if ((ret = GetAuthenticationString(settings, data, encryptedData != NULL && encryptedData->GetSize() != 0)) != 0)
    {
        return ret;
    }
    if ((ret = GenerateUserInformation(settings, cipher, encryptedData, data)) != 0)
    {
        return ret;
    }
    data.SetUInt8(offset, (unsigned char)(data.GetSize() - offset - 1));
    return 0;
}

int handleResultComponent(DLMS_SOURCE_DIAGNOSTIC value)
{
    int ret;
    switch ((int)value)
    {
    case DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN:
        ret = DLMS_ERROR_CODE_NO_REASON_GIVEN;
        break;
    case DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED:
        ret = DLMS_ERROR_CODE_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED;
        break;
    case DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED:
        ret = DLMS_ERROR_CODE_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED;
        break;
    case DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_REQUIRED:
        ret = DLMS_ERROR_CODE_AUTHENTICATION_MECHANISM_NAME_REQUIRED;
        break;
    case DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_FAILURE:
        ret = DLMS_ERROR_CODE_AUTHENTICATION_FAILURE;
        break;
    default:
        //OK.
        ret = 0;
        break;
    }
    return ret;
}

int ParseProtocolVersion(CGXDLMSSettings& settings,
    CGXByteBuffer& buff)
{
    unsigned char cnt, unusedBits, value;
    int ret;
    if ((ret = buff.GetUInt8(&cnt)) != 0)
    {
        return ret;
    }
    if ((ret = buff.GetUInt8(&unusedBits)) != 0)
    {
        return ret;
    }
    if (unusedBits > 8)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((ret = buff.GetUInt8(&value)) != 0)
    {
        return ret;
    }
    CGXByteBuffer sb;
    GXHelpers::ToBitString(sb, value, 8 - unusedBits);
    settings.SetProtocolVersion(sb.ToString().c_str());
    return 0;
}


void AppendServerSystemTitleToXml(
    CGXDLMSSettings& settings,
    CGXDLMSTranslatorStructure* xml,
    int tag)
{
    if (xml != NULL)
    {
        std::string str;
        // RespondingAuthentication
        if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
        {
            str = settings.GetStoCChallenge().ToHexString(false);
            xml->AppendLine(tag, "", str);
        }
        else
        {
            xml->Append(tag, true);
            xml->Append((int)TRANSLATOR_GENERAL_TAGS_CHAR_STRING, true);
            str = settings.GetStoCChallenge().ToHexString(false);
            xml->Append(str);
            xml->Append((int)TRANSLATOR_GENERAL_TAGS_CHAR_STRING, false);
            xml->Append(tag, false);
            str = "\r\n";
            xml->Append(str);
        }
    }
}
int CGXAPDU::ParsePDU2(
    CGXDLMSSettings& settings,
    CGXCipher* cipher,
    CGXByteBuffer& buff,
    DLMS_ASSOCIATION_RESULT& result,
    DLMS_SOURCE_DIAGNOSTIC& diagnostic,
    CGXDLMSTranslatorStructure* xml)
{
    CGXByteBuffer tmp;
    unsigned char tag, len;
    int ret;
    std::string str;
    result = DLMS_ASSOCIATION_RESULT_ACCEPTED;
    while (buff.GetPosition() < buff.GetSize())
    {
        if ((ret = buff.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        switch (tag)
        {
            //0xA1
        case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_APPLICATION_CONTEXT_NAME:
        {
            if ((ret = ParseApplicationContextName(settings, buff, xml)) != 0)
            {
                return DLMS_ERROR_CODE_REJECTED_PERMAMENT;
            }
        }
        break;
        // 0xA2
        case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_CALLED_AP_TITLE:
            // Get len.
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if (len != 3)
            {
                return DLMS_ERROR_CODE_INVALID_TAG;
            }

            if (settings.IsServer())
            {
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag != BER_TYPE_OCTET_STRING)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                CGXByteBuffer bb;
                bb.Set(&buff, buff.GetPosition(), len);
                settings.SetSourceSystemTitle(bb);
                if (xml != NULL)
                {
                    //RespondingAPTitle
                    unsigned char *asBytes = NULL;
                    unsigned long count = 0;
                    bb.ToArray(asBytes, count);

                    if (count >= 3
                        && isprint(asBytes[0])
                        && isprint(asBytes[1])
                        && isprint(asBytes[2])) {
                      std::string title;
                      bool allPrintable = true;
                      char hex[4];
                      for (unsigned long i = 3;
                           i < count && allPrintable;
                           ++i)
                        allPrintable = allPrintable && isprint(asBytes[i]);

                      if (allPrintable) {
                        std::copy(
                              reinterpret_cast<char *>(asBytes),
                              reinterpret_cast<char *>(asBytes) + count,
                              std::back_inserter(title));
                      } else {
                        std::copy(
                              reinterpret_cast<char *>(asBytes),
                              reinterpret_cast<char *>(asBytes) + 3,
                              std::back_inserter(title));

                        title += " (";
                        for (unsigned long i = 3; i < count; ++i) {
                          snprintf(hex, 4, "%02x", asBytes[i]);
                          title += hex;
                        }
                        title += ")";
                      }

                      xml->AppendComment(title);
                    }

                    free(asBytes);

                    str = bb.ToHexString(false);
                    xml->AppendLine(DLMS_TRANSLATOR_TAGS_CALLED_AP_TITLE, "", str);
                }
            }
            else
            {
                // Choice for result (INTEGER, universal)
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag != BER_TYPE_INTEGER)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                // Get len.
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                if (len != 1)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                result = (DLMS_ASSOCIATION_RESULT)tag;
                if (xml != NULL)
                {
                    if (result != DLMS_ASSOCIATION_RESULT_ACCEPTED)
                    {
                        str = CGXDLMSConverter::ToString(result);
                        xml->AppendComment(str);
                    }
                    std::string str;
                    xml->IntegerToHex((unsigned long)result, 2, str);
                    xml->AppendLine(TRANSLATOR_GENERAL_TAGS_ASSOCIATION_RESULT, "", str);
                    xml->AppendStartTag(TRANSLATOR_GENERAL_TAGS_RESULT_SOURCE_DIAGNOSTIC);
                }
            }
            break;
            // 0xA3 SourceDiagnostic
        case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_CALLED_AE_QUALIFIER:
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            // ACSE service user tag.
            if ((ret = buff.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if (settings.IsServer())
            {
                CGXByteBuffer calledAEQualifier;
                calledAEQualifier.Set(&buff, buff.GetPosition(), len);
                if (xml != NULL)
                {
                    str = calledAEQualifier.ToHexString(false);
                    xml->AppendLine(DLMS_TRANSLATOR_TAGS_CALLED_AE_QUALIFIER, "", str);
                }
            }
            else
            {
                // Result source diagnostic component.
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag != BER_TYPE_INTEGER)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                if (len != 1)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                diagnostic = (DLMS_SOURCE_DIAGNOSTIC)tag;
                if (xml != NULL)
                {
                    if (diagnostic != DLMS_SOURCE_DIAGNOSTIC_NONE)
                    {
                        str = CGXDLMSConverter::ToString(diagnostic);
                        xml->AppendComment(str);
                    }
                    std::string str;
                    xml->IntegerToHex((long)diagnostic, 2, str);
                    xml->AppendLine(TRANSLATOR_GENERAL_TAGS_ACSE_SERVICE_USER, "", str);
                    xml->AppendEndTag(TRANSLATOR_GENERAL_TAGS_RESULT_SOURCE_DIAGNOSTIC);
                }
            }
            break;
            // 0xA4 Result
        case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_CALLED_AP_INVOCATION_ID:
            // Get len.
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if (settings.IsServer())
            {
                //Get len.
                if (len != 0xA)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                //Choice for result (Universal, Integer)
                if (tag != BER_TYPE_INTEGER)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                if (len != 1)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                //Get value.
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                if (xml != NULL)
                {
                    //RespondingAPTitle
                    std::string str;
                    xml->IntegerToHex((long)len, 2, str);
                    xml->AppendLine(DLMS_TRANSLATOR_TAGS_CALLED_AP_INVOCATION_ID, "", str);
                }
            }
            else
            {
                if (len != 0xA)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                // Choice for result (Universal, Octet string type)
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag != BER_TYPE_OCTET_STRING)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                // responding-AP-title-field
                // Get len.
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                tmp.Clear();
                tmp.Set(&buff, buff.GetPosition(), len);
                settings.SetSourceSystemTitle(tmp);
                if (xml != NULL)
                {
                    //RespondingAPTitle
                    str = tmp.ToHexString(false);
                    xml->AppendLine(TRANSLATOR_GENERAL_TAGS_RESPONDING_AP_TITLE, "", str);
                }
            }
            break;
            // 0xA6 Client system title.
        case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_CALLING_AP_TITLE:
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if ((ret = buff.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            tmp.Clear();
            tmp.Set(&buff, buff.GetPosition(), len);
            settings.SetSourceSystemTitle(tmp);
            if (xml != NULL)
            {
                //CallingAPTitle
                unsigned char *asBytes = NULL;
                unsigned long count = 0;
                tmp.ToArray(asBytes, count);

                if (count >= 3
                    && isprint(asBytes[0])
                    && isprint(asBytes[1])
                    && isprint(asBytes[2])) {
                  std::string title;
                  bool allPrintable = true;
                  char hex[4];
                  for (unsigned long i = 3;
                       i < count && allPrintable;
                       ++i)
                    allPrintable = allPrintable && isprint(asBytes[i]);

                  if (allPrintable) {
                    std::copy(
                          reinterpret_cast<char *>(asBytes),
                          reinterpret_cast<char *>(asBytes) + count,
                          std::back_inserter(title));
                  } else {
                    std::copy(
                          reinterpret_cast<char *>(asBytes),
                          reinterpret_cast<char *>(asBytes) + 3,
                          std::back_inserter(title));

                    title += " (";
                    for (unsigned long i = 3; i < count; ++i) {
                      snprintf(hex, 4, "%02x", asBytes[i]);
                      title += hex;
                    }
                    title += ")";
                  }


                  xml->AppendComment(title);
                }

                free(asBytes);

                str = tmp.ToHexString(false);
                xml->AppendLine(TRANSLATOR_GENERAL_TAGS_CALLING_AP_TITLE, "", str);
            }
            break;
            // 0xAA Server system title.
        case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_SENDER_ACSE_REQUIREMENTS:
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if ((ret = buff.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            tmp.Clear();
            tmp.Set(&buff, buff.GetPosition(), len);
            settings.SetStoCChallenge(tmp);
            AppendServerSystemTitleToXml(settings, xml, tag);
            break;
            //Client AEInvocationId.
        case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_CALLING_AE_INVOCATION_ID://0xA9
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if ((ret = buff.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if ((ret = buff.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            settings.SetUserID(tag);
            if (xml != NULL)
            {
                std::string str;
                xml->IntegerToHex((long)tag, 2, str);
                xml->AppendLine(TRANSLATOR_GENERAL_TAGS_CALLING_AE_INVOCATION_ID, "", str);
            }
            break;
            //Client CalledAeInvocationId.
        case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_CALLED_AE_INVOCATION_ID://0xA5
            if (settings.IsServer())
            {
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag != 3)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                if (len != 2)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                if (len != 1)
                {
                    return DLMS_ERROR_CODE_INVALID_TAG;
                }
                //Get value.
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (xml != NULL)
                {
                    //CalledAEInvocationId
                    std::string str;
                    xml->IntegerToHex((long)tag, 2, str);
                    xml->AppendLine(DLMS_TRANSLATOR_TAGS_CALLED_AE_INVOCATION_ID, "", str);
                }
            }
            else
            {
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                settings.SetUserID(tag);
                if (xml != NULL)
                {
                    std::string str;
                    xml->IntegerToHex((long)tag, 2, str);
                    xml->AppendLine(TRANSLATOR_GENERAL_TAGS_CALLED_AE_INVOCATION_ID, "", str);
                }
            }
            break;
            //Server RespondingAEInvocationId.
        case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | 7://0xA7
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if ((ret = buff.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if ((ret = buff.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            settings.SetUserID(tag);
            if (xml != NULL)
            {
                xml->IntegerToHex((long)tag, 2, str);
                xml->AppendLine(TRANSLATOR_GENERAL_TAGS_RESPONDING_AE_INVOCATION_ID, "", str);
            }
            break;
        case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_CALLING_AP_INVOCATION_ID://0xA8
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if (len != 3)
            {
                return DLMS_ERROR_CODE_INVALID_TAG;
            }
            if ((ret = buff.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            if (tag != 2)
            {
                return DLMS_ERROR_CODE_INVALID_TAG;
            }
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if (len != 1)
            {
                return DLMS_ERROR_CODE_INVALID_TAG;
            }
            //Get value.
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if (xml != NULL)
            {
                //CallingApInvocationId
                std::string str;
                xml->IntegerToHex((long)len, 2, str);
                xml->AppendLine(DLMS_TRANSLATOR_TAGS_CALLING_AP_INVOCATION_ID, "Value", str);
            }
            break;
            //  0x8A or 0x88
        case BER_TYPE_CONTEXT | PDU_TYPE_SENDER_ACSE_REQUIREMENTS:
        case BER_TYPE_CONTEXT | PDU_TYPE_CALLING_AP_INVOCATION_ID:
            // Get sender ACSE-requirements field component.
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if (len != 2)
            {
                return DLMS_ERROR_CODE_INVALID_TAG;
            }
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            if (len != BER_TYPE_OBJECT_DESCRIPTOR)
            {
                return DLMS_ERROR_CODE_INVALID_TAG;
            }
            //Get only value because client app is sending system title with LOW authentication.
            if ((ret = buff.GetUInt8(&len)) != 0)
            {
                return ret;
            }
            //SenderACSERequirements
            if (xml != NULL)
            {
                str = "1";
                xml->AppendLine(tag, "", str);
            }
            break;
            //  0x8B or 0x89
        case BER_TYPE_CONTEXT | PDU_TYPE_MECHANISM_NAME:
        case BER_TYPE_CONTEXT | PDU_TYPE_CALLING_AE_INVOCATION_ID:
            if ((ret = UpdateAuthentication(settings, buff)) != 0)
            {
                return ret;
            }
            if (xml != NULL)
            {
                if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
                {
                    str = CGXDLMSConverter::ToString(settings.GetAuthentication());
                    xml->AppendLine((unsigned long)tag, "", str);
                }
                else
                {
                    str = GXHelpers::IntToString(settings.GetAuthentication());
                    xml->AppendLine((unsigned long)tag, "", str);
                }
            }
            break;
            // 0xAC
        case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_CALLING_AUTHENTICATION_VALUE:
            if ((ret = UpdatePassword(settings, buff, xml)) != 0)
            {
                return ret;
            }
            break;
            // 0xBE
        case BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_USER_INFORMATION:
            //Check result component. Some meters are returning invalid user-information if connection failed.
            if (xml == NULL && result != DLMS_ASSOCIATION_RESULT_ACCEPTED
                && diagnostic != DLMS_SOURCE_DIAGNOSTIC_NONE)
            {
                return handleResultComponent(diagnostic);
            }
            if ((ret = ParseUserInformation(settings, cipher, buff, xml)) != 0)
            {
                result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
                diagnostic = DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN;
                return 0;
            }
            break;
        case BER_TYPE_CONTEXT: //0x80
            if (ParseProtocolVersion(settings, buff) != 0)
            {
                result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
                diagnostic = DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN;
                return 0;
            }
            break;
        default:
            // Unknown tags.
            if (buff.GetPosition() < buff.GetSize())
            {
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                buff.SetPosition(buff.GetPosition() + len);
            }
            break;
        }
    }
    //All meters don't send user-information if connection is failed.
    //For this reason result component is check again.
    if (xml == NULL)
    {
        return handleResultComponent(diagnostic);
    }
    return 0;
}

int CGXAPDU::ParsePDU(
    CGXDLMSSettings& settings,
    CGXCipher* cipher,
    CGXByteBuffer& buff,
    DLMS_ASSOCIATION_RESULT& result,
    DLMS_SOURCE_DIAGNOSTIC& diagnostic,
    CGXDLMSTranslatorStructure* xml)
{
    CGXByteBuffer tmp;
    unsigned long len;
    int ret;
    diagnostic = DLMS_SOURCE_DIAGNOSTIC_NONE;
    // Get AARE tag and length
    if ((ret = ValidateAare(settings, buff)) != 0)
    {
        return ret;
    }
    if ((ret = GXHelpers::GetObjectCount(buff, len)) != 0)
    {
        return ret;
    }
    unsigned int size = buff.GetSize() - buff.GetPosition();
    if (len > size)
    {
        if (xml == NULL)
        {
            //Encoding failed. Not enough data.
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        xml->AppendComment("Error: Invalid data size.");
    }
    //Opening tags
    if (xml != NULL)
    {
        if (settings.IsServer())
        {
            xml->AppendStartTag(DLMS_COMMAND_AARQ);
        }
        else
        {
            xml->AppendStartTag(DLMS_COMMAND_AARE);
        }
    }
    ret = ParsePDU2(
        settings,
        cipher,
        buff,
        result,
        diagnostic,
        xml);
    //Closing tags
    if (xml != NULL)
    {
        if (settings.IsServer())
        {
            xml->AppendEndTag(DLMS_COMMAND_AARQ);
        }
        else
        {
            xml->AppendEndTag(DLMS_COMMAND_AARE);
        }
    }
    return ret;
}

/**
 * Server generates AARE message.
 */
int CGXAPDU::GenerateAARE(
    CGXDLMSSettings& settings,
    CGXByteBuffer& data,
    DLMS_ASSOCIATION_RESULT result,
    DLMS_SOURCE_DIAGNOSTIC diagnostic,
    CGXCipher* cipher,
    CGXByteBuffer* errorData,
    CGXByteBuffer* encryptedData)
{
    int ret;
    unsigned long offset = data.GetSize();
    // Set AARE tag and length 0x61
    data.SetUInt8(BER_TYPE_APPLICATION | BER_TYPE_CONSTRUCTED | PDU_TYPE_APPLICATION_CONTEXT_NAME);
    // Length is updated later.
    data.SetUInt8(0);
    if ((ret = GenerateApplicationContextName(settings, data, cipher)) != 0)
    {
        return ret;
    }
    // Result 0xA2
    data.SetUInt8(BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | BER_TYPE_INTEGER);
    data.SetUInt8(3); // len
    // Tag
    data.SetUInt8(BER_TYPE_INTEGER);
    // Choice for result (INTEGER, universal)
    data.SetUInt8(1); // Len
    // ResultValue
    data.SetUInt8(result);
    // SourceDiagnostic
    data.SetUInt8(0xA3);
    data.SetUInt8(5); // len
    data.SetUInt8(0xA1); // Tag
    data.SetUInt8(3); // len
    data.SetUInt8(2); // Tag
    // Choice for result (INTEGER, universal)
    data.SetUInt8(1); // Len
    // diagnostic
    data.SetUInt8(diagnostic);
    // SystemTitle
    if (cipher != NULL
        && (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC
            || cipher->IsCiphered()))
    {
        data.SetUInt8(BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_CALLED_AP_INVOCATION_ID);
        GXHelpers::SetObjectCount(2 + cipher->GetSystemTitle().GetSize(), data);
        data.SetUInt8(BER_TYPE_OCTET_STRING);
        GXHelpers::SetObjectCount(cipher->GetSystemTitle().GetSize(), data);
        data.Set(&cipher->GetSystemTitle());
    }
    if (settings.GetAuthentication() > DLMS_AUTHENTICATION_LOW)
    {
        // Add server ACSE-requirenents field component.
        data.SetUInt8(0x88);
        data.SetUInt8(0x02); // Len.
        data.SetUInt16(0x0780);
        // Add tag.
        data.SetUInt8(0x89);
        data.SetUInt8(0x07); // Len
        data.SetUInt8(0x60);
        data.SetUInt8(0x85);
        data.SetUInt8(0x74);
        data.SetUInt8(0x05);
        data.SetUInt8(0x08);
        data.SetUInt8(0x02);
        data.SetUInt8(settings.GetAuthentication());
        // Add tag.
        data.SetUInt8(0xAA);
        GXHelpers::SetObjectCount(2 + settings.GetStoCChallenge().GetSize(), data); // Len
        data.SetUInt8(BER_TYPE_CONTEXT);
        GXHelpers::SetObjectCount(settings.GetStoCChallenge().GetSize(), data);
        data.Set(settings.GetStoCChallenge().GetData(), settings.GetStoCChallenge().GetSize());
    }
    if (result == DLMS_ASSOCIATION_RESULT_ACCEPTED || cipher == NULL || cipher->GetSecurity() == DLMS_SECURITY_NONE)
    {
        // Add User Information. Tag 0xBE
        data.SetUInt8(BER_TYPE_CONTEXT | BER_TYPE_CONSTRUCTED | PDU_TYPE_USER_INFORMATION);
        CGXByteBuffer tmp;
        if (encryptedData != NULL && encryptedData->GetSize() != 0)
        {
            tmp.Capacity(2 + encryptedData->GetSize());
            tmp.SetUInt8(DLMS_COMMAND_GLO_INITIATE_RESPONSE);
            GXHelpers::SetObjectCount(encryptedData->GetSize(), tmp);
            tmp.Set(encryptedData);
        }
        else
        {
            if (errorData != NULL && errorData->GetSize() != 0)
            {
                tmp.Set(errorData);
            }
            else
            {
                if ((ret = GetUserInformation(settings, cipher, tmp)) != 0)
                {
                    return ret;
                }
            }
        }

        GXHelpers::SetObjectCount(2 + tmp.GetSize(), data);
        // Coding the choice for user-information (Octet STRING, universal)
        data.SetUInt8(BER_TYPE_OCTET_STRING);
        // Length
        GXHelpers::SetObjectCount(tmp.GetSize(), data);
        data.Set(&tmp);
    }
    data.SetUInt8(offset + 1, (unsigned char)(data.GetSize() - offset - 2));
    return 0;
}
