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

#include "../include/GXServerReply.h"
#include "../include/GXDLMSServer.h"
#include "../include/GXDLMS.h"
#include "../include/GXDLMSObjectFactory.h"
#include "../include/GXDLMSProfileGeneric.h"
#include "../include/GXDLMSAssociationShortName.h"
#include "../include/GXDLMSAssociationLogicalName.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXAPDU.h"
#include "../include/GXSecure.h"
#include "../include/GXDLMSValueEventCollection.h"
#include "../include/GXDLMSLNCommandHandler.h"
#include "../include/GXDLMSSNCommandHandler.h"

CGXDLMSServer::CGXDLMSServer(bool logicalNameReferencing,
    DLMS_INTERFACE_TYPE type) : m_Transaction(NULL), m_Settings(true)
{
    m_Hdlc = NULL;
    m_Wrapper = NULL;
    m_DataReceived = 0;
    m_Settings.SetUseLogicalNameReferencing(logicalNameReferencing);
    m_Settings.SetInterfaceType(type);
    if (GetUseLogicalNameReferencing())
    {
        SetConformance((DLMS_CONFORMANCE)(DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_ACTION |
            DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_SET_OR_WRITE |
            DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_GET_OR_READ |
            DLMS_CONFORMANCE_SET | DLMS_CONFORMANCE_SELECTIVE_ACCESS |
            DLMS_CONFORMANCE_ACTION | DLMS_CONFORMANCE_MULTIPLE_REFERENCES |
            DLMS_CONFORMANCE_GET | DLMS_CONFORMANCE_GENERAL_PROTECTION));
    }
    else
    {
        SetConformance((DLMS_CONFORMANCE)(DLMS_CONFORMANCE_INFORMATION_REPORT |
            DLMS_CONFORMANCE_READ | DLMS_CONFORMANCE_UN_CONFIRMED_WRITE |
            DLMS_CONFORMANCE_WRITE | DLMS_CONFORMANCE_PARAMETERIZED_ACCESS |
            DLMS_CONFORMANCE_MULTIPLE_REFERENCES |
            DLMS_CONFORMANCE_GENERAL_PROTECTION));
    }
    Reset();
}

CGXDLMSServer::CGXDLMSServer(
    CGXDLMSAssociationLogicalName* ln,
    CGXDLMSIecHdlcSetup* hdlc) :
    CGXDLMSServer(true, DLMS_INTERFACE_TYPE_HDLC)
{
    m_Settings.GetObjects().push_back(ln);
    m_Settings.GetObjects().push_back(hdlc);
    m_Hdlc = hdlc;
}

CGXDLMSServer::CGXDLMSServer(
    CGXDLMSAssociationLogicalName* ln,
    CGXDLMSTcpUdpSetup* wrapper) :
    CGXDLMSServer(true, DLMS_INTERFACE_TYPE_WRAPPER)
{
    m_Settings.GetObjects().push_back(ln);
    m_Settings.GetObjects().push_back(wrapper);
    m_Wrapper = wrapper;
}

CGXDLMSServer::CGXDLMSServer(
    CGXDLMSAssociationShortName* sn,
    CGXDLMSIecHdlcSetup* hdlc) :
    CGXDLMSServer(false, DLMS_INTERFACE_TYPE_HDLC)
{
    m_Settings.GetObjects().push_back(sn);
    m_Settings.GetObjects().push_back(hdlc);
    m_Hdlc = hdlc;
}

CGXDLMSServer::CGXDLMSServer(
    CGXDLMSAssociationShortName* sn,
    CGXDLMSTcpUdpSetup* wrapper) :
    CGXDLMSServer(false, DLMS_INTERFACE_TYPE_WRAPPER)
{
    m_Settings.GetObjects().push_back(sn);
    m_Settings.GetObjects().push_back(wrapper);
    m_Wrapper = wrapper;
}

CGXDLMSServer::~CGXDLMSServer()
{

}

unsigned long CGXDLMSServer::GetPushClientAddress()
{
    return m_Settings.GetPushClientAddress();
}

void CGXDLMSServer::SetPushClientAddress(unsigned long value)
{
    m_Settings.SetPushClientAddress(value);
}

/**
* @return HDLC settings.
*/
CGXDLMSIecHdlcSetup* CGXDLMSServer::GetHdlc() {
    return m_Hdlc;
}

/**
* @param value
*            HDLC settings.
*/
void CGXDLMSServer::SetHdlc(CGXDLMSIecHdlcSetup* value) {
    m_Hdlc = value;
}

/**
* @return Wrapper settings.
*/
CGXDLMSTcpUdpSetup* CGXDLMSServer::GetWrapper() {
    return m_Wrapper;
}

/**
* @param value
*            Wrapper settings.
*/
void CGXDLMSServer::SetWrapper(CGXDLMSTcpUdpSetup* value) {
    m_Wrapper = value;
}

CGXDLMSObjectCollection& CGXDLMSServer::GetItems()
{
    return m_Settings.GetObjects();
}

void CGXDLMSServer::SetCipher(CGXCipher* value)
{
    m_Settings.SetCipher(value);
}

CGXDLMSSettings& CGXDLMSServer::GetSettings()
{
    return m_Settings;
}

CGXByteBuffer& CGXDLMSServer::GetStoCChallenge()
{
    return m_Settings.GetStoCChallenge();
}

void CGXDLMSServer::SetStoCChallenge(CGXByteBuffer& value)
{
    m_Settings.SetUseCustomChallenge(value.GetSize() != 0);
    m_Settings.SetStoCChallenge(value);
}

