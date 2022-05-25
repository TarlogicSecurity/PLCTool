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

#include "BerPrimitiveOctetString.h"

using namespace PLCTool;

BerPrimitiveOctetString::BerPrimitiveOctetString(
    std::vector<uint8_t> octetString)
    : BerOctetString(false)
{
  this->octetString = octetString;
  this->length = BerLength(this->octetString.size());
}

std::vector<uint8_t>
BerPrimitiveOctetString::getContents()
{
  return getContentsLocal();
}

std::vector<uint8_t>
BerPrimitiveOctetString::getContentsLocal()
{
  return this->octetString;
}

std::vector<uint8_t>
BerPrimitiveOctetString::getOctetString() const
{
  return this->octetString;
}

void
BerPrimitiveOctetString::setOctetString(std::vector<uint8_t> value)
{
  this->octetString = value;
  updateLength();
}

const uint8_t &
BerPrimitiveOctetString::at(size_t index) const
{
  return this->octetString.at(index);
}

uint8_t &
BerPrimitiveOctetString::at(size_t index)
{
  return this->octetString.at(index);
}

uint8_t BerPrimitiveOctetString::remove(size_t index)
{
  uint8_t value = this->octetString.at(index);

  this->octetString.erase(this->octetString.begin() + index);
  updateLength();

  return value;
}

void BerPrimitiveOctetString::insert(size_t index, uint8_t value)
{
  this->octetString.insert(this->octetString.begin() + index, value);
  updateLength();
}

const uint8_t &
BerPrimitiveOctetString::front() const
{
  return this->octetString.front();
}

uint8_t &
BerPrimitiveOctetString::front()
{
  return this->octetString.front();
}

const uint8_t &
BerPrimitiveOctetString::back() const
{
  return this->octetString.back();
}

uint8_t &
BerPrimitiveOctetString::back()
{
  return this->octetString.back();
}

uint8_t
BerPrimitiveOctetString::popBack()
{
  uint8_t back = this->octetString.back();

  this->octetString.pop_back();
  updateLength();

  return back;
}

void
BerPrimitiveOctetString::pushBack(uint8_t octet)
{
  this->octetString.push_back(octet);
  updateLength();
}

uint8_t
BerPrimitiveOctetString::popFront()
{
  uint8_t front = this->octetString.front();

  this->octetString.erase(this->octetString.begin());
  updateLength();

  return front;
}

void
BerPrimitiveOctetString::pushFront(uint8_t octet)
{
  this->octetString.insert(this->octetString.begin(), octet);
  updateLength();
}

size_t
BerPrimitiveOctetString::getOctetStringSize()
{
  return this->octetString.size();
}
