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

#ifndef GXDLMSSERVER_H
#define GXDLMSSERVER_H

#include <vector>
#include "GXDLMSLongTransaction.h"
#include "GXReplyData.h"
#include "GXDLMSSettings.h"
#include "GXSNInfo.h"
#include "GXDLMSSNParameters.h"
#include "GXDLMSLNParameters.h"
#include "GXDLMSConnectionEventArgs.h"
#include "GXDLMSHdlcSetup.h"
#include "GXDLMSTcpUdpSetup.h"
#include "GXDLMSAssociationLogicalName.h"
#include "GXDLMSAssociationShortName.h"
#include "GXDLMSPushSetup.h"

class CGXDLMSProfileGeneric;
class CGXServerReply;

class CGXDLMSServer
{
    friend class CGXDLMSProfileGeneric;
    friend class CGXDLMSValueEventArg;
    friend class CGXDLMSAssociationLogicalName;
    friend class CGXDLMSAssociationShortName;
    friend class CGXDLMSLNCommandHandler;
    friend class CGXDLMSSNCommandHandler;
private:
    long m_DataReceived;
    CGXDLMSIecHdlcSetup* m_Hdlc;
    CGXDLMSTcpUdpSetup* m_Wrapper;
    CGXReplyData m_Info;
    /**
     * Received data.
     */
    CGXByteBuffer m_ReceivedData;

    /**
     * Reply data.
     */
    CGXByteBuffer m_ReplyData;

    /**
     * Long get or read transaction information.
     */
    CGXDLMSLongTransaction* m_Transaction;

    /**
     * Is server initialized.
     */
    bool m_Initialized;

    /**
    * Parse SNRM Request. If server do not accept client empty byte array is
    * returned.
    *
    * @return Returns returned UA packet.
    */
    int HandleSnrmRequest(CGXDLMSSettings& settings, CGXByteBuffer& data, CGXByteBuffer& reply);

    /**
    * Reset settings when connection is made or close.
    *
    * @param connected
    *            Is co3nnected.
    */
    void Reset(bool connected);

    int ReportError(
        DLMS_COMMAND command,
        DLMS_ERROR_CODE error,
        CGXByteBuffer& reply);

    /**
    * Generate confirmed service error.
    *
    * @param service
    *            Confirmed service error.
    * @param type
    *            Service error.
    * @param code
    *            code
    * @return
    */
    void GenerateConfirmedServiceError(
        DLMS_CONFIRMED_SERVICE_ERROR service,
        DLMS_SERVICE_ERROR type,
        unsigned char code, CGXByteBuffer& data);

    /**
    * Handle received command.
    */
    int HandleCommand(
        DLMS_COMMAND cmd,
        CGXByteBuffer& data,
        CGXServerReply& sr,
        unsigned char cipheredCommand);

    /**
    * Parse AARQ request that client send and returns AARE request.
    *
    * @return Reply to the client.
    */
    int HandleAarqRequest(
        CGXByteBuffer& data,
        CGXDLMSConnectionEventArgs& connectionInfo);

    /**
    * Count how many rows can fit to one PDU.
    *
    * @param pg
    *            Read profile generic.
    * @return Rows to fit one PDU.
    */
    unsigned short GetRowsToPdu(
        CGXDLMSProfileGeneric* pg);

    /**
    * Update short names.
    *
    * @param force
    *            Force update.
    */
    int UpdateShortNames(bool force);

    /**
    * Handles release request.
    *
    * @param data
    *            Received data.
    * @param connectionInfo
    *            Connection info.
    */
    int HandleReleaseRequest(
        CGXByteBuffer& data);

    int AddData(
        CGXDLMSObject* obj,
        unsigned char index,
        CGXByteBuffer& buff);

    /**
    * Handles GBT.
    *
    * @param data
    *            Received data.
    * @param connectionInfo
    *            Connection info.
    */
    int HandleGeneralBlockTransfer(
        CGXServerReply& sr,
        CGXByteBuffer& data,
        unsigned char cipheredCommand);

protected:
    /**
     * Server Settings.
     */
    CGXDLMSSettings m_Settings;

    /**
     * @param value
     *            Cipher interface that is used to cipher PDU.
     */
    void SetCipher(CGXCipher* value);

    /**
    * @return Get settings.
    */
    CGXDLMSSettings& GetSettings();

    /**
        * Check is data sent to this server.
        *
        * @param serverAddress
        *            Server address.
        * @param clientAddress
        *            Client address.
        * @return True, if data is sent to this server.
        */
    virtual bool IsTarget(
        unsigned long int serverAddress,
        unsigned long clientAddress) = 0;

    /**
     * Check whether the authentication and password are correct.
     *
     * @param authentication
     *            Authentication level.
     * @param password
     *            Password.
     * @return Source diagnostic.
     */
    virtual DLMS_SOURCE_DIAGNOSTIC ValidateAuthentication(
        DLMS_AUTHENTICATION authentication,
        CGXByteBuffer& password) = 0;

