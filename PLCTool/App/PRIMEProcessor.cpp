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

#include "PRIMEProcessor.h"

#include <PLCTool/PRIME/PrimeAdapter.h>

Q_DECLARE_METATYPE(Frame)
Q_DECLARE_METATYPE(Frame *)
static bool typeRegistered = false;

Frame::~Frame()
{
  if (this->frame != nullptr)
    delete this->frame;
}

Frame::Frame()
{
}

Frame::Frame(const Frame &original)
{
  PLCTool::PrimeFrame *dup = original.frame;

  if (dup != nullptr)
    dup = new PLCTool::PrimeFrame(*dup);

  this->SNA = original.SNA;
  this->downlink = original.downlink;
  this->bytes = original.bytes;
  this->timeStamp = original.timeStamp;
  this->frame = dup;
}

QString
Frame::toHtml(void) const
{
  size_t size = this->bytes.size();
  int rows = (size + 15) / 16;
  char string[16];
  int offset;
  QString result = "<html><body>";
  QString style =
      "border-right: solid; font-family: &quot;Cascadia Code PL&quot;";

  result += "<table>\n";
  for (int j = 0; j < rows; ++j) {
    offset = j << 4;
    result += "<tr><td style=\"color: red; " + style + "\">";
    snprintf(string, sizeof(string), "%04x", offset);
    result += string;
    result += "  </td>";

    result += "<td style=\"color: blue; " + style + "\">";
    for (int i = 0, offset = j << 4; i < 16; ++i, ++offset) {
      if (offset < static_cast<int>(size))
        snprintf(string, sizeof(string), "%02x ", this->bytes[offset]);
      else
        strncpy(string, "   ", sizeof(string));

      result += string;

      if (offset == 7)
        result += " ";
    }
    result += "</td>";

    result += "<td style=\"" + style + "\">";
    for (int i = 0, offset = j << 4; i < 16; ++i, ++offset) {
      if (offset < static_cast<int>(size))
        snprintf(
            string,
            sizeof(string),
            "%c",
            isprint(this->bytes[offset]) ? this->bytes[offset] : '.');
      else
        strncpy(string, " ", sizeof(string));

      result += string;
    }
    result += "</td>";

    result += "</tr>\n";
  }

  result += "</table>\n";
  result += "</body>\n";
  result += "</html>\n";

  return result;
}

///////////////////////////////////////////////////////////////////////////////
PRIMEProcessor::PRIMEProcessor(QObject *parent) : QObject(parent)
{
}

void
PRIMEProcessor::registerTypes(void)
{
  if (!typeRegistered) {
    qRegisterMetaType<Frame>();
    typeRegistered = true;
  }
}

void
PRIMEProcessor::process(
    quint64 dcId,
    QDateTime timeStamp,
    bool downlink,
    QVector<uint8_t> data)
{
  uint8_t mac[6];
  Frame currFrame;

  for (int i = 0; i < 6; ++i)
    mac[i] = static_cast<uint8_t>(dcId >> ((5 - i) * 8ull));

  PLCTool::PrimeFrame *primeFrame =
      PLCTool::PrimeFrame::fromRawData(mac, data.data(), data.size());

  currFrame.SNA = QString::fromStdString(PLCTool::PrimeAdapter::idToSna(dcId));
  currFrame.downlink = downlink;
  currFrame.timeStamp = timeStamp;
  currFrame.frame = primeFrame;
  currFrame.bytes = data;

  emit frame(currFrame);
}
