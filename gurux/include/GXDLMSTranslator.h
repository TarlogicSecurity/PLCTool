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

#ifndef GXDLMSTRANSLATOR_H
#define GXDLMSTRANSLATOR_H

#include "enums.h"
#include "GXBytebuffer.h"
#include "TranslatorSimpleTags.h"
#include "TranslatorStandardTags.h"
#include "GXDLMSTranslatorStructure.h"
#include "GXDLMSSettings.h"


class CGXDLMSTranslator
{
    DLMS_TRANSLATOR_OUTPUT_TYPE m_OutputType;
    unsigned short m_SAck, m_RAck;
    bool sending;
    unsigned char m_SSendSequence;
    unsigned char m_SReceiveSequence;
    unsigned char m_RSendSequence;
    unsigned char m_RReceiveSequence;
    bool m_Comments;
    std::map<unsigned long, std::string> m_Tags;
    std::map<std::string, unsigned long> m_TagsByName;
    bool m_PduOnly;
    bool m_CompletePdu;
    bool m_Hex;
    bool m_ShowStringAsHex;
    bool m_OmitXmlDeclaration;
    bool m_OmitXmlNameSpace;
    // Sending data in multiple frames.
    bool m_MultipleFrames;

    // If only PDUs are shown and PDU is received on parts.
    CGXByteBuffer m_PduFrames;

    int PduToXml(CGXByteBuffer& value, bool omitDeclaration, bool omitNameSpace, std::string& output);

    int PduToXml(CGXDLMSTranslatorStructure* xml, CGXByteBuffer& value, bool omitDeclaration, bool omitNameSpace, bool allowUnknownCommand, std::string& output);
    void GetCiphering(CGXDLMSSettings& settings, bool force);
public:
    // Are comments added.
    bool GetComments();
    // Are comments added.
    void SetComments(bool value);

    // Is only PDU shown when data is parsed with MessageToXml.
    bool GetPduOnly();
    // Is only PDU shown when data is parsed with MessageToXml.
    void SetPduOnly(bool value);

    // Is only complete PDU parsed and shown.
    bool GetCompletePdu();
    // Is only complete PDU parsed and shown.
    void SetCompletePdu(bool value);

    // Are numeric values shown as hex.
    bool GetHex();
    // Are numeric values shown as hex.
    void SetHex(bool value);

    // Is string serialized as hex.
    bool GetShowStringAsHex();
    // Is string serialized as hex.
    void SetShowStringAsHex(bool value);

    // Is XML declaration skipped.
    bool GetOmitXmlDeclaration();
    // Is XML declaration skipped.
    void SetOmitXmlDeclaration(bool value);

    // Is XML declaration skipped.
    bool GetOmitXmlNameSpace();
    // Is XML declaration skipped.
    void SetOmitXmlNameSpace(bool value);

    DLMS_SECURITY m_Security;
    /**
    * System title.
    */
    CGXByteBuffer m_SystemTitle;

    /**
    * Server System title.
    */
    CGXByteBuffer m_ServerSystemTitle;

    /**
    * Dedicated key.
    */
    CGXByteBuffer m_DedicatedKey;

    /**
    * Is data encrypted.
    */
    bool m_Encrypt;

    /**
    *  Block cipher key.
    */
    CGXByteBuffer m_BlockCipherKey;
    /**
    *  Block Authentication key.
    */
    CGXByteBuffer m_AuthenticationKey;

    /**
     * Frame counter. AKA Invocation counter.
     */
    unsigned long m_FrameCounter;

