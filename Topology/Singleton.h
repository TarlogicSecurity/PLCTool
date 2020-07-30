//
// Singleton.h: The TOPOLOGY singleton class
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#ifndef _TOPOLOGY_SINGLETON_H
#define _TOPOLOGY_SINGLETON_H

#include "AdapterFactory.h"
#include <map>
#include <list>

namespace PLCTool {
  typedef std::map<std::string, AdapterFactory *> AdapterFactoryMap;
  typedef AdapterFactoryMap::const_iterator AdapterFactoryIterator;

  class Singleton {
    static Singleton *currentInstance;
    AdapterFactoryMap adapterFactoryMap;
    std::list<std::string> pluginSearchPath;

    bool loadPlugin(std::string const &);
    unsigned int loadPluginDir(std::string const &);

  private:
    Singleton(void);

  public:
    static Singleton *instance(void);
    void addPluginSearchPath(std::string const &);
    void addPluginSearchPathFromEnv(void);
    unsigned int scanPlugins(void);
    bool registerAdapterFactory(AdapterFactory *);
    bool hasAdapterFactory(std::string const &);

    Adapter *makeAdapter(std::string const &, StringParams const &);

    AdapterFactoryIterator
    firstAdapterFactory(void) const
    {
      return this->adapterFactoryMap.cbegin();
    }

    AdapterFactoryIterator
    lastAdapterFactory(void) const
    {
      return this->adapterFactoryMap.cend();
    }
  };
}

#endif // _TOPOLOGY_SINGLETON_H
