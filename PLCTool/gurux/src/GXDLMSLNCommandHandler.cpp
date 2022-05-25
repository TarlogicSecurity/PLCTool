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
#include "../include/GXDLMSLNCommandHandler.h"
#include "../include/GXDLMS.h"
#include "../include/GXDLMSValueEventCollection.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXDLMSObjectFactory.h"
#include "../include/GXDLMSSecuritySetup.h"

void AppendAttributeDescriptor(CGXDLMSTranslatorStructure* xml, int ci, unsigned char* ln, unsigned char attributeIndex)
{
    xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR);
    if (xml->GetComments())
    {
        xml->AppendComment(CGXDLMSConverter::ToString((DLMS_OBJECT_TYPE)ci));
    }
    std::string tmp;
    xml->IntegerToHex((long)ci, 4, tmp);
    xml->AppendLine(DLMS_TRANSLATOR_TAGS_CLASS_ID, "", tmp);
    GXHelpers::GetLogicalName(ln, tmp);
    xml->AppendComment(tmp);
    tmp = GXHelpers::BytesToHex(ln, 6, false);
    xml->AppendLine(DLMS_TRANSLATOR_TAGS_INSTANCE_ID, "", tmp);
    xml->IntegerToHex((long)attributeIndex, 2, tmp);
    xml->AppendLine(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_ID, "", tmp);
    xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR);
}

void AppendMethodDescriptor(CGXDLMSTranslatorStructure* xml, int ci, unsigned char* ln, unsigned char attributeIndex)
{
    xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_METHOD_DESCRIPTOR);
    if (xml->GetComments())
    {
        xml->AppendComment(CGXDLMSConverter::ToString((DLMS_OBJECT_TYPE)ci));
    }
    std::string tmp;
    xml->IntegerToHex((long)ci, 4, tmp);
    xml->AppendLine(DLMS_TRANSLATOR_TAGS_CLASS_ID, "", tmp);
    GXHelpers::GetLogicalName(ln, tmp);
    xml->AppendComment(tmp);
    tmp = GXHelpers::BytesToHex(ln, 6, false);
    xml->AppendLine(DLMS_TRANSLATOR_TAGS_INSTANCE_ID, "", tmp);
    xml->IntegerToHex((long)attributeIndex, 2, tmp);
    xml->AppendLine(DLMS_TRANSLATOR_TAGS_METHOD_ID, "", tmp);
    xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_METHOD_DESCRIPTOR);
}

int CGXDLMSLNCommandHandler::GetRequestNormal(
    CGXDLMSSettings& settings,
    unsigned char invokeID,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    CGXByteBuffer* replyData,
    CGXDLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    int ret;
    CGXByteBuffer bb;
    DLMS_ERROR_CODE status = DLMS_ERROR_CODE_OK;
    settings.SetCount(0);
    settings.SetIndex(0);
    settings.ResetBlockIndex();
    CGXDLMSValueEventCollection arr;
    unsigned char attributeIndex;
    unsigned char* ln;
    // CI
    unsigned short tmp;
    if ((ret = data.GetUInt16(&tmp)) != 0)
    {
        return ret;
    }
    DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE)tmp;
    ln = data.GetData() + data.GetPosition();
    data.SetPosition(data.GetPosition() + 6);
    // Attribute Id
    if ((ret = data.GetUInt8(&attributeIndex)) != 0)
    {
        return ret;
    }

    // Access selection
    unsigned char selection, selector = 0;
    if ((ret = data.GetUInt8(&selection)) != 0)
    {
        return ret;
    }
    if (selection != 0)
    {
        if ((ret = data.GetUInt8(&selector)) != 0)
        {
            return ret;
        }
    }
    if (xml != NULL)
    {
        AppendAttributeDescriptor(xml, (int)ci, ln, attributeIndex);
        if (selection != 0)
        {
            std::string tmp;
            xml->IntegerToHex((long)selector, 2, tmp);
            CGXDataInfo info;
            CGXDLMSVariant value;
            info.SetXml(xml);
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ACCESS_SELECTION);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_ACCESS_SELECTOR, "", tmp);
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ACCESS_PARAMETERS);
            if ((ret = GXHelpers::GetData(data, info, value)) != 0)
            {
                return ret;
            }
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ACCESS_PARAMETERS);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ACCESS_SELECTION);
        }
        return 0;
    }
    CGXDLMSVariant parameters;
    if (selection != 0)
    {
        CGXDataInfo i;
        if ((ret = GXHelpers::GetData(data, i, parameters)) != 0)
        {
            return ret;
        }
    }
    CGXDLMSObject* obj = settings.GetObjects().FindByLN(ci, ln);
    if (obj == NULL)
    {
        std::string name;
        GXHelpers::GetLogicalName(ln, name);
        obj = server->FindObject(ci, 0, name);
    }
    CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(server, obj, attributeIndex, selector, parameters);
    e->SetInvokeId(invokeID);
    arr.push_back(e);
    if (obj == NULL)
    {
        // "Access Error : Device reports a undefined object."
        status = DLMS_ERROR_CODE_UNDEFINED_OBJECT;
    }
    else
    {
        if (server->GetAttributeAccess(e) == DLMS_ACCESS_MODE_NONE)
        {
            // Read Write denied.
            status = DLMS_ERROR_CODE_READ_WRITE_DENIED;
        }
        else
        {
            if (obj->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC && attributeIndex == 2)
            {
                e->SetRowToPdu(server->GetRowsToPdu((CGXDLMSProfileGeneric*)obj));
            }
            server->PreRead(arr);
            if (!e->GetHandled())
            {
                settings.SetCount(e->GetRowEndIndex() - e->GetRowBeginIndex());
                if ((ret = obj->GetValue(settings, *e)) != 0)
                {
                    status = DLMS_ERROR_CODE_HARDWARE_FAULT;
                }
                server->PostRead(arr);
            }
            if (status == 0)
            {
                status = e->GetError();
            }
            CGXDLMSVariant& value = e->GetValue();
            if (e->IsByteArray() && value.vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                // If byte array is added do not add type.
                bb.Set(value.byteArr, value.GetSize());
            }
            else if ((ret = CGXDLMS::AppendData(obj, attributeIndex, bb, value)) != 0)
            {
                status = DLMS_ERROR_CODE_HARDWARE_FAULT;
            }
        }
    }
    CGXDLMSLNParameters p(&settings, invokeID, DLMS_COMMAND_GET_RESPONSE, 1, NULL, &bb, status, cipheredCommand);
    ret = CGXDLMS::GetLNPdu(p, *replyData);
    if (settings.GetCount() != settings.GetIndex()
        || bb.GetSize() != bb.GetPosition())
    {
        if (server->m_Transaction != NULL)
        {
            delete server->m_Transaction;
            server->m_Transaction = NULL;
        }
        server->m_Transaction = new CGXDLMSLongTransaction(arr, DLMS_COMMAND_GET_REQUEST, bb);
    }
    return ret;
}

