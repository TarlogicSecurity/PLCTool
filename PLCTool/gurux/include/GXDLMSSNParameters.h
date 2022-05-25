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
#ifndef GXDLMSSNPARAMETERS_H
#define GXDLMSSNPARAMETERS_H

#include "GXBytebuffer.h"
#include "GXDLMSSettings.h"

/**
 * SN Parameters
 */
class CGXDLMSSNParameters
{
private:

    /**
     * DLMS settings.
     */
    CGXDLMSSettings *m_Settings;
    /**
     * DLMS command.
     */
    DLMS_COMMAND m_Command;
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
     * Item Count.
     */
    int m_Count;

    /**
     * Are there more data to send or more data to receive.
     */
    bool m_MultipleBlocks;

    /**
     * Block index.
     */
    unsigned short m_BlockIndex;

public:

    /**
     * Constructor.
     *
     * @param settings
     *            DLMS settings.
     * @param command
     *            Command.
     * @param commandType
     *            command type.
     * @param attributeDescriptor
     * @param data
     *            Attribute descriptor
     */
    CGXDLMSSNParameters(CGXDLMSSettings* settings,
                        DLMS_COMMAND command,
                        int count,
                        int commandType,
                        CGXByteBuffer* attributeDescriptor,
                        CGXByteBuffer* data);

    /**
     * @return DLMS settings.
     */
    CGXDLMSSettings* GetSettings();

    /**
     * @return DLMS command.
     */
    DLMS_COMMAND GetCommand();

    /**
     * @return Request type.
     */
    int GetRequestType();

    /**
     * @param value
     *            Request type.
     */
    void SetRequestType(int value);

    /**
     * @return the attributeDescriptor
     */
    CGXByteBuffer* GetAttributeDescriptor();

    /**
     * @return the data
     */
    CGXByteBuffer* GetData();

    /**
     * @return the time
     */
    struct tm* GetTime();

    /**
     * @param value
     *            the time to set
     */
    void SetTime(struct tm* value);

    /**
     * @return the multipleBlocks
     */
    bool IsMultipleBlocks();

    /**
     * @param value
     *            the multipleBlocks to set
     */
    void SetMultipleBlocks(bool value);

    /**
     * @return Block index.
     */
    unsigned short GetBlockIndex();

    /**
     * @param value
     *            Block index.
     */
    void SetBlockIndex(unsigned short value);

    /**
     * @return Item Count.
     */
    int GetCount();
};

#endif //GXDLMSSNPARAMETERS_H
