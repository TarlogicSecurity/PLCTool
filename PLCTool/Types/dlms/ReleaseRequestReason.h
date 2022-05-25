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

#ifndef RELEASEREQUESTREASON_H
#define RELEASEREQUESTREASON_H

#include <PLCTool/Types/ber/BerData.h>
#include <PLCTool/gurux/include/enums.h>

namespace PLCTool
{
  class ReleaseRequestReason : public BerData
  {
    DLMS_RELEASE_REQUEST_REASON reason;

   public:
    ReleaseRequestReason()
        : ReleaseRequestReason(DLMS_RELEASE_REQUEST_REASON_NORMAL){};
    ReleaseRequestReason(DLMS_RELEASE_REQUEST_REASON reason);

    std::vector<uint8_t> getContents() override;

    DLMS_RELEASE_REQUEST_REASON getReason() const;
    void setReason(DLMS_RELEASE_REQUEST_REASON newReason);
  };
}  // namespace PLCTool

#endif  // RELEASEREQUESTREASON_H
