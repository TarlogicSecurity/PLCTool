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
// and/or modify it under the terms of the GNU General License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#ifndef GXDLMS_H
#define GXDLMS_H

#include "GXDLMSLimits.h"
#include "GXDLMSObject.h"
#include "GXDLMSObjectCollection.h"
#include "GXBytebuffer.h"
#include "GXDLMSSettings.h"
#include "GXDLMSVariant.h"
#include "GXCipher.h"
#include "GXReplyData.h"
#include "GXDLMSLNParameters.h"
#include "GXDLMSSNParameters.h"

class CGXDLMS
{
private:
    friend class CGXDLMSClient;

    static unsigned short CountFCS16(CGXByteBuffer& buff, int index, int count);

    /////////////////////////////////////////////////////////////////////////////
    // Get adress as GXDLMSVariant.
    /////////////////////////////////////////////////////////////////////////////
    static int GetAddress(long value, unsigned long& address, int& size);

    /////////////////////////////////////////////////////////////////////////////
    // Get address as an byte array.
    /////////////////////////////////////////////////////////////////////////////
    static int GetAddressBytes(unsigned long value, CGXByteBuffer& bytes);

    /////////////////////////////////////////////////////////////////////////////
    // Returns true if executed command is reply.
    /////////////////////////////////////////////////////////////////////////////
    // cmd : Executed command.
    // Returns Is command reply.
    /////////////////////////////////////////////////////////////////////////////
    static bool IsReplyMessage(DLMS_COMMAND cmd);

    /////////////////////////////////////////////////////////////////////////////
    // Check LLC bytes.
    /////////////////////////////////////////////////////////////////////////////
    // server : Is server.
    // data : Received data.
    /////////////////////////////////////////////////////////////////////////////
    static void GetLLCBytes(bool server, CGXByteBuffer& data);

    /////////////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////////////
    static int CheckWrapperAddress(
        CGXDLMSSettings& settings,
        CGXByteBuffer& buff,
        CGXReplyData* notify);

