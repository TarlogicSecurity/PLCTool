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
#ifndef GXDLMSLNPARAMETERS_H
#define GXDLMSLNPARAMETERS_H

#include "GXBytebuffer.h"
#include "GXDLMSSettings.h"

/**
 * LN Parameters
 */
class CGXDLMSLNParameters
{
private:

    /**
     * DLMS settings.
     */
    CGXDLMSSettings* m_Settings;
    /**
     * DLMS command.
     */
    DLMS_COMMAND m_Command;
    /*
    * DLMS ciphered Command.
    */
    unsigned char m_CipheredCommand;

    /**
     * Request type.
     */
    int m_RequestType;
    /**
     * Attribute descriptor.
     */
    CGXByteBuffer* m_AttributeDescriptor;
    /**
     * Data.
     */
    CGXByteBuffer* m_Data;
    /**
     * Send date and time. This is used in Data notification messages.
     */
    struct tm* m_Time;
    /**
     * Reply status.
     */
    int m_Status;
    /**
     * Are there more data to send or more data to receive.
     */
    bool m_MultipleBlocks;
    /**
     * Is this last block in send.
     */
    bool m_LastBlock;
    /**
     * Block index.
     */
    unsigned long m_BlockIndex;
    /**
    * Invoke ID.
    */
    unsigned long m_InvokeId;

    /*
     * GBT block number.
    */
    int m_BlockNumber;
    /*
     * GBT block number ACK.
     */
    int m_BlockNumberAck;
    /*
     * Is GBT streaming in use.
     */
    bool m_Streaming;
    /*
     * GBT Window size. This is for internal use.
     */
    unsigned char m_WindowSize;

public:
    /**
     * Constructor.
     *
     * @param settings
     *            DLMS settings.
     * @param command
     *            Command.
     * @param commandType
     *            DLMS_COMMAND type.
     * @param attributeDescriptor
     *            Attribute descriptor.
     * @param data
     *            Data.
     */
    CGXDLMSLNParameters(CGXDLMSSettings* settings,
        unsigned long invokeId,
        DLMS_COMMAND command,
        int commandType,
        CGXByteBuffer* attributeDescriptor,
        CGXByteBuffer* data,
        int status,
        unsigned char cipheredCommand);

    /**
     * Returns DLMS settings.
     */
    CGXDLMSSettings* GetSettings();

    /**
     * Returns DLMS command.
     */
    DLMS_COMMAND GetCommand();

    /**
     * value DLMS command.
     */
    void SetCommand(DLMS_COMMAND value);

    /**
     * Returns Request type.
     */
    int GetRequestType();

    /**
     * @param value
     *            the requestType to set
     */
    void SetRequestType(int value);

    /**
     * Returns the attributeDescriptor
     */
    CGXByteBuffer* GetAttributeDescriptor();

    /**
     * Returns the data
     */
    CGXByteBuffer* GetData();

    /**
     * Returns the time
     */
    struct tm* GetTime();

    /**
     * @param value
     *            the time to set
     */
    void SetTime(struct tm* value);

    /**
     * Returns Status.
     */
    int GetStatus();

    /**
     * @param value
     *            Status to set
     */
    void SetStatus(int value);

    /**
     * Returns the multipleBlocks
     */
    bool IsMultipleBlocks();

    /**
     * @param value
     *            the multipleBlocks to set
     */
    void SetMultipleBlocks(bool value);

    /**
     * Returns the lastBlock
     */
    bool IsLastBlock();

    /**
     * @param value
     *            Block index.
     */
    void SetLastBlock(bool value);

    /**
     * Returns Block index.
     */
    unsigned long GetBlockIndex();

    /**
     * @param value
     *            the blockIndex to set
     */
    void SetBlockIndex(unsigned long value);

    /**
    * Returns Get Invoke ID and priority. This can be used for Priority
    *         Management.
    */
    unsigned long GetInvokeId();

    /**
    * @param value
    *            Set Invoke ID and priority. This can be used for Priority
    *            Management.
    */
    void SetInvokeId(unsigned long value);

    /*
    * GBT block number.
    */
    int GetBlockNumber()
    {
        return m_BlockNumber;
    }

    /*
     * GBT block number.
     */
    void SetBlockNumber(int value)
    {
        m_BlockNumber = value;
    }

    /**
     * GBT block number ACK.
     */
    int GetBlockNumberAck()
    {
        return m_BlockNumberAck;
    }

    /**
     * @param value
     *            GBT block number ACK.
     */
    void SetBlockNumberAck(int value)
    {
        m_BlockNumberAck = value;
    }

    /**
     * Returns Is GBT streaming in use.
     */
    bool GetStreaming()
    {
        return m_Streaming;
    }

    /**
     * Is GBT streaming in use.
     */
    void SetStreaming(bool value)
    {
        m_Streaming = value;
    }

    /**
     * GBT Window size. This is for internal use.
     */
    unsigned char GetWindowSize() {
        return m_WindowSize;
    }

    /**
     * GBT Window size. This is for internal use.
     */
    void SetWindowSize(unsigned char value)
    {
        m_WindowSize = value;
    }


    /**
     * Returns ciphered DLMS Command.
     */
    unsigned char GetCipheredCommand()
    {
        return m_CipheredCommand;
    }
};

#endif //GXDLMSLNPARAMETERS_H
