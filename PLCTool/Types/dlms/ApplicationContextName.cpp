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

#include "ApplicationContextName.h"

using namespace PLCTool;

ApplicationContextName::ApplicationContextName(
    DLMS_APPLICATION_CONTEXT_NAME value)
{
  this->identifier = BerIdentifier(BER_CONTEXT_SPECIFIC_CLASS, true, 0x01);
  this->value = value;
  this->length = BerLength(getContentsLocal().size());
}

DLMS_APPLICATION_CONTEXT_NAME
ApplicationContextName::getValue() const
{
  return this->value;
}

void
ApplicationContextName::setValue(DLMS_APPLICATION_CONTEXT_NAME value)
{
  this->value = value;
  updateLength();
}

std::vector<uint8_t>
ApplicationContextName::getContents()
{
  return getContentsLocal();
}

std::vector<uint8_t>
ApplicationContextName::getContentsLocal()
{
  BerObjectIdentifier oid;

  switch (this->value) {
    case DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME:
      oid = BerObjectIdentifier("2.16.756.5.8.1.1");
      break;
    case DLMS_APPLICATION_CONTEXT_NAME_SHORT_NAME:
      oid = BerObjectIdentifier("2.16.756.5.8.1.2");
      break;
    case DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME_WITH_CIPHERING:
      oid = BerObjectIdentifier("2.16.756.5.8.1.3");
      break;
    case DLMS_APPLICATION_CONTEXT_NAME_SHORT_NAME_WITH_CIPHERING:
      oid = BerObjectIdentifier("2.16.756.5.8.1.4");
      break;
    default:
      oid = BerObjectIdentifier();
      break;
  }

  return oid.getBytes();
}
