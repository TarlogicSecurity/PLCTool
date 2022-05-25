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

#include "InitiateRequestApdu.h"

#include <PLCTool/Types/ber/BerPrimitiveOctetString.h>

#include <cassert>

using namespace PLCTool;

InitiateRequestApdu::InitiateRequestApdu(
    uint8_t proposedDlmsVersionNumber,
    DLMS_CONFORMANCE proposedConformance,
    uint16_t clientMaxReceivePduSize)
{
  this->proposedDlmsVersionNumber = BerInteger(proposedDlmsVersionNumber);
  this->proposedConformance = Conformance(proposedConformance);
  this->clientMaxReceivePduSize = BerInteger(clientMaxReceivePduSize);
}

InitiateRequestApdu::~InitiateRequestApdu()
{
  if (this->dedicatedKey)
    delete this->dedicatedKey;
  if (this->responseAllowed)
    delete this->responseAllowed;
  if (this->proposedQualityOfService)
    delete this->proposedQualityOfService;
}

std::vector<uint8_t>
InitiateRequestApdu::getDedicatedKey() const
{
  std::vector<uint8_t> bytes;

  if (this->dedicatedKey)
    bytes = this->dedicatedKey->getContents();

  return bytes;
}

void
InitiateRequestApdu::setDedicatedKey(std::vector<uint8_t> dedicated_key)
{
  if (!this->dedicatedKey)
    this->dedicatedKey = new BerPrimitiveOctetString();

  this->dedicatedKey->setOctetString(dedicated_key);
}

bool
InitiateRequestApdu::getResponseAllowed() const
{
  bool value = true;

  if (this->responseAllowed)
    value = this->responseAllowed->getValue();

  return value;
}

void
InitiateRequestApdu::setResponseAllowed(bool set)
{
  if (!set) {
    if (!this->responseAllowed)
      this->responseAllowed = new BerBoolean();

    this->responseAllowed->setValue(set);
  }
}

int8_t
InitiateRequestApdu::getProposedQualityOfService() const
{
  int8_t value = 0;

  if (this->proposedQualityOfService)
    value = this->proposedQualityOfService->getValue();

  return value;
}

void
InitiateRequestApdu::setProposedQualityOfService(int8_t value)
{
  if (value) {
    if (!this->proposedQualityOfService)
      this->proposedQualityOfService = new BerInteger();

    this->proposedQualityOfService->setValue(value);
  }
}

uint8_t
InitiateRequestApdu::getProposedDLMSVersionNumber() const
{
  return this->proposedDlmsVersionNumber.getValue();
}

void
InitiateRequestApdu::setProposedDLMSVersionNumber(uint8_t value)
{
  this->proposedDlmsVersionNumber.setValue(value);
}

DLMS_CONFORMANCE
InitiateRequestApdu::getProposedConformance() const
{
  return this->proposedConformance.getValue();
}

void
InitiateRequestApdu::setProposedConformance(DLMS_CONFORMANCE value)
{
  this->proposedConformance.setValue(value);
}

DLMS_CONFORMANCE
InitiateRequestApdu::getProposedConformanceBit(DLMS_CONFORMANCE bit) const
{
  return this->proposedConformance.getConformanceBit(bit);
}

void
InitiateRequestApdu::setProposedConformanceBit(DLMS_CONFORMANCE bit, bool set)
{
  this->proposedConformance.setConformanceBit(bit, set);
}

uint16_t
InitiateRequestApdu::getClientMaxReceivePduSize() const
{
  return this->clientMaxReceivePduSize.getValue();
}

void
InitiateRequestApdu::setClientMaxReceivePduSize(uint16_t value)
{
  this->clientMaxReceivePduSize.setValue(value);
}

std::vector<uint8_t>
InitiateRequestApdu::getBytes()
{
  std::vector<uint8_t> bytes;
  std::vector<uint8_t> buffer;

  bytes.push_back(0x01);

  if (this->dedicatedKey) {
    buffer = this->dedicatedKey->getContents();
    bytes.insert(bytes.end(), buffer.begin(), buffer.end());
  } else {
    bytes.push_back(0x00);
  }

  if (this->responseAllowed) {
    buffer = this->responseAllowed->getContents();
    bytes.insert(bytes.end(), buffer.begin(), buffer.end());
  } else {
    bytes.push_back(0x00);
  }

  if (this->proposedQualityOfService) {
    buffer = this->proposedQualityOfService->getContents();
    bytes.insert(bytes.end(), buffer.begin(), buffer.end());
  } else {
    bytes.push_back(0x00);
  }

  bytes.push_back(this->proposedDlmsVersionNumber.getValue());
  buffer = this->proposedConformance.getBytes();
  bytes.insert(bytes.end(), buffer.begin(), buffer.end());
  buffer = this->clientMaxReceivePduSize.getContents();
  bytes.insert(bytes.end(), buffer.begin(), buffer.end());

  return bytes;
}

void
InitiateRequestApdu::setBytes(std::vector<uint8_t> bytes)
{
  assert(false);
  /* TODO */
}

size_t
InitiateRequestApdu::getSize()
{
  return getBytes().size();
}
