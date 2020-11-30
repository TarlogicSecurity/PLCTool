//
// Copyright (c) 2020, Tarlogic Security SL
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of copyright holders nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

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
