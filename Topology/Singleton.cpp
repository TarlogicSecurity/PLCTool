//
// Singleton.cpp: Interface for all PLC adapters (abstract)
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/Singleton.h>
#include <string>
#include <sstream>
#include <dlfcn.h>
#include <dirent.h>
#include <iostream>
#include <cstring>

using namespace PLCTool;

Singleton *Singleton::currentInstance = nullptr;

Singleton::Singleton(void)
{
  this->addPluginSearchPathFromEnv();
  this->addPluginSearchPath(
      getenv("HOME") + std::string("/") + ".Topology/plugins");
  this->addPluginSearchPath("/usr/local/Topology/plugins");
  this->scanPlugins();
}

Singleton *
Singleton::instance(void)
{
  if (Singleton::currentInstance == nullptr) {
    Singleton::currentInstance = new Singleton();
  }

  return Singleton::currentInstance;
}

bool
Singleton::registerAdapterFactory(AdapterFactory *factory)
{
  if (this->hasAdapterFactory(factory->name()))
    return false;

  this->adapterFactoryMap.emplace(factory->name(), factory);

  return false;
}

bool
Singleton::hasAdapterFactory(std::string const &name)
{
  return this->adapterFactoryMap.find(name) != this->adapterFactoryMap.end();
}

Adapter *
Singleton::makeAdapter(std::string const &name, StringParams const &p)
{
  std::map<std::string, AdapterFactory *>::iterator it;
  it = this->adapterFactoryMap.find(name);

  if (it != this->adapterFactoryMap.end()) {
    Adapter *adapter = it->second->makeAdapter(p);
    return adapter;
  }

  return nullptr;
}

bool
Singleton::loadPlugin(std::string const &path)
{
  void *handle = nullptr;
  void *addr = nullptr;
  void (*entry) (Singleton *);

  bool ok = false;

  if ((handle = dlopen(path.c_str(), RTLD_NOW)) == nullptr) {
    const char *err = dlerror();

    if (strstr(err, "undefined symbol") != nullptr)
      std::cerr << "Cannot open " << path << ": " << err << std::endl;
    goto fail;
  }

  if ((addr = dlsym(handle, "entry")) == nullptr)
    goto fail;

  entry = reinterpret_cast<void (*) (Singleton *)>(addr);

  (entry)(this);

  ok = true;

fail:
  if (!ok && handle != nullptr)
    dlclose(handle);

  return ok;
}

unsigned int
Singleton::loadPluginDir(std::string const &directory)
{
  DIR *dir = nullptr;
  struct dirent *ent;
  unsigned int count = 0;

  if ((dir = opendir(directory.c_str())) == nullptr)
    goto fail;

  (void) seekdir(dir, 2);

  while ((ent = readdir(dir)) != nullptr)
    if (this->loadPlugin(directory + "/" + ent->d_name))
      ++count;

fail:
  if (dir != nullptr)
    closedir(dir);

  return count;
}

unsigned int
Singleton::scanPlugins(void)
{
  unsigned int count = 0;

  for (auto p : this->pluginSearchPath)
    count += this->loadPluginDir(p);

  return count;
}

void
Singleton::addPluginSearchPath(std::string const &path)
{
  this->pluginSearchPath.push_back(path);
}

void
Singleton::addPluginSearchPathFromEnv(void)
{
  const char *contents = getenv("TOPOLOGY_PLUGIN_DIR");
  if (contents == nullptr)
    contents = "";
  std::istringstream copy(contents);
  std::string dir;


  while (std::getline(copy, dir, ':'))
    this->addPluginSearchPath(dir);
}

