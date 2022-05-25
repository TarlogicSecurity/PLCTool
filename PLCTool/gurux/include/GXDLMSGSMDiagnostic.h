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

#ifndef GXDLMSGSMDIAGNOSTIC_H
#define GXDLMSGSMDIAGNOSTIC_H

#include "GXDLMSObject.h"
#include "GXAdjacentCell.h"
#include "GXDLMSGSMCellInfo.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSGSMDiagnostic
*/
class CGXDLMSGSMDiagnostic : public CGXDLMSObject
{
    /**
    * Name of network operator.
    */
    std::string m_Operator;

    /**
    * Registration status of the modem.
    */
    DLMS_GSM_STATUS m_Status;

    /**
    * Registration status of the modem.
    */
    DLMS_GSM_CIRCUIT_SWITCH_STATUS m_CircuitSwitchStatus;

    /**
    * Registration status of the modem.
    */
    DLMS_GSM_PACKET_SWITCH_STATUS m_PacketSwitchStatus;

    /**
    * Registration status of the modem.
    */
    CGXDLMSGSMCellInfo m_CellInfo;

    /**
    * Adjacent cells.
    */
    std::vector<GXAdjacentCell*> m_AdjacentCells;

    /**
    * Date and time when the data have been last captured.
    */
    CGXDateTime m_CaptureTime;
public:
    //Constructor.
    CGXDLMSGSMDiagnostic();

    //SN Constructor.
    CGXDLMSGSMDiagnostic(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSGSMDiagnostic(std::string ln);


    //Destructor.
    ~CGXDLMSGSMDiagnostic();

    /**
    * @return Name of network operator.
    */
    std::string& GetOperator();

    /**
    * @param value
    *            Name of network operator.
    */
    void SetOperator(std::string& value);

    /**
    * @return Registration status of the modem.
    */
    DLMS_GSM_STATUS GetStatus();

    /**
    * @param value
    *            Registration status of the modem.
    */
    void SetStatus(DLMS_GSM_STATUS value);

    /**
    * @return Registration status of the modem.
    */
    DLMS_GSM_CIRCUIT_SWITCH_STATUS GetCircuitSwitchStatus();

    /**
    * @param value
    *            Registration status of the modem.
    */
    void SetCircuitSwitchStatus(DLMS_GSM_CIRCUIT_SWITCH_STATUS value);

    /**
    * @return Registration status of the modem.
    */
    DLMS_GSM_PACKET_SWITCH_STATUS GetPacketSwitchStatus();

    /**
    * @param value
    *            Registration status of the modem.
    */
    void SetPacketSwitchStatus(DLMS_GSM_PACKET_SWITCH_STATUS value);

    CGXDLMSGSMCellInfo& GetCellInfo();

    /**
    * @return Adjacent cells.
    */
    std::vector<GXAdjacentCell*>& GetAdjacentCells();

    /**
    * @return Date and time when the data have been last captured.
    */
    CGXDateTime& GetCaptureTime();

    /**
    * @param value
    *            Date and time when the data have been last captured.
    */
    void SetCaptureTime(CGXDateTime& value);

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    // Returns value of given attribute.
    int GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);
};
#endif //GXDLMSGSMDIAGNOSTIC_H