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

#ifndef OBJECTIDENTIFIER_H
#define OBJECTIDENTIFIER_H

#include <PLCTool/Types/general/Bytevector.h>

#include <string>

#include "BerContents.h"

namespace PLCTool
{
  /**
   * @brief The ObjectIdentifier class
   * Helper class to implement the contents of a BER object identifier. The oid
   * can be indicated as a string value or per element.
   *
   * More info on BER can be found here:
   * https://luca.ntop.org/Teaching/Appunti/asn1.html
   */
  class ObjectIdentifier : public Bytevector
  {
   public:
    ObjectIdentifier(){};
    ObjectIdentifier(std::string dot_separated_id);
    ObjectIdentifier(std::vector<uint8_t> bytes) : Bytevector(bytes){};

    uint64_t getValue(size_t index) const;
    void setValue(size_t index, uint64_t value);

    std::string getObjectId() const;
    void setObjectId(std::string dot_separated_id);

    size_t getNumberOfElements() const;

   private:
    uint8_t getFirstValue() const;
    void setFirstValue(uint8_t value);

    uint8_t getSecondValue() const;
    void setSecondValue(uint8_t value);

    uint8_t getFurtherValue(size_t index) const;
    void setFurtherValue(size_t index, uint64_t value);

    size_t getFirstRealIndexOfValue(size_t index) const;
    size_t getNextRealIndexFrom(size_t real_index) const;
    std::vector<uint8_t> getLongFormatValueFrom(size_t real_index) const;
  };
}  // namespace PLCTool

#endif  // OBJECTIDENTIFIER_H
