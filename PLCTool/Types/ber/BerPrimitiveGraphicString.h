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

#ifndef BERPRIMITIVEGRAPHICSTRING_H
#define BERPRIMITIVEGRAPHICSTRING_H

#include "BerGraphicString.h"

namespace PLCTool
{
  /**
   * @brief The BerPrimitiveGraphicString class
   * Implements a BER graphic string in its primitive data type form.
   *
   * More info on BER can be found here:
   * https://luca.ntop.org/Teaching/Appunti/asn1.html
   */
  class BerPrimitiveGraphicString : public BerGraphicString
  {
   protected:
    std::string value;

   public:
    BerPrimitiveGraphicString() : BerPrimitiveGraphicString(""){};
    BerPrimitiveGraphicString(std::string value);
    virtual ~BerPrimitiveGraphicString(){};

    void setString(std::string value);

    std::vector<uint8_t> getContents() override;

    const char &at(size_t position) const override;
    char &at(size_t position) override;
    char remove(size_t position) override;
    void insert(size_t position, char c) override;
    std::string getString() const override;

   private:
    std::vector<uint8_t> getContentsLocal();
  };
}  // namespace PLCTool

#endif // BERPRIMITIVEGRAPHICSTRING_H
