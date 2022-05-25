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

#ifndef GXCREDIT_CHARGE_CONFIGURATION_H
#define GXCREDIT_CHARGE_CONFIGURATION_H

#include "enums.h"
#include <string>

/**
*
*/
class CGXCreditChargeConfiguration
{
    /**
* Credit reference.<br>
* Online help:<br>
* http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
*/
    std::string m_CreditReference;

    /**
     * Charge reference.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     */
    std::string m_ChargeReference;

    /**
     * Collection configuration.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     */
    DLMS_CREDIT_COLLECTION_CONFIGURATION m_CollectionConfiguration;
public:

    // Constructor.
    CGXCreditChargeConfiguration();

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Credit reference.
     */
    std::string& GetCreditReference();

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Credit reference.
     */
    void SetCreditReference(std::string& value);

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Charge reference.
     */
    std::string& GetChargeReference();

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Charge reference.
     */
    void SetChargeReference(std::string& value);

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Collection configuration.
     */
    DLMS_CREDIT_COLLECTION_CONFIGURATION GetCollectionConfiguration();

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Collection configuration.
     */
    void SetCollectionConfiguration(DLMS_CREDIT_COLLECTION_CONFIGURATION value);

    std::string ToString();
};
#endif //GXCREDIT_CHARGE_CONFIGURATION_H