int CGXDLMSLNCommandHandler::GetRequestNextDataBlock(
    CGXDLMSSettings& settings,
    unsigned char invokeID,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    CGXByteBuffer* replyData,
    CGXDLMSTranslatorStructure* xml,
    bool streaming,
    unsigned char cipheredCommand)
{
    CGXByteBuffer bb;
    int ret;
    if (!streaming)
    {
        unsigned long index;
        // Get block index.
        if ((ret = data.GetUInt32(&index)) != 0)
        {
            return ret;
        }
        if (xml != NULL)
        {
            std::string tmp;
            xml->IntegerToHex(index, 8, tmp);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_BLOCK_NUMBER, "", tmp);
            return 0;
        }

        if (index != settings.GetBlockIndex())
        {
            CGXDLMSLNParameters p(&settings, invokeID, DLMS_COMMAND_GET_RESPONSE, 2,
                NULL, &bb,
                DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID, cipheredCommand);
            return CGXDLMS::GetLNPdu(p, *replyData);
        }
    }
    settings.IncreaseBlockIndex();
    CGXDLMSLNParameters p(&settings, invokeID,
        streaming ? DLMS_COMMAND_GENERAL_BLOCK_TRANSFER : DLMS_COMMAND_GET_RESPONSE,
        2, NULL, &bb, DLMS_ERROR_CODE_OK, cipheredCommand);
    p.SetStreaming(streaming);
    p.SetWindowSize(settings.GetWindowSize());
    // If m_Transaction is not in progress.
    if (server->m_Transaction == NULL)
    {
        p.SetStatus(DLMS_ERROR_CODE_NO_LONG_GET_OR_READ_IN_PROGRESS);
    }
    else
    {
        bb.Set(&server->m_Transaction->GetData());
        unsigned char moreData = settings.GetIndex() != settings.GetCount();
        if (moreData)
        {
            // If there is multiple blocks on the buffer.
            // This might happen when Max PDU size is very small.
            if (bb.GetSize() < settings.GetMaxPduSize())
            {
                CGXDLMSVariant value;
                for (std::vector<CGXDLMSValueEventArg*>::iterator arg = server->m_Transaction->GetTargets().begin();
                    arg != server->m_Transaction->GetTargets().end(); ++arg)
                {
                    server->PreRead(server->m_Transaction->GetTargets());
                    if (!(*arg)->GetHandled())
                    {
                        if ((ret = (*arg)->GetTarget()->GetValue(settings, *(*arg))) != 0)
                        {
                            return ret;
                        }
                        std::vector<CGXDLMSValueEventArg*> arr;
                        arr.push_back(*arg);
                        server->PostRead(arr);
                    }
                    value = (*arg)->GetValue();
                    // Add data.
                    if ((*arg)->IsByteArray() && value.vt == DLMS_DATA_TYPE_OCTET_STRING)
                    {
                        // If byte array is added do not add type.
                        bb.Set(value.byteArr, value.GetSize());
                    }
                    else if ((ret = CGXDLMS::AppendData((*arg)->GetTarget(), (*arg)->GetIndex(), bb, value)) != 0)
                    {
                        return DLMS_ERROR_CODE_HARDWARE_FAULT;
                    }
                }
            }
        }
        p.SetMultipleBlocks(true);
        ret = CGXDLMS::GetLNPdu(p, *replyData);
        moreData = settings.GetIndex() != settings.GetCount();
        if (moreData || bb.GetSize() - bb.GetPosition() != 0)
        {
            server->m_Transaction->SetData(bb);
        }
        else
        {
            delete server->m_Transaction;
            server->m_Transaction = NULL;
            settings.ResetBlockIndex();
        }
    }
    return ret;
}

