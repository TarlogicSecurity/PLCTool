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

#ifndef GXDLMSGSMCELLINFO_H
#define GXDLMSGSMCELLINFO_H

#include <string>

class CGXDLMSGSMCellInfo {
private:
    /*
    * Four byte cell ID.
    */
    unsigned int m_CellId;

    /*
    * Two byte location area code (LAC).
    */
    unsigned short m_LocationId;
    /*
    * Signal quality.
    */
    unsigned char m_SignalQuality;
    /*
    * Bit Error Rate.
    */
    unsigned char m_Ber;

    /*
    * Mobile Country Code.
    */
    unsigned short m_MobileCountryCode;

    /**
     * Mobile Network Code.
     */
    unsigned short m_MobileNetworkCode;

    /**
     * Absolute radio frequency channel number.
     */
    unsigned long m_ChannelNumber;


public:
    //Constructor.
    CGXDLMSGSMCellInfo();
    /**
    * @return Two byte cell ID.
    */
    unsigned int GetCellId();

    /*
    * @param value
    *            Four byte cell ID.
    */
    void SetCellId(unsigned int value);

    /*
    * @return Two byte location area code (LAC).
    */
    unsigned short GetLocationId();

    /*
    * @param value
    *            Two byte location area code (LAC).
    */
    void SetLocationId(unsigned short value);

    /*
    * @return Signal quality.
    */
    unsigned char GetSignalQuality();

    /*
    * @param value
    *            Signal quality.
    */
    void SetSignalQuality(unsigned char value);

    /*
    * @return Bit Error Rate.
    */
    unsigned char GetBer();

    /**
    * @param value
    *            Bit Error Rate.
    */
    void SetBer(unsigned char value);

    /*
    * Mobile Country Code.
    */
    unsigned short GetMobileCountryCode();

    /*
    * Mobile Country Code.
    */
    void SetMobileCountryCode(unsigned short value);

    /**
     * Mobile Network Code.
     */
    unsigned short GetMobileNetworkCode();

    /**
     * Mobile Network Code.
     */
    void SetMobileNetworkCode(unsigned short value);

    /**
     * Absolute radio frequency channel number.
     */
    unsigned long GetChannelNumber();

    /**
     * Absolute radio frequency channel number.
     */
    void SetChannelNumber(unsigned long value);

    std::string ToString();
};

#endif //GXDLMSGSMCELLINFO_H