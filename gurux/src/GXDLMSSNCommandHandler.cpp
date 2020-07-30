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

#include "../include/GXDLMSServer.h"
#include "../include/GXDLMSSNCommandHandler.h"
#include "../include/GXDLMSValueEventCollection.h"
#include "../include/GXDLMSSNParameters.h"
#include "../include/GXDLMS.h"
#include "../include/GXDLMSClient.h"

int CGXDLMSSNCommandHandler::HandleReadRequest(
    CGXDLMSSettings& settings,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    CGXByteBuffer* replyData,
    CGXDLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    CGXByteBuffer bb;
    int ret;
    unsigned char ch;
    unsigned long cnt = 0xFF;
    DLMS_VARIABLE_ACCESS_SPECIFICATION type;
    CGXDLMSValueEventCollection list;
    std::vector<CGXDLMSValueEventArg*> reads;
    std::vector<CGXDLMSValueEventArg*> actions;
    // If get next frame.
    if (xml == NULL && data.GetSize() == 0)
    {
        if (server != NULL && server->m_Transaction != NULL)
        {
            return 0;
        }
        if (replyData != NULL)
        {
            bb.Set(replyData);
            replyData->Clear();
        }
        for (std::vector<CGXDLMSValueEventArg*>::iterator it = server->m_Transaction->GetTargets().begin();
            it != server->m_Transaction->GetTargets().end(); ++it)
        {
            list.push_back(*it);
        }
    }
    else
    {
        std::string str;
        if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
        {
            return ret;
        }
        if (xml != NULL)
        {
            xml->IntegerToHex(cnt, 2, str);
            xml->AppendStartTag(DLMS_COMMAND_READ_REQUEST, "Qty", str);
        }
        CGXSNInfo info;
        for (unsigned long pos = 0; pos != cnt; ++pos)
        {
            if ((ret = data.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            type = (DLMS_VARIABLE_ACCESS_SPECIFICATION)ch;
            switch (type)
            {
            case DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME:
            case DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS:
                ret = HandleRead(settings, server, type, data, list, reads, actions, replyData, xml);
                break;
            case DLMS_VARIABLE_ACCESS_SPECIFICATION_BLOCK_NUMBER_ACCESS:
                return HandleReadBlockNumberAccess(settings, server, data, replyData, xml);
            case DLMS_VARIABLE_ACCESS_SPECIFICATION_READ_DATA_BLOCK_ACCESS:
                return HandleReadDataBlockAccess(settings, server, DLMS_COMMAND_READ_RESPONSE, data, cnt, replyData, xml, cipheredCommand);
            default:
                return ReturnSNError(settings, server, DLMS_COMMAND_READ_RESPONSE, DLMS_ERROR_CODE_READ_WRITE_DENIED, replyData);
            }
        }
        if (reads.size() != 0)
        {
            server->PreRead(reads);
        }
        if (actions.size() != 0)
        {
            server->PreAction(actions);
        }
    }
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_COMMAND_READ_REQUEST);
        return 0;
    }
    DLMS_SINGLE_READ_RESPONSE requestType;
    ret = GetReadData(settings, list, bb, requestType);
    if (reads.size() != 0)
    {
        server->PostRead(reads);
    }
    if (actions.size() != 0)
    {
        server->PostAction(actions);
    }
    CGXDLMSSNParameters p(&settings, DLMS_COMMAND_READ_RESPONSE, cnt,
        requestType, NULL, &bb);
    CGXDLMS::GetSNPdu(p, *replyData);
    if (server->m_Transaction == NULL && (bb.GetSize() != bb.GetPosition()
        || settings.GetCount() != settings.GetIndex()))
    {
        server->m_Transaction = new CGXDLMSLongTransaction(list, DLMS_COMMAND_READ_REQUEST, bb);
    }
    else if (server->m_Transaction != NULL)
    {
        replyData->Set(&bb);
    }
    return 0;
}

