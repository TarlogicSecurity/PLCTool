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

#ifndef GXMACAVAILABLESWITCH_H
#define GXMACAVAILABLESWITCH_H

class CGXMacAvailableSwitch
{
    /*
     * EUI-48 of the subnetwork.
     */
    CGXByteBuffer m_Sna;

    /*
     * SID of this switch.
     */
    short m_LsId;

    /*
     * Level of this switch in subnetwork hierarchy.
     */
    signed char m_Level;

    /*
     * The received signal level for this switch;
     */
    signed char m_RxLevel;
    /*
     * The signal to noise ratio for this switch.
     */
    signed char m_RxSnr;
public:
    /*
     * Returns EUI-48 of the subnetwork.
     */
    CGXByteBuffer& GetSna()
    {
        return m_Sna;
    }

    /*
     * value:
     *            EUI-48 of the subnetwork.
     */
    void SetSna(CGXByteBuffer& value)
    {
        m_Sna = value;
    }

    /*
     * Returns SID of this switch.
     */
    short GetLsId()
    {
        return m_LsId;
    }

    /*
     * value: SID of this switch.
     */
    void SetLsId(short value)
    {
        m_LsId = value;
    }

    /*
     * Returns Level of this switch in subnetwork hierarchy.
     */
    signed char GetLevel()
    {
        return m_Level;
    }

    /*
     * value: Level of this switch in subnetwork hierarchy.
     */
    void SetLevel(signed char value)
    {
        m_Level = value;
    }

    /*
     * Returns The received signal level for this switch;
     */
    signed char GetRxLevel()
    {
        return m_RxLevel;
    }

    /*
     * value: The received signal level for this switch;
     */
    void SetRxLevel(signed char value)
    {
        m_RxLevel = value;
    }

    /*
     * Returns The signal to noise ratio for this switch.
     */
    signed char GetRxSnr()
    {
        return m_RxSnr;
    }

    /*
     * value: The signal to noise ratio for this switch.
     */
    void SetRxSnr(signed char value)
    {
        m_RxSnr = value;
    }

    std::string ToString()
    {
        std::stringstream sb;
        sb << m_Sna.ToHexString();
        sb << ' ';
        sb << m_LsId;
        sb << ' ';
        sb << m_Level;
        sb << ' ';
        sb << m_RxLevel;
        sb << ' ';
        sb << m_RxSnr;
        return sb.str();
    }
};
#endif //GXMACAVAILABLESWITCH_H