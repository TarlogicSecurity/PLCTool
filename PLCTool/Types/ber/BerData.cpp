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

#include "BerData.h"

#include <cassert>

using namespace PLCTool;

void
BerData::updateLength()
{
  this->length.setValue(getContents().size());
}

std::vector<uint8_t>
BerData::getBytes()
{
  updateLength();

  std::vector<uint8_t> bytes;
  std::vector<uint8_t> identifier_bytes = this->identifier.getBytes();
  std::vector<uint8_t> length_bytes = this->length.getBytes();
  std::vector<uint8_t> content_bytes = getContents();

  bytes.insert(bytes.end(), identifier_bytes.begin(), identifier_bytes.end());
  bytes.insert(bytes.end(), length_bytes.begin(), length_bytes.end());
  bytes.insert(bytes.end(), content_bytes.begin(), content_bytes.end());

  return bytes;
}

void
BerData::setBytes(std::vector<uint8_t> bytes)
{
  assert(false);
  /* TODO */
}

size_t
BerData::getSize()
{
  return getBytes().size();
}

bool
BerData::sanitizeBERDataBytes(
    std::vector<uint8_t> bytes,
    std::vector<uint8_t> &identifier_bytes,
    std::vector<uint8_t> &length_bytes,
    std::vector<uint8_t> &contents_bytes)
{
  std::vector<uint8_t> new_identifier_bytes;
  std::vector<uint8_t> new_length_bytes;
  std::vector<uint8_t> new_contents_bytes;
  bool ok = false;

  if (bytes.size()) {
    new_identifier_bytes = BerIdentifier::sanitizeBERIdentifierBytes(bytes);

    std::vector<uint8_t> rest(
        bytes.begin() + new_identifier_bytes.size(),
        bytes.end());
    if (rest.size()) {
      new_length_bytes = BerLength::sanitizeBERLengthBytes(rest);
      BerLength length(new_length_bytes);

      rest.erase(rest.begin(), rest.begin() + new_length_bytes.size());
      if (rest.size()) {
        if (length.isIndefinite())
          new_contents_bytes =
              BerContents::sanitizeBERIndefiniteContentsBytes(rest);
        else if (length.getValue())
          new_contents_bytes =
              BerContents::sanitizeBERContentsBytes(rest, length.getValue());
      }
    }

    ok = new_identifier_bytes.size() && new_length_bytes.size() &&
         new_contents_bytes.size();
  }

  if (ok) {
    identifier_bytes = new_identifier_bytes;
    length_bytes = new_length_bytes;
    contents_bytes = new_contents_bytes;
  }

  return ok;
}

BER_TAG_CLASS
BerData::getTagClass() const
{
  return this->identifier.getTagClass();
}

void
BerData::setTagClass(BER_TAG_CLASS tag_class)
{
  this->identifier.setTagClass(tag_class);
}

bool
BerData::isConstructed() const
{
  return this->identifier.isConstructed();
}

uint64_t
BerData::getTagType() const
{
  return this->identifier.getTagType();
}

void
BerData::setTagType(uint64_t tag_type)
{
  this->identifier.setTagType(tag_type);
}

uint64_t
BerData::getLength()
{
  return getContents().size();
}
