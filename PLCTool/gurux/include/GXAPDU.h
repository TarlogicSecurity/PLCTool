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

#ifndef GXAPDU_H
#define GXAPDU_H

#include <vector>
#include "enums.h"
#include "errorcodes.h"
#include "GXHelpers.h"
#include "GXDLMSSettings.h"
#include "GXDLMSTranslatorStructure.h"

/////////////////////////////////////////////////////////////////////////////
// The services to access the attributes and methods of COSEM objects are
// determined on DLMS/COSEM Application layer. The services are carried by
// Application Protocol Data Units (APDUs).
// <p />
// In DLMS/COSEM the meter is primarily a server, and the controlling system is
// a client. Also unsolicited (received without a request) messages are
// available.
/////////////////////////////////////////////////////////////////////////////
class CGXAPDU
{
    friend class CGXDLMSTranslator;
private:
    /////////////////////////////////////////////////////////////////////////////
    // Constructor.
    /////////////////////////////////////////////////////////////////////////////
    CGXAPDU()
    {
    }
    static int ParsePDU2(
        CGXDLMSSettings& settings,
        CGXCipher* cipher,
        CGXByteBuffer& buff,
        DLMS_ASSOCIATION_RESULT& result,
        DLMS_SOURCE_DIAGNOSTIC& diagnostic,
        CGXDLMSTranslatorStructure* xml);

    static int Parse(bool initiateRequest,
        CGXDLMSSettings& settings,
        CGXCipher* cipher,
        CGXByteBuffer& data,
        CGXDLMSTranslatorStructure* xml,
        unsigned char tag);
public:

    /**
     * Parse User Information from PDU.
     */
    static int ParseUserInformation(
        CGXDLMSSettings& settings,
        CGXCipher* cipher,
        CGXByteBuffer& data,
        CGXDLMSTranslatorStructure* xml);

    /**
     * Generates Aarq.
     */
    static int GenerateAarq(
        CGXDLMSSettings& settings,
        CGXCipher* cipher,
        CGXByteBuffer* encryptedData,
        CGXByteBuffer& data);

    /**
    * Generate user information.
    *
    * @param settings
    *            DLMS settings.
    * @param cipher
    * @param data
    *            Generated user information.
    */
    static int GenerateUserInformation(
        CGXDLMSSettings& settings,
        CGXCipher* cipher,
        CGXByteBuffer* encryptedData,
        CGXByteBuffer& data);

    /**
     * Parse APDU.
     */
    static int ParsePDU(CGXDLMSSettings& settings,
        CGXCipher* cipher,
        CGXByteBuffer& buff,
        DLMS_ASSOCIATION_RESULT &result,
        DLMS_SOURCE_DIAGNOSTIC& diagnostic,
        CGXDLMSTranslatorStructure* xml);

    /**
     * Server generates AARE message.
     */
    static int GenerateAARE(
        CGXDLMSSettings& settings,
        CGXByteBuffer& data,
        DLMS_ASSOCIATION_RESULT result,
        DLMS_SOURCE_DIAGNOSTIC diagnostic,
        CGXCipher* cipher,
        CGXByteBuffer *errorData,
        CGXByteBuffer *encryptedData);

    static int GetUserInformation(
        CGXDLMSSettings& settings,
        CGXCipher* cipher,
        CGXByteBuffer& data);

    static int ParseInitiate(
        bool initiateRequest,
        CGXDLMSSettings& settings,
        CGXCipher* cipher,
        CGXByteBuffer& data,
        CGXDLMSTranslatorStructure* xml);
};

#endif //GXAPDU_H
