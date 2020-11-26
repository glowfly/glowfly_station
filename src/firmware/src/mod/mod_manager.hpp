#ifndef MODMANAGER_H
#define MODMANAGER_H

#include <vector>
#include <string>
#include "mod.hpp"

namespace SyncBlink
{
    class ModManager
    {
        public:
            Mod get(std::string& modName);
            std::vector<std::string> getList();
            
            void add(std::string& modName);
            void save(Mod& mod);
            void remove(std::string& modName);

            std::string getActiveModName();
            void saveActiveModName(std::string modName);
    };
}

#endif // MODMANAGER_H