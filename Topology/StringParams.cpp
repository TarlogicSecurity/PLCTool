//
// StringParams.cpp: Generic string parameter API
//
// (c) 2020 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/StringParams.h>
#include <string>
#include <iostream>
#include <cmath>

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

bool
StringParamEntry::asBool(void) const
{
  std::string asLower = this->value;

  for (auto p : asLower)
    p = std::tolower(static_cast<char>(p));

  return asLower != "no"
       && asLower != "0"
       && asLower != "false"
       && asLower.length() != 0;
}

std::string const &
StringParamEntry::operator = (std::string const &val)
{
  return this->value = val;
}

int
StringParamEntry::operator = (int val)
{
  (void) this->operator=(std::to_string(val));

  return val;
}

float
StringParamEntry::operator = (float val)
{
  (void) this->operator=(std::to_string(val));

  return val;
}

bool
StringParamEntry::operator = (bool val)
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