int CGXDLMSLNCommandHandler::GetRequestWithList(
    CGXDLMSSettings& settings,
    unsigned char invokeID,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    CGXByteBuffer* replyData,
    CGXDLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    CGXDLMSValueEventCollection list;
    CGXByteBuffer bb;
    int ret;
    unsigned char attributeIndex;
    unsigned short id;
    unsigned long pos, cnt;
    if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    GXHelpers::SetObjectCount(cnt, bb);
    if (xml != NULL)
    {
        std::string tmp;
        xml->IntegerToHex(cnt, 2, tmp);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_LIST, "Qty", tmp);
    }
    for (pos = 0; pos != cnt; ++pos)
    {
        if ((ret = data.GetUInt16(&id)) != 0)
        {
            return ret;
        }
        DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE)id;
        unsigned char* ln;
        ln = data.GetData() + data.GetPosition();
        data.SetPosition(data.GetPosition() + 6);
        if ((ret = data.GetUInt8(&attributeIndex)) != 0)
        {
            return ret;
        }

        // AccessSelection
        unsigned char selection, selector = 0;
        CGXDLMSVariant parameters;
        if ((ret = data.GetUInt8(&selection)) != 0)
        {
            return ret;
        }
        if (selection != 0)
        {
            if ((ret = data.GetUInt8(&selector)) != 0)
            {
                return ret;
            }
            CGXDataInfo i;
            if ((ret = GXHelpers::GetData(data, i, parameters)) != 0)
            {
                return ret;
            }
        }
        if (xml != NULL)
        {
            std::string tmp;
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_WITH_SELECTION);
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR);
            xml->AppendComment(CGXDLMSConverter::ToString(ci));
            xml->IntegerToHex((long)ci, 4, tmp);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_CLASS_ID, "", tmp);
            GXHelpers::GetLogicalName(ln, tmp);
            xml->AppendComment(tmp);
            tmp = GXHelpers::BytesToHex(ln, 6, false);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_INSTANCE_ID, "", tmp);
            xml->IntegerToHex((long)attributeIndex, 2, tmp);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_ID, "", tmp);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_WITH_SELECTION);
        }
        else
        {
            CGXDLMSObject* obj = settings.GetObjects().FindByLN(ci, ln);
            if (obj == NULL)
            {
                std::string name;
                GXHelpers::GetLogicalName(ln, name);
                obj = server->FindObject(ci, 0, name);
            }
            if (obj == NULL)
            {
                // Access Error : Device reports a undefined object.
                CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(server, obj, attributeIndex);
                e->SetError(DLMS_ERROR_CODE_UNDEFINED_OBJECT);
                list.push_back(e);
            }
            else
            {
                CGXDLMSValueEventArg* arg = new CGXDLMSValueEventArg(server, obj, attributeIndex, selector, parameters);
                list.push_back(arg);
                if (server->GetAttributeAccess(arg) == DLMS_ACCESS_MODE_NONE)
                {
                    // Read Write denied.
                    arg->SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
                }
            }
        }
    }
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_LIST);
        return 0;
    }
    server->PreRead(list);
    pos = 0;
    for (std::vector<CGXDLMSValueEventArg*>::iterator it = list.begin(); it != list.end(); ++it)
    {
        if (!(*it)->GetHandled())
        {
            ret = (*it)->GetTarget()->GetValue(settings, *(*it));
        }
        CGXDLMSVariant& value = (*it)->GetValue();
        bb.SetUInt8((*it)->GetError());
        if ((*it)->IsByteArray() && value.vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            // If byte array is added do not add type.
            bb.Set(value.byteArr, value.GetSize());
        }
        else if ((ret = CGXDLMS::AppendData((*it)->GetTarget(), (*it)->GetIndex(), bb, value)) != 0)
        {
            return DLMS_ERROR_CODE_HARDWARE_FAULT;
        }
        if (settings.GetIndex() != settings.GetCount())
        {
            if (server->m_Transaction != NULL)
            {
                delete server->m_Transaction;
                server->m_Transaction = NULL;
            }
            CGXByteBuffer empty;
            server->m_Transaction = new CGXDLMSLongTransaction(list, DLMS_COMMAND_GET_REQUEST, empty);
        }
        ++pos;
    }
    server->PostRead(list);
    CGXDLMSLNParameters p(&settings, invokeID, DLMS_COMMAND_GET_RESPONSE, 3, NULL, &bb, 0xFF, cipheredCommand);
    return CGXDLMS::GetLNPdu(p, *replyData);
}

int CGXDLMSLNCommandHandler::HandleGetRequest(
    CGXDLMSSettings& settings,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    CGXByteBuffer* replyData,
    CGXDLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    // Return error if connection is not established.
    if (xml == NULL && (settings.GetConnected() && DLMS_CONNECTION_STATE_DLMS) == 0 &&
        cipheredCommand == DLMS_COMMAND_NONE)
    {
        server->GenerateConfirmedServiceError(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR,
            DLMS_SERVICE_ERROR_SERVICE, DLMS_SERVICE_UNSUPPORTED,
            *replyData);
        return 0;
    }
    int ret;
    unsigned char ch, invokeID;
    DLMS_GET_COMMAND_TYPE type = DLMS_GET_COMMAND_TYPE_NEXT_DATA_BLOCK;
    //If GBT is used data is empty.
    if (data.GetSize() != 0)
    {
        // Get type.
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        type = (DLMS_GET_COMMAND_TYPE)ch;
        // Get invoke ID and priority.
        if ((ret = data.GetUInt8(&invokeID)) != 0)
        {
            return ret;
        }
        settings.UpdateInvokeId(invokeID);
        if (xml != NULL)
        {
            std::string tmp;
            xml->AppendStartTag(DLMS_COMMAND_GET_REQUEST);
            xml->AppendStartTag(DLMS_COMMAND_GET_REQUEST, type);
            xml->IntegerToHex((long)invokeID, 2, tmp);
            xml->AppendLine((unsigned long)DLMS_TRANSLATOR_TAGS_INVOKE_ID, "", tmp);
        }
    }
    // GetRequest normal
    if (type == DLMS_GET_COMMAND_TYPE_NORMAL)
    {
        ret = GetRequestNormal(settings, invokeID, server, data, replyData, xml, cipheredCommand);
    }
    else if (type == DLMS_GET_COMMAND_TYPE_NEXT_DATA_BLOCK)
    {
        // Get request for next data block
        ret = GetRequestNextDataBlock(settings, invokeID, server, data, replyData, xml, false, cipheredCommand);
    }
    else if (type == DLMS_GET_COMMAND_TYPE_WITH_LIST)
    {
        // Get request with a list.
        ret = GetRequestWithList(settings, invokeID, server, data, replyData, xml, cipheredCommand);
    }
    else
    {
        CGXByteBuffer bb;
        settings.ResetBlockIndex();
        // Access Error : Device reports a hardware fault.
        bb.SetUInt8(DLMS_ERROR_CODE_HARDWARE_FAULT);
        CGXDLMSLNParameters p(&settings, invokeID, DLMS_COMMAND_GET_RESPONSE,
            type, NULL, &bb, DLMS_ERROR_CODE_OK, cipheredCommand);
        ret = CGXDLMS::GetLNPdu(p, *replyData);
    }
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_COMMAND_GET_REQUEST, (unsigned long)type);
        xml->AppendEndTag(DLMS_COMMAND_GET_REQUEST);
    }
    return ret;
}

