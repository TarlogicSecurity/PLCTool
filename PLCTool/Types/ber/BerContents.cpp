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

#include "BerContents.h"

#include <algorithm>

#include "BerIdentifier.h"

using namespace PLCTool;

std::vector<uint8_t>
BerContents::sanitizeBERContentsBytes(
    std::vector<uint8_t> bytes,
    uint64_t length)
{
  std::vector<uint8_t> new_bytes;

  if (length < bytes.size()) {
    new_bytes.insert(new_bytes.end(), bytes.begin(), bytes.begin() + length);
  }

  return new_bytes;
}

std::vector<uint8_t>
BerContents::sanitizeBERIndefiniteContentsBytes(std::vector<uint8_t> bytes)
{
  std::vector<uint8_t> new_bytes;

  if (bytes.size()) {
    std::vector<uint8_t> stop_sequence = {BER_EOC, BER_EOC};
    std::vector<uint8_t>::iterator appearance = std::search(
        bytes.begin(),
        bytes.end(),
        stop_sequence.begin(),
        stop_sequence.end());
    if (appearance < bytes.end())
      new_bytes.insert(new_bytes.end(), bytes.begin(), appearance);
  }

  return new_bytes;
}