    /*
    * Get all tags.
    * type: Output type.
    * list: List of tags by ID.
    * tagsByName: List of tags by name.
    */
    static void GetTags(DLMS_TRANSLATOR_OUTPUT_TYPE type,
        std::map<unsigned long, std::string>& list,
        std::map<std::string, unsigned long>& tagsByName)
    {
        if (type == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
        {
            CTranslatorSimpleTags::GetGeneralTags(type, list);
            CTranslatorSimpleTags::GetSnTags(type, list);
            CTranslatorSimpleTags::GetLnTags(type, list);
            CTranslatorSimpleTags::GetGloTags(type, list);
            CTranslatorSimpleTags::GetDedTags(type, list);
            CTranslatorSimpleTags::GetTranslatorTags(type, list);
            CTranslatorSimpleTags::GetDataTypeTags(list);
        }
        else
        {
            CTranslatorStandardTags::GetGeneralTags(type, list);
            CTranslatorStandardTags::GetSnTags(type, list);
            CTranslatorStandardTags::GetLnTags(type, list);
            CTranslatorStandardTags::GetGloTags(type, list);
            CTranslatorStandardTags::GetDedTags(type, list);
            CTranslatorStandardTags::GetTranslatorTags(type, list);
            CTranslatorStandardTags::GetDataTypeTags(list);
        }
        // Simple is not case sensitive.
        bool lowercase = type == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML;
        for (std::map<unsigned long, std::string>::iterator it = list.begin(); it != list.end(); ++it)
        {
            std::string str = it->second;
            if (lowercase)
            {
                std::transform(str.begin(), str.end(), str.begin(), ::tolower);
            }
            if (tagsByName.find(str) == tagsByName.end())
            {
                tagsByName[str] = it->first;
            }
        }
    }

public:
    // Constructor.
    // Translator output type.
    CGXDLMSTranslator(DLMS_TRANSLATOR_OUTPUT_TYPE type);

    /**
   * @return Used security.
   */
    DLMS_SECURITY GetSecurity();

    /**
    * @param value
    *            Used security.
    */
    void SetSecurity(DLMS_SECURITY value);

    /**
     * @return System title.
     */
    CGXByteBuffer& GetSystemTitle();

    /**
    *  @param value System title.
    */
    void SetSystemTitle(CGXByteBuffer& value);


    /*
     * Returns System title.
     */
    CGXByteBuffer& GetServerSystemTitle();

    /*
    *  value: System title.
    */
    void SetServerSystemTitle(CGXByteBuffer& value);

    /*
     * Returns dedicated key.
    */
    CGXByteBuffer& GetDedicatedKey();

    /*
    *  value: Dedicated ky.
    */
    void SetDedicatedKey(CGXByteBuffer& value);


    /**
     * @return Block cipher key.
     */
    CGXByteBuffer& GetBlockCipherKey();

    /**
    *  @param value Block cipher key.
    */
    void SetBlockCipherKey(CGXByteBuffer& value);

    /**
     * @return Authentication key.
     */
    CGXByteBuffer& GetAuthenticationKey();

    /**
     * @param value
     *            Authentication key.
     */
    void SetAuthenticationKey(CGXByteBuffer& value);

    /**
     * @return Frame counter. Invocation counter.
     */
    unsigned long GetFrameCounter();

    void SetFrameCounter(unsigned long value);

    // Convert bytes to xml.
    // value: Bytes to convert.
    // Returns Converted xml.
    int PduToXml(CGXByteBuffer& value, std::string& output)
    {
        return PduToXml(value, m_OmitXmlDeclaration, m_OmitXmlNameSpace, output);
    }

    static DLMS_ERROR_CODE ValueOfErrorCode(
        DLMS_TRANSLATOR_OUTPUT_TYPE type,
        std::string& value);

    static int ErrorCodeToString(
        DLMS_TRANSLATOR_OUTPUT_TYPE type,
        DLMS_ERROR_CODE value,
        std::string& result);

    // Convert data bytes to XML.
    // <param name="data">Data to parse.</param>
    // <param name="xml">Generated xml.</param>
    // <returns>Error code.</returns>
    int DataToXml(CGXByteBuffer& data, std::string& xml);

};
#endif //GXDATE_H