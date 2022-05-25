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

#include "AttackPluginLoader.h"

#include <PLCTool/Attacks/AttackFactoryLibrary.h>
#include <dlfcn.h>

#include <iostream>

using namespace PLCTool;

AttackPluginLoader::AttackPluginLoader(QObject *parent) : QObject(parent)
{
  this->attackPluginPath.append(".");
}

bool
AttackPluginLoader::loadPlugin(const QString &path)
{
  void *handle = nullptr;
  void *addr = nullptr;
  void (*entry)(AttackFactoryLibrary *);

  bool ok = false;

  if ((handle = dlopen(path.toStdString().c_str(), RTLD_NOW)) == nullptr) {
    const char *err = dlerror();

    if (strstr(err, "undefined symbol") != nullptr)
      std::cerr << "Cannot open " << path.toStdString() << ": " << err
                << std::endl;
    goto fail;
  }

  if ((addr = dlsym(handle, "entry")) == nullptr)
    goto fail;

  entry = reinterpret_cast<void (*)(AttackFactoryLibrary *)>(addr);

  entry(&AttackFactoryLibrary::instance());

  ok = true;

fail:
  if (!ok && handle != nullptr)
    dlclose(handle);

  return ok;
}

unsigned
AttackPluginLoader::loadPluginDir(const QString &directoryPath)
{
  QDir dir(directoryPath);
  unsigned int count = 0;

  if (dir.exists() && dir.isReadable()) {
    QFileInfoList entryList = dir.entryInfoList();
    entryList.erase(entryList.begin(), entryList.begin() + 2);

    for (int i = 0; i < entryList.size(); ++i) {
      QFileInfo entry = entryList.at(i);
      if (entry.isReadable() && entry.isFile()) {
        if (this->loadPlugin(entry.absoluteFilePath())) {
          ++count;
        }
      }
    }
  }

  return count;
}

void
AttackPluginLoader::addPluginSearchPath(const QString &path)
{
  QDir sanitizedPath(path);
  sanitizedPath.makeAbsolute();

  if (sanitizedPath.exists() && sanitizedPath.isReadable() &&
      !this->attackPluginPath.contains(sanitizedPath.path()))
    this->attackPluginPath.append(sanitizedPath.path());
}

void
AttackPluginLoader::addPluginSearchPath(const std::vector<QString> &path)
{
  for (QString element : path) addPluginSearchPath(element);
}

void
AttackPluginLoader::addPluginSearchPathFromEnvironment()
{
  const char *contents = getenv("PLCTOOL_PLUGIN_DIR");
  if (contents == nullptr)
    contents = "";
  QString copy(contents);

  QStringList dirList = copy.split(":");
  for (int i = 0; i < dirList.size(); ++i)
    this->addPluginSearchPath(dirList.at(i));
}

unsigned
AttackPluginLoader::scanForPlugins()
{
  unsigned count = 0;

  for (int i = 0; i < this->attackPluginPath.size(); ++i)
    count += this->loadPluginDir(this->attackPluginPath.at(i));

  return count;
}
