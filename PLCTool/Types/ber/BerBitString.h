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

#ifndef BERBITSTRING_H
#define BERBITSTRING_H

#include "BerData.h"

namespace PLCTool
{
  /**
   * @brief The BerBitString class
   * Interface for BER bit string types, that can be implemented as a
   * constructed or primitive BER type. The methods allow to use the bit string
   * as a list type.
   *
   * More info on BER can be found here:
   * https://luca.ntop.org/Teaching/Appunti/asn1.html
   */
  class BerBitString : public BerData
  {
   public:
    BerBitString(bool constructed);
    virtual ~BerBitString(){};

    virtual bool getBit(size_t position) const = 0;
    virtual void setBit(size_t position, bool value) = 0;

    virtual bool removeBit(size_t position) = 0;
    virtual void insertBit(size_t position, bool value) = 0;

    virtual bool front() const = 0;
    virtual bool back() const = 0;

    virtual bool popFront() = 0;
    virtual void pushFront(bool value) = 0;

    virtual bool popBack() = 0;
    virtual void pushBack(bool value) = 0;

    virtual size_t getBitStringSize() const = 0;
  };
}  // namespace PLCTool

#endif // BERBITSTRING_H
