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

#include "MarkedLongFormatInteger.h"

#include <stdexcept>

using namespace PLCTool;

#define MORE_MASK 0x80
#define VALUE_MASK 0x7f

#define MORE_BITS 1
#define VALUE_BITS 7

#define _MORE(octet) ((octet) & (MORE_MASK))
#define MORE(octet) _MORE(octet)

#define _VALUE(octet) ((octet) & (VALUE_MASK))
#define VALUE(octet) _VALUE(octet)

#define GET_MORE(octet) (MORE(octet) >> (VALUE_BITS))
#define GET_VALUE(octet) VALUE(octet)
#define SET_MORE_BIT(octet) ((MORE_MASK) | VALUE(octet))

#define _REMOVE_VALUE_BITS(value) ((value) >> (VALUE_BITS))
#define REMOVE_VALUE_BITS(value) _REMOVE_VALUE_BITS(value)

#define _MAKE_SPACE_FOR_VALUE_BITS(value) ((value) << (VALUE_BITS))
#define MAKE_SPACE_FOR_VALUE_BITS(value) _MAKE_SPACE_FOR_VALUE_BITS(value)

MarkedLongFormatInteger::MarkedLongFormatInteger(uint64_t value)
{
  setValueLocal(value);
}

MarkedLongFormatInteger::MarkedLongFormatInteger(std::vector<uint8_t> bytes)
{
  this->bytes = sanitizeMarkedLongFormatIntegerBytes(bytes);
}

std::vector<uint8_t>
MarkedLongFormatInteger::sanitizeMarkedLongFormatIntegerBytes(
    std::vector<uint8_t> bytes)
{
  size_t i;
  std::vector<uint8_t> new_bytes;

  i = 0;
  while (i < bytes.size() && GET_MORE(bytes[i])) {
    new_bytes.push_back(bytes[i]);
    ++i;
  }

  return new_bytes;
}

uint64_t
MarkedLongFormatInteger::getValue() const
{
  return getValueLocal();
}

void
MarkedLongFormatInteger::setValue(uint64_t value)
{
  setValueLocal(value);
}

uint64_t
MarkedLongFormatInteger::getValueLocal() const
{
  size_t i;
  uint64_t value = 0;

  i = 0;
  while (i < this->bytes.size() && isThereMore(i)) {
    value |= MAKE_SPACE_FOR_VALUE_BITS(getValue(i));
  }

  if (i < this->bytes.size() && !isThereMore(i))
    value |= getValue(i);
  else
    throw std::out_of_range("Format error in size marked long format integer");

  return value;
}

void
MarkedLongFormatInteger::setValueLocal(uint64_t value)
{
  uint64_t rest = value;

  this->bytes.clear();
  this->bytes.push_back(GET_VALUE(rest));
  rest = REMOVE_VALUE_BITS(rest);

  while (rest) {
    this->bytes.insert(this->bytes.begin(), SET_MORE_BIT(rest));
    rest = REMOVE_VALUE_BITS(rest);
  }
}

/*
 * Private members
 */
uint8_t
MarkedLongFormatInteger::getOctet(size_t index) const
{
  if (this->bytes.size() <= index)
    throw std::out_of_range("Long format integer index out of range");

  return this->bytes[index];
}

bool
MarkedLongFormatInteger::isThereMore(size_t index) const
{
  return GET_MORE(getOctet(index));
}

uint8_t
MarkedLongFormatInteger::getValue(size_t index) const
{
  return GET_VALUE(getOctet(index));
}
