#ifndef SYNCBLINKWEB_H
#define SYNCBLINKWEB_H

#include <ESP8266WebServer.h>
#include "wifi/station_wifi.hpp"

namespace SyncBlink
{
    class SyncBlinkWeb
    {
        public:
            SyncBlinkWeb(StationWifi& stationWifi);

            void loop();

        private:
            void setWifi();
            void getWifi();
            
            void addMod();
            void saveMod();
            void deleteMod();
            void getMods();
            void getModContent();

            ESP8266WebServer _server;
            StationWifi _stationWifi;
    };
}

#endif // SYNCBLINKWEB_H