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

#ifndef GXDLMSLN_COMMAND_HANDLER_H
#define GXDLMSLN_COMMAND_HANDLER_H

#include "GXBytebuffer.h"
#include "GXDLMSTranslatorStructure.h"
#include "GXDLMSSettings.h"
#include "GXDLMSConnectionEventArgs.h"


class CGXDLMSLNCommandHandler
{
    /**
 * Handle get request normal command.
 *
 * @param data
 *            Received data.
 */
    static int GetRequestNormal(
        CGXDLMSSettings& settings,
        unsigned char invokeID,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        CGXByteBuffer* replyData,
        CGXDLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    /**
     * Handle get request with list command.
     *
     * @param data
     *            Received data.
     */
    static int GetRequestWithList(
        CGXDLMSSettings& settings,
        unsigned char invokeID,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        CGXByteBuffer* replyData,
        CGXDLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    static int HanleSetRequestWithDataBlock(
        CGXDLMSSettings& settings,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        CGXDLMSLNParameters& p,
        CGXDLMSTranslatorStructure* xml);

    static int HandleSetRequestNormal(
        CGXDLMSSettings& settings,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        short type,
        CGXDLMSLNParameters& p,
        CGXDLMSTranslatorStructure* xml);

    static int HanleSetRequestWithList(
        CGXDLMSSettings& settings,
        unsigned char invoke,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        CGXDLMSLNParameters& p,
        CGXDLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

public:
    /**
    * Handle get request next data block command.
    *
    * @param data
    *            Received data.
    */
    static int GetRequestNextDataBlock(
        CGXDLMSSettings& settings,
        unsigned char invokeID,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        CGXByteBuffer* replyData,
        CGXDLMSTranslatorStructure* xml,
        bool streaming,
        unsigned char cipheredCommand);

    /**
    * Handle Get request.
    *
    * @return Reply to the client.
    */
    static int HandleGetRequest(
        CGXDLMSSettings& settings,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        CGXByteBuffer* replyData,
        CGXDLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    /**
    * Handle Set request.
    *
    * @return Reply to the client.
    */
    static int HandleSetRequest(
        CGXDLMSSettings& settings,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        CGXByteBuffer* replyData,
        CGXDLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    /**
  * Handle action request.
  *
  * @param reply
  *            Received data from the client.
  * @return Reply.
  */
    static int HandleMethodRequest(
        CGXDLMSSettings& settings,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        CGXByteBuffer* replyData,
        CGXDLMSConnectionEventArgs* connectionInfo,
        CGXDLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    //Handle Access request.
    static int HandleAccessRequest(
        CGXDLMSSettings& settings,
        CGXDLMSServer* server,
        CGXByteBuffer& data,
        CGXByteBuffer* replyData,
        CGXDLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    // Handle Event Notification.
    static int HandleEventNotification(
        CGXDLMSSettings& settings,
        CGXReplyData& reply);

};
#endif //GXDLMSLN_COMMAND_HANDLER_H