int CGXDLMSSNCommandHandler::HandleWriteRequest(
    CGXDLMSSettings& settings,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    CGXByteBuffer* replyData,
    CGXDLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    // Return error if connection is not established.
    if (xml == NULL && (settings.GetConnected() && DLMS_CONNECTION_STATE_DLMS) == 0)
    {
        server->GenerateConfirmedServiceError(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR,
            DLMS_SERVICE_ERROR_SERVICE,
            DLMS_SERVICE_UNSUPPORTED, *replyData);
        return 0;
    }
    std::string str;
    int ret;
    unsigned char ch;
    unsigned short sn;
    unsigned long cnt, pos;
    DLMS_VARIABLE_ACCESS_SPECIFICATION type;
    CGXDLMSVariant value;
    std::vector<CGXSNInfo> targets;
    // Get object count.
    if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    if (xml != NULL)
    {
        xml->AppendStartTag(DLMS_COMMAND_WRITE_REQUEST);
        xml->IntegerToHex(cnt, 2, str);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_LIST_OF_VARIABLE_ACCESS_SPECIFICATION, "Qty", str);
        if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_VARIABLE_ACCESS_SPECIFICATION);
        }
    }
    CGXByteBuffer results(cnt);
    for (pos = 0; pos != cnt; ++pos)
    {
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        type = (DLMS_VARIABLE_ACCESS_SPECIFICATION)ch;
        CGXSNInfo i;
        switch (type)
        {
        case DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME:
            if ((ret = data.GetUInt16(&sn)) != 0)
            {
                return ret;
            }
            if (xml != NULL)
            {
                xml->IntegerToHex((long)sn, 4, str);
                xml->AppendLine(DLMS_COMMAND_WRITE_REQUEST << 8 |
                    DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME,
                    "", str);
            }
            else
            {
                if ((ret = FindSNObject(server, sn, i)) != 0)
                {
                    return ret;
                }
                targets.push_back(i);
                // If target is unknown.
                if (i.GetItem() == NULL)
                {
                    // Device reports a undefined object.
                    results.SetUInt8(DLMS_ERROR_CODE_UNDEFINED_OBJECT);
                }
                else
                {
                    results.SetUInt8(DLMS_ERROR_CODE_OK);
                }
            }
            break;
        case DLMS_VARIABLE_ACCESS_SPECIFICATION_WRITE_DATA_BLOCK_ACCESS:
            ret = HandleReadDataBlockAccess(settings, server, DLMS_COMMAND_WRITE_RESPONSE, data, cnt, replyData, xml, cipheredCommand);
            if (xml == NULL)
            {
                return ret;
            }
        default:
            // Device reports a HW error.
            results.SetUInt8(DLMS_ERROR_CODE_HARDWARE_FAULT);
        }
    }
    if (xml != NULL)
    {
        if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_VARIABLE_ACCESS_SPECIFICATION);
        }
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_LIST_OF_VARIABLE_ACCESS_SPECIFICATION);
    }
    // Get data count.
    if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    CGXDataInfo di;
    di.SetXml(xml);
    if (xml != NULL)
    {
        xml->IntegerToHex(cnt, 2, str);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_LIST_OF_DATA, "Qty", str);
    }
    for (pos = 0; pos != cnt; ++pos)
    {
        di.Clear();
        if (xml != NULL)
        {
            if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
            {
                xml->AppendStartTag(DLMS_COMMAND_WRITE_REQUEST, (unsigned long)DLMS_SINGLE_READ_RESPONSE_DATA);
            }
            if ((ret = GXHelpers::GetData(data, di, value)) != 0)
            {
                return ret;
            }
            if (!di.IsComplete())
            {
                str = data.ToHexString(data.GetPosition(), data.Available(), false);
                xml->AppendLine(xml->GetDataType(di.GetType()), "", str);
            }
            if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
            {
                xml->AppendEndTag(DLMS_COMMAND_WRITE_REQUEST, (unsigned long)DLMS_SINGLE_READ_RESPONSE_DATA);
            }
        }
        else
        {
            if ((ret = results.GetUInt8(pos, &ch)) != 0)
            {
                return ret;
            }
            if (ch == 0)
            {
                // If object has found.
                CGXSNInfo target = *(targets.begin() + pos);
                ret = GXHelpers::GetData(data, di, value);
                if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
                {
                    DLMS_DATA_TYPE dt;
                    ret = target.GetItem()->GetDataType(target.GetIndex(), dt);
                    if (dt != DLMS_DATA_TYPE_NONE && dt != DLMS_DATA_TYPE_OCTET_STRING)
                    {
                        CGXByteBuffer bb;
                        bb.Set(value.byteArr, value.GetSize());
                        value.Clear();
                        if ((ret = CGXDLMSClient::ChangeType(bb, dt, value)) != 0)
                        {
                            return ret;
                        }
                    }
                }
                CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(server, target.GetItem(), target.GetIndex());
                e->SetValue(value);
                CGXDLMSValueEventCollection arr;
                arr.push_back(e);
                server->PreWrite(arr);
                DLMS_ACCESS_MODE am = server->GetAttributeAccess(e);
                // If write is denied.
                if (am != DLMS_ACCESS_MODE_WRITE && am != DLMS_ACCESS_MODE_READ_WRITE)
                {
                    results.SetUInt8(pos, DLMS_ERROR_CODE_READ_WRITE_DENIED);
                }
                else
                {
                    if (e->GetError() != 0)
                    {
                        results.SetUInt8(pos, e->GetError());
                    }
                    else if (!e->GetHandled())
                    {
                        target.GetItem()->SetValue(settings, *e);
                        server->PostWrite(arr);
                    }
                }
            }
        }
    }
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_LIST_OF_DATA);
        xml->AppendEndTag(DLMS_COMMAND_WRITE_REQUEST);
        return 0;
    }
    CGXByteBuffer bb((2 * cnt));
    for (pos = 0; pos != cnt; ++pos)
    {
        if ((ret = results.GetUInt8(pos, &ch)) != 0)
        {
            return ret;
        }
        // If meter returns error.
        if (ch != 0)
        {
            bb.SetUInt8(1);
        }
        bb.SetUInt8(ch);
    }
    CGXDLMSSNParameters p(&settings, DLMS_COMMAND_WRITE_RESPONSE, cnt, 0xFF, NULL, &bb);
    return CGXDLMS::GetSNPdu(p, *replyData);
}


