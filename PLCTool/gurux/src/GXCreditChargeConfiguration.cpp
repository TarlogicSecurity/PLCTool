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

#include "../include/GXCreditChargeConfiguration.h"
#include "../include/GXHelpers.h"
 // Constructor.
CGXCreditChargeConfiguration::CGXCreditChargeConfiguration()
{
}

std::string& CGXCreditChargeConfiguration::GetCreditReference()
{
    return m_CreditReference;
}

void CGXCreditChargeConfiguration::SetCreditReference(std::string& value)
{
    m_CreditReference = value;
}

std::string& CGXCreditChargeConfiguration::GetChargeReference()
{
    return m_ChargeReference;
}

void CGXCreditChargeConfiguration::SetChargeReference(std::string& value)
{
    m_ChargeReference = value;
}

DLMS_CREDIT_COLLECTION_CONFIGURATION CGXCreditChargeConfiguration::GetCollectionConfiguration()
{
    return m_CollectionConfiguration;
}

void CGXCreditChargeConfiguration::SetCollectionConfiguration(DLMS_CREDIT_COLLECTION_CONFIGURATION value)
{
    m_CollectionConfiguration = value;
}

std::string CGXCreditChargeConfiguration::ToString()
{
    std::string str = m_CreditReference;
    str.append(", ");
    str.append(m_ChargeReference);
    str.append(", ");
    str.append(GXHelpers::IntToString(m_CollectionConfiguration));
    return str;
}