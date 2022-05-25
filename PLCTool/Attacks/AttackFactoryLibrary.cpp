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

#include "AttackFactoryLibrary.h"

using namespace PLCTool;

AttackFactoryLibrary::AttackFactoryLibrary(QObject *parent) : QObject(parent)
{
}

AttackFactoryLibrary::~AttackFactoryLibrary()
{
  for (const auto &keyValue : library) delete keyValue.second;
}

bool
AttackFactoryLibrary::isAttackFactoryRegistered(QString attackName) const
{
  return library.find(attackName) != library.end();
}

AttackFactoryLibrary &
AttackFactoryLibrary::instance(void)
{
  static AttackFactoryLibrary instance;
  return instance;
}

Attack *
AttackFactoryLibrary::getAttack(
    QString attackName,
    StringParams const &params,
    PrimeAdapter *adapter) const
{
  Attack *newAttack = nullptr;

  if (isAttackFactoryRegistered(attackName))
    newAttack = library.at(attackName)->getAttack(params, adapter);

  return newAttack;
}

bool
AttackFactoryLibrary::getAttackParamList(
    QString attackName,
    QList<QString> &paramList)
{
  bool ok = isAttackFactoryRegistered(attackName);

  if (ok)
    paramList = library.at(attackName)->getAttackParamList();

  return ok;
}

void
AttackFactoryLibrary::registerAttackFactory(AttackFactory *factory)
{
  if (isAttackFactoryRegistered(factory->getAttackName())) {
    AttackFactory *old_factory = library.at(factory->getAttackName());
    delete old_factory;
  } else {
    emit newAttackModuleFound(factory->getAttackName());
  }

  library[factory->getAttackName()] = factory;
}
