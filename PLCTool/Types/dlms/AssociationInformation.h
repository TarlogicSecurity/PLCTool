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

#ifndef ASSOCIATIONINFORMATION_H
#define ASSOCIATIONINFORMATION_H

#include <PLCTool/Types/ber/BerData.h>
#include <PLCTool/Types/ber/BerOctetString.h>

namespace PLCTool
{
  class AssociationInformation : public BerData
  {
    BerOctetString *octetString;

   public:
    AssociationInformation() : AssociationInformation(std::vector<uint8_t>()){};
    AssociationInformation(std::vector<uint8_t> octetString);
    ~AssociationInformation();

    const uint8_t &at(size_t index) const;
    uint8_t &at(size_t index);
    uint8_t remove(size_t index);
    void insert(size_t index, uint8_t value);
    const uint8_t &front() const;
    uint8_t &front();
    const uint8_t &back() const;
    uint8_t &back();
    uint8_t popBack();
    void pushBack(uint8_t octet);
    uint8_t popFront();
    void pushFront(uint8_t octet);
    size_t getOctetStringSize();
    std::vector<uint8_t> getOctetString() const;
    void setOctetString(std::vector<uint8_t> value);

    std::vector<uint8_t> getContents() override;

   private:
    std::vector<uint8_t> getContentsLocal();
  };
}  // namespace PLCTool

#endif  // ASSOCIATIONINFORMATION_H
