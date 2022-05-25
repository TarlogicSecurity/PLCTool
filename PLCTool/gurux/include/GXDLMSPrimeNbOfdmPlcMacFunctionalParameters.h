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

#ifndef GXDLMSPrimeNbOfdmPlcMacFunctionalParameters_H
#define GXDLMSPrimeNbOfdmPlcMacFunctionalParameters_H

#include "GXDLMSObject.h"


/**
 * Present functional state of the node.
 */
typedef enum
{
    /**
    * Disconnected.
    */
    DLMS_MAC_STATE_DISCONNECTED,
    /**
    * Terminal.
    */
    DLMS_MAC_STATE_TERMINAL,
    /**
    * Switch.
    */
    DLMS_MAC_STATE_SWITCH,
    /**
    * Base.
    */
    DLMS_MAC_STATE_BASE
}DLMS_MAC_STATE;


/**
 * Present functional state of the node.
 */
typedef enum
{
    DLMS_MAC_CAPABILITIES_NONE = 0,
    /**
    * Switch capable.
    */
    DLMS_MAC_CAPABILITIES_SWITCH_CAPABLE = 1,
    /**
    * Packet aggregation.
    */
    DLMS_MAC_CAPABILITIES_PACKET_AGGREGATION = 2,
    /**
    * Contention free period.
    */
    DLMS_MAC_CAPABILITIES_CONTENTION_FREE_PERIOD = 4,
    /**
    * Direct connection.
    */
    DLMS_MAC_CAPABILITIES_DIRECT_CONNECTION = 8,
    /**
    * Multicast.
    */
    DLMS_MAC_CAPABILITIES_MULTICAST = 0x10,
    /**
    * PHY Robustness Management.
    */
    DLMS_MAC_CAPABILITIES_PHY_ROBUSTNESS_MANAGEMENT = 0x20,
    /**
    * ARQ.
    */
    DLMS_MAC_CAPABILITIES_ARQ = 0x40,
    /**
    * Reserved for future use.
    */
    DLMS_MAC_CAPABILITIES_RESERVED_FOR_FUTURE_USE = 0x80,
    /**
    * Direct Connection Switching.
    */
    DLMS_MAC_CAPABILITIES_DIRECT_CONNECTION_SWITCHING = 0x100,
    /**
    * Multicast Switching Capability.
    */
    DLMS_MAC_CAPABILITIES_MULTICAST_SWITCHING_CAPABILITY = 0x200,
    /**
    * PHY Robustness Management Switching Capability.
    */
    DLMS_MAC_CAPABILITIES_PHY_ROBUSTNESS_MANAGEMENT_SWITCHING_CAPABILITY = 0x400,
    /**
    * ARQ Buffering Switching Capability.
    */
    DLMS_MAC_CAPABILITIES_ARQ_BUFFERING_SWITCHING_CAPABILITY = 0x800
}
DLMS_MAC_CAPABILITIES;

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSPrimeNbOfdmPlcMacFunctionalParameters
*/
class CGXDLMSPrimeNbOfdmPlcMacFunctionalParameters : public CGXDLMSObject
{
    /*
    * LNID allocated to this node at time of its registration.
    */
    short m_LnId;
    /*
     * LSID allocated to this node at the time of its promotion.
     */
    unsigned char m_LsId;

    /*
     * SID of the switch node through which this node is connected to the
     * subnetwork.
     */
    unsigned char m_SId;
    /*
     * Subnetwork address to which this node is registered.
     */
    CGXByteBuffer m_Sna;
    /*
     * Present functional state of the node.
     */
    DLMS_MAC_STATE m_State;

    /*
     * The SCP length, in symbols, in present frame.
     */
    short m_ScpLength;
    /*
     * Level of this node in subnetwork hierarchy.
     */
    unsigned char m_NodeHierarchyLevel;

    /*
     * Number of beacon slots provisioned in present frame structure.
     */
    unsigned char m_BeaconSlotCount;
    /*
     * Beacon slot in which this device’s switch node transmits its beacon.
     */
    unsigned char m_BeaconRxSlot;
    /*
     * Beacon slot in which this device transmits its beacon.
     */
    unsigned char m_BeaconTxSlot;
    /*
     * Number of frames between receptions of two successive beacons.
     */
    unsigned char m_BeaconRxFrequency;

