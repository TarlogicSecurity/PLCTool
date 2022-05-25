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

#ifndef CGXDLMSSN_COMMAND_HANDLER_H
#define CGXDLMSSN_COMMAND_HANDLER_H

#include "GXBytebuffer.h"
#include "GXDLMSTranslatorStructure.h"
#include "GXDLMSSettings.h"
#include "GXDLMSValueEventCollection.h"
#include "GXSNInfo.h"

class CGXDLMSSNCommandHandler
{
    /**
* Find Short Name object.
*
* @param sn
*/
    static int FindSNObject(CGXDLMSServer* server, int sn, CGXSNInfo& i);

    /**
* Get data for Read command.
*
* @param settings
*            DLMS settings.
* @param list
*            received objects.
* @param data
*            Data as byte array.
* @param type
*            Response type.
*/
    static int GetReadData(
        CGXDLMSSettings& settings,
        std::vector<CGXDLMSValueEventArg*>& list,
        CGXByteBuffer& data,
        DLMS_SINGLE_READ_RESPONSE& type);

    /**
    * Handle read Block in blocks.
    *
    * @param data
    *            Received data.
    */
    static int HandleReadBlockNumberAccess(
        CGXDLMSSettings& settings,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        CGXByteBuffer* replyData,
        CGXDLMSTranslatorStructure* xml);

    static int HandleReadDataBlockAccess(
        CGXDLMSSettings& settings,
        CGXDLMSServer* server,
        DLMS_COMMAND command,
        CGXByteBuffer& data,
        int cnt,
        CGXByteBuffer* replyData,
        CGXDLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    static int ReturnSNError(
        CGXDLMSSettings& settings,
        CGXDLMSServer* server,
        DLMS_COMMAND cmd,
        DLMS_ERROR_CODE error,
        CGXByteBuffer* replyData);

    static int HandleRead(
        CGXDLMSSettings& settings,
        CGXDLMSServer* server,
        DLMS_VARIABLE_ACCESS_SPECIFICATION type,
        CGXByteBuffer& data,
        CGXDLMSValueEventCollection& list,
        std::vector<CGXDLMSValueEventArg*>& reads,
        std::vector<CGXDLMSValueEventArg*>& actions,
        CGXByteBuffer* replyData,
        CGXDLMSTranslatorStructure* xml);
public:
    /**
  * Handle read request.
  */
    static int HandleReadRequest(
        CGXDLMSSettings& settings,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        CGXByteBuffer* replyData,
        CGXDLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    /**
    * Handle write request.
    *
    * @param reply
    *            Received data from the client.
    * @return Reply.
    */
    static  int HandleWriteRequest(
        CGXDLMSSettings& settings,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        CGXByteBuffer* replyData,
        CGXDLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);


    // Handle Information Report.
    static int HandleInformationReport(
        CGXDLMSSettings& settings,
        CGXReplyData& reply);
};
#endif //CGXDLMSSN_COMMAND_HANDLER_H