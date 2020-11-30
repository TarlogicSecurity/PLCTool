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

#include "DLMSProcessor.h"
#include <dlms/dlmsmsg.h>

Q_DECLARE_METATYPE(DlmsMessage)
Q_DECLARE_METATYPE(DlmsMessage *)
static bool typeRegistered = false;


QString
DlmsMessage::toText(void) const
{
  QString asHex;
  char string[4];

  if (this->xml.size() > 0)
    return this->xml;

  for (int i = 0; i < this->pdu.size(); ++i) {
    snprintf(string, sizeof(string), "%02x", this->pdu[i]);
    asHex += string;
  }

  return asHex;
}

///////////////////////////////////////////////////////////////////////////////

DLMSProcessor::DLMSProcessor(QObject *parent) :
  QObject(parent), translator(DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
{
  this->translator.SetComments(true);
}

void
DLMSProcessor::registerTypes(void)
{
  if (!typeRegistered) {
    qRegisterMetaType<DlmsMessage>();
    qRegisterMetaType<DlmsMessage *>();
    typeRegistered = true;
  }
}

void
DLMSProcessor::process(
    QString SNA,
    QDateTime timeStamp,
    quint64 id,
    bool downlink,
    QVector<uint8_t> data)
{
  DlmsMessage msg;
  CGXByteBuffer dataBuffer;
  std::string str;

  msg.downlink = downlink;
  msg.SNA = SNA;
  msg.nodeId = id;
  msg.timeStamp = timeStamp;

  msg.pdu = data;;

  if (msg.pdu.size() > 0)
    msg.type = dlms_command_to_str(
          static_cast<DLMS_COMMAND>(msg.pdu[0]));
  else
    msg.type = "<!-- empty -->";


  dataBuffer.Set(msg.pdu.data(), msg.pdu.size());
  if (this->translator.PduToXml(dataBuffer, str) == 0) {
    msg.xml = QString::fromStdString(str);
    emit dlmsMessage(msg);
  }
}
