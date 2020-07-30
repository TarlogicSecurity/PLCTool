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

#include "../include/GXDLMSSettings.h"

CGXDLMSSettings::CGXDLMSSettings(bool isServer)
{
    m_UseCustomChallenge = false;
    m_BlockIndex = 1;
    m_Connected = DLMS_CONNECTION_STATE_NONE;
    m_DlmsVersionNumber = DLMS_VERSION;
    m_Server = isServer;
    ResetFrameSequence();
    m_InvokeID = 1;
    m_LongInvokeID = 1;
    m_Priority = DLMS_PRIORITY_HIGH;
    m_ServiceClass = DLMS_SERVICE_CLASS_UN_CONFIRMED;
    m_ClientAddress = 0;
    m_ServerAddress = 0;
    m_PushClientAddress = 0;
    m_InterfaceType = DLMS_INTERFACE_TYPE_HDLC;
    m_Authentication = DLMS_AUTHENTICATION_NONE;
    m_MaxServerPDUSize = m_MaxReceivePDUSize = 0xFFFF;
    m_Cipher = NULL;
    m_SourceSystemTitle.Clear();
    m_Index = 0;
    m_Count = 0;
    m_ProposedConformance = m_NegotiatedConformance = (DLMS_CONFORMANCE)0;
    m_ProtocolVersion = NULL;
    m_QualityOfService = 0;
    m_UserId = 0;
    m_UseUtc2NormalTime = false;
    m_BlockNumberAck = 0;
    m_WindowSize = 1;
    m_AutoIncreaseInvokeID = false;
}

//Destructor.
CGXDLMSSettings::~CGXDLMSSettings()
{
    m_Objects.Free();
    m_AllocatedObjects.Free();
}

CGXByteBuffer& CGXDLMSSettings::GetCtoSChallenge()
{
    return m_CtoSChallenge;
}


void CGXDLMSSettings::SetCtoSChallenge(CGXByteBuffer& value)
{
    m_CtoSChallenge = value;
}

CGXByteBuffer& CGXDLMSSettings::GetStoCChallenge()
{
    return m_StoCChallenge;
}

void CGXDLMSSettings::SetStoCChallenge(CGXByteBuffer& value)
{
    m_StoCChallenge = value;
}

DLMS_AUTHENTICATION CGXDLMSSettings::GetAuthentication()
{
    return m_Authentication;
}

void CGXDLMSSettings::SetAuthentication(DLMS_AUTHENTICATION value)
{
    m_Authentication = value;
}

CGXByteBuffer& CGXDLMSSettings::GetPassword()
{
    return m_Password;
}

void CGXDLMSSettings::SetPassword(CGXByteBuffer& value)
{
    m_Password = value;
}

unsigned char CGXDLMSSettings::GetDlmsVersionNumber()
{
    return m_DlmsVersionNumber;
}

void CGXDLMSSettings::SetDlmsVersionNumber(unsigned char value)
{
    m_DlmsVersionNumber = value;
}

void CGXDLMSSettings::ResetFrameSequence()
{
    if (m_Server)
    {
        m_SenderFrame = SERVER_START_SENDER_FRAME_SEQUENCE;
        m_ReceiverFrame = SERVER_START_RECEIVER_FRAME_SEQUENCE;
    }
    else
    {
        m_SenderFrame = CLIENT_START_SENDER_FRAME_SEQUENCE;
        m_ReceiverFrame = CLIENT_START_RCEIVER_FRAME_SEQUENCE;
    }
}

// Increase receiver sequence.
//
// @param value
//            Frame value.
// Increased receiver frame sequence.
static unsigned char IncreaseReceiverSequence(unsigned char value)
{
    return ((value + 0x20) | 0x10 | (value & 0xE));
}

// Increase sender sequence.
//
// @param value
//            Frame value.
// Increased sender frame sequence.
static unsigned char IncreaseSendSequence(unsigned char value)
{
    return (unsigned char)((value & 0xF0) | ((value + 0x2) & 0xE));
}

bool CGXDLMSSettings::CheckFrame(unsigned char frame)
{
    //If notify
    if (frame == 0x13)
    {
        return true;
    }
    // If U frame.
    if ((frame & 0x3) == 3)
    {
        if (frame == 0x73 || frame == 0x93)
        {
            ResetFrameSequence();
        }
        return true;
    }
    // If S -frame
    if ((frame & 0x3) == 1)
    {
        m_ReceiverFrame = IncreaseReceiverSequence(m_ReceiverFrame);
        return true;
    }

    //Handle I-frame.
    unsigned char expected;
    if ((m_SenderFrame & 0x1) == 0)
    {
        expected = IncreaseReceiverSequence(IncreaseSendSequence(m_ReceiverFrame));
        if (frame == expected)
        {
            m_ReceiverFrame = frame;
            return true;
        }
    }
    //If answer for RR.
    else
    {
        expected = IncreaseSendSequence(m_ReceiverFrame);
        if (frame == expected)
        {
            m_ReceiverFrame = frame;
            return true;
        }
    }
    return false;
}

