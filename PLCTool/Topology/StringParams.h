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

#ifndef _TOPOLOGY_STRINGPARAMS_H
#define _TOPOLOGY_STRINGPARAMS_H

#include <map>
#include <vector>
#include <algorithm>
#include <string>

namespace PLCTool {
  struct StringParamEntry {
    std::string key;
    std::string value;

    int asInt(void) const;
    unsigned long asULong(void) const;
    unsigned long asHexULong(void) const;
    float asFloat(void) const;
    std::string const &asString(void) const;
    const std::vector<uint8_t> asByteVector(void) const;
    bool asBool(void) const;

    std::string const &operator = (std::string const &);
    int operator = (int);
    float operator = (float);
    bool operator = (bool);
  };

  typedef std::map<const std::string, StringParamEntry> StringParamEntryMap;

  class StringParams {
    StringParamEntryMap entries;

  public:
    StringParamEntry operator[] (std::string const &) const;
    StringParamEntry &operator[] (std::string const &);

    StringParamEntry operator[] (const char *) const;
    StringParamEntry &operator[] (const char *);

    StringParamEntryMap::const_iterator cbegin(void) const;
    StringParamEntryMap::const_iterator cend(void) const;

    StringParamEntryMap::iterator begin(void);
    StringParamEntryMap::iterator end(void);

    void clear(void);
    bool contains(std::string const &) const;
    bool remove(std::string const &);
  };

  std::vector<uint8_t> hexStrToVector(std::string const &str);
}

#endif /* _TOPOLOGY_STRINGPARAMS_H */
