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

#include "Integer.h"

#include <limits.h>

using namespace PLCTool;

int64_t
Integer::getValue() const
{
  return this->value;
}

void
Integer::setValue(int64_t value)
{
  this->value = value;
}

std::vector<uint8_t>
Integer::getBytes()
{
  std::vector<uint8_t> bytes;
  int64_t buffer = this->value;

  for (size_t i = 0; i < this->getSize(); ++i) {
    bytes.insert(bytes.begin(), buffer & 0xff);
    buffer = buffer >> 8;
  }

  return bytes;
}

void
Integer::setBytes(std::vector<uint8_t> bytes)
{
  int64_t buffer;
  uint64_t *unsigned_buffer_pointer = reinterpret_cast<uint64_t *>(&buffer);

  if (!bytes.empty()) {
    if (bytes.at(0) & 0x80)
      buffer = -1;
    else
      buffer = 0;

    for (uint8_t element : bytes) {
      *unsigned_buffer_pointer = *unsigned_buffer_pointer << 8;
      *unsigned_buffer_pointer = *unsigned_buffer_pointer | element;
    }

    this->value = buffer;
  }
}

size_t
Integer::getSize()
{
  size_t bytes = 8;

  if (this->value >= 0) {
    if (this->value <= INT32_MAX) {
      if (this->value <= 0x7fffff) {
        if (this->value <= INT16_MAX) {
          if (this->value <= INT8_MAX) {
            bytes = 1;
          } else {
            bytes = 2;
          }
        } else {
          bytes = 3;
        }
      } else {
        bytes = 4;
      }
    } else {
      if (this->value <= 0x7fffffffffffff) {
        if (this->value <= 0x7fffffffffff) {
          if (this->value <= 0x7fffffffff) {
            bytes = 5;
          } else {
            bytes = 6;
          }
        } else {
          bytes = 7;
        }
      } else {
        bytes = 8;
      }
    }
  } else {
    if (this->value >= INT32_MIN) {
      if (this->value >= -(0x800000)) {
        if (this->value >= INT16_MIN) {
          if (this->value >= INT8_MIN) {
            bytes = 1;
          } else {
            bytes = 2;
          }
        } else {
          bytes = 3;
        }
      } else {
        bytes = 4;
      }
    } else {
      if (this->value >= -(0x80000000000000)) {
        if (this->value >= -(0x800000000000)) {
          if (this->value >= -(0x8000000000)) {
            bytes = 5;
          } else {
            bytes = 6;
          }
        } else {
          bytes = 7;
        }
      } else {
        bytes = 8;
      }
    }
  }

  return bytes;
}
