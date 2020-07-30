//
// StringParams.h: Generic string parameter API
//
// (c) 2020 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#ifndef _TOPOLOGY_STRINGPARAMS_H
#define _TOPOLOGY_STRINGPARAMS_H

#include <map>
#include <vector>
#include <algorithm>

namespace PLCTool {
  struct StringParamEntry {
    std::string key;
    std::string value;

    int asInt(void) const;
    float asFloat(void) const;
    std::string const &asString(void) const;
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
