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

#ifndef GXDLMSCLIENT_H
#define GXDLMSCLIENT_H

#include "GXDLMS.h"
#include "GXStandardObisCodeCollection.h"
#include "GXDLMSProfileGeneric.h"
#include "GXSecure.h"
#include "GXDateTime.h"

class CGXDLMSClient
{
protected:
    friend class CGXDLMSSchedule;
    CGXDLMSSettings m_Settings;
private:
    // If protected release is used release is including a ciphered xDLMS Initiate request.
    bool m_UseProtectedRelease;
    bool m_IsAuthenticationRequired;
    static void UpdateOBISCodes(CGXDLMSObjectCollection& objects);
    // SN referencing
    int ParseSNObjects(
        CGXByteBuffer& buff,
        bool onlyKnownObjects,
        bool ignoreInactiveObjects);

    //Parse SN object item.
    int ParseSNObjectItem(CGXDLMSVariant& value, bool ignoreInactiveObjects);
    //Parse LN object item.
    int ParseLNObjectItem(CGXDLMSVariant& value, bool ignoreInactiveObjects);

    /**
    * Parse LN objects.
    *
    * buff
    *            Byte stream where objects are parsed.
    * onlyKnownObjects
    *            Only known objects are parsed.
    * @return Collection of COSEM objects.
    */
    int ParseLNObjects(
        CGXByteBuffer& buff,
        bool onlyKnownObjects,
        bool ignoreInactiveObjects);

    /**
    * Generates a read message.
    *
    * name : Short or Logical Name.
    * objectType : COSEM object type.
    * attributeOrdinal : Attribute index of the object.
    * data : Read data parameter.
    * reply : Generated read message(s).
    * Returns error status.
    */
    int Read(
        CGXDLMSVariant& name,
        DLMS_OBJECT_TYPE objectType,
        int attributeOrdinal,
        CGXByteBuffer* data,
        std::vector<CGXByteBuffer>& reply);

public:
    /////////////////////////////////////////////////////////////////////////////
    //Constructor
    /////////////////////////////////////////////////////////////////////////////
    CGXDLMSClient(
        bool UseLogicalNameReferencing = true,
        int ClientAddress = 16,
        int ServerAddress = 1,
        //Authentication type.
        DLMS_AUTHENTICATION authentication = DLMS_AUTHENTICATION_NONE,
        //Password if authentication is used.
        const char* password = NULL,
        DLMS_INTERFACE_TYPE intefaceType = DLMS_INTERFACE_TYPE_HDLC);

    /////////////////////////////////////////////////////////////////////////////
    //Destructor.
    /////////////////////////////////////////////////////////////////////////////
    ~CGXDLMSClient();

    bool GetUseLogicalNameReferencing();

    DLMS_INTERFACE_TYPE GetInterfaceType();

    /**
     * @return Used Priority.
     */
    DLMS_PRIORITY GetPriority();

    /**
     * @param value
     *            Used Priority.
     */
    void SetPriority(DLMS_PRIORITY value);

    /**
     * @return Used service class.
     */
    DLMS_SERVICE_CLASS GetServiceClass();

    /**
     * @param value
     *            Used service class.
     */
    void SetServiceClass(DLMS_SERVICE_CLASS value);


    /**
       * @return Invoke ID.
    */
    unsigned char GetInvokeID();

    /**
     * @param value
     *            Invoke ID.
     */
    void SetInvokeID(unsigned char value);

    /**
     * @return Auto increase Invoke ID.
     */
    bool GetAutoIncreaseInvokeID();

    /**
     * @param value
     *            Auto increase Invoke ID.
     */
    void SetAutoIncreaseInvokeID(bool value);

    // Gets used authentication.
    DLMS_AUTHENTICATION GetAuthentication();

    //Sets Used authentication.
    void SetAuthentication(DLMS_AUTHENTICATION value);

    // Gets client address.
    unsigned long GetClientAddress();

    // Sets client address.
    void SetClientAddress(unsigned long value);

    // Server address.
    unsigned long GetServerAddress();

    // Server address.
    void SetServerAddress(unsigned long value);

    // Maximum client PDU size.
    unsigned short GetMaxPduSize();

    // If protected release is used release is including a ciphered xDLMS Initiate request.
    bool GetUseProtectedRelease();

    // If protected release is used release is including a ciphered xDLMS Initiate request.
    void SetUseProtectedRelease(bool value);

    /////////////////////////////////////////////////////////////////////////////
    // Standard says that Time zone is from normal time to UTC in minutes.
    // If meter is configured to use UTC time (UTC to normal time) set this to true.
    bool GetUseUtc2NormalTime();
    void SetUseUtc2NormalTime(bool value);

