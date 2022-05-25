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

#include "ActionRequestNormal.h"

#include <cassert>

using namespace PLCTool;

ActionRequestNormal::~ActionRequestNormal()
{
  if (this->methodInvocationParameters)
    delete this->methodInvocationParameters;
}

uint8_t
ActionRequestNormal::getInvokeId() const
{
  return this->invokeIdAndPriority.getInvokeId();
}

void
ActionRequestNormal::setInvokeId(uint8_t value)
{
  this->invokeIdAndPriority.setInvokeId(value);
}

DLMS_METHOD_SERVICE_CLASS
ActionRequestNormal::getServiceClass() const
{
  return this->invokeIdAndPriority.getServiceClass();
}

void
ActionRequestNormal::setServiceClass(DLMS_METHOD_SERVICE_CLASS service_class)
{
  this->invokeIdAndPriority.setServiceClass(service_class);
}

DLMS_METHOD_PRIORITY
ActionRequestNormal::getPriority() const
{
  return this->invokeIdAndPriority.getPriority();
}

void
ActionRequestNormal::setPriority(DLMS_METHOD_PRIORITY priority)
{
  this->invokeIdAndPriority.setPriority(priority);
}

uint16_t
ActionRequestNormal::getClassId() const
{
  return this->cosemMethodDescriptor.getClassId();
}

void
ActionRequestNormal::setClassId(uint16_t value)
{
  this->cosemMethodDescriptor.setClassId(value);
}

std::vector<uint8_t>
ActionRequestNormal::getInstanceId() const
{
  return this->cosemMethodDescriptor.getInstanceId();
}

void
ActionRequestNormal::setInstanceId(std::vector<uint8_t> value)
{
  this->cosemMethodDescriptor.setInstanceId(value);
}

int8_t
ActionRequestNormal::getMethodId() const
{
  return this->cosemMethodDescriptor.getMethodId();
}

void
ActionRequestNormal::setMethodId(int8_t value)
{
  this->cosemMethodDescriptor.setMethodId(value);
}

const Data *
ActionRequestNormal::getMethodInvocationParameters() const
{
  Data *method_invocation_parameters = nullptr;

  if (this->methodInvocationParameters)
    method_invocation_parameters = this->methodInvocationParameters;

  return method_invocation_parameters;
}

void
ActionRequestNormal::setMethodInvocationParameters(const Data &parameters)
{
  if (this->methodInvocationParameters)
    delete this->methodInvocationParameters;

  this->methodInvocationParameters = parameters.clone();
}

std::vector<uint8_t>
ActionRequestNormal::getBytes()
{
  std::vector<uint8_t> bytes;
  std::vector<uint8_t> buffer;

  bytes = this->ActionRequest::getBytes();

  /* ActionRequestNormal tag */
  bytes.push_back(0x01);

  buffer = this->invokeIdAndPriority.getBytes();
  bytes.insert(bytes.end(), buffer.begin(), buffer.end());

  buffer = this->cosemMethodDescriptor.getBytes();
  bytes.insert(bytes.end(), buffer.begin(), buffer.end());

  if (this->methodInvocationParameters) {
    buffer = this->methodInvocationParameters->getBytes();
    bytes.insert(bytes.end(), buffer.begin(), buffer.end());
  }

  return bytes;
}

void
ActionRequestNormal::setBytes(std::vector<uint8_t> bytes)
{
  assert(false);
  /* TODO */
}

size_t
ActionRequestNormal::getSize()
{
  return getBytes().size();
}
