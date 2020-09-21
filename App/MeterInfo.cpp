#include "MeterInfo.h"
#include <PRIME/PrimeFrame.h>
#include <Topology/Concentrator.h>

static bool registered = false;

Q_DECLARE_METATYPE(CredInfo);

void
CredInfo::registerTypes(void)
{
  if (!registered) {
    qRegisterMetaType<CredInfo>();
    registered = true;
  }
}

CredInfo::CredInfo(void)
{

}

CredInfo::CredInfo(CredInfo const &info)
{
  *this = info;
}

MeterInfo::MeterInfo(QObject *parent, PLCTool::Meter *meter)
  : QObject(parent), mMeter(meter)
{

}

void
MeterInfo::pushFrame(Frame const &frame)
{
  if (frame.frame != nullptr) {
    if (frame.frame->PDU.macType == PLCTool::PrimeFrame::MACType::GENERIC) {
      switch (frame.frame->PDU.genType) {
        case PLCTool::PrimeFrame::GenericType::REG:
          this->mMac = QString::asprintf(
              "%02x:%02x:%02x:%02x:%02x:%02x",
              frame.frame->PDU.REG.EUI_48[0],
              frame.frame->PDU.REG.EUI_48[1],
              frame.frame->PDU.REG.EUI_48[2],
              frame.frame->PDU.REG.EUI_48[3],
              frame.frame->PDU.REG.EUI_48[4],
              frame.frame->PDU.REG.EUI_48[5]);
          this->meter()->setMacAddr(this->mMac.toStdString());
          break;

        case PLCTool::PrimeFrame::GenericType::PRO:
#ifdef PLCTOOL_PROMOTE_METERS
          if (!frame.frame->PDU.PRO.N
              && !frame.frame->PDU.HDR.DO
              && frame.frame->PDU.PRO.NSID != 0xff) {
            PLCTool::Node *topNode = this->meter()->topNode();
            if (topNode != nullptr
                && topNode->type() == PLCTool::CONCENTRATOR) {
              PLCTool::Concentrator *asDc =
                  static_cast<PLCTool::Concentrator *>(topNode);

              asDc->promote(this->meter(), frame.frame->PDU.PRO.NSID);
            }
          }
#endif // PLCTOOL_PROMOTE_METERS
          break;

        default:
          break;
      }
    }
  }

  this->pendingFrames.append(frame);
  emit frameReceived(frame);
}

void
MeterInfo::pushDlmsMessage(DlmsMessage const &message)
{
  this->messageList()->append(message);
  emit messageReceived(message);
}

CredInfo
MeterInfo::pushCreds(
    QDateTime const &timeStamp,
    QString const &password,
    QStringList const &ctx)
{
  CredInfo info;

  info.owner     = this;
  info.timeStamp = timeStamp;
  info.password  = password;
  info.contexts  = ctx;

  this->creds.append(info);

  emit credentialsFound(info);

  return info;
}