DLMS_INTERFACE_TYPE CGXDLMSServer::GetInterfaceType()
{
    return m_Settings.GetInterfaceType();
}

void CGXDLMSServer::SetStartingPacketIndex(int value)
{
    m_Settings.SetBlockIndex(value);
}

int CGXDLMSServer::GetInvokeID()
{
    return m_Settings.GetInvokeID();
}

CGXDLMSLimits CGXDLMSServer::GetLimits()
{
    return m_Settings.GetLimits();
}

unsigned short CGXDLMSServer::GetMaxReceivePDUSize()
{
    return m_Settings.GetMaxServerPDUSize();
}

void CGXDLMSServer::SetMaxReceivePDUSize(unsigned short value)
{
    m_Settings.SetMaxServerPDUSize(value);
}

bool CGXDLMSServer::GetUseLogicalNameReferencing()
{
    return m_Settings.GetUseLogicalNameReferencing();
}

/**
 * @param value
 *            Is Logical Name referencing used.
 */
void CGXDLMSServer::SetUseLogicalNameReferencing(bool value)
{
    m_Settings.SetUseLogicalNameReferencing(value);
}

int CGXDLMSServer::Initialize()
{
    CGXDLMSObject* associationObject = NULL;
    m_Initialized = true;
    std::string ln;
    for (CGXDLMSObjectCollection::iterator it = m_Settings.GetObjects().begin(); it != m_Settings.GetObjects().end(); ++it)
    {
        (*it)->GetLogicalName(ln);
        if (ln.size() == 0)
        {
            //Invalid Logical Name.
            return DLMS_ERROR_CODE_INVALID_LOGICAL_NAME;
        }
        if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC)
        {
            CGXDLMSProfileGeneric* pg = (CGXDLMSProfileGeneric*)(*it);
            if (pg->GetProfileEntries() < 1)
            {
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            if (pg->GetCapturePeriod() > 0)
            {
                //TODO: Start thread. new GXProfileGenericUpdater(this, pg).start();
            }
        }
        else if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME
            && !m_Settings.GetUseLogicalNameReferencing())
        {
            CGXDLMSObjectCollection& list = ((CGXDLMSAssociationShortName*)* it)->GetObjectList();
            if (list.size() == 0)
            {
                list.insert(list.end(), GetItems().begin(), GetItems().end());
            }
            associationObject = *it;
        }
        else if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME
            && m_Settings.GetUseLogicalNameReferencing())
        {
            CGXDLMSObjectCollection& list = ((CGXDLMSAssociationLogicalName*)* it)->GetObjectList();
            if (list.size() == 0)
            {
                list.insert(list.end(), GetItems().begin(), GetItems().end());
            }
            associationObject = *it;
            ((CGXDLMSAssociationLogicalName*)* it)->GetXDLMSContextInfo().SetMaxReceivePduSize(m_Settings.GetMaxServerPDUSize());
            ((CGXDLMSAssociationLogicalName*)* it)->GetXDLMSContextInfo().SetMaxSendPduSize(m_Settings.GetMaxServerPDUSize());
            ((CGXDLMSAssociationLogicalName*)* it)->GetXDLMSContextInfo().SetConformance(m_Settings.GetProposedConformance());
        }
    }
    if (associationObject == NULL)
    {
        if (GetUseLogicalNameReferencing())
        {
            CGXDLMSAssociationLogicalName* it2 = (CGXDLMSAssociationLogicalName*)CGXDLMSObjectFactory::CreateObject(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME);
            CGXDLMSObjectCollection& list = it2->GetObjectList();
            GetItems().push_back(it2);
            list.insert(list.end(), GetItems().begin(), GetItems().end());
            ((CGXDLMSAssociationLogicalName*)it2)->GetXDLMSContextInfo().SetMaxReceivePduSize(m_Settings.GetMaxServerPDUSize());
            ((CGXDLMSAssociationLogicalName*)it2)->GetXDLMSContextInfo().SetMaxSendPduSize(m_Settings.GetMaxServerPDUSize());
            ((CGXDLMSAssociationLogicalName*)it2)->GetXDLMSContextInfo().SetConformance(m_Settings.GetProposedConformance());
        }
        else
        {
            CGXDLMSAssociationShortName* it2 = (CGXDLMSAssociationShortName*)CGXDLMSObjectFactory::CreateObject(DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME);
            CGXDLMSObjectCollection& list = it2->GetObjectList();
            GetItems().push_back(it2);
            list.insert(list.end(), GetItems().begin(), GetItems().end());
        }
    }
    // Arrange items by Short Name.

    if (!m_Settings.GetUseLogicalNameReferencing())
    {
        UpdateShortNames(false);
    }
    return 0;
}


int CGXDLMSServer::UpdateShortNames()
{
    return UpdateShortNames(true);
}

int CGXDLMSServer::UpdateShortNames(bool force)
{
    int ret;
    short sn = 0xA0;
    unsigned char offset, count;
    for (CGXDLMSObjectCollection::iterator it = m_Settings.GetObjects().begin();
        it != m_Settings.GetObjects().end(); ++it)
    {
        if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME ||
            (*it)->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME)
        {
            continue;
        }
        // Generate Short Name if not given.
        if (force || (*it)->GetShortName() == 0)
        {
            (*it)->SetShortName(sn);
            // Add method index addresses.
            if ((ret = CGXDLMS::GetActionInfo((*it)->GetObjectType(), offset, count)) != 0)
            {
                return ret;
            }
            if (count != 0)
            {
                sn += offset + (8 * count);
            }
            else
            {
                // If there are no methods.
                // Add attribute index addresses.
                sn += 8 * (*it)->GetAttributeCount();
            }
        }
        else
        {
            sn = (*it)->GetShortName();
        }
    }
    return 0;
}

