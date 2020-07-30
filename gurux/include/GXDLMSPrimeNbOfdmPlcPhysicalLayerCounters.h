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

#ifndef GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters_H
#define GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters_H

#include "GXDLMSObject.h"

/*
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters
*/
class CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters : public CGXDLMSObject
{
    /*
     * Number of bursts received on the physical layer for which the CRC was
     * incorrect.
     */
    unsigned short m_CrcIncorrectCount;
    /*
     * Number of bursts received on the physical layer for which the CRC was
     * correct, but the Protocol field of PHY header had invalid value.
     */
    unsigned short m_CrcFailedCount;
    /*
     * Number of times when PHY layer received new data to transmit.
     */
    unsigned short m_TxDropCount;
    /*
     * Number of times when the PHY layer received new data on the channel.
     */
    unsigned short m_RxDropCount;
    int Invoke(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

public:
    //Constructor.
    CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters();

    //SN Constructor.
    CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters(std::string ln);

    /*
    * @return Number of bursts received on the physical layer for which the CRC
    *         was incorrect.
    */
    unsigned short GetCrcIncorrectCount();

    /*
     * @param value
     *            Number of bursts received on the physical layer for which the
     *            CRC was incorrect.
     */
    void SetCrcIncorrectCount(unsigned short value);

    /*
     * @return Number of bursts received on the physical layer for which the CRC
     *         was correct, but the Protocol field of PHY header had invalid
     *         value.
     */
    unsigned short GetCrcFailedCount();

    /*
     * @param value
     *            Number of bursts received on the physical layer for which the
     *            CRC was correct, but the Protocol field of PHY header had
     *            invalid value.
     */
    void SetCrcFailedCount(unsigned short value);

    /*
     * @return Number of times when PHY layer received new data to transmit.
     */
    unsigned short GetTxDropCount();

    /*
     * @param value
     *            Number of times when PHY layer received new data to transmit.
     */
    void SetTxDropCount(unsigned short value);

    /*
     * @return Number of times when the PHY layer received new data on the
     *         channel.
     */
    unsigned short GetRxDropCount();

    /*
     * @param value
     *            Number of times when the PHY layer received new data on the
     *            channel.
     */
    void SetRxDropCount(unsigned short value);

    //Reset the values.
    int Reset(CGXDLMSClient* client, std::vector<CGXByteBuffer>& reply);

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
#endif //GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters_H