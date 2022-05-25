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

#ifndef INVOKEIDANDPRIORITY_H
#define INVOKEIDANDPRIORITY_H

#include <PLCTool/Types/general/Serializable.h>
#include <stdint.h>

namespace PLCTool
{
  enum DLMS_METHOD_SERVICE_CLASS {
    DLMS_METHOD_SERVICE_CLASS_UNCONFIRMED = 0x00,
    DLMS_METHOD_SERVICE_CLASS_CONFIRMED = 0x40
  };

  enum DLMS_METHOD_PRIORITY {
    DLMS_METHOD_PRIORITY_NORMAL = 0x00,
    DLMS_METHOD_PRIORITY_HIGH = 0x80
  };

  class InvokeIdAndPriority : public Serializable
  {
    uint8_t invokeId;
    DLMS_METHOD_SERVICE_CLASS serviceClass;
    DLMS_METHOD_PRIORITY priority;

   public:
    InvokeIdAndPriority()
        : InvokeIdAndPriority(
              0,
              DLMS_METHOD_SERVICE_CLASS_UNCONFIRMED,
              DLMS_METHOD_PRIORITY_NORMAL){};
    InvokeIdAndPriority(
        uint8_t invoke_id,
        DLMS_METHOD_SERVICE_CLASS service_class,
        DLMS_METHOD_PRIORITY priority)
        : invokeId(invoke_id),
          serviceClass(service_class),
          priority(priority){};

    uint8_t getInvokeId() const;
    void setInvokeId(uint8_t value);

    DLMS_METHOD_SERVICE_CLASS getServiceClass() const;
    void setServiceClass(DLMS_METHOD_SERVICE_CLASS service_class);

    DLMS_METHOD_PRIORITY getPriority() const;
    void setPriority(DLMS_METHOD_PRIORITY priority);

    std::vector<uint8_t> getBytes();
    void setBytes(std::vector<uint8_t> bytes);
    size_t getSize();
  };
}  // namespace PLCTool

#endif  // INVOKEIDANDPRIORITY_H
