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

#include "AssociationInformation.h"

#include <PLCTool/Types/ber/BerPrimitiveOctetString.h>

using namespace PLCTool;

AssociationInformation::AssociationInformation(std::vector<uint8_t> octetString)
{
  this->identifier = BerIdentifier(BER_CONTEXT_SPECIFIC_CLASS, true, 0x1e);
  this->octetString = new BerPrimitiveOctetString(octetString);
  this->length = BerLength(getContentsLocal().size());
}

AssociationInformation::~AssociationInformation()
{
  if (this->octetString)
    delete this->octetString;
}

const uint8_t &
AssociationInformation::at(size_t index) const
{
  return this->octetString->at(index);
}

uint8_t &
AssociationInformation::at(size_t index)
{
  return this->octetString->at(index);
}

uint8_t
AssociationInformation::remove(size_t index)
{
  return this->octetString->remove(index);
  updateLength();
}

void
AssociationInformation::insert(size_t index, uint8_t value)
{
  this->octetString->insert(index, value);
  updateLength();
}

const uint8_t &
AssociationInformation::front() const
{
  return this->octetString->front();
}

uint8_t &
AssociationInformation::front()
{
  return this->octetString->front();
}

const uint8_t &
AssociationInformation::back() const
{
  return this->octetString->back();
}

uint8_t &
AssociationInformation::back()
{
  return this->octetString->back();
}

uint8_t
AssociationInformation::popBack()
{
  return this->octetString->popBack();
  updateLength();
}

void
AssociationInformation::pushBack(uint8_t octet)
{
  this->octetString->pushBack(octet);
  updateLength();
}

uint8_t
AssociationInformation::popFront()
{
  return this->octetString->popFront();
  updateLength();
}

void
AssociationInformation::pushFront(uint8_t octet)
{
  this->octetString->pushFront(octet);
  updateLength();
}

size_t
AssociationInformation::getOctetStringSize()
{
  return this->octetString->getOctetStringSize();
}

std::vector<uint8_t>
AssociationInformation::getOctetString() const
{
  return this->octetString->getOctetString();
}

void
AssociationInformation::setOctetString(std::vector<uint8_t> value)
{
  this->octetString->setOctetString(value);
  updateLength();
}

std::vector<uint8_t>
AssociationInformation::getContents()
{
  return getContentsLocal();
}

std::vector<uint8_t>
AssociationInformation::getContentsLocal()
{
  return this->octetString->getBytes();
}
