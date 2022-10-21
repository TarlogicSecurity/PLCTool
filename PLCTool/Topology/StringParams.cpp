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

#include <PLCTool/Topology/StringParams.h>

#include <cmath>
#include <iostream>
#include <string>

using namespace PLCTool;

std::vector<uint8_t>
PLCTool::hexStrToVector(std::string const &str)
{
  std::vector<uint8_t> data;
  uint32_t byte;

  for (unsigned i = 0; i < str.length(); i += 2) {
    if (sscanf(str.data() + i, "%02x", &byte) != 1) {
      return std::vector<uint8_t>();
    }

    data.push_back(byte);
  }

  return data;
}

////////////////////////////// StringParamEntry ///////////////////////////////
int
StringParamEntry::asInt(void) const
{
  int ret = 0;

  try {
    ret = std::stoi(this->value);
  } catch (std::invalid_argument const &e) {
    ret = 0;
  } catch (std::out_of_range const &e) {
    ret = 0;
  }

  return ret;
}

unsigned long
StringParamEntry::asULong(void) const
{
  unsigned long ret = 0;

  try {
    ret = std::stoul(this->value);
  } catch (std::invalid_argument const &e) {
    ret = 0;
  } catch (std::out_of_range const &e) {
    ret = 0;
  }

  return ret;
}

unsigned long
StringParamEntry::asHexULong(void) const
{
  unsigned long ret = 0;

  try {
    ret = std::stoul(this->value, nullptr, 16);
  } catch (std::invalid_argument const &e) {
    ret = 0;
  } catch (std::out_of_range const &e) {
    ret = 0;
  }

  return ret;
}

float
StringParamEntry::asFloat(void) const
{
  float ret = 0;

  try {
    ret = std::stof(this->value);
  } catch (std::invalid_argument const &e) {
    ret = NAN;
  } catch (std::out_of_range const &e) {
    ret = INFINITY;
  }

  return ret;
}

std::string const &
StringParamEntry::asString(void) const
{
  return this->value;
}

const std::vector<uint8_t>
StringParamEntry::asByteVector(void) const
{
  std::string hex_string(this->asString());
  std::vector<uint8_t> byte_vector;

  if (hex_string.size() % 2 == 1)
    hex_string.insert(hex_string.begin(), '0');

  for (int i = 0; i < hex_string.size() / 2; ++i) {
    uint8_t value = (uint8_t) std::stoul(hex_string.substr(i * 2, 2), nullptr, 16);
    byte_vector.push_back(value);
  }

  return byte_vector;
}

bool
StringParamEntry::asBool(void) const
{
  std::string asLower = this->value;

  for (auto p : asLower) p = std::tolower(static_cast<char>(p));

  return asLower != "no" && asLower != "0" && asLower != "false" &&
         asLower.length() != 0;
}

std::string const &
StringParamEntry::operator=(std::string const &val)
{
  return this->value = val;
}

int
StringParamEntry::operator=(int val)
{
  (void) this->operator=(std::to_string(val));

  return val;
}

float
StringParamEntry::operator=(float val)
{
  (void) this->operator=(std::to_string(val));

  return val;
}

bool
StringParamEntry::operator=(bool val)
{
  (void) this->operator=(std::string(val ? "true" : "false"));

  return val;
}

////////////////////////////// StringParams ///////////////////////////////////
bool
StringParams::contains(std::string const &key) const
{
  return this->entries.find(key) != this->entries.end();
}

StringParamEntry
StringParams::operator[](std::string const &key) const
{
  if (!this->contains(key)) {
    StringParamEntry empty;
    empty.key = key;
    return StringParamEntry();
  }

  return this->entries.find(key)->second;
}

StringParamEntry &
StringParams::operator[](std::string const &key)
{
  if (!this->contains(key)) {
    StringParamEntry entry;
    entry.key = key;
    this->entries.emplace(key, entry);
  }

  return this->entries[key];
}

StringParamEntry
StringParams::operator[](const char *key) const
{
  return (*this)[std::string(key)];
}

StringParamEntry &
StringParams::operator[](const char *key)
{
  return (*this)[std::string(key)];
}

bool
StringParams::remove(std::string const &key)
{
  return this->entries.erase(key) > 0;
}

StringParamEntryMap::iterator
StringParams::begin(void)
{
  return this->entries.begin();
}

StringParamEntryMap::iterator
StringParams::end(void)
{
  return this->entries.end();
}

StringParamEntryMap::const_iterator
StringParams::cbegin(void) const
{
  return this->entries.cbegin();
}

StringParamEntryMap::const_iterator
StringParams::cend(void) const
{
  return this->entries.cend();
}

void
StringParams::clear(void)
{
  this->entries.clear();
}