    /////////////////////////////////////////////////////////////////////////////
    //User id is the identifier of the user.
    unsigned char GetUserID();
    void SetUserID(unsigned char value);

    /////////////////////////////////////////////////////////////////////////////
    //Quality of service.
    unsigned char GetQualityOfService();
    void SetQualityOfService(unsigned char value);

    /////////////////////////////////////////////////////////////////////////////
    //  Source system title.
    // Meter returns system title when ciphered connection is made or GMAC authentication is used.
    CGXByteBuffer& GetSourceSystemTitle();


    /////////////////////////////////////////////////////////////////////////////
    // Maximum client PDU size.
    int SetMaxReceivePDUSize(unsigned short value);
    unsigned short GetMaxReceivePDUSize();

    CGXDLMSLimits& GetLimits();

    /////////////////////////////////////////////////////////////////////////////
    // Collection of the objects.
    CGXDLMSObjectCollection& GetObjects();

    /////////////////////////////////////////////////////////////////////////////
    // Returns SNRMRequest query as byte array.
    /////////////////////////////////////////////////////////////////////////////
    // packets: Packets to send.
    // Returns: 0 if succeed. Otherwise error number.
    /////////////////////////////////////////////////////////////////////////////
    int SNRMRequest(
        std::vector<CGXByteBuffer>& packets);

    /////////////////////////////////////////////////////////////////////////////
    // Parses UAResponse.
    /////////////////////////////////////////////////////////////////////////////
    // data: Byte array containing the data from the meter.
    // Returns: 0 if succeed. Otherwise error number.
    int ParseUAResponse(
        CGXByteBuffer& data);

    /////////////////////////////////////////////////////////////////////////////
    // Returns AARQRequest query as byte array.
    /////////////////////////////////////////////////////////////////////////////
    // Packets: Packets to send.
    // Returns: 0 if succeed. Otherwise error number.
    /////////////////////////////////////////////////////////////////////////////
    int AARQRequest(
        std::vector<CGXByteBuffer>& packets);

    /**
    * Parses the AARE response. Parse method will update the following data:
    * <ul>
    * <li>DLMSVersion</li>
    * <li>MaxReceivePDUSize</li>
    * <li>UseLogicalNameReferencing</li>
    * <li>LNSettings or SNSettings</li>
    * </ul>
    * LNSettings or SNSettings will be updated, depending on the referencing,
    * Logical name or Short name.
    *
    * reply
    *            Received data.
    * @see GXDLMSClient#aarqRequest
    * @see GXDLMSClient#GetUseLogicalNameReferencing
    * @see GXDLMSClient#GetLNSettings
    * @see GXDLMSClient#GetSNSettings
    */
    int ParseAAREResponse(
        CGXByteBuffer& data);

    /**
    * @return Is authentication Required.
    */
    bool IsAuthenticationRequired();

    /**
     * @return Get challenge request if HLS authentication is used.
     */
    int GetApplicationAssociationRequest(
        std::vector<CGXByteBuffer>& packets);

