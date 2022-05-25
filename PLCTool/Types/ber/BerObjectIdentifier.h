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

#ifndef BEROBJECTIDENTIFIER_H
#define BEROBJECTIDENTIFIER_H

#include <string>

#include "BerData.h"
#include "ObjectIdentifier.h"

namespace PLCTool
{
  /**
   * @brief The BerObjectIdentifier class
   * Implements the BER object ifentifier data type. Takes oid in the string
   * format.
   *
   * More info on BER can be found here:
   * https://luca.ntop.org/Teaching/Appunti/asn1.html
   */
  class BerObjectIdentifier : public BerData
  {
   protected:
    ObjectIdentifier oid;

   public:
    BerObjectIdentifier();
    BerObjectIdentifier(std::string dot_separated_id);
    virtual ~BerObjectIdentifier(){};

    uint64_t getValue(size_t index) const;
    void setValue(size_t index, uint64_t value);

    std::string getObjectId() const;
    void setObjectId(std::string dot_separated_id);

    size_t getNumberOfElements() const;

    std::vector<uint8_t> getContents() override;

   private:
    std::vector<uint8_t> getContentsLocal();
  };
}  // namespace PLCTool

#endif // BEROBJECTIDENTIFIER_H
