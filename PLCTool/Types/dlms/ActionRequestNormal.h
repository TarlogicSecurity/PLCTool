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

#ifndef ACTIONREQUESTNORMAL_H
#define ACTIONREQUESTNORMAL_H

#include "ActionRequest.h"
#include "CosemMethodDescriptor.h"
#include "Data.h"
#include "InvokeIdAndPriority.h"

namespace PLCTool
{
  /**
   * @brief The ActionRequestNormal class
   * Normal action request DLMS message.
   *
   * More info on DLMS messages can be found on the DLMS Green Book:
   * https://www.dlms.com/files/Green-Book-Ed-83-Excerpt.pdf
   */
  class ActionRequestNormal : public ActionRequest
  {
    InvokeIdAndPriority invokeIdAndPriority;
    CosemMethodDescriptor cosemMethodDescriptor;
    Data *methodInvocationParameters = nullptr;

   public:
    ActionRequestNormal()
        : ActionRequestNormal(
              0x00,
              DLMS_METHOD_SERVICE_CLASS_UNCONFIRMED,
              DLMS_METHOD_PRIORITY_NORMAL,
              0x0000,
              {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
              0x00){};
    ActionRequestNormal(
        uint8_t invoke_id,
        DLMS_METHOD_SERVICE_CLASS service_class,
        DLMS_METHOD_PRIORITY priority,
        uint16_t class_id,
        std::vector<uint8_t> instance_id,
        int8_t method_id)
        : invokeIdAndPriority(invoke_id, service_class, priority),
          cosemMethodDescriptor(class_id, instance_id, method_id){};
    ~ActionRequestNormal();

    uint8_t getInvokeId() const;
    void setInvokeId(uint8_t value);

    DLMS_METHOD_SERVICE_CLASS getServiceClass() const;
    void setServiceClass(DLMS_METHOD_SERVICE_CLASS service_class);

    DLMS_METHOD_PRIORITY getPriority() const;
    void setPriority(DLMS_METHOD_PRIORITY priority);

    uint16_t getClassId() const;
    void setClassId(uint16_t value);

    std::vector<uint8_t> getInstanceId() const;
    void setInstanceId(std::vector<uint8_t> value);

    int8_t getMethodId() const;
    void setMethodId(int8_t value);

    const Data *getMethodInvocationParameters() const;
    void setMethodInvocationParameters(const Data &parameters);

    std::vector<uint8_t> getBytes() override;
    void setBytes(std::vector<uint8_t> bytes) override;
    size_t getSize() override;
  };
}  // namespace PLCTool

#endif // ACTIONREQUESTNORMAL_H