unsigned char CGXDLMSSettings::GetNextSend(unsigned char first)
{
    if (first)
    {
        m_SenderFrame = IncreaseReceiverSequence(IncreaseSendSequence(m_SenderFrame));
    }
    else
    {
        m_SenderFrame = IncreaseSendSequence(m_SenderFrame);
    }
    return m_SenderFrame;
}

unsigned char CGXDLMSSettings::GetReceiverReady()
{
    m_SenderFrame = IncreaseReceiverSequence((unsigned char)(m_SenderFrame | 1));
    return (unsigned char)(m_SenderFrame & 0xF1);
}

unsigned char CGXDLMSSettings::GetKeepAlive()
{
    m_SenderFrame = (unsigned char)(m_SenderFrame | 1);
    return (unsigned char)(m_SenderFrame & 0xF1);
}

unsigned long CGXDLMSSettings::GetBlockIndex()
{
    return m_BlockIndex;
}

void CGXDLMSSettings::SetBlockIndex(unsigned long value)
{
    m_BlockIndex = value;
}

void CGXDLMSSettings::ResetBlockIndex()
{
    m_BlockIndex = 1;
    m_BlockNumberAck = 0;
}

void CGXDLMSSettings::IncreaseBlockIndex()
{
    m_BlockIndex += 1;
}

bool CGXDLMSSettings::IsServer()
{
    return m_Server;
}

CGXDLMSLimits& CGXDLMSSettings::GetLimits()
{
    return m_Limits;
}

DLMS_INTERFACE_TYPE CGXDLMSSettings::GetInterfaceType()
{
    return m_InterfaceType;
}

void CGXDLMSSettings::SetInterfaceType(DLMS_INTERFACE_TYPE value)
{
    m_InterfaceType = value;
}

unsigned long CGXDLMSSettings::GetClientAddress()
{
    return m_ClientAddress;
}

void CGXDLMSSettings::SetClientAddress(unsigned long value)
{
    m_ClientAddress = value;
}

unsigned long CGXDLMSSettings::GetPushClientAddress()
{
    return m_PushClientAddress;
}

void CGXDLMSSettings::SetPushClientAddress(unsigned long value)
{
    m_PushClientAddress = value;
}

unsigned long CGXDLMSSettings::GetServerAddress()
{
    return m_ServerAddress;
}

// Server address.
void CGXDLMSSettings::SetServerAddress(unsigned long value)
{
    m_ServerAddress = value;
}

// DLMS version number.
unsigned char CGXDLMSSettings::GetDLMSVersion()
{
    return m_DlmsVersionNumber;
}

// DLMS version number.
void CGXDLMSSettings::SetDLMSVersion(unsigned char value)
{
    m_DlmsVersionNumber = value;
}

// Maximum PDU size.
unsigned short CGXDLMSSettings::GetMaxPduSize()
{
    return m_MaxReceivePDUSize;
}

int CGXDLMSSettings::SetMaxReceivePDUSize(unsigned short value)
{
    if (value < 64 && !m_Server)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    m_MaxReceivePDUSize = value;
    return 0;
}

// Maximum server PDU size.
unsigned short CGXDLMSSettings::GetMaxServerPDUSize()
{
    return m_MaxServerPDUSize;
}

int CGXDLMSSettings::SetMaxServerPDUSize(unsigned short value)
{
    if (value < 64)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    m_MaxServerPDUSize = value;
    return 0;
}


bool CGXDLMSSettings::GetUseLogicalNameReferencing()
{
    return m_UseLogicalNameReferencing;
}

void CGXDLMSSettings::SetUseLogicalNameReferencing(bool value)
{
    m_UseLogicalNameReferencing = value;
}

DLMS_PRIORITY CGXDLMSSettings::GetPriority()
{
    return m_Priority;
}

void CGXDLMSSettings::SetPriority(DLMS_PRIORITY value)
{
    m_Priority = value;
}

DLMS_SERVICE_CLASS CGXDLMSSettings::GetServiceClass()
{
    return m_ServiceClass;
}

void CGXDLMSSettings::SetServiceClass(DLMS_SERVICE_CLASS value)
{
    m_ServiceClass = value;
}

unsigned char CGXDLMSSettings::GetInvokeID()
{
    return m_InvokeID;
}

void CGXDLMSSettings::SetInvokeID(unsigned char value)
{
    m_InvokeID = value;
}

void CGXDLMSSettings::UpdateInvokeId(unsigned char value) {
    if ((value & 0x80) != 0) {
        m_Priority = DLMS_PRIORITY_HIGH;
    }
    else {
        m_Priority = DLMS_PRIORITY_NORMAL;
    }
    if ((value & 0x40) != 0) {
        m_ServiceClass = DLMS_SERVICE_CLASS_CONFIRMED;
    }
    else
    {
        m_ServiceClass = DLMS_SERVICE_CLASS_UN_CONFIRMED;
    }
    m_InvokeID = (unsigned char)(value & 0xF);
}


