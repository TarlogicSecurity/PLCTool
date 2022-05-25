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

#ifndef BERDATA_H
#define BERDATA_H

#include "BerContents.h"
#include "BerIdentifier.h"
#include "BerLength.h"

namespace PLCTool
{
  /**
   * Some DLMS messages use BER encoding. BER encoding is a TLV (Type, Length,
   * Value) encoding. This class provides a base class for messages encoded this
   * way.
   *
   * More info on BER can be found here:
   * https://luca.ntop.org/Teaching/Appunti/asn1.html
   */
  class BerData : public Serializable
  {
   protected:
    BerIdentifier identifier;
    BerLength length;

    void updateLength();

   public:
    virtual ~BerData(){};

    /**
     * @brief sanitizeBERDataBytes
     * Isolates the bytes of a BER encoded data at the beginning of a
     * byte string.
     *
     * @param bytes Byte string containing the BER encoded data.
     * @param identifier_bytes Reference to store the resulting BER identifier
     * bytes.
     * @param length_bytes Reference to store the resulting BER length
     * bytes.
     * @param contents_bytes Reference to store the resulting BER content
     * bytes.
     * @return Indicates the validity of the results.
     */
    static bool sanitizeBERDataBytes(
        std::vector<uint8_t> bytes,
        std::vector<uint8_t> &identifier_bytes,
        std::vector<uint8_t> &length_bytes,
        std::vector<uint8_t> &contents_bytes);

    BER_TAG_CLASS getTagClass() const;
    void setTagClass(BER_TAG_CLASS tag_class);

    bool isConstructed() const;

    uint64_t getTagType() const;
    void setTagType(uint64_t tag_type);

    uint64_t getLength();

    /**
     * @brief getContents
     * Returns the content bytes of the data, without the type and length bytes.
     * It should be implemented in every child class. This way, child classes
     * are not forced to use any particular C++ class for the contents.
     *
     * @return Byte string with the contents.
     */
    virtual std::vector<uint8_t> getContents() = 0;

    std::vector<uint8_t> getBytes() override;
    void setBytes(std::vector<uint8_t> bytes) override;
    size_t getSize() override;
  };  // namespace PLCTool
}  // namespace PLCTool

#endif // BERDATA_H
