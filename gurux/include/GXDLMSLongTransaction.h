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

#ifndef GXDLMSLONGTRANSACTION_H
#define GXDLMSLONGTRANSACTION_H

#include <vector>
#include "enums.h"
#include "GXBytebuffer.h"
#include "GXDLMSValueEventArg.h"
#include "GXDLMSValueEventCollection.h"

class CGXDLMSLongTransaction
{
private:
    /**
    * Executed command.
    */
    DLMS_COMMAND m_Command;

    /**
     * Targets.
     */
    CGXDLMSValueEventCollection m_Targets;

    /**
     * Extra data from PDU.
     */
    CGXByteBuffer m_Data;

public:

    /**
     * Constructor.
     *
     * @param targets
     * @param command
     * @param data
     */
    CGXDLMSLongTransaction(CGXDLMSValueEventCollection& targets,
                           DLMS_COMMAND command, CGXByteBuffer& data)
    {
        m_Targets.insert(m_Targets.end(), targets.begin(), targets.end());
        targets.clear();
        m_Command = command;
        m_Data.Set(&data, data.GetPosition());
    }

    /**
 * Constructor.
 *
 * @param targets
 * @param command
 * @param data
 */
    CGXDLMSLongTransaction(DLMS_COMMAND command, CGXByteBuffer& data)
    {
        m_Command = command;
        m_Data.Set(&data, data.GetPosition());
    }

    /**
     * @return Executed command.
     */
    DLMS_COMMAND GetCommand()
    {
        return m_Command;
    }

    /**
     * @return Targets.
     */
    std::vector<CGXDLMSValueEventArg*>& GetTargets()
    {
        return m_Targets;
    }

    /**
     * @return data.
     */
    CGXByteBuffer& GetData()
    {
        return m_Data;
    }

    /**
     * @param value
     *            New data.
     */
    void SetData(CGXByteBuffer& value)
    {
        m_Data.Clear();
        m_Data.Set(&value, value.GetPosition());
    }
};
#endif //GXDLMSLONGTRANSACTION_H
