#include <EEPROM.h>
#include <LittleFS.h>
#include "mod_manager.hpp"

namespace SyncBlink
{
    Mod ModManager::get(std::string& modName)
    {
        Mod mod;
        mod.Name = modName;
        mod.Exists = true;

        String path = "/mods/" + String(modName.c_str());
        if(LittleFS.exists(path))
        {
            File file = LittleFS.open(path, "r");
            while (file.available()) {
                mod.Content = file.readString().c_str();
            }
            file.close();
        }
        else
        {
            mod.Exists = false;
        }        

        return mod;
    }

    std::vector<std::string> ModManager::getList()
    {
        std::vector<std::string> modList;

        Dir dir = LittleFS.openDir("mods");
        while (dir.next())
        {
            modList.push_back(dir.fileName().c_str());
        }

        return modList;
    }

    void ModManager::add(std::string& modName)
    {
        File file = LittleFS.open("/mods/" + String(modName.c_str()), "w");
        file.close();
    }

    void ModManager::save(std::string modName, std::string modContent)
    {
        File file = LittleFS.open("/mods/" + String(modName.c_str()), "w");
        file.print(modContent.c_str());
        file.close();

        if(getActiveModName() == modName)
        {
            for(auto event : activeModChangedEvents.getEventHandlers()) event.second();
        }
    }

    void ModManager::remove(std::string& modName)
    {
        LittleFS.remove("/mods/" + String(modName.c_str()));
    }

    std::string ModManager::getActiveModName()
    {
        std::string activeMod;
        for (int i = 96; i < 193; ++i)
        {
            activeMod += char(EEPROM.read(i));
        }
        Serial.printf("Saved Active MOD: %s\n", activeMod.c_str());

        Mod mod = get(activeMod);
        if(!mod.Exists) 
        {
            Serial.println("Currently active mod not found! Falling back ...");
            std::vector<std::string> modList = getList();
            if(modList.size() > 0)
            {
                saveActiveModName(getList().front());
                activeMod = getActiveModName();
            }
        }
        return activeMod;
    }

    void ModManager::saveActiveModName(std::string modName)
    {
        if (modName.length() > 0) 
        {
            Serial.println("Clearing active Mod ...");
            for (int i = 96; i < 193; ++i) { EEPROM.write(i, 0); }

            Serial.printf("Saving active Mod (%s) ...\n", modName.c_str());
            for (uint i = 0; i < modName.length(); ++i) EEPROM.write(i + 96, modName[i]);

            EEPROM.commit();
            for(auto event : activeModChangedEvents.getEventHandlers()) event.second();
        }
    }

    AnalyzerSource ModManager::getActiveSource()
    {
        uint rawSource = EEPROM.read(193);
        if(rawSource != 0 && rawSource != 1) {
            Serial.println("Currently active source not valid! Falling back ...");
            saveActiveSource(AnalyzerSource::Base);
        }
        return static_cast<AnalyzerSource>(EEPROM.read(193));
    }

    void ModManager::saveActiveSource(AnalyzerSource source)
    {
        Serial.printf("Saving active Source (%d) ...\n", source);
        EEPROM.write(193, static_cast<uint>(source));
        EEPROM.commit();
    }
}