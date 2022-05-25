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

#ifndef BERPRIMITIVEOCTETSTRING_H
#define BERPRIMITIVEOCTETSTRING_H

#include "BerOctetString.h"

namespace PLCTool
{
  /**
   * @brief The BerPrimitiveOctetString class
   * Implements a BER octet string in its primitive data type form.
   *
   * More info on BER can be found here:
   * https://luca.ntop.org/Teaching/Appunti/asn1.html
   */
  class BerPrimitiveOctetString : public BerOctetString
  {
   protected:
    std::vector<uint8_t> octetString;

   public:
    BerPrimitiveOctetString()
        : BerPrimitiveOctetString(std::vector<uint8_t>()){};
    BerPrimitiveOctetString(std::vector<uint8_t> octetString);
    virtual ~BerPrimitiveOctetString(){};

    const uint8_t &at(size_t index) const override;
    uint8_t &at(size_t index) override;
    uint8_t remove(size_t index) override;
    void insert(size_t index, uint8_t value) override;
    const uint8_t &front() const override;
    uint8_t &front() override;
    const uint8_t &back() const override;
    uint8_t &back() override;
    uint8_t popBack() override;
    void pushBack(uint8_t octet) override;
    uint8_t popFront() override;
    void pushFront(uint8_t octet) override;
    size_t getOctetStringSize() override;
    std::vector<uint8_t> getOctetString() const override;
    void setOctetString(std::vector<uint8_t> value) override;

    std::vector<uint8_t> getContents() override;

   private:
    std::vector<uint8_t> getContentsLocal();
  };
}  // namespace PLCTool

#endif // BERPRIMITIVEOCTETSTRING_H