int CGXDLMSLNCommandHandler::HandleSetRequestNormal(
    CGXDLMSSettings& settings,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    short type,
    CGXDLMSLNParameters& p,
    CGXDLMSTranslatorStructure* xml)
{
    CGXDataInfo i;
    CGXDLMSVariant value;
    std::string str;
    int ret;
    unsigned char index, ch, lastBlock;
    unsigned short tmp;
    // Get CI.
    if ((ret = data.GetUInt16(&tmp)) != 0)
    {
        return ret;
    }
    DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE)tmp;
    unsigned char* ln;
    ln = data.GetData() + data.GetPosition();
    data.SetPosition(data.GetPosition() + 6);
    // Attribute index.
    if ((ret = data.GetUInt8(&index)) != 0)
    {
        return ret;
    }
    // Get Access Selection.
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (type == 2)
    {
        unsigned long size, blockNumber;
        if ((ret = data.GetUInt8(&lastBlock)) != 0)
        {
            return ret;
        }
        p.SetMultipleBlocks(lastBlock == 0);
        ret = data.GetUInt32(&blockNumber);
        if (ret != 0)
        {
            return ret;
        }
        if (blockNumber != settings.GetBlockIndex())
        {
            p.SetStatus(DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID);
            return 0;
        }
        settings.IncreaseBlockIndex();
        ret = GXHelpers::GetObjectCount(data, size);
        if (ret != 0)
        {
            return ret;
        }
        unsigned long realSize = data.GetSize() - data.GetPosition();
        if (size != realSize)
        {
            p.SetStatus(DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE);
            return 0;
        }
        if (xml != NULL)
        {
            AppendAttributeDescriptor(xml, ci, ln, index);
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_DATA_BLOCK);
            xml->IntegerToHex((long)lastBlock, 2, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_LAST_BLOCK, "", str);
            xml->IntegerToHex(blockNumber, 8, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_BLOCK_NUMBER, "", str);
            str = data.ToHexString(data.GetPosition(), data.Available(), false);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_RAW_DATA, "", str);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_DATA_BLOCK);
        }
        return 0;
    }
    if (xml != NULL)
    {
        AppendAttributeDescriptor(xml, (int)ci, ln, index);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_VALUE);
        CGXDLMSVariant value;
        CGXDataInfo di;
        di.SetXml(xml);
        if ((ret = GXHelpers::GetData(data, di, value)) != 0)
        {
            return ret;
        }
        if (!di.IsComplete())
        {
            value = data.ToHexString(data.GetPosition(), data.Available(), false);
        }
        else if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            value = GXHelpers::BytesToHex(value.byteArr, value.GetSize(), false);
        }
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_VALUE);
        return 0;
    }
    if (!p.IsMultipleBlocks())
    {
        settings.ResetBlockIndex();
        ret = GXHelpers::GetData(data, i, value);
        if (ret != 0)
        {
            return ret;
        }
    }

    CGXDLMSObject* obj = settings.GetObjects().FindByLN(ci, ln);
    if (obj == NULL)
    {
        std::string name;
        GXHelpers::GetLogicalName(ln, name);
        obj = server->FindObject(ci, 0, name);
    }
    // If target is unknown.
    if (obj == NULL)
    {
        // Device reports a undefined object.
        p.SetStatus(DLMS_ERROR_CODE_UNAVAILABLE_OBJECT);
    }
    else
    {
        CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(server, obj, index);
        e->SetValue(value);
        CGXDLMSValueEventCollection list;
        list.push_back(e);
        DLMS_ACCESS_MODE am = server->GetAttributeAccess(e);
        // If write is denied.
        if (am != DLMS_ACCESS_MODE_WRITE && am != DLMS_ACCESS_MODE_READ_WRITE)
        {
            //Read Write denied.
            p.SetStatus(DLMS_ERROR_CODE_READ_WRITE_DENIED);
        }
        else
        {
            if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                DLMS_DATA_TYPE dt;
                ret = obj->GetDataType(index, dt);
                if (ret != 0)
                {
                    return ret;
                }
                if (dt != DLMS_DATA_TYPE_NONE && dt != DLMS_DATA_TYPE_OCTET_STRING)
                {
                    CGXByteBuffer tmp;
                    tmp.Set(value.byteArr, value.GetSize());
                    value.Clear();
                    if ((ret = CGXDLMSClient::ChangeType(tmp, dt, value)) != 0)
                    {
                        return ret;
                    }
                }
            }
            if (p.IsMultipleBlocks())
            {
                server->m_Transaction = new CGXDLMSLongTransaction(list, DLMS_COMMAND_GET_REQUEST, data);
            }
            server->PreWrite(list);
            if (e->GetError() != 0)
            {
                p.SetStatus(e->GetError());
            }
            else if (!e->GetHandled() && !p.IsMultipleBlocks())
            {
                obj->SetValue(settings, *e);
                server->PostWrite(list);
            }
        }
    }
    return ret;
}

