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

#include "../include/GXDLMSLNParameters.h"

CGXDLMSLNParameters::CGXDLMSLNParameters(
    CGXDLMSSettings* settings,
    unsigned long invokeId,
    DLMS_COMMAND command,
    int commandType,
    CGXByteBuffer* attributeDescriptor,
    CGXByteBuffer* data,
    int status,
    unsigned char cipheredCommand)
{
    m_Settings = settings;
    m_BlockIndex = settings->GetBlockIndex();
    m_BlockNumberAck = settings->GetBlockNumberAck();
    m_Command = command;
    m_CipheredCommand = cipheredCommand;
    m_RequestType = commandType;
    m_AttributeDescriptor = attributeDescriptor;
    m_Data = data;
    m_Time = NULL;
    m_Status = status;
    m_MultipleBlocks = settings->GetCount() != settings->GetIndex();
    m_LastBlock = settings->GetCount() == settings->GetIndex();
    m_InvokeId = invokeId;
    if (settings != NULL) {
        settings->SetCommand(command);
        if (command == DLMS_COMMAND_GET_REQUEST && commandType != DLMS_GET_COMMAND_TYPE_NEXT_DATA_BLOCK)
        {
            settings->SetCommandType(commandType);
        }
    }
    m_BlockNumber = 0;
    m_Streaming = false;
    m_WindowSize = 1;
}

CGXDLMSSettings* CGXDLMSLNParameters::GetSettings()
{
    return m_Settings;
}

DLMS_COMMAND CGXDLMSLNParameters::GetCommand()
{
    return m_Command;
}

void CGXDLMSLNParameters::SetCommand(DLMS_COMMAND value)
{
    m_Command = value;
}

int CGXDLMSLNParameters::GetRequestType()
{
    return m_RequestType;
}

void CGXDLMSLNParameters::SetRequestType(int value)
{
    m_RequestType = value;
}

CGXByteBuffer* CGXDLMSLNParameters::GetAttributeDescriptor()
{
    return m_AttributeDescriptor;
}

CGXByteBuffer* CGXDLMSLNParameters::GetData()
{
    return m_Data;
}

struct tm* CGXDLMSLNParameters::GetTime()
{
    return m_Time;
}

void CGXDLMSLNParameters::SetTime(struct tm* value)
{
    m_Time = value;
}

int CGXDLMSLNParameters::GetStatus()
{
    return m_Status;
}

void CGXDLMSLNParameters::SetStatus(int value)
{
    m_Status = value;
}

bool CGXDLMSLNParameters::IsMultipleBlocks()
{
    return m_MultipleBlocks;
}

void CGXDLMSLNParameters::SetMultipleBlocks(bool value)
{
    m_MultipleBlocks = value;
}

bool CGXDLMSLNParameters::IsLastBlock()
{
    return m_LastBlock;
}

void CGXDLMSLNParameters::SetLastBlock(bool value)
{
    m_LastBlock = value;
}

unsigned long CGXDLMSLNParameters::GetBlockIndex()
{
    return m_BlockIndex;
}

void CGXDLMSLNParameters::SetBlockIndex(unsigned long value)
{
    m_BlockIndex = value;
}

unsigned long CGXDLMSLNParameters::GetInvokeId() {
    return m_InvokeId;
}

void CGXDLMSLNParameters::SetInvokeId(unsigned long value) {
    m_InvokeId = value;
}