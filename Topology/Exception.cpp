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
