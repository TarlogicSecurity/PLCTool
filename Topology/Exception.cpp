//
// Exception.cpp: The TOPOLOGY exception handling class
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/Exception.h>
#include <iostream>

using namespace PLCTool;

Exception::Exception(
    std::string const &file,
    unsigned long line,
    std::string const &method,
    std::string const &desc,
    ExceptionNature nature)
{
  this->sFile = file;
  this->uiLine = line;
  this->sMethod = method;
  this->sDescription = desc;
  this->eNature = nature;
}

std::string const &
Exception::file(void) const
{
  return this->sFile;
}

std::string const &
Exception::method(void) const
{
  return this->sMethod;
}

unsigned long
Exception::line(void) const
{
  return this->uiLine;
}

ExceptionNature
Exception::nature(void) const
{
  return this->eNature;
}

std::string
Exception::toString(void) const
{
  switch (this->nature()) {
    case GENERIC:
      return "Runtime exception: " + this->sDescription;
      break;

    case ASSERTION_FAILED:
      return "Assertion failed in "
      + this->file()
      + ":"
      + std::to_string(this->line())
      + ": "
      + this->sDescription;
      break;

    case ALLOCATION_ERROR:
      return "Allocation error in "
      + this->file()
      + ":" + std::to_string(this->line())
      + ": " + this->sDescription;
      break;

    case INVALID_PARAMETER:
      return "Invalid parameter passed to "
      + this->method()
      + ": " + this->sDescription;
      break;
  }

  return "Unknown exception";
}

void
Exception::printInfo(void) const
{
  std::cerr << this->toString() << std::endl;
}
