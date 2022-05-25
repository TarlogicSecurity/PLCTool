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

#include "MarklessLongFormatInteger.h"

#include <limits.h>

#include <stdexcept>

using namespace PLCTool;

#define VALUE_MASK 0xff
#define VALUE_BITS 8

#define _GET_VALUE_BITS(octet) ((octet) & (VALUE_MASK))
#define GET_VALUE_BITS(octet) _GET_VALUE_BITS(octet)

#define _REMOVE_VALUE_BITS(value) ((value) >> (VALUE_BITS))
#define REMOVE_VALUE_BITS(value) _REMOVE_VALUE_BITS(value)

#define _MAKE_SPACE_FOR_VALUE_BITS(value) ((value) << (VALUE_BITS))
#define MAKE_SPACE_FOR_VALUE_BITS(value) _MAKE_SPACE_FOR_VALUE_BITS(value)

MarklessLongFormatInteger::MarklessLongFormatInteger(uint64_t value)
{
  setValueLocal(value);
}

size_t MarklessLongFormatInteger::getLongFormatSize(uint64_t value)
{
  size_t bytes = 8;

  if (value <= UINT32_MAX) {
    if (value <= 0xffffff) {
      if (value <= UINT16_MAX) {
        if (value <= UINT8_MAX) {
          bytes = 1;
        } else {
          bytes = 2;
        }
      } else {
        bytes = 3;
      }
    } else {
      bytes = 4;
    }
  } else {
    if (value <= 0xffffffffffffff) {
      if (value <= 0xffffffffffff) {
        if (value <= 0xffffffffff) {
          bytes = 5;
        } else {
          bytes = 6;
        }
      } else {
        bytes = 7;
      }
    } else {
      bytes = 8;
    }
  }

  return bytes;
}

uint64_t
MarklessLongFormatInteger::getValue() const
{
  return getValueLocal();
}

void
MarklessLongFormatInteger::setValue(uint64_t value)
{
  setValueLocal(value);
}

uint64_t
MarklessLongFormatInteger::getValueLocal() const
{
  uint64_t value = 0;

  for (uint8_t octet : this->bytes) {
    value = MAKE_SPACE_FOR_VALUE_BITS(value) | octet;
  }

  return value;
}

void
MarklessLongFormatInteger::setValueLocal(uint64_t value)
{
  uint64_t rest = value;

  this->bytes.clear();
  while (rest) {
    this->bytes.insert(this->bytes.begin(), GET_VALUE_BITS(rest));
    rest = REMOVE_VALUE_BITS(rest);
  }
}

/*
 * Private members
 */
uint8_t
MarklessLongFormatInteger::getOctet(size_t index) const
{
  if (this->bytes.size() <= index)
    throw std::out_of_range("Long format integer index out of range");

  return this->bytes[index];
}
