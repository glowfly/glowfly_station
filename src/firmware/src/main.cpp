#include <Wire.h>
#include <EEPROM.h>
#include <LittleFS.h>

#include "station_context.hpp"

SyncBlink::StationContext stationContext;

void setup()
{
    Serial.begin(74880);
    Serial.println("Starting SyncBlink Station...");

    EEPROM.begin(512);
    LittleFS.begin();
    Wire.begin();

    stationContext.setup();
    
    pinMode(A0, INPUT);         // Mic Input
    pinMode(D6, INPUT);         // Check if module inserted
    pinMode(LED_PIN, OUTPUT);   // LED
}

void loop() 
{
    stationContext.loop();
}