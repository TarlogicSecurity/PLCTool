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

#ifndef GXDLMSVALUEEVENTARGS_H
#define GXDLMSVALUEEVENTARGS_H

#include "enums.h"
#include "errorcodes.h"
#include "GXDLMSVariant.h"

class CGXDLMSObject;
class CGXDLMSServer;
class CGXDLMSClient;
class CGXDLMSNotify;
class CGXDLMSSettings;
class CGXDLMSAssociationLogicalName;

class CGXDLMSValueEventArg
{
    friend class CGXDLMSClient;
    friend class CGXDLMSServer;
    friend class CGXDLMSNotify;
    friend class CGXDLMSProfileGeneric;
    friend class CGXDLMSAssociationLogicalName;
    friend class CGXDLMSAssociationShortName;
    friend class CGXDLMSLNCommandHandler;
    friend class CGXDLMSSNCommandHandler;
private:
    /**
    * CGXDLMSVariant value.
    */
    CGXDLMSVariant m_Value;
    /**
    * Is request handled.
    */
    bool m_Handled;
    /**
    * Target DLMS object
    */
    CGXDLMSObject* m_Target;
    /**
    * Attribute index.
    */
    int m_Index;
    /**
    * Optional selector.
    */
    int m_Selector;
    /**
    * Optional parameters.
    */
    CGXDLMSVariant m_Parameters;

    /**
     * Occurred error.
     */
    DLMS_ERROR_CODE m_Error;
    /**
    * Is action. This is reserved for internal use.
    */
    bool m_Action;

    /**
    * Is data handled as byte array.
    */
    bool m_ByteArray;

    /**
    * Is value max PDU size skipped.
    */
    bool m_SkipMaxPduSize;

    /**
    * Row to PDU is used with Profile Generic to tell how many rows are fit to
    * one PDU.
    */
    unsigned short m_RowToPdu;
    /**
    * Rows begin index.
    */
    unsigned short m_RowBeginIndex;
    /**
    * Rows end index.
    */
    unsigned short m_RowEndIndex;

    /**
    * DLMS settings.
    */
    CGXDLMSSettings* m_Settings;

    /**
    * DLMS server.
    */
    CGXDLMSServer* m_Server;

    /**
    * Received invoke ID.
    */
    unsigned int m_InvokeId;

    void Init(
        CGXDLMSServer* server,
        CGXDLMSObject* target,
        int index,
        int selector);

    /**
    * Constructor.
    *
    * @param target
    *            Event target.
    * @param index
    *            Event index.
    */
    CGXDLMSValueEventArg(
        CGXDLMSServer* server,
        CGXDLMSObject* target,
        int index);

    /**
    * Constructor.
    *
    * @param target
    *            Event target.
    * @param index
    *            Event index.
    * @param selector
    *            Optional read event selector.
    * @param parameters
    *            Optional parameters.
    */
    CGXDLMSValueEventArg(
        CGXDLMSServer* server,
        CGXDLMSObject* target,
        int index,
        int selector,
        CGXDLMSVariant& parameters);

    /**
    * DLMS server.
    */
    CGXDLMSServer* GetServer();
public:
    /**
    * @return Target DLMS object.
    */
    CGXDLMSObject* GetTarget();

    void SetTarget(CGXDLMSObject* value);

    /**
    * @return Attribute index of queried object.
    */
    int GetIndex();

    void SetIndex(int value);

    /**
    * @return CGXDLMSVariant value.
    */
    CGXDLMSVariant& GetValue();

    /**
    * @param value
    *            CGXDLMSVariant value.
    */
    void SetValue(CGXDLMSVariant value);

    /**
    * @return Is request handled.
    */
    bool GetHandled();

    /**
    * @param value
    *            Is request handled.
    */
    void SetHandled(bool value);

    /**
    * @return Optional selector.
    */
    int GetSelector();

    /**
    * @param value
    *           Selector.
    */
    void SetSelector(int value);

    /**
    * @return Optional parameters.
    */
    CGXDLMSVariant& GetParameters();

    /**
    * @param value
    *           Parameters.
    */
    void SetParameters(CGXDLMSVariant& value);


    /**
    * Constructor.
    *
    * @param target
    *            Event target.
    * @param index
    *            Event index.
    */
    CGXDLMSValueEventArg(
        CGXDLMSObject* target,
        int index);

    /**
    * Constructor.
    *
    * @param target
    *            Event target.
    * @param index
    *            Event index.
    * @param selector
    *            Optional read event selector.
    * @param parameters
    *            Optional parameters.
    */
    CGXDLMSValueEventArg(
        CGXDLMSObject* target,
        int index,
        int selector,
        CGXDLMSVariant& parameters);

    /**
    * @return Occurred error.
    */
    DLMS_ERROR_CODE GetError();

    /**
     * @param error
     *            Occurred error.
     */
    void SetError(DLMS_ERROR_CODE value);

    /**
    * @return Is action.
    */
    bool IsAction();

    /**
    * @param value
    *            Is action.
    */
    void SetAction(bool value);

    /**
       * @return Is byte array.
       */
    bool IsByteArray();

    /**
    * @param value
    *            Is byte array.
    */
    void SetByteArray(bool value);

    /**
    * @return Is value max PDU size skipped.
    */
    bool GetSkipMaxPduSize();

    /**
     * @param value
     *            Is value max PDU size skipped.
     */
    void SetSkipMaxPduSize(bool value);


    /**
    * @return How many rows are read to one PDU.
    */
    unsigned short GetRowToPdu();

    /**
    * @param value
    *            How many rows are read to one PDU.
    */
    void SetRowToPdu(unsigned short value);

    /**
    * @return Rows end index.
    */
    unsigned int GetRowEndIndex();

    /**
    * @param value
    *            Rows end index.
    */
    void SetRowEndIndex(unsigned int value);

    /**
    * @return Rows begin index.
    */
    unsigned int GetRowBeginIndex();

    /**
    * @param value
    *            Rows begin index.
    */
    void SetRowBeginIndex(unsigned int value);

    /**
    * DLMS settings.
    */
    CGXDLMSSettings* GetSettings();

    /**
    * @param value
    *            Received invoke ID.
    */
    void SetInvokeId(unsigned int value);

    /**
    * @return Received invoke ID.
    */
    unsigned int GetInvokeId();
};
#endif //GXDLMSVALUEEVENTARGS_H