int CGXDLMSLNCommandHandler::HanleSetRequestWithDataBlock(
    CGXDLMSSettings& settings,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    CGXDLMSLNParameters& p,
    CGXDLMSTranslatorStructure* xml)
{
    CGXDataInfo reply;
    int ret;
    unsigned long blockNumber, size;
    unsigned char lastBlock;
    if ((ret = data.GetUInt8(&lastBlock)) != 0)
    {
        return ret;
    }
    p.SetMultipleBlocks(lastBlock == 0);
    if ((ret = data.GetUInt32(&blockNumber)) != 0)
    {
        return ret;
    }
    if (xml == NULL && blockNumber != settings.GetBlockIndex())
    {
        p.SetStatus(DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID);
    }
    else
    {
        settings.IncreaseBlockIndex();
        if ((ret = GXHelpers::GetObjectCount(data, size)) != 0)
        {
            return ret;
        }
        unsigned long realSize = data.GetSize() - data.GetPosition();
        if (size != realSize)
        {
            p.SetStatus(DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE);
        }
        if (xml != NULL)
        {
            std::string str;
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_DATA_BLOCK);
            xml->IntegerToHex((long)lastBlock, 2, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_LAST_BLOCK, "", str);
            xml->IntegerToHex(blockNumber, 8, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_BLOCK_NUMBER, "", str);
            str = data.ToHexString(data.GetPosition(), data.Available(), false);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_RAW_DATA, "", str);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_DATA_BLOCK);
            return 0;
        }
        server->m_Transaction->GetData().Set(&data, data.GetPosition());
        // If all data is received.
        if (!p.IsMultipleBlocks())
        {
            CGXDLMSVariant value;
            if ((ret != GXHelpers::GetData(server->m_Transaction->GetData(), reply, value)) != 0)
            {
                return ret;
            }
            CGXDLMSValueEventArg* target = *server->m_Transaction->GetTargets().begin();
            if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                DLMS_DATA_TYPE dt;
                ret = target->GetTarget()->GetDataType(target->GetIndex(), dt);
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
            target->SetValue(value);
            server->PreWrite(server->m_Transaction->GetTargets());
            if (!target->GetHandled() && !p.IsMultipleBlocks())
            {
                target->GetTarget()->SetValue(settings, *target);
                server->PostWrite(server->m_Transaction->GetTargets());
            }
            if (server->m_Transaction != NULL)
            {
                delete server->m_Transaction;
                server->m_Transaction = NULL;
            }
            settings.ResetBlockIndex();
        }
    }
    p.SetMultipleBlocks(true);
    return 0;
}

int CGXDLMSLNCommandHandler::HanleSetRequestWithList(
    CGXDLMSSettings& settings,
    unsigned char,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    CGXDLMSLNParameters& p,
    CGXDLMSTranslatorStructure* xml,
    unsigned char)
{
    std::string str;
    int ret;
    unsigned long cnt;
    unsigned char attributeIndex, selection, selector;
    DLMS_OBJECT_TYPE ci;
    unsigned short tmp;
    unsigned char* ln;
    CGXDLMSVariant parameters;
    if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    std::map<unsigned short, unsigned char> status;
    if (xml != NULL)
    {
        xml->IntegerToHex(cnt, 2, str);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_LIST, "Qty", str);
    }
    for (unsigned short pos = 0; pos != cnt; ++pos)
    {
        status[pos] = 0;
        if ((ret = data.GetUInt16(&tmp)) != 0)
        {
            return ret;
        }
        ci = (DLMS_OBJECT_TYPE)tmp;
        ln = data.GetData() + data.GetPosition();
        data.SetPosition(data.GetPosition() + 6);
        // Attribute Id
        if ((ret = data.GetUInt8(&attributeIndex)) != 0)
        {
            return ret;
        }

        // AccessSelection
        if ((ret = data.GetUInt8(&selection)) != 0)
        {
            return ret;
        }
        if (selection != 0)
        {
            if ((ret = data.GetUInt8(&selector)) != 0)
            {
                return ret;
            }
            CGXDataInfo info;
            if ((ret = GXHelpers::GetData(data, info, parameters)) != 0)
            {
                return ret;
            }
        }
        else
        {
            selector = 0;
        }
        if (xml != NULL)
        {
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_WITH_SELECTION);
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR);
            xml->AppendComment(CGXDLMSConverter::ToString(ci));
            xml->IntegerToHex((long)ci, 4, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_CLASS_ID, "", str);
            GXHelpers::GetLogicalName(ln, str);
            xml->AppendComment(str);
            str = GXHelpers::BytesToHex(ln, 6, false);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_INSTANCE_ID, "", str);
            xml->IntegerToHex((long)attributeIndex, 2, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_ID, "", str);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_WITH_SELECTION);
        }
        else
        {
            CGXDLMSObject* obj = settings.GetObjects().FindByLN(ci, ln);
            if (obj == NULL)
            {
                std::string name;
                GXHelpers::GetLogicalName(ln, name);
                obj = server->FindObject(ci, 0, name);
            }
            if (obj == NULL)
            {
                // Access Error : Device reports a undefined object.
                status[pos] = DLMS_ERROR_CODE_UNDEFINED_OBJECT;
            }
            else
            {
                CGXDLMSValueEventArg e (server, obj, attributeIndex, selector, parameters);
                if ((server->GetAttributeAccess(&e) & DLMS_ACCESS_MODE_WRITE) == 0)
                {
                    status[pos] = DLMS_ERROR_CODE_READ_WRITE_DENIED;
                }
            }
        }
    }
    if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_LIST);
        xml->IntegerToHex(cnt, 2, str);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_VALUE_LIST, "Qty", str);
    }
    for (unsigned short pos = 0; pos != cnt; ++pos)
    {
        CGXDLMSVariant value;
        CGXDataInfo di;
        di.SetXml(xml);
        if (xml != NULL && xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendStartTag(DLMS_COMMAND_WRITE_REQUEST, DLMS_SINGLE_READ_RESPONSE_DATA);
        }
        if ((ret = GXHelpers::GetData(data, di, value)) != 0)
        {
            status[pos] = DLMS_ERROR_CODE_READ_WRITE_DENIED;
        }
        if (ret == 0)
        {
            if (!di.IsComplete())
            {
                value = data.ToHexString(data.GetPosition(), data.Available(), false);
            }
            else if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                value = GXHelpers::BytesToHex(value.byteArr, value.GetSize(), false);
            }
            if (xml != NULL && xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
            {
                xml->AppendEndTag(DLMS_COMMAND_WRITE_REQUEST, (unsigned long)DLMS_SINGLE_READ_RESPONSE_DATA);
            }
        }
    }
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_VALUE_LIST);
    }
    else
    {
        p.SetStatus(0xFF);
        GXHelpers::SetObjectCount((unsigned long) status.size(), *p.GetAttributeDescriptor());
        for(std::map<unsigned short, unsigned char >::iterator it = status.begin(); it != status.end(); ++it)
        {
            p.GetAttributeDescriptor()->SetUInt8(it->second);
        }
        p.SetRequestType(DLMS_SET_RESPONSE_TYPE_WITH_LIST);
    }
    return 0;
}

