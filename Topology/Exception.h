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

#ifndef _TOPOLOGY_EXCEPTION_H
#define _TOPOLOGY_EXCEPTION_H

#include <exception>
#include <string>

#define PH_STRINGIFY(expr) #expr
#define PH_STRINGIFY_EXPAND(expr) PH_STRINGIFY(expr)

#define PH_THROW(nature, desc) \
  throw Exception(__FILE__, __LINE__, __PRETTY_FUNCTION__, desc, nature)

#define PH_ASSERT(expr)       \
  if (!(expr))                  \
    PH_THROW(                   \
      ASSERTION_FAILED,         \
      "Precondition \"" + std::string(PH_STRINGIFY(expr)) + "\" not met")

#define PH_CHECK_PARAM(expr, desc) \
  if (!(expr))                        \
    PH_THROW(                         \
      INVALID_PARAMETER,              \
      std::string("Invalid value for ") + desc + std::string(" parameter"))


#define PH_CHECK_GENERIC(expr, desc) \
  if (!(expr))                          \
    PH_THROW(                           \
      GENERIC,                          \
      desc)

namespace PLCTool {
  enum ExceptionNature {
    GENERIC,
    ASSERTION_FAILED,
    INVALID_PARAMETER,
    ALLOCATION_ERROR,
  };

  class Exception : public std::exception {
    std::string sFile;
    std::string sMethod;
    std::string sDescription;
    unsigned long uiLine;
    ExceptionNature eNature;

  public:
    Exception(
        std::string const &file,
        unsigned long line,
        std::string const &method,
        std::string const &desc,
        ExceptionNature nature = GENERIC);

    std::string const &file(void) const;
    std::string const &method(void) const;
    unsigned long line(void) const;
    ExceptionNature nature(void) const;
    std::string  toString(void) const;
    void printInfo(void) const;
  };
}

#endif // _TOPOLOGY_EXCEPTION_H
