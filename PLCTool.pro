#-------------------------------------------------
#
# Project created by QtCreator 2020-03-10T11:14:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

isEmpty(PREFIX) {
  PREFIX=/usr/local
}

target.path=$$PREFIX/bin

TARGET = PLCTool
TEMPLATE = app

INCLUDEPATH += gurux/include

SOURCES += main.cpp\
    App/GXDLMSTranslatorInterface.cpp \
    Attacks/Attack.cpp \
    Attacks/AttackController.cpp \
    Attacks/AttackFactory.cpp \
    Attacks/AttackFactoryLibrary.cpp \
    Attacks/AttackManager.cpp \
    Attacks/AttackManagerWorker.cpp \
    Attacks/BlinkAttack/BlinkAttack.cpp \
    Attacks/BlinkAttack/BlinkAttackFactory.cpp \
    Attacks/BlinkAttack/BlinkController.cpp \
    Attacks/UIAttackController.cpp \
    Ui/DisclaimerDialog.cpp \
    Topology/Adapter.cpp \
    Topology/AdapterEventListener.cpp \
    Topology/Concentrator.cpp \
    Topology/Exception.cpp \
    Topology/Hub.cpp \
    Topology/Meter.cpp \
    Topology/Node.cpp \
    Topology/StringParams.cpp \
    Topology/SubNet.cpp \
    Topology/Switch.cpp \
    Topology/Worker.cpp \
    PRIME/PrimeFrame.cpp \
    PRIME/libprime/layer.c \
    PRIME/libprime/state.c \
    PRIME/PrimeAdapter.cpp \
    PRIME/spip/crc32.c \
    PRIME/spip/loop.c \
    PRIME/spip/spip.c \
    App/Application.cpp \
    Attacks/BlinkAttack/BlinkUI.cpp \
    Ui/CredInfoTableModel.cpp \
    Ui/QtUi.cpp \
    Ui/TopologyModel.cpp \
    Ui/ModemDialog.cpp \
    Ui/MeterUI.cpp \
    Ui/MainWindow.cpp \
    Ui/CredentialsUI.cpp \
    Ui/FrameLogUI.cpp \
    ber/ber.c \
    ber/parser.c \
    ber/stream.c \
    dlms/dlms.c \
    dlms/dlmsmsg.c \
    dlms/meter.c \
    gurux/src/GXAPDU.cpp \
    gurux/src/GXAdjacentCell.cpp \
    gurux/src/GXApplicationContextName.cpp \
    gurux/src/GXAuthenticationMechanismName.cpp \
    gurux/src/GXBitString.cpp \
    gurux/src/GXBytebuffer.cpp \
    gurux/src/GXChargePerUnitScaling.cpp \
    gurux/src/GXChargeTable.cpp \
    gurux/src/GXCipher.cpp \
    gurux/src/GXCommodity.cpp \
    gurux/src/GXCreditChargeConfiguration.cpp \
    gurux/src/GXCurrency.cpp \
    gurux/src/GXDLMS.cpp \
    gurux/src/GXDLMSAccount.cpp \
    gurux/src/GXDLMSActionItem.cpp \
    gurux/src/GXDLMSActionSchedule.cpp \
    gurux/src/GXDLMSActionSet.cpp \
    gurux/src/GXDLMSActivityCalendar.cpp \
    gurux/src/GXDLMSAssociationLogicalName.cpp \
    gurux/src/GXDLMSAssociationShortName.cpp \
    gurux/src/GXDLMSAutoAnswer.cpp \
    gurux/src/GXDLMSAutoConnect.cpp \
    gurux/src/GXDLMSCaptureObject.cpp \
    gurux/src/GXDLMSCertificateInfo.cpp \
    gurux/src/GXDLMSCharge.cpp \
    gurux/src/GXDLMSClient.cpp \
    gurux/src/GXDLMSClock.cpp \
    gurux/src/GXDLMSCompactData.cpp \
    gurux/src/GXDLMSContextType.cpp \
    gurux/src/GXDLMSConverter.cpp \
    gurux/src/GXDLMSCredit.cpp \
    gurux/src/GXDLMSData.cpp \
    gurux/src/GXDLMSDayProfile.cpp \
    gurux/src/GXDLMSDayProfileAction.cpp \
    gurux/src/GXDLMSDemandRegister.cpp \
    gurux/src/GXDLMSDisconnectControl.cpp \
    gurux/src/GXDLMSEmergencyProfile.cpp \
    gurux/src/GXDLMSExtendedRegister.cpp \
    gurux/src/GXDLMSGPRSSetup.cpp \
    gurux/src/GXDLMSGSMCellInfo.cpp \
    gurux/src/GXDLMSGSMDiagnostic.cpp \
    gurux/src/GXDLMSHdlcSetup.cpp \
    gurux/src/GXDLMSIECOpticalPortSetup.cpp \
    gurux/src/GXDLMSImageActivateInfo.cpp \
    gurux/src/GXDLMSImageTransfer.cpp \
    gurux/src/GXDLMSIp4Setup.cpp \
    gurux/src/GXDLMSIp4SetupIpOption.cpp \
    gurux/src/GXDLMSLNCommandHandler.cpp \
    gurux/src/GXDLMSLNParameters.cpp \
    gurux/src/GXDLMSLimiter.cpp \
    gurux/src/GXDLMSLimits.cpp \
    gurux/src/GXDLMSLlcSscsSetup.cpp \
    gurux/src/GXDLMSMBusClient.cpp \
    gurux/src/GXDLMSMBusMasterPortSetup.cpp \
    gurux/src/GXDLMSMBusSlavePortSetup.cpp \
    gurux/src/GXDLMSMacAddressSetup.cpp \
    gurux/src/GXDLMSMd5.cpp \
    gurux/src/GXDLMSMessageHandler.cpp \
    gurux/src/GXDLMSModemConfiguration.cpp \
    gurux/src/GXDLMSModemInitialisation.cpp \
    gurux/src/GXDLMSMonitoredValue.cpp \
    gurux/src/GXDLMSNotify.cpp \
    gurux/src/GXDLMSObject.cpp \
    gurux/src/GXDLMSObjectCollection.cpp \
    gurux/src/GXDLMSObjectDefinition.cpp \
    gurux/src/GXDLMSObjectFactory.cpp \
    gurux/src/GXDLMSParameterMonitor.cpp \
    gurux/src/GXDLMSPppSetup.cpp \
    gurux/src/GXDLMSPppSetupIPCPOption.cpp \
    gurux/src/GXDLMSPppSetupLcpOption.cpp \
    gurux/src/GXDLMSPrimeNbOfdmPlcApplicationsIdentification.cpp \
    gurux/src/GXDLMSPrimeNbOfdmPlcMacCounters.cpp \
    gurux/src/GXDLMSPrimeNbOfdmPlcMacFunctionalParameters.cpp \
    gurux/src/GXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData.cpp \
    gurux/src/GXDLMSPrimeNbOfdmPlcMacSetup.cpp \
    gurux/src/GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters.cpp \
    gurux/src/GXDLMSProfileGeneric.cpp \
    gurux/src/GXDLMSPushObject.cpp \
    gurux/src/GXDLMSPushSetup.cpp \
    gurux/src/GXDLMSQualityOfService.cpp \
    gurux/src/GXDLMSRegister.cpp \
    gurux/src/GXDLMSRegisterActivation.cpp \
    gurux/src/GXDLMSRegisterMonitor.cpp \
    gurux/src/GXDLMSSNCommandHandler.cpp \
    gurux/src/GXDLMSSNParameters.cpp \
    gurux/src/GXDLMSSapAssignment.cpp \
    gurux/src/GXDLMSSchedule.cpp \
    gurux/src/GXDLMSScheduleEntry.cpp \
    gurux/src/GXDLMSScript.cpp \
    gurux/src/GXDLMSScriptAction.cpp \
    gurux/src/GXDLMSScriptTable.cpp \
    gurux/src/GXDLMSSeasonProfile.cpp \
    gurux/src/GXDLMSSecureClient.cpp \
    gurux/src/GXDLMSSecureServer.cpp \
    gurux/src/GXDLMSSecuritySetup.cpp \
    gurux/src/GXDLMSServer.cpp \
    gurux/src/GXDLMSSettings.cpp \
    gurux/src/GXDLMSSha1.cpp \
    gurux/src/GXDLMSSha256.cpp \
    gurux/src/GXDLMSSpecialDay.cpp \
    gurux/src/GXDLMSSpecialDaysTable.cpp \
    gurux/src/GXDLMSTarget.cpp \
    gurux/src/GXDLMSTcpUdpSetup.cpp \
    gurux/src/GXDLMSTokenGateway.cpp \
    gurux/src/GXDLMSTranslator.cpp \
    gurux/src/GXDLMSTranslatorStructure.cpp \
    gurux/src/GXDLMSUtilityTables.cpp \
    gurux/src/GXDLMSValueEventArg.cpp \
    gurux/src/GXDLMSVariant.cpp \
    gurux/src/GXDLMSWeekProfile.cpp \
    gurux/src/GXDateTime.cpp \
    gurux/src/GXHelpers.cpp \
    gurux/src/GXReplyData.cpp \
    gurux/src/GXSNInfo.cpp \
    gurux/src/GXSecure.cpp \
    gurux/src/GXSerialNumberCounter.cpp \
    gurux/src/GXStandardObisCode.cpp \
    gurux/src/GXStandardObisCodeCollection.cpp \
    gurux/src/GXTokenGatewayConfiguration.cpp \
    gurux/src/GXUnitCharge.cpp \
    gurux/src/GXXmlReader.cpp \
    gurux/src/GXXmlWriter.cpp \
    gurux/src/GXXmlWriterSettings.cpp \
    util/util.c \
    Ui/DLMSLogUI.cpp \
    Ui/TranslatorUI.cpp \
    Ui/XMLHighlighter.cpp \
    PRIME/spip/serial.c \
    Ui/FrameTableModel.cpp \
    Ui/LoadingStatusDialog.cpp \
    Ui/DLMSTableModel.cpp \
    App/DLMSProcessor.cpp \
    App/PRIMEProcessor.cpp \
    App/MeterInfo.cpp

