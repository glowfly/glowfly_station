#include <LittleFS.h>
#include <ArduinoJson.h>
#include <network/websocket/messages/audio_analyzer_message.hpp>

#include "syncblink_web.hpp"

namespace SyncBlink
{
    SyncBlinkWeb::SyncBlinkWeb(StationWifi& stationWifi, ModManager& modManager) : _server(80), _stationWifi(stationWifi), _modManager(modManager)
    {
        _server.on("/api/wifi/set", [this]() { setWifi(); });
        _server.on("/api/wifi/get", [this]() { getWifi(); });

        _server.on("/api/mods/list", [this]() { getMods(); });
        _server.on("/api/mods/get", [this]() { getModContent(); });

        _server.on("/api/mods/add", [this]() { addMod(); });
        _server.on("/api/mods/save", [this]() { saveMod(); });
        _server.on("/api/mods/delete", [this]() { deleteMod(); });

        _server.on("/api/mods/getModSettings", [this]() { getModSettings(); });
        _server.on("/api/mods/setModSettings", [this]() { setModSettings(); });

        _server.serveStatic("/", LittleFS, "/public/");
        _server.begin();
    }

    void SyncBlinkWeb::loop()
    {
        _server.handleClient();
    }

    void SyncBlinkWeb::setWifi()
    {
        String ssid = _server.arg("ssid");
        String pass = _server.arg("pass");
        _stationWifi.saveWifi(ssid.c_str(), pass.c_str());
        _server.send(200, "application/json", "{ \"saved\": true }");
    }

    void SyncBlinkWeb::getWifi()
    {
        String JSON;
        StaticJsonDocument<500> jsonBuffer;
        JsonObject wifi = jsonBuffer.createNestedObject("wifi");               

        auto ssidRef = wifi.getOrAddMember("ssid");
        auto passRef = wifi.getOrAddMember("pass");
        auto connectedRef = wifi.getOrAddMember("connected");

        std::string ssid = _stationWifi.getSavedSSID();
        std::string pass = _stationWifi.getSavedPass();

        ssidRef.set(ssid.c_str());
        passRef.set(pass.c_str());
        connectedRef.set(WiFi.status() == WL_CONNECTED);

        serializeJson(jsonBuffer, JSON);
        _server.send(200, "application/json", JSON);
    }    

    void SyncBlinkWeb::addMod()
    {
        std::string modName = _server.arg("name").c_str();

        _modManager.add(modName);
        _server.send(200, "application/json", "{ \"saved\": true }");
    }

    void SyncBlinkWeb::saveMod()
    {
        String body = _server.arg("plain");
        StaticJsonDocument<5000> mod;
        deserializeJson(mod, body);

        String modName = mod["name"];
        String modContent = mod["content"];

        _modManager.save(modName.c_str(), modContent.c_str());
        _server.send(200, "application/json", "{ \"saved\": true }");
    }

    void SyncBlinkWeb::deleteMod()
    {
        std::string modName = _server.arg("name").c_str();

        _modManager.remove(modName);
        _server.send(200, "application/json", "{ \"saved\": true }");
    }

    void SyncBlinkWeb::getMods()
    {
        String JSON;
        StaticJsonDocument<1000> jsonBuffer;
        JsonArray files = jsonBuffer.createNestedArray("mods");

        std::vector<std::string> modList = _modManager.getList();
        for(std::string modName : modList)
            files.add(modName.c_str());

        serializeJson(jsonBuffer, JSON);
        _server.send(200, "application/json", JSON);
    }

    void SyncBlinkWeb::getModContent()
    {
        String JSON, modContent;
        StaticJsonDocument<5000> jsonBuffer;
        JsonObject modJson = jsonBuffer.createNestedObject("mod");      

        std::string modName = _server.arg("name").c_str();
        Mod mod = _modManager.get(modName);

        auto nameRef = modJson.getOrAddMember("name");
        auto contentRef = modJson.getOrAddMember("content");
        auto existsRef = modJson.getOrAddMember("exists");

        nameRef.set(mod.Name.c_str());
        contentRef.set(mod.Content.c_str());
        existsRef.set(mod.Exists);

        serializeJson(jsonBuffer, JSON);
        _server.send(200, "application/json", JSON);
    }

    void SyncBlinkWeb::getModSettings()
    {
        std::string activeMod = _modManager.getActiveModName();
        AudioAnalyzerSource activeSource = _modManager.getActiveSource();

        String JSON;
        StaticJsonDocument<1000> jsonBuffer;
        JsonObject mod = jsonBuffer.createNestedObject("modSettings");

        auto nameRef = mod.getOrAddMember("name");
        nameRef.set(activeMod.c_str());

        auto sourceRef = mod.getOrAddMember("source");
        sourceRef.set(activeSource);

        serializeJson(jsonBuffer, JSON);
        _server.send(200, "application/json", JSON);
    }

    void SyncBlinkWeb::setModSettings()
    {
        std::string modName = _server.arg("name").c_str();
        uint source = std::atoi(_server.arg("source").c_str());

        if(_modManager.get(modName).Exists && (source == 0 || source == 1)) {
            _modManager.saveActiveModName(modName);
            _modManager.saveActiveSource(static_cast<AudioAnalyzerSource>(source));
            _server.send(200, "application/json", "{ \"saved\": true }");
        }
        else {
            _server.send(200, "application/json", "{ \"saved\": false }");
        }        
    }
}