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

#ifndef BERIDENTIFIER_H
#define BERIDENTIFIER_H

#include <PLCTool/Types/general/Bytevector.h>

namespace PLCTool
{
  /**
   * Tag classes available for BER types.
   */
  enum BER_TAG_CLASS {
    BER_UNIVERSAL_CLASS = 0x00,
    BER_APPLICATION_CLASS = 0x01,
    BER_CONTEXT_SPECIFIC_CLASS = 0x02,
    BER_PRIVATE_CLASS = 0x03
  };

  /**
   * Special bytes for BER types.
   */
  enum BER_SYMBOLS {
    BER_EOC = 0x00
  };

  /**
   * BER types of the universal class.
   */
  enum BER_UNIVERSAL_TAG_TYPE {
    BER_BOOLEAN = 0x1,
    BER_INTEGER = 0x2,
    BER_BIT_STRING = 0x3,
    BER_OCTET_STRING = 0x4,
    BER_NULL = 0x5,
    BER_OBJECT_IDENTIFIER = 0x6,
    BER_OBJECT_DESCRIPTOR = 0x7,
    BER_EXTERNAL = 0x8,
    BER_REAL = 0x9,
    BER_ENUMERATED = 0xA,
    BER_EMBEDDED_PDV = 0xB,
    BER_UTF8_STRING = 0xC,
    BER_RELATIVE_OID = 0xD,
    BER_SEQUENCE = 0x10,
    BER_SET = 0x11,
    BER_NUMERIC_STRING = 0x12,
    BER_PRINTABLE_STRING = 0x13,
    BER_TELETEX_STRING = 0x14,
    BER_VIDEOTEX_STRING = 0x15,
    BER_IA5_STRING = 0x16,
    BER_UTC_TIME = 0x17,
    BER_GENERALIZED_TIME = 0x18,
    BER_GRAPHIC_STRING = 0x19,
    BER_VISIBLE_STRING = 0x1A,
    BER_GENERAL_STRING = 0x1B,
    BER_UNIVERSAL_STRING = 0x1C,
    BER_CHARACTER_STRING = 0x1D,
    BER_BMP_STRING = 0x1E,
  };

  /**
   * Some DLMS messages use BER encoding. BER encoding is a TLV (Type, Length,
   * Value) encoding. This class provides implementation of the BER types, that
   * are composed of a class, a constructed indication bit and a tag type.
   *
   * More info on BER can be found here:
   * https://luca.ntop.org/Teaching/Appunti/asn1.html
   */
  class BerIdentifier : public Bytevector
  {
   public:
    BerIdentifier() : Bytevector({0}){};
    BerIdentifier(std::vector<uint8_t> bytes);
    BerIdentifier(BER_TAG_CLASS tag_class, bool constructed, uint64_t tag_type);

    /**
     * @brief sanitizeBERIdentifierBytes
     * Obtains the bytes corresponding to a BER identifier in the first bytes of
     * a byte string.
     *
     * @param bytes Byte string containing the BER identifier.
     * @return Bytes corresponding to the BER identifier.
     */
    static std::vector<uint8_t> sanitizeBERIdentifierBytes(
        std::vector<uint8_t> bytes);

    BER_TAG_CLASS getTagClass() const;
    void setTagClass(BER_TAG_CLASS tag_class);

    bool isConstructed() const;
    void setConstructed(bool constructed);

    uint64_t getTagType() const;
    void setTagType(uint64_t tag_type);

   private:
    uint8_t getHead() const;
    void setHead(uint8_t head);

    std::vector<uint8_t> getTail() const;
    void setTail(std::vector<uint8_t> tail);

    bool isTagTypeLongForm() const;
    uint8_t getTagTypeShortForm() const;
    void setTagTypeShortForm(uint8_t tag_type);
    uint64_t getTagTypeLongForm() const;
    void setTagTypeLongForm(uint64_t tag_type);
  };
}  // namespace PLCTool

#endif  // BERIDENTIFIER_H