int CGXDLMSLNCommandHandler::HandleSetRequest(
    CGXDLMSSettings& settings,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    CGXByteBuffer* replyData,
    CGXDLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    CGXDLMSVariant value;
    unsigned char type, invoke;
    int ret;
    CGXDataInfo i;
    CGXByteBuffer bb;
    // Return error if connection is not established.
    if (xml == NULL && (settings.GetConnected() && DLMS_CONNECTION_STATE_DLMS) == 0)
    {
        server->GenerateConfirmedServiceError(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR,
            DLMS_SERVICE_ERROR_SERVICE, DLMS_SERVICE_UNSUPPORTED,
            *replyData);
        return 0;
    }
    // Get type.
    if ((ret = data.GetUInt8(&type)) != 0)
    {
        return ret;
    }
    // Get invoke ID and priority.
    if ((ret = data.GetUInt8(&invoke)) != 0)
    {
        return ret;
    }
    settings.UpdateInvokeId(invoke);
    if (xml != NULL) {
        xml->AppendStartTag(DLMS_COMMAND_SET_REQUEST);
        xml->AppendStartTag(DLMS_COMMAND_SET_REQUEST, type);
        // InvokeIdAndPriority
        std::string str;
        xml->IntegerToHex((long)invoke, 2, str);
        xml->AppendLine(DLMS_TRANSLATOR_TAGS_INVOKE_ID, "", str);
    }

    CGXByteBuffer attributeDescriptor;
    CGXDLMSLNParameters p(&settings, invoke, DLMS_COMMAND_SET_RESPONSE, type, &attributeDescriptor, NULL, 0, cipheredCommand);
    if (type == DLMS_SET_COMMAND_TYPE_NORMAL || type == DLMS_SET_COMMAND_TYPE_FIRST_DATABLOCK)
    {
        ret = HandleSetRequestNormal(settings, server, data, type, p, xml);
    }
    else if (type == DLMS_SET_COMMAND_TYPE_WITH_DATABLOCK)
    {
        // Set Request With Data Block
        ret = HanleSetRequestWithDataBlock(settings, server, data, p, xml);
    }
    else if (type == DLMS_SET_COMMAND_TYPE_WITH_LIST)
    {
        // Set Request With Data Block
        ret = HanleSetRequestWithList(settings, invoke, server, data, p, xml, cipheredCommand);
    }
    else
    {
        settings.ResetBlockIndex();
        p.SetStatus(DLMS_ERROR_CODE_HARDWARE_FAULT);
    }
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_COMMAND_SET_REQUEST, (unsigned long)type);
        xml->AppendEndTag(DLMS_COMMAND_SET_REQUEST);
        return 0;
    }
    return CGXDLMS::GetLNPdu(p, *replyData);
}


/**
 * Handle action request.
 *
 * @param reply
 *            Received data from the client.
 * @return Reply.
 */
