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

#include "BerPrimitiveBitString.h"

using namespace PLCTool;

BerPrimitiveBitString::BerPrimitiveBitString(std::vector<bool> bit_string)
    : BerBitString(false)
{
  this->bitString = BitString(bit_string);
  this->length = BerLength(this->bitString.getSize());
}

bool
BerPrimitiveBitString::getBit(size_t position) const
{
  return this->bitString.getBit(position);
}

void
BerPrimitiveBitString::setBit(size_t position, bool value)
{
  this->bitString.setBit(position, value);
}

std::vector<uint8_t>
BerPrimitiveBitString::getContents()
{
  return getContentsLocal();
}

std::vector<uint8_t>
BerPrimitiveBitString::getContentsLocal()
{
  return this->bitString.getBytes();
}

bool BerPrimitiveBitString::removeBit(size_t position)
{
  bool value = this->bitString.removeBit(position);
  updateLength();

  return value;
}

void BerPrimitiveBitString::insertBit(size_t position, bool value)
{
  this->bitString.insertBit(position, value);
  updateLength();
}

bool BerPrimitiveBitString::front() const
{
  return this->bitString.front();
}

bool BerPrimitiveBitString::back() const
{
  return this->bitString.back();
}

bool BerPrimitiveBitString::popFront()
{
  bool value = this->bitString.popFront();
  updateLength();

  return value;
}

void BerPrimitiveBitString::pushFront(bool value)
{
  this->bitString.pushFront(value);
  updateLength();
}

bool BerPrimitiveBitString::popBack()
{
  bool value = this->bitString.popBack();
  updateLength();

  return value;
}

void BerPrimitiveBitString::pushBack(bool value)
{
  this->bitString.pushBack(value);
  updateLength();
}

size_t BerPrimitiveBitString::getBitStringSize() const
{
  return this->bitString.getBitStringSize();
}