    /**
     * Find object.
     *
     * @param objectType
     *            Object type.
     * @param sn
     *            Short Name. In Logical name referencing this is not used.
     * @param ln
     *            Logical Name. In Short Name referencing this is not used.
     * @return Found object or NULL if object is not found.
     */
    virtual CGXDLMSObject* FindObject(
        DLMS_OBJECT_TYPE objectType,
        int sn,
        std::string& ln) = 0;

    /**
     * Read selected item(s).
     *
     * @param args
     *            Handled read requests.
     */
    virtual void PreRead(
        std::vector<CGXDLMSValueEventArg*>& args) = 0;

    /**
     * Write selected item(s).
     *
     * @param args
     *            Handled write requests.
     */
    virtual void PreWrite(
        std::vector<CGXDLMSValueEventArg*>& args) = 0;

    /**
     * Accepted connection is made for the server. All initialization is done
     * here.
     */
    virtual void Connected(CGXDLMSConnectionEventArgs& connectionInfo) = 0;

    /**
     * Client has try to made invalid connection. Password is incorrect.
     *
     * @param connectionInfo
     *            Connection information.
     */
    virtual void InvalidConnection(CGXDLMSConnectionEventArgs& connectionInfo) = 0;

    /**
     * Server has close the connection. All clean up is made here.
     */
    virtual void Disconnected(CGXDLMSConnectionEventArgs& connectionInfo) = 0;

    /**
    * Get attribute access mode.
    *
    * @param arg
    *            Value event argument.
    * @return Access mode.
    * @throws Exception
    *             Server handler occurred exceptions.
    */
    virtual DLMS_ACCESS_MODE GetAttributeAccess(CGXDLMSValueEventArg* arg) = 0;

    /**
    * Get method access mode.
    *
    * @param arg
    *            Value event argument.
    * @return Method access mode.
    * @throws Exception
    *             Server handler occurred exceptions.
    */
    virtual DLMS_METHOD_ACCESS_MODE GetMethodAccess(CGXDLMSValueEventArg* arg) = 0;

    /**
     * Action is occurred.
     *
     * @param args
     *            Handled action requests.
     */
    virtual void PreAction(
        std::vector<CGXDLMSValueEventArg*>& args) = 0;

    /**
    * Read selected item(s).
    *
    * @param args
    *            Handled read requests.
    */
    virtual void PostRead(
        std::vector<CGXDLMSValueEventArg*>& args) = 0;

    /**
    * Write selected item(s).
    *
    * @param args
    *            Handled write requests.
    */
    virtual void PostWrite(
        std::vector<CGXDLMSValueEventArg*>& args) = 0;

    /**
    * Action is occurred.
    *
    * @param args
    *            Handled action requests.
    */
    virtual void PostAction(
        std::vector<CGXDLMSValueEventArg*>& args) = 0;

    /**
    * Get selected value(s). This is called when example profile generic
    * request current value.
    *
    * @param args
    *            Value event arguments.
    */
    virtual void PreGet(
        std::vector<CGXDLMSValueEventArg*>& args) = 0;

    /**
    * Get selected value(s). This is called when example profile generic
    * request current value.
    *
    * @param args
    *            Value event arguments.
    */
    virtual void PostGet(
        std::vector<CGXDLMSValueEventArg*>& args) = 0;

    /**
    * Update short names.
    */
    int UpdateShortNames();

    int HandleRequest(CGXServerReply& sr);
public:
    /**
    * @return HDLC settings.
    */
    CGXDLMSIecHdlcSetup* GetHdlc();

    /**
    * @param value
    *            HDLC settings.
    */
    void SetHdlc(CGXDLMSIecHdlcSetup* value);

    /**
    * @return Wrapper settings.
    */
    CGXDLMSTcpUdpSetup* GetWrapper();

    /**
    * @param value
    *            Wrapper settings.
    */
    void SetWrapper(CGXDLMSTcpUdpSetup* value);

    /**
     * @return Server to client challenge.
     */
    CGXByteBuffer& GetStoCChallenge();

    /**
     * Server to Client custom challenge. This is for debugging purposes. Reset
     * custom challenge settings StoCChallenge to NULL.
     *
     * @param value
     *            Server to Client challenge.
     */
    void SetStoCChallenge(
        CGXByteBuffer& value);

    /**
     * @return Interface type.
     */
    DLMS_INTERFACE_TYPE GetInterfaceType();

    /**
     * Set starting packet index. Default is One based, but some meters use Zero
     * based value. Usually this is not used.
     *
     * @param value
     *            Zero based starting index.
     */
    void SetStartingPacketIndex(int value);

    /**
     * @return Invoke ID.
     */
    int GetInvokeID();

    /**
     * Constructor.
     *
     * @param logicalNameReferencing
     *            Is logical name referencing used.
     * @param type
     *            Interface type.
     */
    CGXDLMSServer(
        bool logicalNameReferencing,
        DLMS_INTERFACE_TYPE type);

