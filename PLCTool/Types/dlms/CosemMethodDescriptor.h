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

#ifndef COSEMMETHODDESCRIPTOR_H
#define COSEMMETHODDESCRIPTOR_H

#include <PLCTool/Types/general/Serializable.h>

namespace PLCTool
{
  class CosemMethodDescriptor : public Serializable
  {
    uint16_t classId;
    std::vector<uint8_t> instanceId;
    int8_t methodId;

   public:
    CosemMethodDescriptor()
        : CosemMethodDescriptor(
              0x0000,
              {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
              0x00){};
    CosemMethodDescriptor(
        uint16_t class_id,
        std::vector<uint8_t> instance_id,
        uint8_t method_id)
        : classId(class_id), instanceId(instance_id), methodId(method_id){};

    uint16_t getClassId() const;
    void setClassId(uint16_t value);

    std::vector<uint8_t> getInstanceId() const;
    void setInstanceId(std::vector<uint8_t> value);

    int8_t getMethodId() const;
    void setMethodId(int8_t value);

    std::vector<uint8_t> getBytes() override;
    void setBytes(std::vector<uint8_t> bytes) override;
    size_t getSize() override;
  };
}  // namespace PLCTool

#endif  // COSEMMETHODDESCRIPTOR_H
