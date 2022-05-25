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

#include "InvokeIdAndPriority.h"

using namespace PLCTool;

#define INVOKE_ID_BITS 4
#define RESERVED_BITS 2
#define SERVICE_CLASS_BITS 1
#define PRIORITY_BITS 1

#define INVOKE_ID_MASK 0x0f
#define RESERVED_MASK 0x30
#define SERVICE_CLASS_MASK 0x40
#define PRIORITY_MASK 0x80

uint8_t
InvokeIdAndPriority::getInvokeId() const
{
  return this->invokeId;
}

void
InvokeIdAndPriority::setInvokeId(uint8_t value)
{
  this->invokeId = value & INVOKE_ID_MASK;
}

DLMS_METHOD_SERVICE_CLASS
InvokeIdAndPriority::getServiceClass() const
{
  return this->serviceClass;
}

void
InvokeIdAndPriority::setServiceClass(DLMS_METHOD_SERVICE_CLASS service_class)
{
  this->serviceClass = service_class;
}

DLMS_METHOD_PRIORITY
InvokeIdAndPriority::getPriority() const
{
  return this->priority;
}

void
InvokeIdAndPriority::setPriority(DLMS_METHOD_PRIORITY priority)
{
  this->priority = priority;
}

std::vector<uint8_t>
InvokeIdAndPriority::getBytes()
{
  uint8_t byte = this->invokeId & INVOKE_ID_MASK;

  byte = (byte & ~SERVICE_CLASS_MASK) | this->serviceClass;
  byte = (byte & ~PRIORITY_MASK) | this->priority;

  return std::vector<uint8_t>(byte);
}

void
InvokeIdAndPriority::setBytes(std::vector<uint8_t> bytes)
{
  if (bytes.size()) {
    this->invokeId = bytes[0] & INVOKE_ID_MASK;
    this->serviceClass =
        DLMS_METHOD_SERVICE_CLASS(bytes[0] & SERVICE_CLASS_MASK);
    this->priority = DLMS_METHOD_PRIORITY(bytes[0] & PRIORITY_MASK);
  }
}

size_t
InvokeIdAndPriority::getSize()
{
  return 1;
}
