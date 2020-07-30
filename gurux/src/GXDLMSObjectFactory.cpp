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

#include "../include/GXDLMSObjectFactory.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXDLMSActionSchedule.h"
#include "../include/GXDLMSActivityCalendar.h"
#include "../include/GXDLMSAssociationLogicalName.h"
#include "../include/GXDLMSAssociationShortName.h"
#include "../include/GXDLMSAutoAnswer.h"
#include "../include/GXDLMSAutoConnect.h"
#include "../include/GXDLMSClock.h"
#include "../include/GXDLMSData.h"
#include "../include/GXDLMSRegister.h"
#include "../include/GXDLMSExtendedRegister.h"
#include "../include/GXDLMSDemandRegister.h"
#include "../include/GXDLMSRegisterMonitor.h"
#include "../include/GXDLMSProfileGeneric.h"
#include "../include/GXDLMSTcpUdpSetup.h"
#include "../include/GXDLMSGPRSSetup.h"
#include "../include/GXDLMSActionSchedule.h"
#include "../include/GXDLMSSpecialDaysTable.h"
#include "../include/GXDLMSImageTransfer.h"
#include "../include/GXDLMSDisconnectControl.h"
#include "../include/GXDLMSLimiter.h"
#include "../include/GXDLMSMBusClient.h"
#include "../include/GXDLMSScriptTable.h"
#include "../include/GXDLMSMBusSlavePortSetup.h"
#include "../include/GXDLMSSecuritySetup.h"
#include "../include/GXDLMSPppSetup.h"
#include "../include/GXDLMSMacAddressSetup.h"
#include "../include/GXDLMSHdlcSetup.h"
#include "../include/GXDLMSIECOpticalPortSetup.h"
#include "../include/GXDLMSIp4Setup.h"
//#include "../include/GXDLMSIp6Setup.h"
#include "../include/GXDLMSRegisterActivation.h"
#include "../include/GXDLMSSchedule.h"
#include "../include/GXDLMSModemConfiguration.h"
#include "../include/GXDLMSSapAssignment.h"
#include "../include/GXDLMSMBusMasterPortSetup.h"
#include "../include/GXDLMSMessageHandler.h"
#include "../include/GXDLMSPushSetup.h"
#include "../include/GXDLMSGSMDiagnostic.h"
#include "../include/GXDLMSAccount.h"
#include "../include/GXDLMSCredit.h"
#include "../include/GXDLMSCharge.h"
#include "../include/GXDLMSParameterMonitor.h"
#include "../include/GXDLMSTokenGateway.h"
#include "../include/GXDLMSCompactData.h"
#include "../include/GXDLMSUtilityTables.h"
#include "../include/GXDLMSLlcSscsSetup.h"
#include "../include/GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters.h"
#include "../include/GXDLMSPrimeNbOfdmPlcMacSetup.h"
#include "../include/GXDLMSPrimeNbOfdmPlcMacFunctionalParameters.h"
#include "../include/GXDLMSPrimeNbOfdmPlcMacCounters.h"
#include "../include/GXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData.h"
#include "../include/GXDLMSPrimeNbOfdmPlcApplicationsIdentification.h"

