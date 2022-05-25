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

#include "BitString.h"

#include <cassert>
#include <cmath>

using namespace PLCTool;

bool
BitString::getBit(size_t position) const
{
  return this->bits.at(position);
}

void
BitString::setBit(size_t position, bool value)
{
  this->bits.at(position) = value;
}

bool
BitString::removeBit(size_t position)
{
  bool value = this->bits.at(position);

  this->bits.erase(this->bits.begin() + position);

  return value;
}

void
BitString::insertBit(size_t position, bool value)
{
  this->bits.insert(this->bits.begin() + position, value);
}

bool
BitString::front() const
{
  return this->bits.front();
}

bool
BitString::back() const
{
  return this->bits.back();
}

bool
BitString::popFront()
{
  return removeBit(0);
}

void
BitString::pushFront(bool value)
{
  this->bits.insert(this->bits.begin(), value);
}

bool
BitString::popBack()
{
  return removeBit(this->bits.size() - 1);
}

void
BitString::pushBack(bool value)
{
  this->bits.push_back(value);
}

size_t
BitString::getBitStringSize() const
{
  return this->bits.size();
}

std::vector<uint8_t>
BitString::getBytes()
{
  std::vector<uint8_t> octets;

  /* Set unused bits at the end of the string */
  octets.push_back((8 - (this->bits.size() % 8)) % 8);
  for (size_t byte_index = 0; byte_index * 8 < this->bits.size();
       ++byte_index) {
    size_t bit_index = 0;
    uint8_t byte = 0;

    while ((bit_index < 8) &&
           (byte_index * 8 + bit_index < this->bits.size())) {
      if (this->bits.at(byte_index * 8 + bit_index))
        byte = byte | (0x80 >> bit_index);
      else
        byte = byte & ~(0x80 >> bit_index);

      ++bit_index;
    }

    octets.push_back(byte);
  }

  return octets;
}

void BitString::setBytes(std::vector<uint8_t> bytes)
{
  assert(false);
  /* TODO */
}

size_t
BitString::getSize()
{
  return std::ceil(this->getBitStringSize() / 8) + 1;
}