    /////////////////////////////////////////////////////////////////////////////
    // Get value from data.
    /////////////////////////////////////////////////////////////////////////////
    // settings : DLMS settings.
    // reply : Received data.
    /////////////////////////////////////////////////////////////////////////////
    static int GetValueFromData(
        CGXDLMSSettings& settings,
        CGXReplyData& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Get data from HDLC or wrapper frame.
    /////////////////////////////////////////////////////////////////////////////
    // reply : Received data that includes HDLC frame.
    // info : Reply data.
    // hdlc : Is HDLC framing used.
    /////////////////////////////////////////////////////////////////////////////
    static void GetDataFromFrame(
        CGXByteBuffer& reply,
        CGXReplyData& info,
        bool hdlc);

    /**
    * Handle read response data block result.
    *
    * @param settings
    *            DLMS settings.
    * @param reply
    *            Received reply.
    * @param index
    *            Starting index.
    */
    static int ReadResponseDataBlockResult(
        CGXDLMSSettings& settings,
        CGXReplyData& reply,
        int index);

    static int HandleGloDedRequest(
        CGXDLMSSettings& settings,
        CGXReplyData& data);

    static int HandleGloDedResponse(
        CGXDLMSSettings& settings,
        CGXReplyData& data, int index);

    static int HandleGeneralCiphering(
        CGXDLMSSettings& settings,
        CGXReplyData& data);


    static int HandleGetResponseWithList(
        CGXDLMSSettings& settings,
        CGXReplyData& reply);

public:

    /////////////////////////////////////////////////////////////////////////////
    // Check client and server addresses. Reserved for internal use.
    /////////////////////////////////////////////////////////////////////////////
    static int CheckInit(CGXDLMSSettings& settings);

    /**
     * Get all Logical name messages. Client uses this to generate messages.
     *
     * @param p
     *            LN settings.
     * @param reply
     *            Generated messages.
     * @return    Status code.
     */
    static int GetLnMessages(
        CGXDLMSLNParameters& p,
        std::vector<CGXByteBuffer>& reply);

    /**
    * Get all Short Name messages. Client uses this to generate messages.
    *
    * @param p
    *            DLMS SN parameters.
    * @param reply
    *            Generated messages.
    * @return    Status code.
    */
    static int GetSnMessages(
        CGXDLMSSNParameters& p,
        std::vector<CGXByteBuffer>& reply);

    /**
    * Handle General block transfer message.
    *
    * @param settings
    *            DLMS settings.
    * @param data
    *            received data.
    */
    static int HandleGbt(CGXDLMSSettings& settings, CGXReplyData& data);

    /**
     * Split DLMS PDU to wrapper frames.
     *
     * @param settings
     *            DLMS settings.
     * @param data
     *            Wrapped data.
     * @return Wrapper frames.
    */
    static int GetWrapperFrame(
        CGXDLMSSettings& settings,
        DLMS_COMMAND command,
        CGXByteBuffer& data,
        CGXByteBuffer& reply);

    /**
    * Get next logical name PDU.
    *
    * @param p
    *            LN parameters.
    * @param reply
    *            Generated message.
    */
    static int GetLNPdu(
        CGXDLMSLNParameters& p,
        CGXByteBuffer& reply);

    /**
    * @param p
    * @param reply
    */
    static int GetSNPdu(
        CGXDLMSSNParameters& p,
        CGXByteBuffer& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Generates an acknowledgment message, with which the server is informed to
    // send next packets.
    /////////////////////////////////////////////////////////////////////////////
    // type : Frame type
    // Returns : Acknowledgment message as unsigned char array.
    /////////////////////////////////////////////////////////////////////////////
    static int ReceiverReady(
        CGXDLMSSettings& settings,
        DLMS_DATA_REQUEST_TYPES type,
        CGXCipher* cipher,
        CGXByteBuffer& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Generates an acknowledgment message, with which the server is informed to
    // send next packets.
    /////////////////////////////////////////////////////////////////////////////
    // type : Frame type
    // Returns : Acknowledgment message as unsigned char array.
    /////////////////////////////////////////////////////////////////////////////
    static int ReceiverReady(
        CGXDLMSSettings& settings,
        CGXReplyData& data,
        CGXCipher* cipher,
        CGXByteBuffer& reply);

    /**
    * Get HDLC frame for data.
    *
    * @param settings
    *            DLMS settings.
    * @param frame
    *            Frame ID. If zero new is generated.
    * @param data
    *            Data to add.
    * @return HDLC frame.
    */
    static int GetHdlcFrame(
        CGXDLMSSettings& settings,
        unsigned char frame,
        CGXByteBuffer* data,
        CGXByteBuffer& reply);

    static int GetHdlcData(
        bool server,
        CGXDLMSSettings& settings,
        CGXByteBuffer& reply,
        CGXReplyData& data,
        unsigned char& frame,
        CGXReplyData* notify);

    /**
     * Get HDLC address from byte array.
     *
     * @param buff
     *            byte array.
     * @return HDLC address.
     */
    static int GetHDLCAddress(
        CGXByteBuffer& buff,
        unsigned long& address);

    /**
     * Check that client and server address match.
     *
     * @param server
     *            Is server.
     * @param settings
     *            DLMS settings.
     * @param reply
     *            Received data.
     * @param index
     *            Position.
     * @return True, if client and server address match.
     */
    static int CheckHdlcAddress(
        bool server,
        CGXDLMSSettings& settings,
        CGXByteBuffer& reply,
        int index,
        unsigned long& source,
        unsigned long& target);

    /////////////////////////////////////////////////////////////////////////////
    // Get data from TCP/IP frame.
    /////////////////////////////////////////////////////////////////////////////
    // settings : DLMS settigns.
    // buff : Received data.
    // data : Reply information.
    /////////////////////////////////////////////////////////////////////////////
    static int GetTcpData(
        CGXDLMSSettings& settings,
        CGXByteBuffer& buff,
        CGXReplyData& data,
        CGXReplyData* notify);

    /////////////////////////////////////////////////////////////////////////////
    // Handle read response and get data from block and/or update error status.
    /////////////////////////////////////////////////////////////////////////////
    // data : Received data from the client.
    /////////////////////////////////////////////////////////////////////////////
    static int HandleReadResponse(
        CGXDLMSSettings& settings,
        CGXReplyData& data,
        int index);

    /////////////////////////////////////////////////////////////////////////////
    // Handle method response and get data from block and/or update error status.
    /////////////////////////////////////////////////////////////////////////////
    // data : Received data from the client.
    /////////////////////////////////////////////////////////////////////////////
    static int HandleMethodResponse(
        CGXDLMSSettings& settings,
        CGXReplyData& data);

    /////////////////////////////////////////////////////////////////////////////
    // Handle push and get data from block and/or update error status.
    /////////////////////////////////////////////////////////////////////////////
    // reply : Received data from the client.
    /////////////////////////////////////////////////////////////////////////////
    static int HandlePush(CGXReplyData& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Handle set response and update error status.
    /////////////////////////////////////////////////////////////////////////////
    // reply : Received data from the client.
    /////////////////////////////////////////////////////////////////////////////
    static int HandleSetResponse(
        CGXDLMSSettings& settings,
        CGXReplyData& data);

    static int HandleAccessResponse(
        CGXDLMSSettings& settings,
        CGXReplyData& reply);

    /**
    * Handle data notification get data from block and/or update error status.
    *
    * @param settings
    *            DLMS settings.
    * @param reply
    *            Received data from the client.
    */
    static int HandleDataNotification(
        CGXDLMSSettings& settings,
        CGXReplyData& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Handle write response and update error status.
    /////////////////////////////////////////////////////////////////////////////
    // reply : Received data from the client.
    /////////////////////////////////////////////////////////////////////////////
    static int HandleWriteResponse(
        CGXReplyData& data);

    /////////////////////////////////////////////////////////////////////////////
    // Handle get response and get data from block and/or update error status.
    /////////////////////////////////////////////////////////////////////////////
    // settings : DLMS settings.
    // reply : Received data from the client.
    // index : Block index number.
    /////////////////////////////////////////////////////////////////////////////
    static int HandleGetResponse(
        CGXDLMSSettings& settings,
        CGXReplyData& reply,
        int index);

    /////////////////////////////////////////////////////////////////////////////
    // Get PDU from the packet.
    /////////////////////////////////////////////////////////////////////////////
    // settings : DLMS settings.
    // data : received data.
    // cipher : Cipher interface.
    /////////////////////////////////////////////////////////////////////////////
    static int GetPdu(
        CGXDLMSSettings& settings,
        CGXReplyData& data);

    /////////////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////////////
    static int GetData(
        CGXDLMSSettings& settings,
        CGXByteBuffer& reply,
        CGXReplyData& data,
        CGXReplyData* notify);

    /////////////////////////////////////////////////////////////////////////////
    // Get action info.
    /////////////////////////////////////////////////////////////////////////////
    // objectType : Object type.
    // value : Starting address.
    // count : Attribute count.
    /////////////////////////////////////////////////////////////////////////////
    static int GetActionInfo(
        DLMS_OBJECT_TYPE objectType,
        unsigned char& value,
        unsigned char& count);


    static int AppendData(
        CGXDLMSObject* obj,
        unsigned char index,
        CGXByteBuffer& bb,
        CGXDLMSVariant& value);

    static int ParseSnrmUaResponse(
        CGXByteBuffer& data,
        CGXDLMSLimits* limits);

    // Add HDLC parameter.
    static void AppendHdlcParameter(CGXByteBuffer& data, unsigned short value);

    static int HandleConfirmedServiceError(CGXReplyData& data);
};
#endif //GXDLMS_H
