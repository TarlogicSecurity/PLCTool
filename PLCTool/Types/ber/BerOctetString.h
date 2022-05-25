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

#ifndef BEROCTETSTRING_H
#define BEROCTETSTRING_H

#include "BerData.h"

namespace PLCTool
{
  /**
   * @brief The BerOctetString class
   * Interface for BER octet string data types. Octet strings can be implemented
   * as a constructed or primitive data type. The methods allow to use the type
   * as a list of bytes.
   *
   * More info on BER can be found here:
   * https://luca.ntop.org/Teaching/Appunti/asn1.html
   */
  class BerOctetString : public BerData
  {
   public:
    BerOctetString(bool constructed);
    virtual ~BerOctetString(){};

    virtual const uint8_t &at(size_t index) const = 0;
    virtual uint8_t &at(size_t index) = 0;
    virtual uint8_t remove(size_t index) = 0;
    virtual void insert(size_t index, uint8_t value) = 0;
    virtual const uint8_t &front() const = 0;
    virtual uint8_t &front() = 0;
    virtual const uint8_t &back() const = 0;
    virtual uint8_t &back() = 0;
    virtual uint8_t popBack() = 0;
    virtual void pushBack(uint8_t octet) = 0;
    virtual uint8_t popFront() = 0;
    virtual void pushFront(uint8_t octet) = 0;

    virtual std::vector<uint8_t> getOctetString() const = 0;
    virtual void setOctetString(std::vector<uint8_t> value) = 0;
    virtual size_t getOctetStringSize() = 0;
  };
}  // namespace PLCTool

#endif // BEROCTETSTRING_H
