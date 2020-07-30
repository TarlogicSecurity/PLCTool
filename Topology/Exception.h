//
// Exception.h: The TOPOLOGY exception handling class
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
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