/**
 * Find Short Name object.
 *
 * @param sn
 */
int CGXDLMSSNCommandHandler::FindSNObject(CGXDLMSServer* server, int sn, CGXSNInfo& i)
{
    unsigned char offset, count;
    for (CGXDLMSObjectCollection::iterator it = server->GetItems().begin(); it != server->GetItems().end(); ++it)
    {
        if (sn >= (*it)->GetShortName())
        {
            // If attribute is accessed.
            if (sn < (*it)->GetShortName() + (*it)->GetAttributeCount() * 8)
            {
                i.SetAction(false);
                i.SetItem(*it);
                i.SetIndex(((sn - (*it)->GetShortName()) / 8) + 1);
                break;
            }
            else
            {
                // If method is accessed.
                CGXDLMS::GetActionInfo((*it)->GetObjectType(), offset, count);
                if (sn < (*it)->GetShortName() + offset + (8 * count))
                {
                    i.SetItem(*it);
                    i.SetAction(true);
                    i.SetIndex((sn - (*it)->GetShortName() - offset) / 8 + 1);
                    break;
                }
            }
        }
    }
    if (i.GetItem() == NULL)
    {
        std::string ln;
        i.SetItem(server->FindObject(DLMS_OBJECT_TYPE_NONE, sn, ln));
    }
    return 0;
}

