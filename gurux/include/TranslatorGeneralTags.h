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
// Gurux Device Framework is Open Source software, you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY, without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#ifndef TRANSLATOR_GENERAL_TAGS_H
#define TRANSLATOR_GENERAL_TAGS_H

enum TRANSLATOR_GENERAL_TAGS {
    TRANSLATOR_GENERAL_TAGS_APPLICATION_CONTEXT_NAME = 0xA1,
    TRANSLATOR_GENERAL_TAGS_NEGOTIATED_QUALITY_OF_SERVICE = 0xBE00,
    TRANSLATOR_GENERAL_TAGS_PROPOSED_DLMS_VERSION_NUMBER = 0xBE01,
    TRANSLATOR_GENERAL_TAGS_PROPOSED_MAX_PDU_SIZE = 0xBE02,
    TRANSLATOR_GENERAL_TAGS_PROPOSED_CONFORMANCE = 0xBE03,
    TRANSLATOR_GENERAL_TAGS_VAA_NAME = 0xBE04,
    TRANSLATOR_GENERAL_TAGS_NEGOTIATED_CONFORMANCE = 0xBE05,
    TRANSLATOR_GENERAL_TAGS_NEGOTIATED_DLMS_VERSION_NUMBER = 0xBE06,
    TRANSLATOR_GENERAL_TAGS_NEGOTIATED_MAX_PDU_SIZE = 0xBE07,
    TRANSLATOR_GENERAL_TAGS_CONFORMANCE_BIT = 0xBE08,
    TRANSLATOR_GENERAL_TAGS_PROPOSED_QUALITY_OF_SERVICE = 0xBE09,
    TRANSLATOR_GENERAL_TAGS_SENDER_ACSE_REQUIREMENTS = 0x8A,
    TRANSLATOR_GENERAL_TAGS_RESPONDER_ACSE_REQUIREMENT = 0x88,
    TRANSLATOR_GENERAL_TAGS_RESPONDING_MECHANISM_NAME = 0x89,
    TRANSLATOR_GENERAL_TAGS_CALLING_MECHANISM_NAME = 0x8B,
    TRANSLATOR_GENERAL_TAGS_CALLING_AUTHENTICATION = 0xAC,
    TRANSLATOR_GENERAL_TAGS_RESPONDING_AUTHENTICATION = 0x80,
    TRANSLATOR_GENERAL_TAGS_ASSOCIATION_RESULT = 0xA2,
    TRANSLATOR_GENERAL_TAGS_RESULT_SOURCE_DIAGNOSTIC = 0xA3,
    TRANSLATOR_GENERAL_TAGS_ACSE_SERVICE_USER = 0xA301,
    TRANSLATOR_GENERAL_TAGS_RESPONDING_AP_TITLE = 0xA4,
    TRANSLATOR_GENERAL_TAGS_DEDICATED_KEY = 0xA8,
    TRANSLATOR_GENERAL_TAGS_CALLING_AP_TITLE = 0xA6,
    TRANSLATOR_GENERAL_TAGS_CALLING_AE_INVOCATION_ID = 0xA9,
    TRANSLATOR_GENERAL_TAGS_CALLED_AE_INVOCATION_ID = 0xA5,
    TRANSLATOR_GENERAL_TAGS_RESPONDING_AE_INVOCATION_ID = 0xA7,
    TRANSLATOR_GENERAL_TAGS_CHAR_STRING = 0xAA,
    TRANSLATOR_GENERAL_TAGS_USER_INFORMATION = 0xAB
};
#endif //TRANSLATOR_GENERAL_TAGS_H