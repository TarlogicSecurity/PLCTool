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

#include "BerIdentifier.h"

#include <math.h>

#include <stdexcept>

#include "MarkedLongFormatInteger.h"

using namespace PLCTool;

#define TAG_CLASS_MASK 0xc0
#define CONSTRUCTED_MASK 0x20
#define TAG_TYPE_MASK 0x1f

#define TAG_CLASS_BITS 2
#define CONSTRUCTED_BITS 1
#define TAG_TYPE_BITS 5

#define _TAG_CLASS(octet) ((octet) & (TAG_CLASS_MASK))
#define TAG_CLASS(octet) _TAG_CLASS(octet)
#define _CONSTRUCTED(octet) ((octet) & (CONSTRUCTED_MASK))
#define CONSTRUCTED(octet) _CONSTRUCTED(octet)
#define _TAG_TYPE(octet) ((octet) & (TAG_TYPE_MASK))
#define TAG_TYPE(octet) _TAG_TYPE(octet)

#define _NOT_TAG_CLASS(octet) ((octet) & ~(TAG_CLASS_MASK))
#define NOT_TAG_CLASS(octet) _NOT_TAG_CLASS(octet)
#define _NOT_CONSTRUCTED(octet) ((octet) & ~(CONSTRUCTED_MASK))
#define NOT_CONSTRUCTED(octet) _NOT_CONSTRUCTED(octet)
#define _NOT_TAG_TYPE(octet) ((octet) & ~(TAG_TYPE_MASK))
#define NOT_TAG_TYPE(octet) _NOT_TAG_TYPE(octet)

#define GET_TAG_CLASS(octet) \
  (TAG_CLASS(octet) >> ((CONSTRUCTED_BITS) + (TAG_TYPE_BITS)))
#define GET_CONSTRUCTED(octet) (CONSTRUCTED(octet) >> (TAG_TYPE_BITS))
#define GET_TAG_TYPE(octet) TAG_TYPE(octet)

#define _SHIFT_TO_TAG_CLASS(octet) \
  ((octet) << ((CONSTRUCTED_BITS) + (TAG_TYPE_BITS)))
#define SHIFT_TO_TAG_CLASS(octet) _SHIFT_TO_TAG_CLASS(octet)
#define _SHIFT_TO_CONSTRUCTED(octet) ((octet) << (TAG_TYPE_BITS))
#define SHIFT_TO_CONSTRUCTED(octet) _SHIFT_TO_CONSTRUCTED(octet)
#define _SHIFT_TO_TAG_TYPE(octet) (octet)
#define SHIFT_TO_TAG_TYPE(octet) _SHIFT_TO_TAG_TYPE(octet)

#define SET_TAG_CLASS(octet, tag_class) \
  (TAG_CLASS(SHIFT_TO_TAG_CLASS(tag_class)) | NOT_TAG_CLASS(octet))
#define _SET_CONSTRUCTED(octet, constructed) \
  (((constructed) ? (CONSTRUCTED_MASK) : 0) | NOT_CONSTRUCTED(octet))
#define SET_CONSTRUCTED(octet, constructed) _SET_CONSTRUCTED(octet, constructed)
#define SET_TAG_TYPE(octet, tag_type) \
  (TAG_TYPE(SHIFT_TO_TAG_TYPE(tag_type)) | NOT_TAG_TYPE(octet))

#define SET_TAG_FIELDS(tag_class, constructed, tag_type)                 \
  (SET_TAG_CLASS(0x00, tag_class) | SET_CONSTRUCTED(0x00, constructed) | \
   SET_TAG_TYPE(0x00, tag_type))

BerIdentifier::BerIdentifier(std::vector<uint8_t> bytes) : BerIdentifier()
{
  std::vector<uint8_t> new_bytes = sanitizeBERIdentifierBytes(bytes);

  if (new_bytes.size())
    setBytes(new_bytes);
};

BerIdentifier::BerIdentifier(
    BER_TAG_CLASS tag_class,
    bool constructed,
    uint64_t tag_type)
    : BerIdentifier()
{
  setTagClass(tag_class);
  setConstructed(constructed);
  setTagType(tag_type);
}

std::vector<uint8_t>
BerIdentifier::sanitizeBERIdentifierBytes(std::vector<uint8_t> bytes)
{
  std::vector<uint8_t> new_bytes;

  if (bytes.size() > 1) {
    uint8_t head = bytes[0];
    new_bytes.push_back(head);
    if (GET_TAG_TYPE(head) == TAG_TYPE_MASK) {
      std::vector<uint8_t> tail(bytes.begin() + 1, bytes.end());
      std::vector<uint8_t> long_format_bytes =
          MarkedLongFormatInteger::sanitizeMarkedLongFormatIntegerBytes(tail);
      new_bytes.insert(
          new_bytes.begin(),
          long_format_bytes.begin(),
          long_format_bytes.end());
    }
  }

  return new_bytes;
}

BER_TAG_CLASS
BerIdentifier::getTagClass() const
{
  return (BER_TAG_CLASS) GET_TAG_CLASS(getHead());
}

void
BerIdentifier::setTagClass(BER_TAG_CLASS tag_class)
{
  setHead(SET_TAG_CLASS(getHead(), tag_class));
}

bool
BerIdentifier::isConstructed() const
{
  return GET_CONSTRUCTED(getHead());
}

void
BerIdentifier::setConstructed(bool constructed)
{
  setHead(SET_CONSTRUCTED(getHead(), constructed));
}

uint64_t
BerIdentifier::getTagType() const
{
  uint64_t tagType;

  if (!isTagTypeLongForm())
    tagType = getTagTypeShortForm();
  else
    tagType = getTagTypeLongForm();

  return tagType;
}

void
BerIdentifier::setTagType(uint64_t tag_type)
{
  if (tag_type < TAG_TYPE_MASK)
    setTagTypeShortForm(tag_type);
  else
    setTagTypeLongForm(tag_type);
}

/*
 * Private members
 */
uint8_t
BerIdentifier::getHead() const
{
  return this->bytes[0];
}

void
BerIdentifier::setHead(uint8_t head)
{
  this->bytes[0] = head;
}

std::vector<uint8_t>
BerIdentifier::getTail() const
{
  return std::vector<uint8_t>(this->bytes.begin() + 1, this->bytes.end());
}

void
BerIdentifier::setTail(std::vector<uint8_t> tail)
{
  this->bytes.erase(this->bytes.begin() + 1, this->bytes.end());
  this->bytes.insert(this->bytes.end(), tail.begin(), tail.end());
}

bool
BerIdentifier::isTagTypeLongForm() const
{
  return getTagTypeShortForm() == TAG_TYPE_MASK;
}

uint8_t
BerIdentifier::getTagTypeShortForm() const
{
  return GET_TAG_TYPE(getHead());
}

void
BerIdentifier::setTagTypeShortForm(uint8_t tag_type)
{
  setHead(SET_TAG_TYPE(getHead(), tag_type));
}

uint64_t
BerIdentifier::getTagTypeLongForm() const
{
  if (!isTagTypeLongForm())
    throw std::out_of_range(
        "Tried to read a short format tag type as long format");

  return MarkedLongFormatInteger(getTail()).getValue();
}

void
BerIdentifier::setTagTypeLongForm(uint64_t tag_type)
{
  setTagTypeShortForm(TAG_TYPE_MASK);
  setTail(MarkedLongFormatInteger(tag_type).getBytes());
}