    /**
     * Parse server's challenge if HLS authentication is used.
     *
     * @param reply
     *            Received reply from the server.
     */
    int ParseApplicationAssociationResponse(
        CGXByteBuffer& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Returns ReceiverReady query as byte array.
    /////////////////////////////////////////////////////////////////////////////
    // reply: Reply data.
    // Data: Data to send.
    // Returns: 0 if succeed. Otherwise error number.
    /////////////////////////////////////////////////////////////////////////////
    int ReceiverReady(
        CGXReplyData& reply,
        CGXByteBuffer& Data);

    /////////////////////////////////////////////////////////////////////////////
    // Returns ReceiverReady query as byte array.
    /////////////////////////////////////////////////////////////////////////////
    // Type: type of the next requested packet.
    // Data: Data to send.
    // Returns: 0 if succeed. Otherwise error number.
    /////////////////////////////////////////////////////////////////////////////
    int ReceiverReady(
        DLMS_DATA_REQUEST_TYPES Type,
        CGXByteBuffer& Data);

    /////////////////////////////////////////////////////////////////////////////
    // Changes byte array received from the meter to given type.
    /////////////////////////////////////////////////////////////////////////////
    // value: Byte array received from the meter.
    // type: Wanted type.
    // useUtc: Standard says that Time zone is from normal time to UTC in minutes.
    //         If meter is configured to use UTC time (UTC to normal time)
    //         set this to true.
    // returns Value changed by type.
    static int ChangeType(
        CGXByteBuffer& value,
        DLMS_DATA_TYPE type,
        bool useUtc,
        CGXDLMSVariant& newValue);

    /////////////////////////////////////////////////////////////////////////////
    // Changes byte array received from the meter to given type.
    /////////////////////////////////////////////////////////////////////////////
    // value Byte array received from the meter.
    // type Wanted type.
    // returns Value changed by type.
    static int ChangeType(
        CGXByteBuffer& value,
        DLMS_DATA_TYPE type,
        CGXDLMSVariant& newValue);

    /////////////////////////////////////////////////////////////////////////////
    // Changes byte array received from the meter to given type.
    /////////////////////////////////////////////////////////////////////////////
    // value Byte array received from the meter.
    // type Wanted type.
    // returns Value changed by type.
    static int ChangeType(
        CGXDLMSVariant& value,
        DLMS_DATA_TYPE type,
        CGXDLMSVariant& newValue);

    /**
    * Parses the COSEM objects of the received data.
    *
    * objects : Read objects.
    * objects : Collection of COSEM objects.
    * onlyKnownObjects : Only known objects are parsed.
    */
    int ParseObjects(
        std::vector<CGXDLMSVariant>& objects,
        bool onlyKnownObjects);

    /**
    * Parses the COSEM objects of the received data.
    *
    * objects : Read objects.
    * objects : Collection of COSEM objects.
    * onlyKnownObjects : Only known objects are parsed.
    * ignoreInactiveObjects : Inactivity objects are ignored.
    */
    int ParseObjects(
        std::vector<CGXDLMSVariant>& objects,
        bool onlyKnownObjects,
        bool ignoreInactiveObjects);

    /**
    * Parses the COSEM objects of the received data.
    *
    * data : Received data, from the device, as byte array.
    * objects : Collection of COSEM objects.
    * onlyKnownObjects : Only known objects are parsed.
    */
    int ParseObjects(
        CGXByteBuffer& data,
        bool onlyKnownObjects);

    /**
    * Parses the COSEM objects of the received data.
    *
    * data : Received data, from the device, as byte array.
    * objects : Collection of COSEM objects.
    * onlyKnownObjects : Only known objects are parsed.
    * ignoreInactiveObjects : Inactivity objects are ignored.
    */
    int ParseObjects(
        CGXByteBuffer& data,
        bool onlyKnownObjects,
        bool ignoreInactiveObjects);

    /*
    * Get Value from byte array received from the meter.
    */
    int UpdateValue(
        CGXDLMSObject& target,
        int attributeIndex,
        CGXDLMSVariant& value);

    /**
        * Get Value from byte array received from the meter.
        *
        * data
        *            Byte array received from the meter.
        * @return Received data.
        */
    int GetValue(
        CGXByteBuffer& data,
        CGXDLMSVariant& value);

    /**
    * Update list of values.
    *
    * list : List of read objects and atributes.
    * values :  Received values.
    */
    int UpdateValues(
        std::vector< std::pair<CGXDLMSObject*, unsigned char> >& list,
        std::vector<CGXDLMSVariant>& values);

    /**
    * Generates a release request.
    *
    * reply : Generated release message(s).
    * Returns error status.
    */
    int ReleaseRequest(
        std::vector<CGXByteBuffer>& packets);

    /**
    * Generates a disconnect request.
    *
    * reply : Generated disconnect message(s).
    * Returns error status.
    */
    int DisconnectRequest(
        std::vector<CGXByteBuffer>& packets);

    /**
    * Removes the HDLC frame from the packet, and returns COSEM data only.
    *
    * @param reply
    *            The received data from the device.
    * @param data
    *            The exported reply information.
    * @return Is frame complete.
    */
    int GetData(
        CGXByteBuffer& reply,
        CGXReplyData& data);

    /**
    * Removes the HDLC frame from the packet, and returns COSEM data only.
    *
    * @param reply
    *            The received data from the device.
    * @param data
    *            The exported reply information.
    * @param notify
    *            Information from the notify message.
    * @return Is frame complete.
    */
    int GetData(
        CGXByteBuffer& reply,
        CGXReplyData& data,
        CGXReplyData& notify);

    /**
    * Reads the Association view from the device. This method is used to get
    * all objects in the device.
    *
    * reply : Generated read message(s).
    * Returns error status.
    */
    int GetObjectsRequest(
        std::vector<CGXByteBuffer>& reply);

    /**
    Generates the keep alive message.

    Keepalive message is needed only HDLC framing.
    For keepalive we are reading logical name for Association object.
    This is done because all the meters can't handle HDLC keep alive message.
    */
    int GetKeepAlive(
        std::vector<CGXByteBuffer>& reply);


    /**
    * Generates a read message.
    *
    * name : Short or Logical Name.
    * objectType : COSEM object type.
    * attributeOrdinal : Attribute index of the object.
    * reply : Generated read message(s).
    * Returns error status.
    */
    int Read(
        CGXDLMSVariant& name,
        DLMS_OBJECT_TYPE objectType,
        int attributeOrdinal,
        std::vector<CGXByteBuffer>& reply);

    /**
    * Generates a read message.
    *
    * @param pObject
    *            COSEM object to write.
    * @param attributeOrdinal
    *            Attribute index of the object.
    * @param reply
    *            Generated read message(s).
    * Returns error status.
    */
    int Read(
        CGXDLMSObject* pObject,
        int attributeOrdinal,
        std::vector<CGXByteBuffer>& reply);

    /**
    * Read list of COSEM objects.
    *
    * @param list
    *            DLMS objects to read.
    * @return Read request as byte array.
    */
    int ReadList(
        std::vector<std::pair<CGXDLMSObject*, unsigned char> >& list,
        std::vector<CGXByteBuffer>& reply);

    /**
    * Write list of COSEM objects.
    *
    * @param list
    *            DLMS objects to read.
    * @return Write request as byte array.
    */
    int WriteList(
        std::vector<std::pair<CGXDLMSObject*, unsigned char> >& list,
        std::vector<CGXByteBuffer>& reply);

    /**
    * Generates a write message.
    *
    * @param name
    *            Short or Logical Name.
    * @param objectType
    *            Object type.
    * @param index
    *            Attribute index where data is write.
    * @param value
    *            Data to Write.
    * @param reply
    *             Generated write message(s).
    * Returns error status.
    */
    int Write(
        CGXDLMSVariant& name,
        DLMS_OBJECT_TYPE objectType,
        int index,
        CGXDLMSVariant& data,
        std::vector<CGXByteBuffer>& reply);

    /**
     * Generates a write message.
     *
     * @param pObject
     *            COSEM object to write.
     * @param index
     *            Attribute index where data is write.
     * @param data
     *            Data to Write.
     * @param reply
     *            Generated write message(s).
     * Returns error status.
     */
    int Write(
        CGXDLMSObject* pObject,
        int index,
        CGXDLMSVariant& data,
        std::vector<CGXByteBuffer>& reply);

    /**
    * Generates a write message.
    *
    * @param pObject
    *            COSEM object to write.
    * @param index
    *            Attribute index where data is write.
    * @param reply
    *            Generated write message(s).
    * Returns error status.
    */
    int Write(
        CGXDLMSObject* pObject,
        int index,
        std::vector<CGXByteBuffer>& reply);

    /**
    * Generate Method (Action) request.
    *
    * @param item
    *            Method object short name or Logical Name.
    * @param index
    *            Method index.
    * @param data
    *            Method data.
    * @param type
    *            Data type.
    * @return DLMS action message.
    */
    int Method(
        CGXDLMSObject* item,
        int index,
        CGXDLMSVariant& data,
        std::vector<CGXByteBuffer>& reply);


    /**
    * Generate Method (Action) request.
    *
    * @param item
    *            Method object short name or Logical Name.
    * @param index
    *            Method index.
    * @param data
    *            Method data.
    * @param type
    *            Data type.
    * @return DLMS action message.
    */
    int Method(
        CGXDLMSObject* item,
        int index,
        CGXDLMSVariant& data,
        DLMS_DATA_TYPE dataType,
        std::vector<CGXByteBuffer>& reply);

    /**
   * Generate Method (Action) request..
   *
   * @param name
   *            Method object short name or Logical Name.
   * @param objectType
   *            Object type.
   * @param methodIndex
   *            Method index.
   * @param value
   *            Method data.
   * @param dataType
   *            Data type.
   * @return DLMS action message.
   */
    int Method(
        CGXDLMSVariant name,
        DLMS_OBJECT_TYPE objectType,
        int methodIndex,
        CGXDLMSVariant& data,
        std::vector<CGXByteBuffer>& reply);

    /**
    * Generate Method (Action) request..
    *
    * @param name
    *            Method object short name or Logical Name.
    * @param objectType
    *            Object type.
    * @param methodIndex
    *            Method index.
    * @param value
    *            Method data.
    * @param dataType
    *            Data type.
    * @return DLMS action message.
    */
    int Method(
        CGXDLMSVariant name,
        DLMS_OBJECT_TYPE objectType,
        int methodIndex,
        CGXDLMSVariant& data,
        DLMS_DATA_TYPE dataType,
        std::vector<CGXByteBuffer>& reply);

    /**
    * Read rows by entry.
    *
    * @param pg
    *            Profile generic object to read.
    * @param index
    *            Zero bases start index.
    * @param count
    *            Rows count to read.
    * @return Read message as byte array.
    */
    int ReadRowsByEntry(
        CGXDLMSProfileGeneric* pg,
        int index,
        int count,
        std::vector<CGXByteBuffer>& reply);


    /**
    * Read rows by entry.
    *
    * @param pg
    *            Profile generic object to read.
    * @param index
    *            Zero bases start index.
    * @param count
    *            Rows count to read.
    * @return Read message as byte array.
    */
    int ReadRowsByEntry(
        CGXDLMSProfileGeneric* pg,
        int index,
        int count,
        std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >& columns,
        std::vector<CGXByteBuffer>& reply);


    /**
    * Read rows by range. Use this method to read Profile Generic table between
    * dates.
    *
    * @param pg
    *            Profile generic object to read.
    * @param start
    *            Start time.
    * @param end
    *            End time.
    * @return Generated read message.
    */
    int ReadRowsByRange(
        CGXDLMSProfileGeneric* pObject,
        CGXDateTime& start,
        CGXDateTime& end,
        std::vector<CGXByteBuffer>& reply);

    /**
     * Read rows by range. Use this method to read Profile Generic table between
     * dates.
     *
     * @param pg
     *            Profile generic object to read.
     * @param start
     *            Start time.
     * @param end
     *            End time.
     * @return Generated read message.
     */
    int ReadRowsByRange(
        CGXDLMSProfileGeneric* pg,
        struct tm* start,
        struct tm* end,
        std::vector<CGXByteBuffer>& reply);

    /**
    * Read rows by range. Use this method to read Profile Generic table between
    * dates.
    *
    * @param pg
    *            Profile generic object to read.
    * @param start
    *            Start time.
    * @param end
    *            End time.
    * @return Generated read message.
    */
    int ReadRowsByRange(
        CGXDLMSProfileGeneric* pg,
        struct tm* start,
        struct tm* end,
        std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >& columns,
        std::vector<CGXByteBuffer>& reply);


    /**
    *  Client will know what functionality server offers.
    *
    * @return Functionality.
    */
    DLMS_CONFORMANCE GetNegotiatedConformance();

    /**
    * Negotiated functionality for the server. Client can set this if meter report error value.
    *
    * @param value
    *            Proposed functionality.
    */
    void SetNegotiatedConformance(DLMS_CONFORMANCE value);

    /**
    * Proposed functionality for the server.
    * @return Proposed functionality.
    */
    DLMS_CONFORMANCE GetProposedConformance();

    /**
    * Proposed functionality for the server.
    *
    * @param value
    *            Proposed functionality.
    */
    void SetProposedConformance(DLMS_CONFORMANCE value);

    /**
     * Converts meter serial number to server address. Default formula is used.
     * All meters do not use standard formula or support serial number
     * addressing at all.
     *
     * @param serialNumber
     *            Meter serial number
     * @param formula
     *            Formula used to convert serial number to server address.
     *            Set to NULL if standard formula is used.
     * @return Server address.
     */
    static int GetServerAddress(
        unsigned long serialNumber,
        const char* formula = NULL);

    /**
     * Convert physical address and logical address to server address.
     *
     * @param logicalAddress
     *            Server logical address.
     * @param physicalAddress
     *            Server physical address.
     * @param addressSize
     *            Address size in bytes.
     * @return Server address.
     */
    static int  GetServerAddress(unsigned long logicalAddress,
        unsigned long physicalAddress,
        unsigned char addressSize = 0);

    /**
    * @return Protocol version.
    */
    char* GetProtocolVersion();

    /**
    * @param value
    *            Protocol version.
    */
    void SetProtocolVersion(char* value);

    //Convert object type enum value to string.
    static const std::string ObjectTypeToString(DLMS_OBJECT_TYPE type)
    {
        return CGXDLMSConverter::ToString(type);
    }

    //Parse push objects.
    int ParsePushObjects(
        std::vector<CGXDLMSVariant>& data,
        std::vector<std::pair<CGXDLMSObject*, unsigned char> >& items);

    /*
    Set custom challenge.
    */
    void SetCtoSChallenge(CGXByteBuffer& value);

    /*
    Get custom challenge.
    */
    CGXByteBuffer& GetCtoSChallenge();
};
#endif //GXDLMSCLIENT_H