int CGXDLMSLNCommandHandler::HandleMethodRequest(
    CGXDLMSSettings& settings,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    CGXByteBuffer* replyData,
    CGXDLMSConnectionEventArgs* connectionInfo,
    CGXDLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    CGXDLMSValueEventArg* e = NULL;
    CGXByteBuffer bb;
    DLMS_ERROR_CODE error = DLMS_ERROR_CODE_OK;
    CGXDLMSVariant parameters;
    int ret;
    unsigned char id, invokeId, selection, type;
    unsigned short tmp;
    // Get type.
    if ((ret = data.GetUInt8(&type)) != 0)
    {
        return ret;
    }
    // Get invoke ID and priority.
    if ((ret = data.GetUInt8(&invokeId)) != 0)
    {
        return ret;
    }
    settings.UpdateInvokeId(invokeId);
    // CI
    if ((ret = data.GetUInt16(&tmp)) != 0)
    {
        return ret;
    }
    DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE)tmp;
    unsigned char* ln;
    ln = data.GetData() + data.GetPosition();
    data.SetPosition(data.GetPosition() + 6);
    // Attribute ID
    if ((ret = data.GetUInt8(&id)) != 0)
    {
        return ret;
    }
    if ((ret = data.GetUInt8(&selection)) != 0)
    {
        return ret;
    }
    if (xml != NULL)
    {
        xml->AppendStartTag(DLMS_COMMAND_METHOD_REQUEST);
        if (type == DLMS_ACTION_REQUEST_TYPE_NORMAL)
        {
            std::string str;
            xml->AppendStartTag(DLMS_COMMAND_METHOD_REQUEST, DLMS_ACTION_REQUEST_TYPE_NORMAL);
            xml->IntegerToHex((unsigned long)invokeId, 2, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_INVOKE_ID, "", str);
            AppendMethodDescriptor(xml, (int)ci, ln, id);
            if (selection != 0)
            {
                CGXDLMSVariant value;
                //MethodInvocationParameters
                xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_METHOD_INVOCATION_PARAMETERS);
                CGXDataInfo di;
                di.SetXml(xml);
                if ((ret = GXHelpers::GetData(data, di, value)) != 0)
                {
                    return ret;
                }
                xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_METHOD_INVOCATION_PARAMETERS);
            }
            xml->AppendEndTag(DLMS_COMMAND_METHOD_REQUEST, (unsigned long)DLMS_ACTION_REQUEST_TYPE_NORMAL);
        }
        xml->AppendEndTag(DLMS_COMMAND_METHOD_REQUEST);
        return 0;
    }
    // Get parameters.
    if (selection != 0)
    {
        CGXDataInfo i;
        if ((ret = GXHelpers::GetData(data, i, parameters)) != 0)
        {
            return ret;
        }
    }
    CGXDLMSObject* obj = settings.GetObjects().FindByLN(ci, ln);
    if (obj == NULL)
    {
        std::string name;
        GXHelpers::GetLogicalName(ln, name);
        obj = server->FindObject(ci, 0, name);
    }
    if (obj == NULL)
    {
        error = DLMS_ERROR_CODE_UNDEFINED_OBJECT;
    }
    else
    {
        e = new CGXDLMSValueEventArg(server, obj, id, 0, parameters);
        CGXDLMSValueEventCollection arr;
        arr.push_back(e);
        if (server->GetMethodAccess(e) == DLMS_METHOD_ACCESS_MODE_NONE)
        {
            error = DLMS_ERROR_CODE_READ_WRITE_DENIED;
        }
        else
        {
            server->PreAction(arr);
            if (!e->GetHandled())
            {
                if ((ret = obj->Invoke(settings, *e)) != 0)
                {
                    return ret;
                }
                server->PostAction(arr);
            }
            CGXDLMSVariant& actionReply = e->GetValue();
            // Set default action reply if not given.
            if (actionReply.vt != DLMS_DATA_TYPE_NONE && e->GetError() == DLMS_ERROR_CODE_OK)
            {
                // Add return parameters
                bb.SetUInt8(1);
                //Add parameters error code.
                bb.SetUInt8(0);
                GXHelpers::SetData(bb, actionReply.vt, actionReply);
            }
            else
            {
                // Add parameters error code.
                error = e->GetError();
                bb.SetUInt8(0);
            }
        }
    }
    CGXDLMSLNParameters p(&settings, invokeId, DLMS_COMMAND_METHOD_RESPONSE, 1, NULL, &bb, error, cipheredCommand);
    ret = CGXDLMS::GetLNPdu(p, *replyData);
    if (error == 0 && obj->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME && id == 1)
    {
        if (((CGXDLMSAssociationLogicalName*)obj)->GetAssociationStatus() == DLMS_ASSOCIATION_STATUS_ASSOCIATED)
        {
            server->Connected(*connectionInfo);
            settings.SetConnected((DLMS_CONNECTION_STATE)(settings.GetConnected() | DLMS_CONNECTION_STATE_DLMS));
        }
        else
        {
            // If High level authentication fails.
            server->InvalidConnection(*connectionInfo);
            settings.SetConnected((DLMS_CONNECTION_STATE)(settings.GetConnected() & ~DLMS_CONNECTION_STATE_DLMS));
        }
    }
    if (e != NULL && error == 0 && obj->GetObjectType() == DLMS_OBJECT_TYPE_SECURITY_SETUP && id == 2)
    {
        ((CGXDLMSSecuritySetup*)obj)->ApplyKeys(settings, *e);
    }
    return ret;
}

