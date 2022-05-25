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

#ifndef MARKEDLONGFORMATINTEGER_H
#define MARKEDLONGFORMATINTEGER_H

#include "LongFormatInteger.h"

namespace PLCTool
{
  /**
   * @brief The MarkedLongFormatInteger class
   * Helper class to implement long number representation. The first bit of
   * every byte indicate if there are more bytes.
   */
  class MarkedLongFormatInteger : public LongFormatInteger
  {
   public:
    MarkedLongFormatInteger(){};
    MarkedLongFormatInteger(uint64_t value);
    MarkedLongFormatInteger(std::vector<uint8_t> bytes);

    /**
     * @brief sanitizeMarkedLongFormatIntegerBytes
     * Obtains the long number reprpesentation from the first bytes of a byte
     * string.
     *
     * @param bytes Byte string containing the long number representation.
     * @return Long number representation in bytes.
     */
    static std::vector<uint8_t> sanitizeMarkedLongFormatIntegerBytes(
        std::vector<uint8_t> bytes);

    uint64_t getValue() const override;
    void setValue(uint64_t value) override;

   private:
    uint64_t getValueLocal() const;
    void setValueLocal(uint64_t value);

    uint8_t getOctet(size_t index) const;
    bool isThereMore(size_t index) const;
    uint8_t getValue(size_t index) const;
  };
}  // namespace PLCTool

#endif // MARKEDLONGFORMATINTEGER_H