    /**
    * Constructor.
    *
    * @param ln
    *           Logical name settings..
    * @param hdlc
    *            HDLC settings.
    */
    CGXDLMSServer(
        CGXDLMSAssociationLogicalName* ln, CGXDLMSIecHdlcSetup* hdlc);

    /**
    * Constructor.
    *
    * @param ln
    *           Logical name settings..
    * @param wrapper
    *            WRAPPER settings.
    */
    CGXDLMSServer(
        CGXDLMSAssociationLogicalName* ln, CGXDLMSTcpUdpSetup* wrapper);

    /**
    * Constructor.
    *
    * @param sn
    *           Short name settings..
    * @param hdlc
    *            HDLC settings.
    */
    CGXDLMSServer(
        CGXDLMSAssociationShortName* sn, CGXDLMSIecHdlcSetup* hdlc);

    /**
    * Constructor.
    *
    * @param sn
    *           Short name settings..
    * @param wrapper
    *            WRAPPER settings.
    */
    CGXDLMSServer(
        CGXDLMSAssociationShortName* sn, CGXDLMSTcpUdpSetup* wrapper);


    /**
    * Destructor.
    */
    ~CGXDLMSServer();

    //Server is using push client address when sending push messages. Client address is used if PushAddress is zero.
    unsigned long GetPushClientAddress();
    void SetPushClientAddress(unsigned long value);


    /**
     * @return List of objects that meter supports.
     */
    CGXDLMSObjectCollection& GetItems();

    /**
     * @return Information from the connection size that server can handle.
     */
    CGXDLMSLimits GetLimits();

    /**
     * Retrieves the maximum size of received PDU. PDU size tells maximum size
     * of PDU packet. Value can be from 0 to 0xFFFF. By default the value is
     * 0xFFFF.
     *
     * @return Maximum size of received PDU.
     */
    unsigned short GetMaxReceivePDUSize();

    /**
     * @param value
     *            Maximum size of received PDU.
     */
    void SetMaxReceivePDUSize(
        unsigned short value);

    /**
     * Determines, whether Logical, or Short name, referencing is used.
     * Referencing depends on the device to communicate with. Normally, a device
     * supports only either Logical or Short name referencing. The referencing
     * is defined by the device manufacturer. If the referencing is wrong, the
     * SNMR message will fail.
     *
     * @see #getMaxReceivePDUSize
     * @return Is logical name referencing used.
     */
    bool GetUseLogicalNameReferencing();

    /**
     * @param value
     *            Is Logical Name referencing used.
     */
    void SetUseLogicalNameReferencing(
        bool value);

    /**
     * Initialize server. This must call after server objects are set.
     */
    int Initialize();

    /**
     * Reset after connection is closed.
     */
    void Reset();

    /**
     * Handles client request.
     *
     * @param data
     *            Received data from the client.
     * @return Response to the request. Response is NULL if request packet is
     *         not complete.
     */
    int HandleRequest(
        CGXByteBuffer& data,
        CGXByteBuffer& reply);

    /**
    * Handles client request.
    *
    * @param data
    *            Received data from the client.
    * @return Response to the request. Response is NULL if request packet is
    *         not complete.
    */
    int HandleRequest(
        CGXDLMSConnectionEventArgs& connectionInfo,
        CGXByteBuffer& data,
        CGXByteBuffer& reply);

    /**
     * Handles client request.
     *
     * @param data
     *            Received data from the client.
     * @return Response to the request. Response is NULL if request packet is
     *         not complete.
     */
    int HandleRequest(
        unsigned char* data,
        unsigned short size,
        CGXByteBuffer& reply);

    /**
     * Handles client request.
     *
     * @param data
     *            Received data from the client.
     * @return Response to the request. Response is NULL if request packet is
     *         not complete.
     */
    int HandleRequest(
        CGXDLMSConnectionEventArgs& connectionInfo,
        unsigned char* data,
        unsigned short size,
        CGXByteBuffer& reply);


    /**
    * Server will tell what functionality is available for the client.
    * @return Available functionality.
    */
    DLMS_CONFORMANCE GetConformance();

    /**
    * Server will tell what functionality is available for the client.
    *
    * @param value
    *            Available functionality.
    */
    void SetConformance(DLMS_CONFORMANCE value);

    int GenerateDataNotificationMessages(
        struct tm* time,
        CGXByteBuffer& data,
        std::vector<CGXByteBuffer>& reply);

    int GenerateDataNotificationMessages(
        struct tm* date,
        std::vector<std::pair<CGXDLMSObject*, unsigned char> >& objects,
        std::vector<CGXByteBuffer>& reply);

    int GeneratePushSetupMessages(
        struct tm* date,
        CGXDLMSPushSetup* push,
        std::vector<CGXByteBuffer>& reply);
};
#endif //GXDLMSSERVER_H