/**
* Get data for Read command.
*
* @param settings
*            DLMS settings.
* @param list
*            received objects.
* @param data
*            Data as byte array.
* @param type
*            Response type.
*/
int CGXDLMSSNCommandHandler::GetReadData(CGXDLMSSettings& settings,
    std::vector<CGXDLMSValueEventArg*>& list,
    CGXByteBuffer& data,
    DLMS_SINGLE_READ_RESPONSE& type)
{
    int ret = 0;
    unsigned char first = 1;
    type = DLMS_SINGLE_READ_RESPONSE_DATA;
    for (std::vector<CGXDLMSValueEventArg*>::iterator e = list.begin(); e != list.end(); ++e)
    {
        if (!(*e)->GetHandled())
        {
            // If action.
            if ((*e)->IsAction())
            {
                ret = (*e)->GetTarget()->Invoke(settings, *(*e));
            }
            else
            {
                ret = (*e)->GetTarget()->GetValue(settings, *(*e));
            }
        }
        if (ret != 0)
        {
            return ret;
        }
        CGXDLMSVariant& value = (*e)->GetValue();
        if ((*e)->GetError() == DLMS_ERROR_CODE_OK)
        {
            if (!first && list.size() != 1)
            {
                data.SetUInt8(DLMS_SINGLE_READ_RESPONSE_DATA);
            }
            if ((*e)->IsByteArray())
            {
                data.Set(value.byteArr, value.GetSize());
            }
            else
            {
                ret = CGXDLMS::AppendData((*e)->GetTarget(), (*e)->GetIndex(), data, value);
            }
        }
        else
        {
            if (!first && list.size() != 1)
            {
                data.SetUInt8(DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR);
            }
            data.SetUInt8((*e)->GetError());
            type = DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR;
        }
        first = false;
    }
    return ret;
}

int CGXDLMSSNCommandHandler::HandleRead(
    CGXDLMSSettings& settings,
    CGXDLMSServer* server,
    DLMS_VARIABLE_ACCESS_SPECIFICATION type,
    CGXByteBuffer& data,
    CGXDLMSValueEventCollection& list,
    std::vector<CGXDLMSValueEventArg*>& reads,
    std::vector<CGXDLMSValueEventArg*>& actions,
    CGXByteBuffer* replyData,
    CGXDLMSTranslatorStructure* xml)
{
    CGXSNInfo i;
    int ret;
    unsigned char ch;
    unsigned short sn;
    if ((ret = data.GetUInt16(&sn)) != 0)
    {
        return ret;
    }
    if (xml != NULL)
    {
        std::string str;
        if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_VARIABLE_ACCESS_SPECIFICATION);
        }
        if (type == DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS)
        {
            xml->AppendStartTag(DLMS_COMMAND_READ_REQUEST,
                DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS);
            xml->IntegerToHex((long)sn, 4, str);
            xml->AppendLine(
                DLMS_COMMAND_READ_REQUEST << 8 | DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME,
                "", str);
            if ((ret = data.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            xml->IntegerToHex((long)ch, 2, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_SELECTOR, "", str);
            CGXDataInfo di;
            di.SetXml(xml);
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_PARAMETER);
            CGXDLMSVariant value;
            if ((ret = GXHelpers::GetData(data, di, value)) != 0)
            {
                return ret;
            }
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_PARAMETER);
            xml->AppendEndTag(DLMS_COMMAND_READ_REQUEST, (unsigned long)DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS);
        }
        else
        {
            xml->IntegerToHex((long)sn, 4, str);
            xml->AppendLine(
                (int)DLMS_COMMAND_READ_REQUEST << 8
                | (int)DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME,
                "", str);
        }
        if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_VARIABLE_ACCESS_SPECIFICATION);
        }
        return 0;
    }
    if ((ret = FindSNObject(server, sn, i)) != 0)
    {
        return ret;
    }
    CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(server, i.GetItem(), i.GetIndex());
    e->SetAction(i.IsAction());
    list.push_back(e);
    if (type == DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS)
    {
        CGXDLMSVariant params;
        CGXDataInfo di;
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        e->SetSelector(ch);
        if ((ret = GXHelpers::GetData(data, di, params)) != 0)
        {
            return ret;
        }
        e->SetParameters(params);
    }
    // Return error if connection is not established.
    if (settings.GetConnected() == DLMS_CONNECTION_STATE_NONE &&
        (!e->IsAction() || e->GetTarget()->GetShortName() != 0xFA00 || e->GetIndex() != 8))
    {
        server->GenerateConfirmedServiceError(
            DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR, DLMS_SERVICE_ERROR_SERVICE,
            DLMS_SERVICE_UNSUPPORTED, *replyData);
        return 0;
    }

    if (server->GetAttributeAccess(e) == DLMS_ACCESS_MODE_NONE)
    {
        e->SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    else
    {
        if (e->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC && e->GetIndex() == 2)
        {
            e->SetRowToPdu(server->GetRowsToPdu((CGXDLMSProfileGeneric*)e->GetTarget()));
        }
        if (e->IsAction())
        {
            actions.push_back(e);
        }
        else
        {
            reads.push_back(e);
        }
    }
    return ret;
}