    /*
     * Number of frames between transmissions of two successive beacons.
     */
    unsigned char m_BeaconTxFrequency;

    /*
     * This attribute defines the capabilities of the node.
     */
    DLMS_MAC_CAPABILITIES m_Capabilities;

public:
    //Constructor.
    CGXDLMSPrimeNbOfdmPlcMacFunctionalParameters();

    //SN Constructor.
    CGXDLMSPrimeNbOfdmPlcMacFunctionalParameters(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSPrimeNbOfdmPlcMacFunctionalParameters(std::string ln);

    /*
     * Returns LNID allocated to this node at time of its registration.
     */
    unsigned short GetLnId();

    /*
     * value: LNID allocated to this node at time of its registration.
     */
    void SetLnId(short value);

    /*
     * Returns LSID allocated to this node at the time of its promotion.
     */
    unsigned char GetLsId();

    /*
     * value:
     *            LSID allocated to this node at the time of its promotion.
     */
    void SetLsId(unsigned char value);

    /*
     * Returns SID of the switch node through which this node is connected to
     *         the subnetwork.
     */
    unsigned char GetSId();

    /*
     * value:
     *            SID of the switch node through which this node is connected to
     *            the subnetwork.
     */
    void SetSId(unsigned char value);

    /*
     * Returns Subnetwork address to which this node is registered.
     */
    CGXByteBuffer& GetSna();

    /*
     * value:
     *            Subnetwork address to which this node is registered.
     */
    void SetSna(CGXByteBuffer& value);

    /*
     * Returns Present functional state of the node.
     */
    DLMS_MAC_STATE GetState();

    /*
     * value:
     *            Present functional state of the node.
     */
    void SetState(DLMS_MAC_STATE value);

    /*
     * Returns The SCP length, in symbols, in present frame.
     */
    int GetScpLength();

    /*
     * value:
     *            The SCP length, in symbols, in present frame.
     */
    void SetScpLength(int value);

    /*
     * Returns Level of this node in subnetwork hierarchy.
     */
    unsigned char GetNodeHierarchyLevel();

    /*
     * value:
     *            Level of this node in subnetwork hierarchy.
     */
    void SetNodeHierarchyLevel(unsigned char value);

    /*
     * Returns Number of beacon slots provisioned in present frame structure.
     */
    unsigned char GetBeaconSlotCount();

    /*
     * value:
     *            Number of beacon slots provisioned in present frame structure.
     */
    void SetBeaconSlotCount(unsigned char value);

    /*
     * Returns Beacon slot in which this device’s switch node transmits its
     *         beacon.
     */
    unsigned char GetBeaconRxSlot();

    /*
     * value: Beacon slot in which this device’s switch node transmits its beacon.
     */
    void SetBeaconRxSlot(unsigned char value);

    /*
     * Returns Beacon slot in which this device transmits its beacon.
     */
    unsigned char GetBeaconTxSlot();

    /*
     * value:
     *            Beacon slot in which this device transmits its beacon.
     */
    void SetBeaconTxSlot(unsigned char value);

    /*
     * Returns Number of frames between receptions of two successive beacons.
     */
    unsigned char GetBeaconRxFrequency();

    /*
     * value:
     *            Number of frames between receptions of two successive beacons.
     */
    void SetBeaconRxFrequency(unsigned char value);

    /*
     * Returns Number of frames between transmissions of two successive beacons.
     */
    unsigned char GetBeaconTxFrequency();

    /*
     * value:
     *            Number of frames between transmissions of two successive
     *            beacons.
     */
    void SetBeaconTxFrequency(unsigned char value);

    /*
     * Returns This attribute defines the capabilities of the node.
     */
    DLMS_MAC_CAPABILITIES GetCapabilities();

    /*
     * value: This attribute defines the capabilities of the node.
    */
    void SetCapabilities(DLMS_MAC_CAPABILITIES value);

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
#endif //GXDLMSPrimeNbOfdmPlcMacFunctionalParameters_H