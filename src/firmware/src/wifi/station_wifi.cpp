#include <EEPROM.h>
#include "station_wifi.hpp"

namespace SyncBlink
{
    StationWifi::StationWifi()
    {
        WiFi.disconnect();
        _mesh.startMesh();
    }

    void StationWifi::connectWifi()
    {
        Serial.println("Connecting to Network ...");
        
        std::string ssid = getSavedSSID();        
        std::string password = getSavedPass();

        if (ssid.length() > 1 ) 
        {
            WiFi.begin(ssid.c_str(), password.c_str());
            int waitCounter = 0;
            Serial.print("Waiting for Wifi to connect ");
            while (WiFi.status() != WL_CONNECTED && waitCounter < 20) 
            {
                delay(500);
                Serial.print(".");
                waitCounter++;
            }
            
            if(WiFi.status() != WL_CONNECTED)
            {
                Serial.println("Couldn't connect to network!");
                WiFi.disconnect();
            }
            else { Serial.println("Connected!"); }
        }
    }

    void StationWifi::saveWifi(std::string ssid, std::string pass)
    {
        if (ssid.length() > 0 && pass.length() > 0) 
        {
            Serial.println("Clearing SSID ...");
            for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
        }

        Serial.printf("Writing SSID (%s) ...\n", ssid.c_str());
        for (uint i = 0; i < ssid.length(); ++i) EEPROM.write(i, ssid[i]);

        Serial.printf("Writing Password (%s)\n ...", pass.c_str()); 
        for (uint i = 0; i < pass.length(); ++i) EEPROM.write(32 + i, pass[i]);

        EEPROM.commit();
        ESP.restart();
    }

    std::string StationWifi::getSavedSSID()
    {
        std::string ssid;
        for (int i = 0; i < 32; ++i)
        {
            ssid += char(EEPROM.read(i));
        }
        Serial.printf("SSID: %s\n", ssid.c_str());
        return ssid;
    }

    std::string StationWifi::getSavedPass()
    {
        std::string password;
        for (int i = 32; i < 96; ++i)
        {
            password += char(EEPROM.read(i));
        }
        Serial.printf("Password: %s\n", password.c_str());
        return password;
    }
}