int CGXDLMSSNCommandHandler::HandleReadBlockNumberAccess(
    CGXDLMSSettings& settings,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    CGXByteBuffer* replyData,
    CGXDLMSTranslatorStructure*)
{
    unsigned short blockNumber;
    int ret;
    if ((ret = data.GetUInt16(&blockNumber)) != 0)
    {
        return ret;
    }
    if (blockNumber != settings.GetBlockIndex())
    {
        CGXByteBuffer bb;
        bb.SetUInt8(DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID);
        CGXDLMSSNParameters p(&settings,
            DLMS_COMMAND_READ_RESPONSE, 1,
            DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR, &bb, NULL);
        ret = CGXDLMS::GetSNPdu(p, *replyData);
        settings.ResetBlockIndex();
        return ret;
    }
    if (settings.GetIndex() != settings.GetCount()
        && server->m_Transaction->GetData().GetSize() < settings.GetMaxPduSize())
    {
        std::vector<CGXDLMSValueEventArg*> reads;
        std::vector<CGXDLMSValueEventArg*> actions;

        for (std::vector<CGXDLMSValueEventArg*>::iterator it = server->m_Transaction->GetTargets().begin();
            it != server->m_Transaction->GetTargets().end(); ++it)
        {
            if ((*it)->IsAction())
            {
                actions.push_back(*it);
            }
            else
            {
                reads.push_back(*it);
            }
        }
        if (reads.size() != 0)
        {
            server->PreRead(reads);
        }

        if (actions.size() != 0)
        {
            server->PreAction(actions);
        }
        DLMS_SINGLE_READ_RESPONSE requestType;
        std::vector<CGXDLMSValueEventArg*>& list = server->m_Transaction->GetTargets();
        CGXByteBuffer& data2 = server->m_Transaction->GetData();
        ret = GetReadData(settings, list, data2, requestType);
        if (reads.size() != 0)
        {
            server->PostRead(reads);
        }
        if (actions.size() != 0)
        {
            server->PostAction(actions);
        }
    }
    settings.IncreaseBlockIndex();
    CGXByteBuffer& tmp = server->m_Transaction->GetData();
    CGXDLMSSNParameters p(&settings, DLMS_COMMAND_READ_RESPONSE, 1,
        DLMS_SINGLE_READ_RESPONSE_DATA_BLOCK_RESULT, NULL, &tmp);
    p.SetMultipleBlocks(true);
    ret = CGXDLMS::GetSNPdu(p, *replyData);
    // If all data is sent.
    if (server->m_Transaction->GetData().GetSize() == server->m_Transaction->GetData().GetPosition())
    {
        delete server->m_Transaction;
        server->m_Transaction = NULL;
        settings.ResetBlockIndex();
    }
    else
    {
        server->m_Transaction->GetData().Trim();
    }
    return ret;
}

