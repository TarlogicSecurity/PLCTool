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

#include "DataArray.h"

#include <cassert>

using namespace PLCTool;

const std::vector<Data *> &
DataArray::getElements() const
{
  return elements;
}

void
DataArray::setElements(const std::vector<Data *> &newElements)
{
  deleteAll();

  for (Data *element : newElements) this->elements.push_back(element->clone());
}

size_t
DataArray::getElementNumber() const
{
  return this->elements.size();
}

void
DataArray::deleteAll()
{
  for (Data *element : this->elements) delete element;

  this->elements.clear();
}

DataArray::~DataArray()
{
  deleteAll();
}

const Data &
DataArray::at(size_t position) const
{
  return *this->elements.at(position);
}

Data &
DataArray::at(size_t position)
{
  return *this->elements.at(position);
}

void
DataArray::erase(size_t position)
{
  delete this->elements.at(position);
  this->elements.erase(this->elements.begin() + position);
}

void
DataArray::insert(size_t position, const Data &element)
{
  this->elements.insert(this->elements.begin() + position, element.clone());
}

std::vector<uint8_t>
DataArray::getBytes()
{
  std::vector<uint8_t> bytes;
  std::vector<uint8_t> buffer;

  /* DataArray tag */
  bytes.push_back(0x01);
  for (Data *element : this->elements) {
    buffer = element->getBytes();
    bytes.insert(bytes.end(), buffer.begin(), buffer.end());
  }

  return bytes;
}

void
DataArray::setBytes(std::vector<uint8_t> bytes)
{
  assert(false);
  /* TODO */
}

size_t
DataArray::getSize()
{
  return this->getBytes().size();
}

Data *
DataArray::clone() const
{
  return new DataArray(this->getElements());
}
