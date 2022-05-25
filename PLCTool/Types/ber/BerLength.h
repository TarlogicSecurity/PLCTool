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

#ifndef BERLENGTH_H
#define BERLENGTH_H

#include <PLCTool/Types/general/Bytevector.h>

namespace PLCTool
{
  /**
   * Some DLMS messages use BER encoding. BER encoding is a TLV (Type, Length,
   * Value) encoding. This class provides implementation of the BER length, that
   * is composed of a bit indicating the format (short or long) and 7 more bits
   * indicating the length of the BER data, in case it is short format. Some
   * data need more bits for the length, which can be accomplished with the long
   * format. Some other data may not indicate the length, which is called
   * indefinite form. These data terminate with two null bytes.
   *
   * For simplicity BER clases use uint64_t and int64_t for big numbers,
   * although bigger values are allowed by BER encoding.
   *
   * More info on BER can be found here:
   * https://luca.ntop.org/Teaching/Appunti/asn1.html
   */
  class BerLength : public Bytevector
  {
   public:
    BerLength() : Bytevector({0}){};
    BerLength(std::vector<uint8_t> bytes);
    BerLength(uint64_t length);

    /**
     * @brief sanitizeBERLengthBytes
     * Obtains the bytes corresponding to a BER length in the first bytes of
     * a byte string.
     *
     * @param bytes Byte string containing the BER length.
     * @return Bytes corresponding to the BER length.
     */
    static std::vector<uint8_t> sanitizeBERLengthBytes(
        std::vector<uint8_t> bytes);

    bool isIndefinite() const;
    void setIndefinite();

    uint64_t getValue() const;
    void setValue(uint64_t length);

   private:
    uint8_t getHead() const;
    void setHead(uint8_t head);

    std::vector<uint8_t> getTail() const;
    void setTail(std::vector<uint8_t> tail);

    bool isDefinite() const;
    bool isReserved() const;
    bool isShortForm() const;
  };
}  // namespace PLCTool

#endif  // BERLENGTH_H
