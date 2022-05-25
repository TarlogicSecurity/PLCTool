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

#ifndef BERPRIMITIVEBITSTRING_H
#define BERPRIMITIVEBITSTRING_H

#include "BerBitString.h"
#include "BitString.h"

namespace PLCTool
{
  /**
   * @brief The BerPrimitiveBitString class
   * Implements a BER bit string in its primitive data type form.
   *
   * More info on BER can be found here:
   * https://luca.ntop.org/Teaching/Appunti/asn1.html
   */
  class BerPrimitiveBitString : public BerBitString
  {
   protected:
    BitString bitString;

   public:
    BerPrimitiveBitString() : BerPrimitiveBitString(std::vector<bool>()){};
    BerPrimitiveBitString(std::vector<bool> bit_string);
    virtual ~BerPrimitiveBitString(){};

    bool getBit(size_t position) const override;
    void setBit(size_t position, bool value) override;
    bool removeBit(size_t position) override;
    void insertBit(size_t position, bool value) override;
    bool front() const override;
    bool back() const override;
    bool popFront() override;
    void pushFront(bool value) override;
    bool popBack() override;
    void pushBack(bool value) override;
    size_t getBitStringSize() const override;

    std::vector<uint8_t> getContents() override;

   private:
    std::vector<uint8_t> getContentsLocal();
  };
}  // namespace PLCTool

#endif // BERPRIMITIVEBITSTRING_H