// Reserved for internal use.
CGXDLMSObject* CGXDLMSObjectFactory::CreateObject(DLMS_OBJECT_TYPE type)
{
    switch (type)
    {
    case DLMS_OBJECT_TYPE_ACTION_SCHEDULE:
        return new CGXDLMSActionSchedule();
    case DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR:
        return new CGXDLMSActivityCalendar();
    case DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME:
        return new CGXDLMSAssociationLogicalName();
    case DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME:
        return new CGXDLMSAssociationShortName();
    case DLMS_OBJECT_TYPE_AUTO_ANSWER:
        return new CGXDLMSAutoAnswer();
    case DLMS_OBJECT_TYPE_AUTO_CONNECT:
        return new CGXDLMSAutoConnect();
    case DLMS_OBJECT_TYPE_CLOCK:
        return new CGXDLMSClock();
    case DLMS_OBJECT_TYPE_DATA:
        return new CGXDLMSData();
    case DLMS_OBJECT_TYPE_DEMAND_REGISTER:
        return new CGXDLMSDemandRegister();
    case DLMS_OBJECT_TYPE_MAC_ADDRESS_SETUP:
        return new CGXDLMSMacAddressSetup();
    case DLMS_OBJECT_TYPE_EXTENDED_REGISTER:
        return new CGXDLMSExtendedRegister();
    case DLMS_OBJECT_TYPE_GPRS_SETUP:
        return new CGXDLMSGPRSSetup();
    case DLMS_OBJECT_TYPE_SECURITY_SETUP:
        return new CGXDLMSSecuritySetup();
    case DLMS_OBJECT_TYPE_IEC_HDLC_SETUP:
        return new CGXDLMSIecHdlcSetup();
    case DLMS_OBJECT_TYPE_IEC_LOCAL_PORT_SETUP:
        return new CGXDLMSIECLocalPortSetup();
    case DLMS_OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP:
        return new CGXDLMSCustomObject(type);
    case DLMS_OBJECT_TYPE_IP4_SETUP:
        return new CGXDLMSIp4Setup();
//    case DLMS_OBJECT_TYPE_IP6_SETUP:
//        return new CGXDLMSIp6Setup();
    case DLMS_OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP:
        return new CGXDLMSMBusSlavePortSetup();
    case DLMS_OBJECT_TYPE_IMAGE_TRANSFER:
        return new CGXDLMSImageTransfer();
    case DLMS_OBJECT_TYPE_DISCONNECT_CONTROL:
        return new CGXDLMSDisconnectControl();
    case DLMS_OBJECT_TYPE_LIMITER:
        return new CGXDLMSLimiter();
    case DLMS_OBJECT_TYPE_MBUS_CLIENT:
        return new CGXDLMSMBusClient();
    case DLMS_OBJECT_TYPE_MODEM_CONFIGURATION:
        return new CGXDLMSModemConfiguration();
    case DLMS_OBJECT_TYPE_PPP_SETUP:
        return new CGXDLMSPppSetup();
    case DLMS_OBJECT_TYPE_PROFILE_GENERIC:
        return new CGXDLMSProfileGeneric();
    case DLMS_OBJECT_TYPE_REGISTER:
        return new CGXDLMSRegister();
    case DLMS_OBJECT_TYPE_REGISTER_ACTIVATION:
        return new CGXDLMSRegisterActivation();
    case DLMS_OBJECT_TYPE_REGISTER_MONITOR:
        return new CGXDLMSRegisterMonitor();
    case DLMS_OBJECT_TYPE_REGISTER_TABLE:
        return new CGXDLMSCustomObject(type);
    case DLMS_OBJECT_TYPE_ZIG_BEE_SAS_STARTUP:
        return new CGXDLMSCustomObject(type);
    case DLMS_OBJECT_TYPE_ZIG_BEE_SAS_JOIN:
        return new CGXDLMSCustomObject(type);
    case DLMS_OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION:
        return new CGXDLMSCustomObject(type);
    case DLMS_OBJECT_TYPE_SAP_ASSIGNMENT:
        return new CGXDLMSSapAssignment();
    case DLMS_OBJECT_TYPE_SCHEDULE:
        return new CGXDLMSSchedule();
    case DLMS_OBJECT_TYPE_SCRIPT_TABLE:
        return new CGXDLMSScriptTable();
    case DLMS_OBJECT_TYPE_SMTP_SETUP:
        return new CGXDLMSCustomObject(type);
    case DLMS_OBJECT_TYPE_SPECIAL_DAYS_TABLE:
        return new CGXDLMSSpecialDaysTable();
    case DLMS_OBJECT_TYPE_STATUS_MAPPING:
        return new CGXDLMSCustomObject(type);
    case DLMS_OBJECT_TYPE_TCP_UDP_SETUP:
        return new CGXDLMSTcpUdpSetup();
    case DLMS_OBJECT_TYPE_UTILITY_TABLES:
        return new CGXDLMSUtilityTables();
    case DLMS_OBJECT_TYPE_MBUS_MASTER_PORT_SETUP:
        return new CGXDLMSMBusMasterPortSetup();
    case DLMS_OBJECT_TYPE_MESSAGE_HANDLER:
        return new CGXDLMSMessageHandler();
    case DLMS_OBJECT_TYPE_PUSH_SETUP:
        return new CGXDLMSPushSetup();
    case DLMS_OBJECT_TYPE_GSM_DIAGNOSTIC:
        return new CGXDLMSGSMDiagnostic();
    case DLMS_OBJECT_TYPE_ACCOUNT:
        return new CGXDLMSAccount();
    case DLMS_OBJECT_TYPE_CREDIT:
        return new CGXDLMSCredit();
    case DLMS_OBJECT_TYPE_CHARGE:
        return new CGXDLMSCharge();
    case DLMS_OBJECT_TYPE_PARAMETER_MONITOR:
        return new CGXDLMSParameterMonitor();
    case DLMS_OBJECT_TYPE_TOKEN_GATEWAY:
        return new CGXDLMSTokenGateway();
    case DLMS_OBJECT_TYPE_COMPACT_DATA:
        return new CGXDLMSCompactData();
    case DLMS_OBJECT_TYPE_LLC_SSCS_SETUP:
        return new CGXDLMSLlcSscsSetup();
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_PHYSICAL_LAYER_COUNTERS:
        return new CGXDLMSPrimeNbOfdmPlcPhysicalLayerCounters();
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_SETUP:
        return new CGXDLMSPrimeNbOfdmPlcMacSetup();
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_FUNCTIONAL_PARAMETERS:
        return new CGXDLMSPrimeNbOfdmPlcMacFunctionalParameters();
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_COUNTERS:
        return new CGXDLMSPrimeNbOfdmPlcMacCounters();
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_NETWORK_ADMINISTRATION_DATA:
        return new CGXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData();
    case DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_APPLICATIONS_IDENTIFICATION:
        return new CGXDLMSPrimeNbOfdmPlcApplicationsIdentification();
    default:
#ifdef _DEBUG
        printf("Unknown object: %d\r\n", type);
#endif //_DEBUG
        return NULL;
    }
}

CGXDLMSObject* CGXDLMSObjectFactory::CreateObject(DLMS_OBJECT_TYPE type, std::string ln)
{
    CGXDLMSObject* pObj = CreateObject(type);
    if (pObj != NULL)
    {
        GXHelpers::SetLogicalName(ln.c_str(), pObj->m_LN);
    }
    return pObj;
}
