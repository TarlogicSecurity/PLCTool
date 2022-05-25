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

#ifndef INITIATEREQUESTAPDU_H
#define INITIATEREQUESTAPDU_H

#include <PLCTool/Types/ber/BerBoolean.h>
#include <PLCTool/Types/ber/BerInteger.h>
#include <PLCTool/Types/ber/BerOctetString.h>
#include <PLCTool/Types/general/Serializable.h>
#include <PLCTool/gurux/include/enums.h>

#include "Conformance.h"
#include "XDlmsApdu.h"

namespace PLCTool
{
  class InitiateRequestApdu : public XDlmsApdu, public Serializable
  {
    BerOctetString *dedicatedKey = nullptr;
    BerBoolean *responseAllowed = nullptr;
    BerInteger *proposedQualityOfService = nullptr;
    BerInteger proposedDlmsVersionNumber;
    Conformance proposedConformance;
    BerInteger clientMaxReceivePduSize;

   public:
    InitiateRequestApdu()
        : InitiateRequestApdu(0, DLMS_CONFORMANCE_NONE, 0xfff){};
    InitiateRequestApdu(
        uint8_t proposedDlmsVersionNumber,
        DLMS_CONFORMANCE proposedConformance,
        uint16_t clientMaxReceivePduSize);
    ~InitiateRequestApdu();

    std::vector<uint8_t> getDedicatedKey() const;
    void setDedicatedKey(std::vector<uint8_t> dedicated_key);

    bool getResponseAllowed() const;
    void setResponseAllowed(bool set);

    int8_t getProposedQualityOfService() const;
    void setProposedQualityOfService(int8_t value);

    uint8_t getProposedDLMSVersionNumber() const;
    void setProposedDLMSVersionNumber(uint8_t value);

    DLMS_CONFORMANCE getProposedConformance() const;
    void setProposedConformance(DLMS_CONFORMANCE value);

    DLMS_CONFORMANCE getProposedConformanceBit(DLMS_CONFORMANCE bit) const;
    void setProposedConformanceBit(DLMS_CONFORMANCE bit, bool set);

    uint16_t getClientMaxReceivePduSize() const;
    void setClientMaxReceivePduSize(uint16_t value);

    std::vector<uint8_t> getBytes() override;
    void setBytes(std::vector<uint8_t> bytes) override;
    size_t getSize() override;
  };
}  // namespace PLCTool

#endif  // INITIATEREQUESTAPDU_H
