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

#include "MechanismName.h"

#include <PLCTool/Types/ber/BerObjectIdentifier.h>

using namespace PLCTool;

MechanismName::MechanismName(DLMS_AUTHENTICATION value)
{
  this->identifier = BerIdentifier(BER_CONTEXT_SPECIFIC_CLASS, false, 0x0B);
  this->value = value;
  this->length = BerLength(getContentsLocal().size());
};

DLMS_AUTHENTICATION
MechanismName::getValue() const
{
  return this->value;
}

void
MechanismName::setValue(DLMS_AUTHENTICATION value)
{
  this->value = value;
  updateLength();
}

std::vector<uint8_t>
MechanismName::getContents()
{
  return getContentsLocal();
}

std::vector<uint8_t>
MechanismName::getContentsLocal()
{
  BerObjectIdentifier oid;

  switch (this->value) {
    case DLMS_AUTHENTICATION_NONE:
      oid = BerObjectIdentifier("2.16.756.5.8.2.0");
      break;
    case DLMS_AUTHENTICATION_LOW:
      oid = BerObjectIdentifier("2.16.756.5.8.2.1");
      break;
    case DLMS_AUTHENTICATION_HIGH:
      oid = BerObjectIdentifier("2.16.756.5.8.2.2");
      break;
    case DLMS_AUTHENTICATION_HIGH_MD5:
      oid = BerObjectIdentifier("2.16.756.5.8.2.3");
      break;
    case DLMS_AUTHENTICATION_HIGH_SHA1:
      oid = BerObjectIdentifier("2.16.756.5.8.2.4");
      break;
    case DLMS_AUTHENTICATION_HIGH_GMAC:
      oid = BerObjectIdentifier("2.16.756.5.8.2.5");
      break;
    case DLMS_AUTHENTICATION_HIGH_SHA256:
      oid = BerObjectIdentifier("2.16.756.5.8.2.6");
      break;
    case DLMS_AUTHENTICATION_HIGH_ECDSA:
      oid = BerObjectIdentifier("2.16.756.5.8.2.7");
      break;
  }

  return oid.getContents();
}
