#include "MeterInfo.h"
#include <PRIME/PrimeFrame.h>

MeterInfo::MeterInfo(QObject *parent, PLCTool::Meter *meter)
  : QObject(parent), mMeter(meter)
{

}

void
MeterInfo::pushFrame(Frame const &frame)
{
  if (frame.frame != nullptr) {
    if (frame.frame->PDU.macType == PLCTool::PrimeFrame::MACType::GENERIC) {
      if (frame.frame->PDU.genType == PLCTool::PrimeFrame::GenericType::REG) {
        this->mMac.sprintf(
            "%02x:%02x:%02x:%02x:%02x:%02x",
            frame.frame->PDU.REG.EUI_48[0],
            frame.frame->PDU.REG.EUI_48[1],
            frame.frame->PDU.REG.EUI_48[2],
            frame.frame->PDU.REG.EUI_48[3],
            frame.frame->PDU.REG.EUI_48[4],
            frame.frame->PDU.REG.EUI_48[5]);
        this->meter()->setMacAddr(this->mMac.toStdString());
      }
    }
  }

  this->frameList()->append(frame);
  emit frameReceived(frame);
}

void
MeterInfo::pushDlmsMessage(DlmsMessage const &message)
{
  this->messageList()->append(message);
  emit messageReceived(message);
}

void
MeterInfo::pushCreds(QDateTime timeStamp, QString password, QString ctx)
{
  CredInfo info;

  info.timeStamp = timeStamp;
  info.password  = password;
  info.context   = ctx;

  this->creds.append(info);

  emit credentialsFound(timeStamp, password, ctx);
}