int CGXDLMSSNCommandHandler::HandleReadDataBlockAccess(
    CGXDLMSSettings& settings,
    CGXDLMSServer* server,
    DLMS_COMMAND command,
    CGXByteBuffer& data,
    int cnt,
    CGXByteBuffer* replyData,
    CGXDLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    int ret;
    unsigned long size;
    unsigned short blockNumber;
    unsigned char lastBlock;
    if ((ret = data.GetUInt8(&lastBlock)) != 0)
    {
        return ret;
    }
    if ((ret = data.GetUInt16(&blockNumber)) != 0)
    {
        return ret;
    }
    if (xml != NULL)
    {
        std::string str;
        if (command == DLMS_COMMAND_WRITE_RESPONSE)
        {
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_WRITE_DATA_BLOCK_ACCESS);
        }
        else
        {
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_READ_DATA_BLOCK_ACCESS);
        }
        xml->IntegerToHex((long)lastBlock, 2, str);
        xml->AppendLine("<LastBlock Value=\"" + str + "\" />");
        xml->IntegerToHex((long)blockNumber, 4, str);
        xml->AppendLine("<BlockNumber Value=\"" + str + "\" />");
        if (command == DLMS_COMMAND_WRITE_RESPONSE)
        {
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_WRITE_DATA_BLOCK_ACCESS);
        }
        else
        {
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_READ_DATA_BLOCK_ACCESS);
        }
        return 0;
    }
    if (blockNumber != settings.GetBlockIndex())
    {
        CGXByteBuffer bb;
        bb.SetUInt8(DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID);
        CGXDLMSSNParameters p(&settings, command, 1, DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR, &bb, NULL);
        ret = CGXDLMS::GetSNPdu(p, *replyData);
        settings.ResetBlockIndex();
        return ret;
    }
    unsigned char count = 1, type = DLMS_DATA_TYPE_OCTET_STRING;
    if (command == DLMS_COMMAND_WRITE_RESPONSE)
    {
        if ((ret = data.GetUInt8(&count)) != 0 ||
            (ret = data.GetUInt8(&type)) != 0)
        {
            return ret;
        }
    }
    if ((ret = GXHelpers::GetObjectCount(data, size)) != 0)
    {
        return ret;
    }
    unsigned long realSize = data.GetSize() - data.GetPosition();
    if (count != 1 || type != DLMS_DATA_TYPE_OCTET_STRING || size != realSize)
    {
        CGXByteBuffer bb;
        bb.SetUInt8(DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE);
        CGXDLMSSNParameters p(&settings, command, cnt,
            DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR, &bb, NULL);
        ret = CGXDLMS::GetSNPdu(p, *replyData);
        settings.ResetBlockIndex();
        return ret;
    }
    if (server->m_Transaction == NULL)
    {
        CGXDLMSValueEventCollection tmp;
        server->m_Transaction = new CGXDLMSLongTransaction(tmp, command, data);
    }
    else
    {
        server->m_Transaction->GetData().Set(&data, data.GetPosition());
    }
    if (lastBlock == 0)
    {
        CGXByteBuffer bb;
        bb.SetUInt16(blockNumber);
        settings.IncreaseBlockIndex();
        if (command == DLMS_COMMAND_READ_RESPONSE)
        {
            type = DLMS_SINGLE_READ_RESPONSE_BLOCK_NUMBER;
        }
        else
        {
            type = DLMS_SINGLE_WRITE_RESPONSE_BLOCK_NUMBER;
        }
        CGXDLMSSNParameters p(&settings, command, cnt, type, NULL, &bb);
        ret = CGXDLMS::GetSNPdu(p, *replyData);
    }
    else
    {
        if (server->m_Transaction != NULL)
        {
            data.SetSize(0);
            data.Set(&server->m_Transaction->GetData());
            delete server->m_Transaction;
            server->m_Transaction = NULL;
        }
        if (command == DLMS_COMMAND_READ_RESPONSE)
        {
            ret = CGXDLMSSNCommandHandler::HandleReadRequest(settings, server, data, replyData, NULL, cipheredCommand);
        }
        else
        {
            ret = CGXDLMSSNCommandHandler::HandleWriteRequest(settings, server, data, replyData, NULL, cipheredCommand);
        }
        settings.ResetBlockIndex();
    }
    return ret;
}

