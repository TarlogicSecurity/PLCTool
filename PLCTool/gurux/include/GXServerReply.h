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

#ifndef GXSERVERREPLY_H
#define GXSERVERREPLY_H

#include "GXBytebuffer.h"
#include "GXDLMSConnectionEventArgs.h"

class CGXServerReply
{
private:
    /*
    * Connection info.
    */
    CGXDLMSConnectionEventArgs m_ConnectionInfo;

    /*
     * Server received data.
     */
    CGXByteBuffer m_Data;

    /*
     * Server reply message.
     */
    CGXByteBuffer m_Reply;

    /*
     * Message count to send.
     */
    int m_Count;
public:
    /*
    * Constructor.
    */
    CGXServerReply()
    {
        m_Count = 0;
    }

    /*
     * Constructor.
     *
     * @param value
     *            Received data.
     */
    CGXServerReply(CGXByteBuffer& value)
    {
        m_Count = 0;
        m_Data = value;
    }

    /*
     * returns the data
     */
    CGXByteBuffer& GetData()
    {
        return m_Data;
    }

    /*
     * value: The data to set.
     */
    void SetData(CGXByteBuffer& value)
    {
        m_Data = value;
    }

    /*
     * returns The reply message.
     */
    CGXByteBuffer& GetReply()
    {
        return m_Reply;
    }

    /*
     * value: the replyMessages to set
     */
    void SetReply(CGXByteBuffer& value)
    {
        m_Reply = value;
    }

    /*
     * returns Connection info.
     */
    CGXDLMSConnectionEventArgs& GetConnectionInfo()
    {
        return m_ConnectionInfo;
    }

    /*
     * value: Connection info.
     */
    void SetConnectionInfo(CGXDLMSConnectionEventArgs& value)
    {
        m_ConnectionInfo = value;
    }

    /*
     * returns Is GBT streaming in progress.
     */
    bool IsStreaming()
    {
        return GetCount() != 0;
    }

    /*
     * returns Message count to send.
     */
    int GetCount()
    {
        return m_Count;
    }

    /*
     * value: Message count to send.
     */
    void SetCount(int value)
    {
        m_Count = value;
    }
};

#endif //GXSERVERREPLY_H
