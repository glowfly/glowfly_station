#ifndef MODMANAGER_H
#define MODMANAGER_H

#include <vector>
#include <string>
#include <network/websocket/commands/analyzer_command.hpp>
#include <event_registration.hpp>
#include "mod.hpp"

namespace SyncBlink
{
    typedef std::function<void()> ActiveModChangedEvent;

    class ModManager
    {
        public:
            Mod get(std::string& modName);
            std::vector<std::string> getList();
            
            void add(std::string& modName);
            void save(std::string modName, std::string modContent);
            void remove(std::string& modName);

            std::string getActiveModName();
            void saveActiveModName(std::string modName);
            
            AnalyzerSource getActiveSource();
            void saveActiveSource(AnalyzerSource source);

            EventRegistration<ActiveModChangedEvent> activeModChangedEvents;

        private:
            bool _activeModChanged = false;
    };
}

#endif // MODMANAGER_H