int CGXDLMSSNCommandHandler::ReturnSNError(
    CGXDLMSSettings& settings,
    CGXDLMSServer*,
    DLMS_COMMAND cmd, DLMS_ERROR_CODE error,
    CGXByteBuffer* replyData)
{
    int ret;
    CGXByteBuffer bb;
    bb.SetUInt8(error);
    CGXDLMSSNParameters p(&settings, cmd, 1,
        DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR, &bb, NULL);
    ret = CGXDLMS::GetSNPdu(p, *replyData);
    settings.ResetBlockIndex();
    return ret;
}


/// Handle Information Report.
int CGXDLMSSNCommandHandler::HandleInformationReport(
    CGXDLMSSettings&,
    CGXReplyData& reply)
{
    unsigned long count;
    std::string str;
    int ret;
    unsigned char ch;
    reply.SetTime(0);
    if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }

    CGXByteBuffer tmp;
    // If date time is given.
    if (ch != 0)
    {
        tmp.Set(&reply.GetData(), reply.GetData().GetPosition(), ch);
        CGXDLMSVariant value;
        if ((ret = CGXDLMSClient::ChangeType(tmp, DLMS_DATA_TYPE_DATETIME, value)) != 0)
        {
            return ret;
        }
        reply.SetTime(&value.dateTime.GetValue());
    }
    if ((ret = GXHelpers::GetObjectCount(reply.GetData(), count)) != 0)
    {
        return ret;
    }
    if (reply.GetXml() != NULL)
    {
        reply.GetXml()->AppendStartTag(DLMS_COMMAND_INFORMATION_REPORT);
        if (reply.GetTime() != NULL)
        {
            CGXDateTime dt(reply.GetTime());
            reply.GetXml()->AppendComment(dt.ToString());
            if (reply.GetXml()->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
            {
                str = tmp.ToHexString(false);
                reply.GetXml()->AppendLine(DLMS_TRANSLATOR_TAGS_CURRENT_TIME, "", str);
            }
            else
            {
                str = GXHelpers::GeneralizedTime(reply.GetTime());
                reply.GetXml()->AppendLine(DLMS_TRANSLATOR_TAGS_CURRENT_TIME, "", str);
            }
        }
        reply.GetXml()->IntegerToHex(count, 2, str);
        reply.GetXml()->AppendStartTag(DLMS_TRANSLATOR_TAGS_LIST_OF_VARIABLE_ACCESS_SPECIFICATION, "Qty", str);
    }
    for (unsigned long pos = 0; pos != count; ++pos)
    {
        if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch == DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME)
        {
            unsigned short sn;
            if ((ret = reply.GetData().GetUInt16(&sn)) != 0)
            {
                return ret;
            }
            if (reply.GetXml() != NULL)
            {
                reply.GetXml()->IntegerToHex((long)sn, 4, str);
                reply.GetXml()->AppendLine(
                    DLMS_COMMAND_WRITE_REQUEST << 8 | DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME,
                    "", str);
            }
        }
    }
    if (reply.GetXml() != NULL)
    {
        reply.GetXml()->IntegerToHex(count, 2, str);
        reply.GetXml()->AppendEndTag(DLMS_TRANSLATOR_TAGS_LIST_OF_VARIABLE_ACCESS_SPECIFICATION);
        reply.GetXml()->AppendStartTag(DLMS_TRANSLATOR_TAGS_LIST_OF_DATA, "Qty", str);
    }
    //Get values.
    if ((ret = GXHelpers::GetObjectCount(reply.GetData(), count)) != 0)
    {
        return ret;
    }
    CGXDataInfo di;
    di.SetXml(reply.GetXml());
    CGXDLMSVariant value;
    for (unsigned long pos = 0; pos != count; ++pos)
    {
        di.Clear();
        if (reply.GetXml() != NULL)
        {
            if ((ret = GXHelpers::GetData(reply.GetData(), di, value)) != 0)
            {
                return ret;
            }
        }
    }
    if (reply.GetXml() != NULL)
    {
        reply.GetXml()->AppendEndTag(DLMS_TRANSLATOR_TAGS_LIST_OF_DATA);
        reply.GetXml()->AppendEndTag(DLMS_COMMAND_INFORMATION_REPORT);
    }
    return 0;
}
