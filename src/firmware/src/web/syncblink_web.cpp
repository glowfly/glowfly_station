#include <EEPROM.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "syncblink_web.hpp"

namespace SyncBlink
{
    SyncBlinkWeb::SyncBlinkWeb(StationWifi& stationWifi) : _server(80), _stationWifi(stationWifi)
    {
        _server.on("/api/wifi/set", [this]() { setWifi(); });
        _server.on("/api/wifi/get", [this]() { getWifi(); });

        _server.on("/api/mods/list", [this]() { getMods(); });
        _server.on("/api/mods/get", [this]() { getModContent(); });
        _server.on("/api/mods/add", [this]() { addMod(); });
        _server.on("/api/mods/save", [this]() { saveMod(); });
        _server.on("/api/mods/delete", [this]() { deleteMod(); });

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
    }

    void SyncBlinkWeb::getWifi()
    {
        String JSON;
        StaticJsonDocument<500> jsonBuffer;
        JsonObject wifi = jsonBuffer.createNestedObject("wifi");               

        auto ssidRef = wifi.getOrAddMember("ssid");
        auto passRef = wifi.getOrAddMember("pass");
        auto connectedRef = wifi.getOrAddMember("connected");

        ssidRef.set(_stationWifi.getSavedSSID().c_str());
        passRef.set(_stationWifi.getSavedPass().c_str());
        connectedRef.set(WiFi.status() == WL_CONNECTED);

        serializeJson(jsonBuffer, JSON);
        _server.send(200, "application/json", JSON);
    }    

    void SyncBlinkWeb::addMod()
    {
        String modName = _server.arg("name");
        LittleFS.open("/mods/" + modName, "w");

        _server.send(200, "application/json", "{ \"saved\": true }");
    }

    void SyncBlinkWeb::saveMod()
    {
        String body = _server.arg("plain");
        StaticJsonDocument<5000> mod;
        deserializeJson(mod, body);

        String modName = mod["name"];
        String modContent = mod["content"];

        File file = LittleFS.open("/mods/" + modName, "w");
        file.print(modContent);

        _server.send(200, "application/json", "{ \"saved\": true }");
    }

    void SyncBlinkWeb::deleteMod()
    {
        String modName = _server.arg("name");
        LittleFS.remove("/mods/" + modName);

        _server.send(200, "application/json", "{ \"saved\": true }");
    }

    void SyncBlinkWeb::getMods()
    {
        String JSON;
        StaticJsonDocument<1000> jsonBuffer;
        JsonArray files = jsonBuffer.createNestedArray("mods");

        Dir dir = LittleFS.openDir("mods");
        while (dir.next())
            files.add(dir.fileName());

        serializeJson(jsonBuffer, JSON);
        _server.send(200, "application/json", JSON);
    }

    void SyncBlinkWeb::getModContent()
    {
        String JSON, modContent;
        StaticJsonDocument<5000> jsonBuffer;
        JsonObject mod = jsonBuffer.createNestedObject("mod");
        
        String modName = _server.arg("name");
        File file = LittleFS.open("/mods/" + modName, "r");
        while (file.available()) {
            modContent = file.readString();
        }
        file.close();

        auto nameRef = mod.getOrAddMember("name");
        auto contentRef = mod.getOrAddMember("content");

        nameRef.set(modName);
        contentRef.set(modContent);

        serializeJson(jsonBuffer, JSON);
        _server.send(200, "application/json", JSON);
    }
}