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

#include "BerLength.h"

#include "MarklessLongFormatInteger.h"

using namespace PLCTool;

#define FORM_MASK 0x80
#define LENGTH_MASK 0x7f

#define FORM_BITS 1
#define LENGTH_BITS 7

#define _FORM(octet) ((octet) & (FORM_MASK))
#define FORM(octet) _FORM(octet)
#define _LENGTH(octet) ((octet) & (LENGTH_MASK))
#define LENGTH(octet) _LENGTH(octet)

#define _NOT_FORM(octet) ((octet) & ~(FORM_MASK))
#define NOT_FORM(octet) _NOT_FORM(octet)
#define _NOT_LENGTH(octet) ((octet) & ~(LENGTH_MASK))
#define NOT_LENGTH(octet) _NOT_LENGTH(octet)

#define GET_FORM(octet) (FORM(octet) >> (LENGTH_BITS))
#define GET_LENGTH(octet) LENGTH(octet)

#define _SHIFT_TO_FORM(octet) ((octet) << (LENGTH_BITS))
#define SHIFT_TO_FORM(octet) _SHIFT_TO_FORM(octet)
#define _SHIFT_TO_LENGTH(octet) (octet)
#define SHIFT_TO_LENGTH(octet) _SHIFT_TO_LENGTH(octet)

#define _SET_FORM(octet, form) (((form) ? (FORM_MASK) : 0) | NOT_FORM(octet))
#define SET_FORM(octet, form) _SET_FORM(octet, form)
#define SET_LENGTH(octet, length) \
  (LENGTH(SHIFT_TO_LENGTH(length)) | NOT_LENGTH(octet))

#define SHORT_FORM_LENGTH(length) SET_LENGTH(0, length)
#define LONG_FORM_LENGTH_HEAD(length_size) SET_LENGTH(FORM_MASK, length_size)

BerLength::BerLength(std::vector<uint8_t> bytes) : BerLength()
{
  std::vector<uint8_t> new_bytes = sanitizeBERLengthBytes(bytes);

  if (new_bytes.size())
    setBytes(new_bytes);
}

BerLength::BerLength(uint64_t length)
{
  if (length < 128) {
    this->bytes.push_back(SHORT_FORM_LENGTH(length));
  } else {
    this->bytes.push_back(LONG_FORM_LENGTH_HEAD(
        MarklessLongFormatInteger::getLongFormatSize(length)));
    std::vector<uint8_t> tail = MarklessLongFormatInteger(length).getBytes();
    this->bytes.insert(this->bytes.begin(), tail.begin(), tail.end());
  }
}

std::vector<uint8_t>
BerLength::sanitizeBERLengthBytes(std::vector<uint8_t> bytes)
{
  std::vector<uint8_t> new_bytes;

  if (bytes.size()) {
    uint8_t head = bytes[0];
    if (!GET_FORM(head) || GET_LENGTH(head) < 127) {
      new_bytes.push_back(head);

      if (GET_FORM(head) && GET_LENGTH(head) > 0) {
        size_t length_size = GET_LENGTH(head);
        new_bytes.insert(
            new_bytes.end(),
            bytes.begin() + 1,
            bytes.begin() + 1 + length_size);
      }
    }
  }

  return new_bytes;
}

bool
BerLength::isIndefinite() const
{
  uint8_t head = getHead();

  return GET_FORM(head) && GET_LENGTH(head) == 0;
}

void
BerLength::setIndefinite()
{
  setHead(SET_FORM(0, true));
}

uint64_t
BerLength::getValue() const
{
  uint64_t length = 0;

  if (isDefinite()) {
    if (isShortForm()) {
      length = GET_LENGTH(getHead());
    } else {
      length = MarklessLongFormatInteger(getTail()).getValue();
    }
  }

  return length;
}

void
BerLength::setValue(uint64_t length)
{
  if (length < 128) {
    setHead(SHORT_FORM_LENGTH(length));
  } else {
    std::vector<uint8_t> new_tail =
        MarklessLongFormatInteger(length).getBytes();
    setHead(LONG_FORM_LENGTH_HEAD(new_tail.size()));
    setTail(new_tail);
  }
}

/*
 * Private members
 */
uint8_t
BerLength::getHead() const
{
  return this->bytes[0];
}

void
BerLength::setHead(uint8_t head)
{
  this->bytes[0] = head;
}

std::vector<uint8_t>
BerLength::getTail() const
{
  return std::vector<uint8_t>(this->bytes.begin() + 1, this->bytes.end());
}

void
BerLength::setTail(std::vector<uint8_t> tail)
{
  this->bytes.erase(this->bytes.begin() + 1, this->bytes.end());
  this->bytes.insert(this->bytes.end(), tail.begin(), tail.end());
}

bool
BerLength::isDefinite() const
{
  return !isIndefinite() && !isReserved();
}

bool
BerLength::isReserved() const
{
  uint8_t head = getHead();

  return GET_FORM(head) && GET_LENGTH(head) == 127;
}

bool
BerLength::isShortForm() const
{
  return !GET_FORM(getHead());
}
