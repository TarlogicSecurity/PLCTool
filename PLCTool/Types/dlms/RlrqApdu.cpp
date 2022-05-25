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

#include "RlrqApdu.h"

using namespace PLCTool;

RlrqApdu::RlrqApdu()
{
  this->identifier = BerIdentifier(BER_APPLICATION_CLASS, true, 0x02);
}

RlrqApdu::~RlrqApdu()
{
  if (this->reason)
    delete this->reason;

  if (this->userInformation)
    delete this->userInformation;
}

DLMS_RELEASE_REQUEST_REASON
RlrqApdu::getReason() const
{
  DLMS_RELEASE_REQUEST_REASON value = DLMS_RELEASE_REQUEST_REASON_NORMAL;

  if (this->reason)
    value = this->reason->getReason();

  return value;
}

void RlrqApdu::setReason(DLMS_RELEASE_REQUEST_REASON newReason)
{
  if (!this->reason)
    this->reason = new ReleaseRequestReason();

  this->reason->setReason(newReason);
}

std::vector<uint8_t> RlrqApdu::getUserInformation() const
{
  std::vector<uint8_t> info;

  if (this->userInformation)
    info = this->userInformation->getOctetString();

  return info;
}

void RlrqApdu::setUserInformation(std::vector<uint8_t> value)
{
  if (!this->userInformation)
    this->userInformation = new AssociationInformation();

  this->userInformation->setOctetString(value);
}

std::vector<uint8_t> RlrqApdu::getContents()
{
  std::vector<uint8_t> bytes;
  std::vector<uint8_t> buffer;

  if (this->reason)
    bytes = this->reason->getBytes();

  if (this->userInformation) {
    buffer = this->userInformation->getBytes();
    bytes.insert(bytes.end(), buffer.begin(), buffer.end());
  }

  return bytes;
}
