#include "MeterInfo.h"

MeterInfo::MeterInfo(QObject *parent, PLCTool::Meter *meter)
  : QObject(parent), mMeter(meter)
{

}

void
MeterInfo::pushFrame(Frame const &frame)
{
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
MeterInfo::pushCreds(QString password, QString ctx)
{
  emit credentialsFound(password, ctx);
}
