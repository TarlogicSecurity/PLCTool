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

#ifndef GXMACPHYCOMMUNICATION_H
#define GXMACPHYCOMMUNICATION_H

//MAC PHY communication parameters.
class CGXMacPhyCommunication
{
    /*
    * EUI is the EUI-48 of the other device.
    */
    CGXByteBuffer m_Eui;

    /*
     * The tx power of GPDU packets sent to the device.
     */
    signed char m_TxPower;

    /*
     * The Tx coding of GPDU packets sent to the device.
     */
    signed char m_TxCoding;

    /*
     * The Rx coding of GPDU packets received from the device.
     */
    signed char m_RxCoding;
    /*
     * The Rx power level of GPDU packets received from the device.
     */
    signed char m_RxLvl;
    /*
     * SNR of GPDU packets received from the device.
     */
    signed char m_Snr;
    /*
     * The number of times the Tx power was modified.
     */
    signed char m_TxPowerModified;
    /*
     * The number of times the Tx coding was modified.
     */
    signed char m_TxCodingModified;
    /*
     * The number of times the Rx coding was modified.
     */
    signed char m_RxCodingModified;
public:
    /*
     * Returns EUI is the EUI-48 of the other device.
     */
    CGXByteBuffer& GetEui()
    {
        return m_Eui;
    }

    /*
     * value:
     *            EUI is the EUI-48 of the other device.
     */
    void SetEui(CGXByteBuffer& value)
    {
        m_Eui = value;
    }

    /*
     * Returns The tx power of GPDU packets sent to the device.
     */
    signed char GetTxPower()
    {
        return m_TxPower;
    }

    /*
     * value:
     *            The tx power of GPDU packets sent to the device.
     */
    void SetTxPower(signed char value)
    {
        m_TxPower = value;
    }

    /*
     * Returns The Tx coding of GPDU packets sent to the device.
     */
    signed char GetTxCoding()
    {
        return m_TxCoding;
    }

    /*
     * value:
     *            The Tx coding of GPDU packets sent to the device.
     */
    void SetTxCoding(signed char value)
    {
        m_TxCoding = value;
    }

    /*
     * Returns The Rx coding of GPDU packets received from the device.
     */
    signed char GetRxCoding()
    {
        return m_RxCoding;
    }

    /*
     * value:
     *            The Rx coding of GPDU packets received from the device.
     */
    void SetRxCoding(signed char value)
    {
        m_RxCoding = value;
    }

    /*
     * Returns The Rx power level of GPDU packets received from the device.
     */
    signed char GetRxLvl()
    {
        return m_RxLvl;
    }

    /*
     * value:
     *            The Rx power level of GPDU packets received from the device.
     */
    void SetRxLvl(signed char value)
    {
        m_RxLvl = value;
    }

    /*
     * Returns SNR of GPDU packets received from the device.
     */
    signed char GetSnr()
    {
        return m_Snr;
    }

    /*
     * value:
     *            SNR of GPDU packets received from the device.
     */
    void SetSnr(signed char value)
    {
        m_Snr = value;
    }

    /*
     * Returns The number of times the Tx power was modified.
     */
    signed char GetTxPowerModified()
    {
        return m_TxPowerModified;
    }

    /*
     * value:
     *            The number of times the Tx power was modified.
     */
    void SetTxPowerModified(signed char value)
    {
        m_TxPowerModified = value;
    }

    /*
     * Returns The number of times the Tx coding was modified.
     */
    signed char GetTxCodingModified()
    {
        return m_TxCodingModified;
    }

    /*
     * value:
     *            The number of times the Tx coding was modified.
     */
    void SetTxCodingModified(signed char value)
    {
        m_TxCodingModified = value;
    }

    /*
     * Returns The number of times the Rx coding was modified.
     */
    signed char GetRxCodingModified()
    {
        return m_RxCodingModified;
    }

    /*
     * value:
     *            The number of times the Rx coding was modified.
     */
    void SetRxCodingModified(signed char value)
    {
        m_RxCodingModified = value;
    }

    std::string ToString()
    {
        std::stringstream sb;
        sb << m_Eui.ToHexString();
        sb << ' ';
        sb << m_TxPower;
        sb << ' ';
        sb << m_TxCoding;
        sb << ' ';
        sb << m_RxCoding;
        sb << ' ';
        sb << m_RxLvl;
        sb << ' ';
        sb << m_Snr;
        sb << ' ';
        sb << m_TxPowerModified;
        sb << ' ';
        sb << m_TxCodingModified;
        sb << ' ';
        sb << m_RxCodingModified;
        sb << ' ';
        return sb.str();
    }

};
#endif //GXMACPHYCOMMUNICATION_H