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

#include "AarqApdu.h"

#include "AssociationInformation.h"

using namespace PLCTool;

AarqApdu::AarqApdu(DLMS_APPLICATION_CONTEXT_NAME context_name)
{
  this->identifier = BerIdentifier(BER_APPLICATION_CLASS, true, 0x00);
  this->applicationContextName = ApplicationContextName(context_name);
  this->length = BerLength(getContentsLocal().size());
}

AarqApdu::~AarqApdu()
{
  if (this->senderAcseRequirements)
    delete this->senderAcseRequirements;

  if (this->mechanismName)
    delete this->mechanismName;

  if (this->callingAuthenticationValue)
    delete this->callingAuthenticationValue;

  if (this->userInformation)
    delete this->userInformation;
}

DLMS_APPLICATION_CONTEXT_NAME
AarqApdu::getApplicationContextName() const
{
  return this->applicationContextName.getValue();
}

void
AarqApdu::setApplicationContextName(DLMS_APPLICATION_CONTEXT_NAME value)
{
  this->applicationContextName.setValue(value);
  updateLength();
}

DLMS_AUTHENTICATION
AarqApdu::getMechanismName() const
{
  DLMS_AUTHENTICATION auth = DLMS_AUTHENTICATION_NONE;

  if (getSenderAcseRequirements() && this->mechanismName)
    auth = this->mechanismName->getValue();

  return auth;
}

void
AarqApdu::setMechanismName(DLMS_AUTHENTICATION value)
{
  if (!getSenderAcseRequirements())
    setSenderAcseRequirements();

  if (!this->mechanismName)
    this->mechanismName = new MechanismName();

  this->mechanismName->setValue(value);
}

std::string
AarqApdu::getCallingAuthenticationValue() const
{
  std::string auth;

  if (getSenderAcseRequirements() && this->mechanismName)
    auth = this->callingAuthenticationValue->getValue();

  return auth;
}

void
AarqApdu::setCallingAuthenticationValue(const std::string &value)
{
  if (!getSenderAcseRequirements())
    setSenderAcseRequirements();

  if (!this->callingAuthenticationValue)
    this->callingAuthenticationValue = new GraphicStringAuthenticationValue();

  this->callingAuthenticationValue->setValue(value);
}

std::vector<uint8_t>
AarqApdu::getUserInformation() const
{
  std::vector<uint8_t> info;

  if (getSenderAcseRequirements() && this->userInformation)
    info = this->userInformation->getOctetString();

  return info;
}

void
AarqApdu::setUserInformation(std::vector<uint8_t> user_information)
{
  if (!getSenderAcseRequirements())
    setSenderAcseRequirements();

  if (!this->userInformation)
    this->userInformation = new AssociationInformation();

  this->userInformation->setOctetString(user_information);
}

std::vector<uint8_t>
AarqApdu::getContents()
{
  return getContentsLocal();
}

std::vector<uint8_t>
AarqApdu::getContentsLocal()
{
  std::vector<uint8_t> bytes;
  std::vector<uint8_t> buffer;

  bytes = this->applicationContextName.getBytes();
  if (getSenderAcseRequirements()) {
    buffer = this->senderAcseRequirements->getBytes();
    bytes.insert(bytes.end(), buffer.begin(), buffer.end());

    if (this->mechanismName) {
      buffer = this->mechanismName->getBytes();
      bytes.insert(bytes.end(), buffer.begin(), buffer.end());
    }

    if (this->callingAuthenticationValue) {
      buffer = this->callingAuthenticationValue->getBytes();
      bytes.insert(bytes.end(), buffer.begin(), buffer.end());
    }

    if (this->userInformation) {
      buffer = this->userInformation->getBytes();
      bytes.insert(bytes.end(), buffer.begin(), buffer.end());
    }
  }

  return bytes;
}

bool
AarqApdu::getSenderAcseRequirements() const
{
  return this->senderAcseRequirements &&
         this->senderAcseRequirements->getValue();
}

void
AarqApdu::setSenderAcseRequirements()
{
  if (!this->senderAcseRequirements)
    this->senderAcseRequirements = new AcseRequirements();

  this->senderAcseRequirements->setValue(true);
}