unsigned long CGXDLMSSettings::GetLongInvokeID()
{
    return m_LongInvokeID;
}
int CGXDLMSSettings::SetLongInvokeID(unsigned long value)
{
    if (value > 0xFFFFFF)
    {
        //Invalid InvokeID.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    m_LongInvokeID = value;
    return 0;
}
CGXDLMSObjectCollection& CGXDLMSSettings::GetObjects()
{
    return m_Objects;
}

bool CGXDLMSSettings::GetUseCustomChallenge()
{
    return m_UseCustomChallenge;
}

void CGXDLMSSettings::SetUseCustomChallenge(bool value)
{
    m_UseCustomChallenge = value;
}

DLMS_CONNECTION_STATE CGXDLMSSettings::GetConnected()
{
    return m_Connected;
}

void CGXDLMSSettings::SetConnected(DLMS_CONNECTION_STATE value)
{
    m_Connected = value;
}

CGXCipher* CGXDLMSSettings::GetCipher()
{
    return m_Cipher;
}

void CGXDLMSSettings::SetCipher(CGXCipher* value)
{
    m_Cipher = value;
}

/**
     * @return Source system title.
     */
CGXByteBuffer& CGXDLMSSettings::GetSourceSystemTitle()
{
    return m_SourceSystemTitle;
}

/**
 * @param value
 *            Source system title.
 */
int  CGXDLMSSettings::SetSourceSystemTitle(CGXByteBuffer& value)
{
    if (value.GetSize() != 8)
    {
        //Invalid client system title.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    m_SourceSystemTitle = value;
    return DLMS_ERROR_CODE_OK;
}

/**
 * @return Key Encrypting Key, also known as Master key.
 */
CGXByteBuffer& CGXDLMSSettings::GetKek()
{
    return m_Kek;
}

/**
 * @param value
 *            Key Encrypting Key, also known as Master key.
 */
void CGXDLMSSettings::SetKek(CGXByteBuffer& value)
{
    m_Kek = value;
}

/**
 * @return Long data count.
 */
unsigned short CGXDLMSSettings::GetCount()
{
    return m_Count;
}

/**
 * @param count
 *            Long data count.
 */
void CGXDLMSSettings::SetCount(unsigned short value)
{
    m_Count = value;
}

/**
 * @return Long data index.
 */
unsigned short CGXDLMSSettings::GetIndex()
{
    return m_Index;
}

/**
 * @param index
 *            Long data index
 */
void CGXDLMSSettings::SetIndex(unsigned short value)
{
    m_Index = value;
}

DLMS_CONFORMANCE CGXDLMSSettings::GetNegotiatedConformance()
{
    return m_NegotiatedConformance;
}

void CGXDLMSSettings::SetNegotiatedConformance(DLMS_CONFORMANCE value)
{
    m_NegotiatedConformance = value;
}

DLMS_CONFORMANCE CGXDLMSSettings::GetProposedConformance()
{
    return m_ProposedConformance;
}

void CGXDLMSSettings::SetProposedConformance(DLMS_CONFORMANCE value)
{
    m_ProposedConformance = value;
}

char* CGXDLMSSettings::GetProtocolVersion()
{
    return m_ProtocolVersion;
}

void CGXDLMSSettings::SetProtocolVersion(const char* value)
{
    if (m_ProtocolVersion != NULL)
    {
        delete m_ProtocolVersion;
        m_ProtocolVersion = NULL;
    }
    if (value != NULL)
    {
        short len = (short) strlen(value);
        m_ProtocolVersion = new char[len + 1];
        memcpy(m_ProtocolVersion, value, len);
        m_ProtocolVersion[len] = 0;
    }
}

bool CGXDLMSSettings::GetUseUtc2NormalTime()
{
    return m_UseUtc2NormalTime;
}

void CGXDLMSSettings::SetUseUtc2NormalTime(bool value)
{
    m_UseUtc2NormalTime = value;
}

unsigned char CGXDLMSSettings::GetUserID()
{
    return m_UserId;
}

void CGXDLMSSettings::SetUserID(unsigned char value)
{
    m_UserId = value;
}

unsigned char CGXDLMSSettings::GetQualityOfService()
{
    return m_QualityOfService;
}

void CGXDLMSSettings::SetQualityOfService(unsigned char value)
{
    m_QualityOfService = value;
}

bool CGXDLMSSettings::GetAutoIncreaseInvokeID()
{
    return m_AutoIncreaseInvokeID;
}

void CGXDLMSSettings::SetAutoIncreaseInvokeID(bool value)
{
    m_AutoIncreaseInvokeID = value;
}

void CGXDLMSSettings::AddAllocateObject(CGXDLMSObject* pObj)
{
    m_AllocatedObjects.push_back(pObj);
}