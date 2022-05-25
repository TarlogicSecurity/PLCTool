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

#ifndef CONFORMANCE_H
#define CONFORMANCE_H

#include <PLCTool/Types/ber/BerData.h>
#include <PLCTool/gurux/include/enums.h>

namespace PLCTool
{
  class Conformance : public BerData
  {
    DLMS_CONFORMANCE value;

   public:
    Conformance() : Conformance(DLMS_CONFORMANCE_NONE){};
    Conformance(DLMS_CONFORMANCE value);

    DLMS_CONFORMANCE getValue() const;
    void setValue(DLMS_CONFORMANCE value);

    DLMS_CONFORMANCE getConformanceBit(DLMS_CONFORMANCE bit) const;
    void setConformanceBit(DLMS_CONFORMANCE bit, bool set);

    std::vector<uint8_t> getContents() override;

   private:
    static DLMS_CONFORMANCE bytesToConformance(std::vector<uint8_t> bytes);
    static std::vector<uint8_t> conformanceToBytes(
        DLMS_CONFORMANCE conformance);

    std::vector<uint8_t> getContentsLocal();
  };
}  // namespace PLCTool

#endif  // CONFORMANCE_H