HEADERS  += Ui/MainWindow.h \
    App/DLMSTranslator.h \
    App/GXDLMSTranslatorInterface.h \
    Attacks/Attack.h \
    Attacks/AttackController.h \
    Attacks/AttackFactory.h \
    Attacks/AttackFactoryLibrary.h \
    Attacks/AttackManager.h \
    Attacks/AttackManagerWorker.h \
    Attacks/BlinkAttack/BlinkAttack.h \
    Attacks/BlinkAttack/BlinkAttackFactory.h \
    Attacks/BlinkAttack/BlinkController.h \
    Attacks/UIAttackController.h \
    Ui/DisclaimerDialog.h \
    Topology/Adapter.h \
    Topology/AdapterEventListener.h \
    Topology/Concentrator.h \
    Topology/Exception.h \
    Topology/Hub.h \
    Topology/Meter.h \
    Topology/Node.h \
    Topology/StringParams.h \
    Topology/SubNet.h \
    Topology/Switch.h \
    Topology/Worker.h \
    PRIME/libprime/layer.h \
    PRIME/libprime/pdu.h \
    PRIME/libprime/prime.h \
    PRIME/libprime/state.h \
    PRIME/PrimeFrame.h \
    PRIME/PrimeAdapter.h \
    PRIME/spip/spip.h \
    App/Application.h \
    Attacks/BlinkAttack/BlinkUI.h \
    Ui/CredInfoTableModel.h \
    Ui/QtUi.h \
    Ui/TopologyModel.h \
    Ui/MainWindow.h \
    Ui/ModemDialog.h \
    Ui/MeterUI.h \
    Ui/CredentialsUI.h \
    Ui/FrameLogUI.h \
    ber/ber.h \
    ber/stream.h \
    dlms/dlms.h \
    dlms/dlmsmsg.h \
    dlms/meter.h \
    gurux/include/GXAPDU.h \
    gurux/include/GXAdjacentCell.h \
    gurux/include/GXApplicationContextName.h \
    gurux/include/GXAttributeCollection.h \
    gurux/include/GXAuthentication.h \
    gurux/include/GXAuthenticationMechanismName.h \
    gurux/include/GXBitString.h \
    gurux/include/GXBytebuffer.h \
    gurux/include/GXChargePerUnitScaling.h \
    gurux/include/GXChargeTable.h \
    gurux/include/GXCipher.h \
    gurux/include/GXCommodity.h \
    gurux/include/GXCreditChargeConfiguration.h \
    gurux/include/GXCurrency.h \
    gurux/include/GXDLMS.h \
    gurux/include/GXDLMSAccount.h \
    gurux/include/GXDLMSActionItem.h \
    gurux/include/GXDLMSActionSchedule.h \
    gurux/include/GXDLMSActionSet.h \
    gurux/include/GXDLMSActivityCalendar.h \
    gurux/include/GXDLMSAssociationLogicalName.h \
    gurux/include/GXDLMSAssociationShortName.h \
    gurux/include/GXDLMSAttribute.h \
    gurux/include/GXDLMSAutoAnswer.h \
    gurux/include/GXDLMSAutoConnect.h \
    gurux/include/GXDLMSCaptureObject.h \
    gurux/include/GXDLMSCertificateInfo.h \
    gurux/include/GXDLMSCharge.h \
    gurux/include/GXDLMSClient.h \
    gurux/include/GXDLMSClock.h \
    gurux/include/GXDLMSCompactData.h \
    gurux/include/GXDLMSConnectionEventArgs.h \
    gurux/include/GXDLMSContextType.h \
    gurux/include/GXDLMSConverter.h \
    gurux/include/GXDLMSCredit.h \
    gurux/include/GXDLMSData.h \
    gurux/include/GXDLMSDayProfile.h \
    gurux/include/GXDLMSDayProfileAction.h \
    gurux/include/GXDLMSDemandRegister.h \
    gurux/include/GXDLMSDisconnectControl.h \
    gurux/include/GXDLMSEmergencyProfile.h \
    gurux/include/GXDLMSExtendedRegister.h \
    gurux/include/GXDLMSGPRSSetup.h \
    gurux/include/GXDLMSGSMCellInfo.h \
    gurux/include/GXDLMSGSMDiagnostic.h \
    gurux/include/GXDLMSHdlcSetup.h \
    gurux/include/GXDLMSIECOpticalPortSetup.h \
    gurux/include/GXDLMSImageActivateInfo.h \
    gurux/include/GXDLMSImageTransfer.h \
    gurux/include/GXDLMSIp4Setup.h \
    gurux/include/GXDLMSIp4SetupIpOption.h \
    gurux/include/GXDLMSLNCommandHandler.h \
    gurux/include/GXDLMSLNParameters.h \
    gurux/include/GXDLMSLimiter.h \
    gurux/include/GXDLMSLimits.h \
    gurux/include/GXDLMSLlcSscsSetup.h \
    gurux/include/GXDLMSLongTransaction.h \
    gurux/include/GXDLMSMBusClient.h \
    gurux/include/GXDLMSMBusMasterPortSetup.h \
    gurux/include/GXDLMSMBusSlavePortSetup.h \
    gurux/include/GXDLMSMacAddressSetup.h \
    gurux/include/GXDLMSMd5.h \
    gurux/include/GXDLMSMessageHandler.h \
    gurux/include/GXDLMSModemConfiguration.h \
    gurux/include/GXDLMSModemInitialisation.h \
    gurux/include/GXDLMSMonitoredValue.h \
    gurux/include/GXDLMSNotify.h \
    gurux/include/GXDLMSObject.h \
    gurux/include/GXDLMSObjectCollection.h \
    gurux/include/GXDLMSObjectDefinition.h \
    gurux/include/GXDLMSObjectFactory.h \
    gurux/include/GXDLMSParameterMonitor.h \
    gurux/include/GXDLMSPppSetup.h \
    gurux/include/GXDLMSPppSetupIPCPOption.h \
    gurux/include/GXDLMSPppSetupLcpOption.h \
    gurux/include/GXDLMSPrimeNbOfdmPlcApplicationsIdentification.h \
    gurux/include/GXDLMSPrimeNbOfdmPlcMacCounters.h \
    gurux/include/GXDLMSPrimeNbOfdmPlcMacFunctionalParameters.h \
    gurux/include/GXDLMSPrimeNbOfdmPlcMacNetworkAdministrationData.h \
    gurux/include/GXDLMSPrimeNbOfdmPlcMacSetup.h \
    gurux/include/GXDLMSPrimeNbOfdmPlcPhysicalLayerCounters.h \
    gurux/include/GXDLMSProfileGeneric.h \
    gurux/include/GXDLMSPushObject.h \
    gurux/include/GXDLMSPushSetup.h \
    gurux/include/GXDLMSQualityOfService.h \
    gurux/include/GXDLMSRegister.h \
    gurux/include/GXDLMSRegisterActivation.h \
    gurux/include/GXDLMSRegisterMonitor.h \
    gurux/include/GXDLMSSNCommandHandler.h \
    gurux/include/GXDLMSSNParameters.h \
    gurux/include/GXDLMSSapAssignment.h \
    gurux/include/GXDLMSSchedule.h \
    gurux/include/GXDLMSScheduleEntry.h \
    gurux/include/GXDLMSScript.h \
    gurux/include/GXDLMSScriptAction.h \
    gurux/include/GXDLMSScriptTable.h \
    gurux/include/GXDLMSSeasonProfile.h \
    gurux/include/GXDLMSSecureClient.h \
    gurux/include/GXDLMSSecureServer.h \
    gurux/include/GXDLMSSecuritySetup.h \
    gurux/include/GXDLMSServer.h \
    gurux/include/GXDLMSSettings.h \
    gurux/include/GXDLMSSha1.h \
    gurux/include/GXDLMSSha256.h \
    gurux/include/GXDLMSSpecialDay.h \
    gurux/include/GXDLMSSpecialDaysTable.h \
    gurux/include/GXDLMSTarget.h \
    gurux/include/GXDLMSTcpUdpSetup.h \
    gurux/include/GXDLMSTokenGateway.h \
    gurux/include/GXDLMSTranslator.h \
    gurux/include/GXDLMSTranslatorStructure.h \
    gurux/include/GXDLMSUtilityTables.h \
    gurux/include/GXDLMSValueEventArg.h \
    gurux/include/GXDLMSValueEventCollection.h \
    gurux/include/GXDLMSVariant.h \
    gurux/include/GXDLMSWeekProfile.h \
    gurux/include/GXDataInfo.h \
    gurux/include/GXDate.h \
    gurux/include/GXDateTime.h \
    gurux/include/GXHelpers.h \
    gurux/include/GXMacAvailableSwitch.h \
    gurux/include/GXMacDirectTable.h \
    gurux/include/GXMacMulticastEntry.h \
    gurux/include/GXMacPhyCommunication.h \
    gurux/include/GXReplyData.h \
    gurux/include/GXSNInfo.h \
    gurux/include/GXSecure.h \
    gurux/include/GXSerialNumberCounter.h \
    gurux/include/GXServerReply.h \
    gurux/include/GXStandardObisCode.h \
    gurux/include/GXStandardObisCodeCollection.h \
    gurux/include/GXTime.h \
    gurux/include/GXTokenGatewayConfiguration.h \
    gurux/include/GXUnitCharge.h \
    gurux/include/GXXmlReader.h \
    gurux/include/GXXmlWriter.h \
    gurux/include/GXXmlWriterSettings.h \
    gurux/include/IGXDLMSBase.h \
    gurux/include/OBiscodes.h \
    gurux/include/TranslatorGeneralTags.h \
    gurux/include/TranslatorSimpleTags.h \
    gurux/include/TranslatorStandardTags.h \
    gurux/include/TranslatorTags.h \
    gurux/include/chipperingenums.h \
    gurux/include/enums.h \
    gurux/include/errorcodes.h \
    util/defs.h \
    util/util.h \
    Ui/DLMSLogUI.h \
    Ui/TranslatorUI.h \
    Ui/XMLHighlighter.h \
    Ui/FrameTableModel.h \
    Ui/LoadingStatusDialog.h \
    Ui/DLMSTableModel.h \
    App/DLMSProcessor.h \
    App/PRIMEProcessor.h \
    App/MeterInfo.h


FORMS    += Forms/MainWindow.ui \
    Attacks/BlinkAttack/BlinkUI.ui \
    Forms/DisclaimerDialog.ui \
    Forms/MeterUI.ui \
    Forms/ModemDialog.ui \
    Forms/CredentialsUI.ui \
    Forms/FrameLogUI.ui \
    Forms/DLMSLogUI.ui \
    Forms/TranslatorUI.ui \
    Forms/LoadingStatusDialog.ui

unix: LIBS += -ldl

RESOURCES += \
    icons.qrc

unix:  QMAKE_SUBSTITUTES += PLCTool.desktop.in

unix:  desktop.path  = $$PREFIX/share/applications
unix:  desktop.files = PLCTool.desktop
unix:  icons.path    = $$PREFIX/share/icons/hicolor/256x256/apps/
unix:  icons.files   = Icons/PLCTool.png
unix:  INSTALLS     += desktop icons

INSTALLS += target
