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

#ifndef GXDLMSPRIMENBOFDMPLCMACSETUP_H
#define GXDLMSPRIMENBOFDMPLCMACSETUP_H

#include "GXDLMSObject.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSPrimeNbOfdmPlcMacSetup
*/
class CGXDLMSPrimeNbOfdmPlcMacSetup : public CGXDLMSObject
{
    /**
    * PIB attribute 0x0010.
    */
    unsigned char m_MacMinSwitchSearchTime;

    /**
     * PIB attribute 0x0011.
     */
    unsigned char m_MacMaxPromotionPdu;

    /**
     * PIB attribute 0x0012.
     */
    unsigned char m_MacPromotionPduTxPeriod;
    /**
     * PIB attribute 0x0013.
     */
    unsigned char m_MacBeaconsPerFrame;
    /**
     * PIB attribute 0x0014.
     */
    unsigned char m_MacScpMaxTxAttempts;
    /**
     * PIB attribute 0x0015.
     */
    unsigned char m_MacCtlReTxTimer;
    /**
     * PIB attribute 0x0018.
     */
    unsigned char m_MacMaxCtlReTx;
public:
    //Constructor.
    CGXDLMSPrimeNbOfdmPlcMacSetup();

    //SN Constructor.
    CGXDLMSPrimeNbOfdmPlcMacSetup(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSPrimeNbOfdmPlcMacSetup(std::string ln);

    /*
     * Returns PIB attribute 0x0010.
     */
    unsigned char GetMacMinSwitchSearchTime();

    /*
     * value
     *            PIB attribute 0x0010.
     */
    void SetMacMinSwitchSearchTime(unsigned char value);

    /*
     * Returns PIB attribute 0x0011.
     */
    unsigned char GetMacMaxPromotionPdu();

    /*
     * value
     *            PIB attribute 0x0011.
     */
    void SetMacMaxPromotionPdu(unsigned char value);

    /*
     * Returns PIB attribute 0x0012.
     */
    unsigned char GetMacPromotionPduTxPeriod();

    /*
     * value
     *            PIB attribute 0x0012.
     */
    void SetMacPromotionPduTxPeriod(unsigned char value);

    /*
     * Returns PIB attribute 0x0013.
     */
    unsigned char GetMacBeaconsPerFrame();

    /*
     * value
     *            PIB attribute 0x0013.
     */
    void SetMacBeaconsPerFrame(unsigned char value);

    /*
     * Returns PIB attribute 0x0014.
     */
    unsigned char GetMacScpMaxTxAttempts();

    /*
     * value
     *            PIB attribute 0x0014.
     */
    void SetMacScpMaxTxAttempts(unsigned char value);

    /*
     * Returns PIB attribute 0x0015.
     */
    unsigned char GetMacCtlReTxTimer();

    /*
     * value
     *            PIB attribute 0x0015.
     */
    void SetMacCtlReTxTimer(unsigned char value);

    /*
     * Returns PIB attribute 0x0018.
     */
    unsigned char GetMacMaxCtlReTx();

    /*
     * value
     *            PIB attribute 0x0018.
     */
    void SetMacMaxCtlReTx(unsigned char value);

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
#endif //GXDLMSPRIMENBOFDMPLCMACSETUP_H