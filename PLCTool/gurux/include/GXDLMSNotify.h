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

#ifndef GXDLMSNOTIFY_H
#define GXDLMSNOTIFY_H

#include <vector>
#include "GXReplyData.h"
#include "GXDLMSSettings.h"
#include "GXDLMSLimits.h"
#include "GXDLMSPushSetup.h"
#include "GXDLMS.h"

#include "GXDLMSClient.h"
#include "GXDLMSObjectFactory.h"

class CGXDLMSNotify
{
private:

protected:
    /**
     * Server settings.
     */
    CGXDLMSSettings m_Settings;

    /**
     * @param value
     *            Cipher interface that is used to cipher PDU.
     */
    void SetCipher(CGXCipher* value);

    /**
    * @return Get m_Settings.
    */
    CGXDLMSSettings& GetSettings();

public:
    /**
         * Constructor.
         *
         * @param useLogicalNameReferencing
         *            Is Logical Name referencing used.
         * @param clientAddress
         *            Server address.
         * @param serverAddress
         *            Client address.
         * @param interfaceType
         *            Object type.
         */
    CGXDLMSNotify(bool useLogicalNameReferencing,
        int clientAddress,
        int serverAddress,
        DLMS_INTERFACE_TYPE interfaceType);

    /**
     * @return Get list of meter's objects.
     */
    CGXDLMSObjectCollection& GetObjects();

    /**
     * @return Information from the connection size that server can handle.
     */
    CGXDLMSLimits& GetLimits();

    /**
     * Retrieves the maximum size of received PDU. PDU size tells maximum size
     * of PDU packet. Value can be from 0 to 0xFFFF. By default the value is
     * 0xFFFF.
     *
     * @see GXDLMSClient#getClientAddress
     * @see GXDLMSClient#getServerAddress
     * @see GXDLMSClient#getDLMSVersion
     * @see GXDLMSClient#getUseLogicalNameReferencing
     * @return Maximum size of received PDU.
     */
    int GetMaxPduSize();

    /**
     * @param value
     *            Maximum size of received PDU.
     */
    void SetMaxReceivePDUSize(int value);

    /**
     * Determines, whether Logical, or Short name, referencing is used.
     * Referencing depends on the device to communicate with. Normally, a device
     * supports only either Logical or Short name referencing. The referencing
     * is defined by the device manufacturer. If the referencing is wrong, the
     * SNMR message will fail.
     *
     * @return Is Logical Name referencing used.
     */
    bool GetUseLogicalNameReferencing();

    /**
     * @param value
     *            Is Logical Name referencing used.
     */
    void SetUseLogicalNameReferencing(bool value);

    /**
     * @return Used Priority.
     */
    DLMS_PRIORITY GetPriority();

    /**
     * @param value
     *            Used Priority.
     */
    void SetPriority(DLMS_PRIORITY value);

    /**
     * @return Used service class.
     */
    DLMS_SERVICE_CLASS GetServiceClass();

    /**
     * @param value
     *            Used service class.
     */
    void SetServiceClass(DLMS_SERVICE_CLASS value);

    /**
     * @return Invoke ID.
     */
    unsigned char GetInvokeID();

    /**
     * @param value
     *            Invoke ID.
     */
    void SetInvokeID(unsigned char value);

    /**
     * Removes the HDLC frame from the packet, and returns COSEM data only.
     *
     * @param reply
     *            The received data from the device.
     * @param data
     *            Information from the received data.
     * @return Is frame complete.
     */
    int GetData(CGXByteBuffer& reply, CGXReplyData& data);

    /**
     * Add value of COSEM object to byte buffer. AddData method can be used with
     * GetDataNotificationMessage -method. DLMS specification do not specify the
     * structure of Data-Notification body. So each manufacture can sent
     * different data.
     *
     * @param obj
     *            COSEM object.
     * @param index
     *            Attribute index.
     * @param buff
     *            Byte buffer.
     */
    int AddData(
        CGXDLMSObject* obj,
        unsigned char index,
        CGXByteBuffer& buff);

    /**
     * Generates data notification message.
     *
     * @param date
     *            Date time. Set to null or Date(0) if not used
     * @param data
     *            Notification body.
     * @return Generated data notification message(s).
     */
    int GenerateDataNotificationMessages(
        struct tm* date,
        CGXByteBuffer& data,
        std::vector<CGXByteBuffer>& reply);

    /**
     * Generates data notification message.
     *
     * @param date
     *            Date time. Set To Min or Max if not added.
     * @param objects
     *            List of objects and attribute indexes to notify.
     * @return Generated data notification message(s).
     */
    int GenerateDataNotificationMessages(
        struct tm* date,
        std::vector<std::pair<CGXDLMSObject*, unsigned char> >& objects,
        std::vector<CGXByteBuffer>& reply);

    /**
     * Generates push setup message.
     *
     * @param date
     *            Date time. Set to null or Date(0) if not used.
     * @param push
     *            Target Push object.
     * @return Generated data notification message(s).
     */
    int GeneratePushSetupMessages(
        struct tm* date,
        CGXDLMSPushSetup* push,
        std::vector<CGXByteBuffer>& reply);

    /**
    * Returns collection of push objects. If this method is used Push object
    * must be set for first object on push object list.
    *
    * @param data
    *            Received value.
    * @return Array of objects and called indexes.
    */
    int ParsePush(
        std::vector<CGXDLMSVariant>& data,
        std::vector<std::pair<CGXDLMSObject*, unsigned char> >& items);

};
#endif //GXDLMSNOTIFY_H