int CGXDLMSLNCommandHandler::HandleAccessRequest(
    CGXDLMSSettings& settings,
    CGXDLMSServer* server,
    CGXByteBuffer& data,
    CGXByteBuffer* replyData,
    CGXDLMSTranslatorStructure* xml,
    unsigned char)
{
    //Return error if connection is not established.
    if (xml == NULL && settings.GetConnected() == DLMS_CONNECTION_STATE_NONE)
    {
        server->GenerateConfirmedServiceError(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR,
            DLMS_SERVICE_ERROR_SERVICE, DLMS_SERVICE_UNSUPPORTED,
            *replyData);
        return 0;
    }
    int ret;
    unsigned char ch;
    //Get long invoke id and priority.
    unsigned long invokeId;
    if ((ret = data.GetUInt32(&invokeId)) != 0)
    {
        return ret;
    }
    settings.SetLongInvokeID(invokeId);
    unsigned long len, cnt;
    if ((ret = GXHelpers::GetObjectCount(data, len)) != 0)
    {
        return ret;
    }

    CGXByteBuffer tmp;
    // If date time is given.
    if (len != 0)
    {
        tmp.Set(&data, data.GetPosition(), len);
        if (xml == NULL)
        {
            DLMS_DATA_TYPE dt = DLMS_DATA_TYPE_DATETIME;
            if (len == 4)
            {
                dt = DLMS_DATA_TYPE_TIME;
            }
            else if (len == 5)
            {
                dt = DLMS_DATA_TYPE_DATE;
            }
            CGXDataInfo info;
            CGXDLMSVariant value;
            info.SetType(dt);
            if ((ret = GXHelpers::GetData(data, info, value)) != 0)
            {
                return ret;
            }
        }
    }
    // Get object count.
    if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    if (xml != NULL)
    {
        std::string tmp2;
        xml->IntegerToHex(invokeId, 2, tmp2);
        xml->AppendStartTag(DLMS_COMMAND_ACCESS_REQUEST);
        xml->AppendLine(DLMS_TRANSLATOR_TAGS_LONG_INVOKE_ID, "Value", tmp2);
        tmp2 = tmp.ToHexString(false);
        xml->AppendLine(DLMS_TRANSLATOR_TAGS_DATE_TIME, "Value", tmp2);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_BODY);
        xml->IntegerToHex(cnt, 2, tmp2);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_LIST_OF_ACCESS_REQUEST_SPECIFICATION, "Qty", tmp2);
    }
    DLMS_ACCESS_SERVICE_COMMAND_TYPE type;
    unsigned short id;
    unsigned char* ln;
    unsigned char attributeIndex;
    for (unsigned long pos = 0; pos != cnt; ++pos)
    {
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        type = (DLMS_ACCESS_SERVICE_COMMAND_TYPE)ch;
        if (!(type == DLMS_ACCESS_SERVICE_COMMAND_TYPE_GET ||
            type == DLMS_ACCESS_SERVICE_COMMAND_TYPE_SET ||
            type == DLMS_ACCESS_SERVICE_COMMAND_TYPE_ACTION))
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        // CI
        if ((ret = data.GetUInt16(&id)) != 0)
        {
            return ret;
        }
        DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE)id;
        ln = data.GetData() + data.GetPosition();
        data.SetPosition(data.GetPosition() + 6);
        // Attribute Id
        if ((ret = data.GetUInt8(&attributeIndex)) != 0)
        {
            return ret;
        }
        if (xml != NULL)
        {
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_SPECIFICATION);
            xml->AppendStartTag(DLMS_COMMAND_ACCESS_REQUEST, type);
            AppendAttributeDescriptor(xml, (int)ci, ln, attributeIndex);
            xml->AppendEndTag(DLMS_COMMAND_ACCESS_REQUEST, (unsigned long)type);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_SPECIFICATION);

        }
    }
    if (xml != NULL)
    {
        std::string tmp2;
        xml->IntegerToHex(cnt, 2, tmp2);
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_LIST_OF_ACCESS_REQUEST_SPECIFICATION);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_LIST_OF_DATA, "Qty", tmp2);
    }
    // Get data count.
    if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    for (unsigned long pos = 0; pos != cnt; ++pos)
    {
        CGXDLMSVariant value;
        CGXDataInfo di;
        di.SetXml(xml);
        if (xml != NULL && xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendStartTag(DLMS_COMMAND_WRITE_REQUEST, DLMS_SINGLE_READ_RESPONSE_DATA);
        }
        if ((ret = GXHelpers::GetData(data, di, value)) != 0)
        {
            return ret;
        }
        if (!di.IsComplete())
        {
            value = data.ToHexString(data.GetPosition(), data.Available(), false);
        }
        else if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            value = GXHelpers::BytesToHex(value.byteArr, value.GetSize());
        }
        if (xml != NULL && xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendEndTag(DLMS_COMMAND_WRITE_REQUEST, (unsigned long)DLMS_SINGLE_READ_RESPONSE_DATA);
        }
    }
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_LIST_OF_DATA);
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_BODY);
        xml->AppendEndTag(DLMS_COMMAND_ACCESS_REQUEST);
    }
    return 0;
}

// Handle Event Notification.
int CGXDLMSLNCommandHandler::HandleEventNotification(
    CGXDLMSSettings&,
    CGXReplyData& reply)
{
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
        if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        tmp.Set(&reply.GetData(), reply.GetData().GetPosition(), ch);
        CGXDLMSVariant value;
        if ((ret = CGXDLMSClient::ChangeType(tmp, DLMS_DATA_TYPE_DATETIME, value)) != 0)
        {
            return ret;
        }
        reply.SetTime(&value.dateTime.GetValue());
    }
    if (reply.GetXml() != NULL)
    {
        reply.GetXml()->AppendStartTag(DLMS_COMMAND_EVENT_NOTIFICATION);
        if (reply.GetTime() != 0)
        {
            CGXDateTime dt(reply.GetTime());
            reply.GetXml()->AppendComment(dt.ToString());
            std::string str = tmp.ToHexString(false);
            reply.GetXml()->AppendLine(DLMS_TRANSLATOR_TAGS_TIME, "", str);
        }
    }
    unsigned short ci;
    if ((ret = reply.GetData().GetUInt16(&ci)) != 0)
    {
        return ret;
    }
    unsigned char* ln = reply.GetData().GetData() + reply.GetData().GetPosition();
    reply.GetData().SetPosition(reply.GetData().GetPosition() + 6);
    if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (reply.GetXml() != NULL)
    {
        AppendAttributeDescriptor(reply.GetXml(), ci, ln, ch);
        reply.GetXml()->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_VALUE);
    }
    CGXDataInfo di;
    CGXDLMSVariant value;
    di.SetXml(reply.GetXml());
    if ((ret = GXHelpers::GetData(reply.GetData(), di, value)) != 0)
    {
        return ret;
    }
    if (reply.GetXml() != NULL)
    {
        reply.GetXml()->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_VALUE);
        reply.GetXml()->AppendEndTag(DLMS_COMMAND_EVENT_NOTIFICATION);
    }
    return 0;
}