void CGXDLMSServer::Reset(bool connected)
{
    if (m_Transaction != NULL)
    {
        delete m_Transaction;
        m_Transaction = NULL;
    }
    m_Settings.SetProtocolVersion(NULL);
    m_Settings.SetCount(0);
    m_Settings.SetIndex(0);
    m_Settings.SetConnected(DLMS_CONNECTION_STATE_NONE);
    m_ReceivedData.Clear();
    m_ReplyData.Clear();
    if (!connected)
    {
        m_Info.Clear();
        m_Settings.SetServerAddress(0);
        m_Settings.SetClientAddress(0);
    }

    m_Settings.SetAuthentication(DLMS_AUTHENTICATION_NONE);
    if (m_Settings.GetCipher() != NULL)
    {
        if (!connected)
        {
            m_Settings.GetCipher()->Reset();
        }
        else
        {
            m_Settings.GetCipher()->SetSecurity(DLMS_SECURITY_NONE);
        }
    }
}

void CGXDLMSServer::Reset()
{
    Reset(false);
}

/**
    * Parse AARQ request that client send and returns AARE request.
    *
    * @return Reply to the client.
    */
int CGXDLMSServer::HandleAarqRequest(
    CGXByteBuffer& data,
    CGXDLMSConnectionEventArgs& connectionInfo)
{
    int ret;
    CGXByteBuffer error;
    DLMS_ASSOCIATION_RESULT result;
    DLMS_SOURCE_DIAGNOSTIC diagnostic;
    m_Settings.GetCtoSChallenge().Clear();
    if (!m_Settings.GetUseCustomChallenge())
    {
        m_Settings.GetStoCChallenge().Clear();
    }
    if (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
    {
        Reset(true);
    }
    ret = CGXAPDU::ParsePDU(m_Settings, m_Settings.GetCipher(), data, result, diagnostic, NULL);
    if (ret != 0)
    {
        return ret;
    }
    if (result == DLMS_ASSOCIATION_RESULT_ACCEPTED)
    {
        if (m_Settings.GetDLMSVersion() != 6)
        {
            result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
            diagnostic = DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN;
            error.SetUInt8(0xE);
            error.SetUInt8(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR);
            error.SetUInt8(DLMS_SERVICE_ERROR_INITIATE);
            error.SetUInt8(DLMS_INITIATE_DLMS_VERSION_TOO_LOW);
            m_Settings.SetDLMSVersion(6);
        }
        else if (m_Settings.GetMaxPduSize() < 64)
        {
            result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
            diagnostic = DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN;
            error.SetUInt8(0xE);
            error.SetUInt8(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR);
            error.SetUInt8(DLMS_SERVICE_ERROR_INITIATE);
            error.SetUInt8(DLMS_INITIATE_PDU_SIZE_TOOSHORT);
            m_Settings.SetMaxReceivePDUSize(64);
        }
        else if (m_Settings.GetNegotiatedConformance() == 0)
        {
            result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
            diagnostic = DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN;
            error.SetUInt8(0xE);
            error.SetUInt8(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR);
            error.SetUInt8(DLMS_SERVICE_ERROR_INITIATE);
            error.SetUInt8(DLMS_INITIATE_INCOMPATIBLE_CONFORMANCE);
        }
        else if (diagnostic != DLMS_SOURCE_DIAGNOSTIC_NONE)
        {
            result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
            diagnostic = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED;
            InvalidConnection(connectionInfo);
        }
        else
        {
            diagnostic = ValidateAuthentication(m_Settings.GetAuthentication(), m_Settings.GetPassword());
            if (diagnostic != DLMS_SOURCE_DIAGNOSTIC_NONE)
            {
                result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
                InvalidConnection(connectionInfo);
            }
            else if (m_Settings.GetAuthentication() > DLMS_AUTHENTICATION_LOW)
            {
                // If High authentication is used.
                result = DLMS_ASSOCIATION_RESULT_ACCEPTED;
                diagnostic = DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_REQUIRED;
            }
            else
            {
                Connected(connectionInfo);
                m_Settings.SetConnected((DLMS_CONNECTION_STATE)(m_Settings.GetConnected() | DLMS_CONNECTION_STATE_DLMS));
            }
        }
    }
    if (m_Settings.GetAuthentication() > DLMS_AUTHENTICATION_LOW)
    {
        // If High authentication is used.
        if (!m_Settings.GetUseCustomChallenge())
        {
            CGXByteBuffer challenge;
            if ((ret = CGXSecure::GenerateChallenge(m_Settings.GetAuthentication(), challenge)) != 0)
            {
                return ret;
            }
            m_Settings.SetStoCChallenge(challenge);
        }
        if (m_Settings.GetUseLogicalNameReferencing())
        {
            unsigned char l[] = { 0,0,40,0,0,255 };
            CGXDLMSAssociationLogicalName* ln = (CGXDLMSAssociationLogicalName*)m_Settings.GetObjects().FindByLN(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, l);
            if (ln != NULL)
            {
                if (m_Settings.GetCipher() == NULL || m_Settings.GetCipher()->GetSecurity() == DLMS_SECURITY_NONE)
                {
                    ln->GetApplicationContextName().SetContextId(DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME);
                }
                else
                {
                    ln->GetApplicationContextName().SetContextId(DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME_WITH_CIPHERING);
                }
                ln->GetAuthenticationMechanismName().SetMechanismId(m_Settings.GetAuthentication());
                ln->SetAssociationStatus(DLMS_ASSOCIATION_STATUS_ASSOCIATION_PENDING);
            }
        }
    }
    else
    {
        unsigned char l[] = { 0,0,40,0,0,255 };
        CGXDLMSAssociationLogicalName* ln = (CGXDLMSAssociationLogicalName*)m_Settings.GetObjects().FindByLN(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, l);
        if (ln != NULL)
        {
            if (m_Settings.GetCipher() == NULL || m_Settings.GetCipher()->GetSecurity() == DLMS_SECURITY_NONE)
            {
                ln->GetApplicationContextName().SetContextId(DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME);
            }
            else
            {
                ln->GetApplicationContextName().SetContextId(DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME_WITH_CIPHERING);
            }
            ln->GetAuthenticationMechanismName().SetMechanismId(m_Settings.GetAuthentication());
            ln->SetAssociationStatus(DLMS_ASSOCIATION_STATUS_ASSOCIATED);
        }
    }
    if (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_HDLC)
    {
        m_ReplyData.Set(LLC_REPLY_BYTES, 3);
    }
    // Generate AARE packet.
    return CGXAPDU::GenerateAARE(m_Settings, m_ReplyData, result, diagnostic, m_Settings.GetCipher(), &error, NULL);
}

/**
 * Parse SNRM Request. If server do not accept client empty byte array is
 * returned.
 *
 * @return Returns returned UA packet.
 */
int CGXDLMSServer::HandleSnrmRequest(
    CGXDLMSSettings&,
    CGXByteBuffer& data,
    CGXByteBuffer& reply)
{
    int ret;
    Reset(true);
    if ((ret = CGXDLMS::ParseSnrmUaResponse(data, &m_Settings.GetLimits())) != 0)
    {
        return ret;
    }
    reply.SetUInt8(0x81); // FromatID
    reply.SetUInt8(0x80); // GroupID
    reply.SetUInt8(0); // Length

    if (m_Hdlc != NULL)
    {
        //If client wants send larger HDLC frames what meter accepts.
        if (m_Settings.GetLimits().GetMaxInfoTX() > m_Hdlc->GetMaximumInfoLengthReceive())
        {
            m_Settings.GetLimits().SetMaxInfoTX(m_Hdlc->GetMaximumInfoLengthReceive());
        }
        //If client wants receive larger HDLC frames what meter accepts.
        if (m_Settings.GetLimits().GetMaxInfoRX() > m_Hdlc->GetMaximumInfoLengthTransmit())
        {
            m_Settings.GetLimits().SetMaxInfoRX(m_Hdlc->GetMaximumInfoLengthTransmit());
        }
        //If client asks higher window size what meter accepts.
        if (m_Settings.GetLimits().GetWindowSizeTX() > m_Hdlc->GetWindowSizeReceive())
        {
            m_Settings.GetLimits().SetWindowSizeTX(m_Hdlc->GetWindowSizeReceive());
        }
        //If client asks higher window size what meter accepts.
        if (m_Settings.GetLimits().GetWindowSizeRX() > m_Hdlc->GetWindowSizeTransmit())
        {
            m_Settings.GetLimits().SetWindowSizeRX(m_Hdlc->GetWindowSizeTransmit());
        }
    }

    reply.SetUInt8(HDLC_INFO_MAX_INFO_TX);
    CGXDLMS::AppendHdlcParameter(reply, GetLimits().GetMaxInfoTX());

    reply.SetUInt8(HDLC_INFO_MAX_INFO_RX);
    CGXDLMS::AppendHdlcParameter(reply, GetLimits().GetMaxInfoRX());

    reply.SetUInt8(HDLC_INFO_WINDOW_SIZE_TX);
    reply.SetUInt8(4);
    reply.SetUInt32(m_Settings.GetLimits().GetWindowSizeTX());

    reply.SetUInt8(HDLC_INFO_WINDOW_SIZE_RX);
    reply.SetUInt8(4);
    reply.SetUInt32(m_Settings.GetLimits().GetWindowSizeRX());
    int len = reply.GetSize() - 3;
    reply.SetUInt8(2, len); // Length
    return ret;
}

/**
 * Generates disconnect request.
 *
 * @return Disconnect request.
 */
int GenerateDisconnectRequest(CGXDLMSSettings& settings, CGXByteBuffer& reply)
{
    if (settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
    {
        reply.SetUInt8(0x63);
        reply.SetUInt8(0x0);
    }
    else
    {
        reply.SetUInt8(0x81); // FromatID
        reply.SetUInt8(0x80); // GroupID
        reply.SetUInt8(0); // Length

        reply.SetUInt8(HDLC_INFO_MAX_INFO_TX);
        CGXDLMS::AppendHdlcParameter(reply, settings.GetLimits().GetMaxInfoTX());

        reply.SetUInt8(HDLC_INFO_MAX_INFO_RX);
        CGXDLMS::AppendHdlcParameter(reply, settings.GetLimits().GetMaxInfoRX());

        reply.SetUInt8(HDLC_INFO_WINDOW_SIZE_TX);
        reply.SetUInt8(4);
        reply.SetUInt32(settings.GetLimits().GetWindowSizeTX());

        reply.SetUInt8(HDLC_INFO_WINDOW_SIZE_RX);
        reply.SetUInt8(4);
        reply.SetUInt32(settings.GetLimits().GetWindowSizeRX());
        int len = reply.GetSize() - 3;
        reply.SetUInt8(2, len); // Length.
    }
    return 0;
}

int CGXDLMSServer::ReportError(DLMS_COMMAND command, DLMS_ERROR_CODE error, CGXByteBuffer& reply)
{
    int ret;
    DLMS_COMMAND cmd;
    CGXByteBuffer data;
    switch (command)
    {
    case DLMS_COMMAND_READ_REQUEST:
        cmd = DLMS_COMMAND_READ_RESPONSE;
        break;
    case DLMS_COMMAND_WRITE_REQUEST:
        cmd = DLMS_COMMAND_WRITE_RESPONSE;
        break;
    case DLMS_COMMAND_GET_REQUEST:
        cmd = DLMS_COMMAND_GET_RESPONSE;
        break;
    case DLMS_COMMAND_SET_REQUEST:
        cmd = DLMS_COMMAND_SET_RESPONSE;
        break;
    case DLMS_COMMAND_METHOD_REQUEST:
        cmd = DLMS_COMMAND_METHOD_RESPONSE;
        break;
    default:
        // Return HW error and close connection.
        cmd = DLMS_COMMAND_NONE;
        break;
    }

    if (m_Settings.GetUseLogicalNameReferencing())
    {
        CGXDLMSLNParameters p(&m_Settings, 0, cmd, 1, NULL, NULL, error, m_Info.GetCipheredCommand());
        ret = CGXDLMS::GetLNPdu(p, data);
    }
    else
    {
        CGXByteBuffer bb;
        bb.SetUInt8(error);
        CGXDLMSSNParameters p(&m_Settings, cmd, 1, 1, NULL, &bb);
        ret = CGXDLMS::GetSNPdu(p, data);
    }
    if (ret == 0)
    {
        if (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
        {
            ret = CGXDLMS::GetWrapperFrame(m_Settings, command, data, reply);
        }
        else
        {
            ret = CGXDLMS::GetHdlcFrame(m_Settings, 0, &data, reply);
        }
    }
    return ret;
}

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
void CGXDLMSServer::GenerateConfirmedServiceError(
    DLMS_CONFIRMED_SERVICE_ERROR service,
    DLMS_SERVICE_ERROR type,
    unsigned char code, CGXByteBuffer& data)
{
    data.SetUInt8(DLMS_COMMAND_CONFIRMED_SERVICE_ERROR);
    data.SetUInt8(service);
    data.SetUInt8(type);
    data.SetUInt8(code);
}

unsigned short CGXDLMSServer::GetRowsToPdu(CGXDLMSProfileGeneric* pg)
{
    DLMS_DATA_TYPE dt;
    int rowsize = 0;
    // Count how many rows we can fit to one PDU.
    for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator it = pg->GetCaptureObjects().begin();
        it != pg->GetCaptureObjects().end(); ++it)
    {
        it->first->GetDataType(it->second->GetAttributeIndex(), dt);
        if (dt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            it->first->GetUIDataType(it->second->GetAttributeIndex(), dt);
            if (dt == DLMS_DATA_TYPE_DATETIME)
            {
                rowsize += GXHelpers::GetDataTypeSize(DLMS_DATA_TYPE_DATETIME);
            }
            else if (dt == DLMS_DATA_TYPE_DATE)
            {
                rowsize += GXHelpers::GetDataTypeSize(DLMS_DATA_TYPE_DATE);
            }
            else if (dt == DLMS_DATA_TYPE_TIME)
            {
                rowsize += GXHelpers::GetDataTypeSize(DLMS_DATA_TYPE_TIME);
            }
        }
        else if (dt == DLMS_DATA_TYPE_NONE)
        {
            rowsize += 2;
        }
        else
        {
            rowsize += GXHelpers::GetDataTypeSize(dt);
        }
    }
    if (rowsize != 0)
    {
        return m_Settings.GetMaxPduSize() / rowsize;
    }
    return 0;
}

/**
* Handles release request.
*
* @param data
*            Received data.
* @param connectionInfo
*            Connection info.
*/
int CGXDLMSServer::HandleReleaseRequest(CGXByteBuffer&)
{
    if (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_HDLC)
    {
        m_ReplyData.Set(LLC_REPLY_BYTES, 3);
    }
    CGXByteBuffer tmp;
    int ret = CGXAPDU::GetUserInformation(m_Settings, m_Settings.GetCipher(), tmp);
    if (ret != 0)
    {
        return ret;
    }
    m_ReplyData.SetUInt8(0x63);
    //Len.
    m_ReplyData.SetUInt8((unsigned char)(tmp.GetSize() + 3));
    m_ReplyData.SetUInt8(0x80);
    m_ReplyData.SetUInt8(0x01);
    m_ReplyData.SetUInt8(0x00);
    m_ReplyData.SetUInt8(0xBE);
    m_ReplyData.SetUInt8((unsigned char)(tmp.GetSize() + 1));
    m_ReplyData.SetUInt8(4);
    m_ReplyData.SetUInt8((unsigned char)tmp.GetSize());
    m_ReplyData.Set(&tmp, 0, tmp.GetSize());
    return 0;
}

int CGXDLMSServer::HandleGeneralBlockTransfer(
    CGXServerReply& sr,
    CGXByteBuffer& data,
    unsigned char cipheredCommand)
{
    if (m_Transaction != NULL)
    {
        if (m_Transaction->GetCommand() == DLMS_COMMAND_GET_REQUEST)
        {
            // Get request for next data block
            if (sr.GetCount() == 0)
            {
                m_Settings.SetBlockNumberAck(m_Settings.GetBlockNumberAck() + 1);
                sr.SetCount(m_Settings.GetWindowSize());
            }
            CGXDLMSLNCommandHandler::GetRequestNextDataBlock(m_Settings, 0,
                this, data, &m_ReplyData, NULL, true, cipheredCommand);
            if (sr.GetCount() != 0)
            {
                sr.SetCount(sr.GetCount() - 1);
            }
            if (m_Transaction == NULL)
            {
                sr.SetCount(0);
            }
        }
        else
        {
            // BlockControl
            unsigned char bc;
            data.GetUInt8(&bc);
            // Block number.
            unsigned short blockNumber, blockNumberAck;
            data.GetUInt16(&blockNumber);
            // Block number acknowledged.
            data.GetUInt16(&blockNumberAck);
            unsigned long len;
            GXHelpers::GetObjectCount(data, len);
            if (len > data.GetSize() - data.GetPosition())
            {
                GenerateConfirmedServiceError(
                    DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR,
                    DLMS_SERVICE_ERROR_SERVICE,
                    DLMS_SERVICE_UNSUPPORTED, m_ReplyData);
            }
            else
            {
                m_Transaction->GetData().Set(&data);
                // Send ACK.
                unsigned char igonoreAck = (bc & 0x40) != 0 &&
                    (blockNumberAck * m_Settings.GetWindowSize()) + 1 > blockNumber;
                int windowSize = m_Settings.GetWindowSize();
                int bn = m_Settings.GetBlockIndex();
                if ((bc & 0x80) != 0)
                {
                    HandleCommand(m_Transaction->GetCommand(),
                        m_Transaction->GetData(), sr, cipheredCommand);
                    delete m_Transaction;
                    m_Transaction = NULL;
                    igonoreAck = false;
                    windowSize = 1;
                }
                if (igonoreAck)
                {
                    return false;
                }
                m_ReplyData.SetUInt8(DLMS_COMMAND_GENERAL_BLOCK_TRANSFER);
                m_ReplyData.SetUInt8((0x80 | windowSize));
                m_Settings.SetBlockIndex(m_Settings.GetBlockIndex() + 1);
                m_ReplyData.SetUInt16(bn);
                m_ReplyData.SetUInt16(blockNumber);
                m_ReplyData.SetUInt8(0);
            }
        }
    }
    else
    {
        // BlockControl
        unsigned char bc;
        unsigned short blockNumber, blockNumberAck;
        data.GetUInt8(&bc);
        // Block number.
        data.GetUInt16(&blockNumber);
        // Block number acknowledged.
        data.GetUInt16(&blockNumberAck);
        unsigned long len;
        GXHelpers::GetObjectCount(data, len);
        if (len > data.GetSize() - data.GetPosition())
        {
            GenerateConfirmedServiceError(
                DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR,
                DLMS_SERVICE_ERROR_SERVICE, DLMS_SERVICE_UNSUPPORTED, m_ReplyData);
        }
        else
        {
            unsigned char ch;
            data.GetUInt8(&ch);
            m_Transaction = new CGXDLMSLongTransaction((DLMS_COMMAND)ch, data);
            m_ReplyData.SetUInt8(DLMS_COMMAND_GENERAL_BLOCK_TRANSFER);
            m_ReplyData.SetUInt8((0x80 | m_Settings.GetWindowSize()));
            m_ReplyData.SetUInt16(blockNumber);
            ++blockNumberAck;
            m_ReplyData.SetUInt16(blockNumberAck);
            m_ReplyData.SetUInt8(0);
        }
    }
    return 0;
}

int CGXDLMSServer::HandleCommand(
    DLMS_COMMAND cmd,
    CGXByteBuffer& data,
    CGXServerReply& sr,
    unsigned char cipheredCommand)
{
    int ret = 0;
    unsigned char frame = 0;
    if (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_HDLC && m_ReplyData.GetSize() != 0)
    {
        //Get next frame.
        frame = m_Settings.GetNextSend(false);
    }
    CGXDLMSConnectionEventArgs& connectionInfo = sr.GetConnectionInfo();
    switch (cmd)
    {
    case DLMS_COMMAND_SET_REQUEST:
        ret = CGXDLMSLNCommandHandler::HandleSetRequest(m_Settings, this, data, &m_ReplyData, NULL, cipheredCommand);
        break;
    case DLMS_COMMAND_WRITE_REQUEST:
        ret = CGXDLMSSNCommandHandler::HandleWriteRequest(m_Settings, this, data, &m_ReplyData, NULL, cipheredCommand);
        break;
    case DLMS_COMMAND_GET_REQUEST:
        if (data.GetSize() != 0)
        {
            ret = CGXDLMSLNCommandHandler::HandleGetRequest(m_Settings, this, data, &m_ReplyData, NULL, cipheredCommand);
        }
        break;
    case DLMS_COMMAND_READ_REQUEST:
        ret = CGXDLMSSNCommandHandler::HandleReadRequest(m_Settings, this, data, &m_ReplyData, NULL, cipheredCommand);
        break;
    case DLMS_COMMAND_METHOD_REQUEST:
        ret = CGXDLMSLNCommandHandler::HandleMethodRequest(m_Settings, this, data, &m_ReplyData, &connectionInfo, NULL, cipheredCommand);
        break;
    case DLMS_COMMAND_SNRM:
        ret = HandleSnrmRequest(m_Settings, data, m_ReplyData);
        m_Settings.SetConnected(DLMS_CONNECTION_STATE_HDLC);
        frame = DLMS_COMMAND_UA;
        break;
    case DLMS_COMMAND_AARQ:
        ret = HandleAarqRequest(data, connectionInfo);
        break;
    case DLMS_COMMAND_RELEASE_REQUEST:
        ret = HandleReleaseRequest(data);
        m_Settings.SetConnected((DLMS_CONNECTION_STATE)(m_Settings.GetConnected() & ~DLMS_CONNECTION_STATE_DLMS));
        Disconnected(connectionInfo);
        break;
    case DLMS_COMMAND_DISCONNECT_REQUEST:
        ret = GenerateDisconnectRequest(m_Settings, m_ReplyData);
        if (m_Settings.GetConnected() > DLMS_CONNECTION_STATE_NONE)
        {
            if (m_Settings.GetConnected() == DLMS_CONNECTION_STATE_DLMS)
            {
                Disconnected(connectionInfo);
            }
            m_Settings.SetConnected(DLMS_CONNECTION_STATE_NONE);
        }
        m_Settings.SetConnected(DLMS_CONNECTION_STATE_NONE);
        Disconnected(connectionInfo);
        frame = DLMS_COMMAND_UA;
        break;
    case DLMS_COMMAND_GENERAL_BLOCK_TRANSFER:
        ret = HandleGeneralBlockTransfer(sr, data, m_Info.GetCipheredCommand());
        break;
    case DLMS_COMMAND_NONE:
        // Client wants to get next block.
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_COMMAND;
        break;
    }
    if (ret == 0)
    {
        CGXByteBuffer& reply = sr.GetReply();
        if (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
        {
            ret = CGXDLMS::GetWrapperFrame(m_Settings, cmd, m_ReplyData, reply);
        }
        else
        {
            ret = CGXDLMS::GetHdlcFrame(m_Settings, frame, &m_ReplyData, reply);
        }
    }
    if (cmd == DLMS_COMMAND_DISCONNECT_REQUEST ||
        (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER && cmd == DLMS_COMMAND_RELEASE_REQUEST))
    {
        Reset();
    }
    return ret;
}

int CGXDLMSServer::HandleRequest(
    CGXByteBuffer& data,
    CGXByteBuffer& reply)
{
    return HandleRequest(
        data.GetData(),
        (unsigned short)(data.GetSize() - data.GetPosition()),
        reply);
}

int CGXDLMSServer::HandleRequest(
    CGXDLMSConnectionEventArgs& connectionInfo,
    CGXByteBuffer& data,
    CGXByteBuffer& reply)
{
    CGXServerReply sr(data);
    sr.SetConnectionInfo(connectionInfo);
    int ret = HandleRequest(sr);
    if (ret == 0)
    {
        reply.Set(&sr.GetReply());
    }
    return ret;
}

int CGXDLMSServer::HandleRequest(
    unsigned char* buff,
    unsigned short size,
    CGXByteBuffer& reply)
{
    CGXDLMSConnectionEventArgs connectionInfo;
    return HandleRequest(connectionInfo, buff, size, reply);
}

int CGXDLMSServer::HandleRequest(
    CGXDLMSConnectionEventArgs& connectionInfo,
    unsigned char* buff,
    unsigned short size,
    CGXByteBuffer& reply)
{
    CGXByteBuffer data;
    data.Set(buff, size);
    return HandleRequest(connectionInfo, data, reply);
}

int CGXDLMSServer::HandleRequest(
    CGXServerReply& sr)
{
    int ret;
    if (!sr.IsStreaming() && sr.GetData().GetSize() == 0)
    {
        return 0;
    }
    if (!m_Initialized)
    {
        //Server not Initialized.
        return DLMS_ERROR_CODE_NOT_INITIALIZED;
    }
    m_ReceivedData.Set(&sr.GetData());
    CGXByteBuffer& reply = sr.GetReply();
    bool first = m_Settings.GetServerAddress() == 0
        && m_Settings.GetClientAddress() == 0;
    if ((ret = CGXDLMS::GetData(m_Settings, m_ReceivedData, m_Info, NULL)) != 0)
    {
        //If all data is not received yet.
        if (ret == DLMS_ERROR_CODE_FALSE)
        {
            ret = 0;
        }
        else
        {
            ret = CGXDLMS::GetHdlcFrame(m_Settings, DLMS_COMMAND_UNACCEPTABLE_FRAME, NULL, reply);
            m_ReceivedData.SetSize(0);
        }
        return ret;
    }
    // If all data is not received yet.
    if (!m_Info.IsComplete())
    {
        return 0;
    }
    m_ReceivedData.Clear();
    if (m_Info.GetCommand() == DLMS_COMMAND_DISCONNECT_REQUEST && m_Settings.GetConnected() == DLMS_CONNECTION_STATE_NONE)
    {
        ret = CGXDLMS::GetHdlcFrame(m_Settings, DLMS_COMMAND_DISCONNECT_MODE, NULL, reply);
        m_Info.Clear();
        return ret;
    }

    if (first || m_Info.GetCommand() == DLMS_COMMAND_SNRM ||
        (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER && m_Info.GetCommand() == DLMS_COMMAND_AARQ))
    {
        // Check is data send to this server.
        if (!IsTarget(m_Settings.GetServerAddress(), m_Settings.GetClientAddress()))
        {
            m_Info.Clear();
            return 0;
        }
    }
    // If client want next frame.
    if ((m_Info.GetMoreData() & DLMS_DATA_REQUEST_TYPES_FRAME) == DLMS_DATA_REQUEST_TYPES_FRAME)
    {
        m_DataReceived = (long)time(NULL);
        return CGXDLMS::GetHdlcFrame(m_Settings, m_Settings.GetReceiverReady(), &m_ReplyData, reply);
    }
    // Update command if m_Transaction and next frame is asked.
    if (m_Info.GetCommand() == DLMS_COMMAND_NONE)
    {
        if (m_Transaction != NULL)
        {
            m_Info.SetCommand(m_Transaction->GetCommand());
        }
        else if (m_ReplyData.GetSize() == 0)
        {
            m_DataReceived = (long)time(NULL);
            return CGXDLMS::GetHdlcFrame(m_Settings, m_Settings.GetReceiverReady(), NULL, reply);
        }
    }
    // Check inactivity time out.
    if (m_Hdlc != NULL && m_Hdlc->GetInactivityTimeout() != 0)
    {
        if (m_Info.GetCommand() != DLMS_COMMAND_SNRM)
        {
            long elapsed = (long)(time(NULL) - m_DataReceived);
            // If inactivity time out is elapsed.
            if (elapsed >= m_Hdlc->GetInactivityTimeout())
            {
                Reset();
                m_DataReceived = 0;
                return 0;
            }
        }
    }
    else if (m_Wrapper != NULL && m_Wrapper->GetInactivityTimeout() != 0)
    {
        if (m_Info.GetCommand() != DLMS_COMMAND_AARQ)
        {
            long elapsed = (long)(time(NULL) - m_DataReceived);
            // If inactivity time out is elapsed.
            if (elapsed >= m_Wrapper->GetInactivityTimeout())
            {
                Reset();
                m_DataReceived = 0;
                return 0;
            }
        }
    }
    ret = HandleCommand(m_Info.GetCommand(), m_Info.GetData(), sr, m_Info.GetCipheredCommand());
    if (ret != 0)
    {
        ret = CGXDLMS::GetHdlcFrame(m_Settings, DLMS_COMMAND_UNACCEPTABLE_FRAME, NULL, reply);
        m_ReceivedData.SetSize(0);
    }
    m_Info.Clear();
    m_DataReceived = (long)time(NULL);
    return ret;
}

DLMS_CONFORMANCE CGXDLMSServer::GetConformance()
{
    return m_Settings.GetProposedConformance();
}

void CGXDLMSServer::SetConformance(DLMS_CONFORMANCE value)
{
    m_Settings.SetProposedConformance(value);
}

int CGXDLMSServer::AddData(
    CGXDLMSObject* obj,
    unsigned char index,
    CGXByteBuffer& buff)
{
    int ret;
    DLMS_DATA_TYPE dt;
    CGXDLMSValueEventArg e(NULL, obj, index);
    if ((ret = obj->GetValue(m_Settings, e)) != 0)
    {
        return ret;
    }
    if ((ret = obj->GetDataType(index, dt)) != 0)
    {
        return ret;
    }
    if (dt == DLMS_DATA_TYPE_ARRAY)
    {
        buff.Set(e.GetValue().byteArr, e.GetValue().GetSize());
        return 0;
    }
    if (dt == DLMS_DATA_TYPE_NONE)
    {
        dt = e.GetValue().vt;
    }
    return GXHelpers::SetData(buff, e.GetValue().vt, e.GetValue());
}

int CGXDLMSServer::GenerateDataNotificationMessages(
    struct tm* time,
    CGXByteBuffer& data,
    std::vector<CGXByteBuffer>& reply)
{
    int ret;
    if (GetUseLogicalNameReferencing())
    {
        CGXDLMSLNParameters p(&m_Settings, 0, DLMS_COMMAND_DATA_NOTIFICATION, 0, NULL, &data, 0xff, DLMS_COMMAND_NONE);
        p.SetTime(time);
        ret = CGXDLMS::GetLnMessages(p, reply);
    }
    else
    {
        CGXDLMSSNParameters p(&m_Settings, DLMS_COMMAND_DATA_NOTIFICATION, 1, 0, &data, NULL);
        ret = CGXDLMS::GetSnMessages(p, reply);
    }
    return ret;
}

int CGXDLMSServer::GenerateDataNotificationMessages(
    struct tm* date,
    std::vector<std::pair<CGXDLMSObject*, unsigned char> >& objects,
    std::vector<CGXByteBuffer>& reply)
{
    CGXByteBuffer buff;
    buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    GXHelpers::SetObjectCount((unsigned long)objects.size(), buff);
    for (std::vector<std::pair<CGXDLMSObject*, unsigned char> >::iterator it = objects.begin(); it != objects.end(); ++it)
    {
        AddData(it->first, it->second, buff);
    }
    return GenerateDataNotificationMessages(date, buff, reply);
}

int CGXDLMSServer::GeneratePushSetupMessages(
    struct tm* date,
    CGXDLMSPushSetup* push,
    std::vector<CGXByteBuffer>& reply)
{
    if (push == NULL)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    CGXByteBuffer buff;
    buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    GXHelpers::SetObjectCount((unsigned long)push->GetPushObjectList().size(), buff);
    for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject> >::iterator it = push->GetPushObjectList().begin(); it != push->GetPushObjectList().end(); ++it)
    {
        AddData(it->first, it->second.GetAttributeIndex(), buff);
    }
    return GenerateDataNotificationMessages(date, buff, reply);
}
