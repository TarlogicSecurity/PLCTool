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

#include "Conformance.h"

using namespace PLCTool;

Conformance::Conformance(DLMS_CONFORMANCE value)
{
  this->identifier = BerIdentifier(BER_APPLICATION_CLASS, false, 0x1f);
  this->value = value;
  this->length = BerLength(getContentsLocal().size());
}

DLMS_CONFORMANCE
Conformance::getValue() const
{
  return this->value;
}

void
Conformance::setValue(DLMS_CONFORMANCE value)
{
  this->value = value;
  updateLength();
}

DLMS_CONFORMANCE
Conformance::getConformanceBit(DLMS_CONFORMANCE mask) const
{
  uint32_t *value_pointer = (uint32_t *) (&this->value);
  uint32_t *mask_pointer = (uint32_t *) (&mask);
  uint32_t conformance_bit = *value_pointer & *mask_pointer;
  DLMS_CONFORMANCE *conformance_bit_pointer =
      (DLMS_CONFORMANCE *) (&conformance_bit);

  return *conformance_bit_pointer;
}

void
Conformance::setConformanceBit(DLMS_CONFORMANCE mask, bool set)
{
  uint32_t *value_pointer = (uint32_t *) (&this->value);
  uint32_t *mask_pointer = (uint32_t *) (&mask);
  uint32_t conformance_bit;

  if (set)
    conformance_bit = *value_pointer | *mask_pointer;
  else
    conformance_bit = *value_pointer & ~(*mask_pointer);

  DLMS_CONFORMANCE *conformance_bit_pointer =
      (DLMS_CONFORMANCE *) (&conformance_bit);

  this->value = *conformance_bit_pointer;
  updateLength();
}

DLMS_CONFORMANCE
Conformance::bytesToConformance(std::vector<uint8_t> bytes)
{
  std::vector<uint8_t> conformance_bytes;
  uint32_t conformance = 0x00;

  if (bytes.size() > 3)
    conformance_bytes =
        std::vector<uint8_t>(bytes.begin() + 1, bytes.begin() + 4);

  for (uint8_t byte : conformance_bytes) conformance = conformance << 8 | byte;

  return (DLMS_CONFORMANCE) conformance;
}

std::vector<uint8_t>
Conformance::conformanceToBytes(DLMS_CONFORMANCE conformance)
{
  std::vector<uint8_t> conformance_bytes;
  uint32_t conformance_buffer = conformance;
  size_t i;

  for (i = 0; i < 3; ++i) {
    conformance_bytes.insert(
        conformance_bytes.begin(),
        conformance_buffer & 0xff);
    conformance_buffer = conformance_buffer >> 8;
  }
  conformance_bytes.insert(conformance_bytes.begin(), 0x00);

  return conformance_bytes;
}

std::vector<uint8_t>
Conformance::getContents()
{
  return getContentsLocal();
}

std::vector<uint8_t>
Conformance::getContentsLocal()
{
  return conformanceToBytes(this->value);
}
