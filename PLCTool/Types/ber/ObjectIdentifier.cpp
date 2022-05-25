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

#include "ObjectIdentifier.h"

#include <stdexcept>

#include "MarkedLongFormatInteger.h"

using namespace PLCTool;

#define ENCODING_FACTOR 40

static std::vector<uint64_t> split(std::string delimiter, std::string str);
static std::string join(std::string delimiter, std::vector<uint64_t> values);

std::vector<uint64_t>
split(std::string delimiter, std::string str)
{
  std::vector<uint64_t> integers;
  size_t start = 0;
  size_t end = 0;

  while ((end = str.find(delimiter, start)) != std::string::npos) {
    integers.push_back(std::stoi(str.substr(start, end - start)));
    start = end + delimiter.length();
  }

  integers.push_back(std::stoi(str.substr(start)));

  return integers;
}

std::string
join(std::string delimiter, std::vector<uint64_t> values)
{
  size_t i;
  std::string str;

  for (i = 0; i < values.size() - 1; ++i)
    str.append(std::to_string(values[i]) + delimiter);

  str.append(std::to_string(values[i]));

  return str;
}

/*
 * ObjectIdentifier
 */

ObjectIdentifier::ObjectIdentifier(std::string dot_separated_id)
{
  setObjectId(dot_separated_id);
}

size_t
ObjectIdentifier::getNumberOfElements() const
{
  size_t size = 0;
  size_t real_index = 0;

  if (this->bytes.size() > 0) {
    size = 2;
    real_index = 1;
  }

  while (real_index < this->bytes.size()) {
    real_index = getNextRealIndexFrom(real_index);
    ++size;
  }

  return size;
}

uint64_t
ObjectIdentifier::getValue(size_t index) const
{
  uint64_t value;

  if (index == 0)
    value = getFirstValue();
  else if (index == 1)
    value = getSecondValue();
  else
    value = getFurtherValue(index);

  return value;
}

void
ObjectIdentifier::setValue(size_t index, uint64_t value)
{
  if (index == 0)
    setFirstValue(value);
  else if (index == 1)
    setSecondValue(value);
  else
    setFurtherValue(index, value);
}

std::string
ObjectIdentifier::getObjectId() const
{
  size_t i;
  std::vector<uint64_t> values;

  for (i = 0; i < getNumberOfElements(); ++i) values.push_back(getValue(i));

  return join(".", values);
}

void
ObjectIdentifier::setObjectId(std::string dot_separated_id)
{
  std::vector<uint64_t> values = split(".", dot_separated_id);

  this->bytes.push_back(values[0] * ENCODING_FACTOR + values[1]);
  values.erase(values.begin(), values.begin() + 2);

  for (uint64_t value : values) {
    if (value < 128) {
      this->bytes.push_back(value);
    } else {
      std::vector<uint8_t> long_format_value =
          MarkedLongFormatInteger(value).getBytes();
      this->bytes.insert(
          this->bytes.end(),
          long_format_value.begin(),
          long_format_value.end());
    }
  }
}

/*
 * Private members
 */

uint8_t
ObjectIdentifier::getFirstValue() const
{
  return this->bytes[0] / ENCODING_FACTOR;
}

void
ObjectIdentifier::setFirstValue(uint8_t value)
{
  this->bytes[0] = value * ENCODING_FACTOR + getSecondValue();
}

uint8_t
ObjectIdentifier::getSecondValue() const
{
  return this->bytes[0] - getFirstValue() * ENCODING_FACTOR;
}

void
ObjectIdentifier::setSecondValue(uint8_t value)
{
  this->bytes[0] = getFirstValue() * ENCODING_FACTOR + value;
}

uint8_t
ObjectIdentifier::getFurtherValue(size_t index) const
{
  size_t first_index = getFirstRealIndexOfValue(index);
  uint8_t value;

  if (this->bytes[first_index] < 128)
    value = this->bytes[first_index];
  else
    value =
        MarkedLongFormatInteger(getLongFormatValueFrom(first_index)).getValue();

  return value;
}

void
ObjectIdentifier::setFurtherValue(size_t index, uint64_t value)
{
  size_t first_index = getFirstRealIndexOfValue(index);
  size_t next_index = getNextRealIndexFrom(first_index);

  this->bytes.erase(
      this->bytes.begin() + first_index,
      this->bytes.begin() + next_index);
  if (value < 128) {
    this->bytes.insert(this->bytes.begin() + first_index, value);
  } else {
    std::vector<uint8_t> value_bytes =
        MarkedLongFormatInteger(value).getBytes();
    this->bytes.insert(
        this->bytes.begin() + first_index,
        value_bytes.begin(),
        value_bytes.end());
  }
}

size_t
ObjectIdentifier::getFirstRealIndexOfValue(size_t index) const
{
  size_t first_index;

  if (index < 2)
    first_index = 0;
  else if (index == 2)
    first_index = 1;
  else
    first_index = getNextRealIndexFrom(getFirstRealIndexOfValue(index - 1));

  return first_index;
}

size_t
ObjectIdentifier::getNextRealIndexFrom(size_t real_index) const
{
  size_t next_index;

  if (this->bytes[real_index] < 128)
    next_index = real_index + 1;
  else
    next_index = real_index + getLongFormatValueFrom(real_index).size();

  return next_index;
}

std::vector<uint8_t>
ObjectIdentifier::getLongFormatValueFrom(size_t real_index) const
{
  size_t i = real_index;
  std::vector<uint8_t> long_format_value;

  while (i < this->bytes.size() && this->bytes[i] >= 128) {
    long_format_value.push_back(this->bytes[i]);
    ++i;
  }
  if (i < this->bytes.size())
    long_format_value.push_back(this->bytes[i]);
  else
    throw std::out_of_range("Format error parsing object id long format value");

  return long_format_value;
}
