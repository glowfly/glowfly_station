#include "station_context.hpp"
#include "states/read_mod_state.cpp"
#include "views/splash_view.cpp"

namespace SyncBlink
{
    StationContext::StationContext() : _web(_wifi)
    {
        currentState = std::make_shared<ReadModState>(_modManager);;
    }

    void StationContext::setup()
    {
        _display.init();
        _display.setView(std::make_shared<SyncBlink::SplashView>());
        _display.loop();
        
        _socketServer
            .serverCommandEvents
            .addEventHandler([this](Server::Command command) { onSocketServerCommandReceived(command); });
        _socketServer
            .meshConnectionEvents
            .addEventHandler([this]() { onSocketServerMeshConnection(); });

        _wifi.connectWifi();
    }

    void StationContext::loop()
    {        
        _display.setLeftStatus("");
        _display.setRightStatus(WiFi.localIP().toString().c_str());
        
        currentState->run(*this);
        _socketServer.loop();
        _led.loop();
        _web.loop();
        _display.loop();
    }

    void StationContext::startMeshCount()
    {
        Serial.println("Connection state changed. Starting MESH_COUNT_REQUEST ...");
        if(_socketServer.getClientsCount() == 0)
        {
            Serial.println("No nodes connected!");
            Serial.printf("SyncBlink Station alone with %d LEDs :(\n", LED_COUNT);
            _meshLedCount = LED_COUNT;
        }
        else
        {
            Client::CountCommand countCommand = { LED_COUNT, LED_COUNT, 1 };
            Client::Command command = { millis(), Client::MESH_COUNT_REQUEST };
            command.countCommand = countCommand;
            
            _socketServer.broadcast(command);
        }
    }

    void StationContext::onSocketServerMeshConnection()
    {
        startMeshCount();
    }

    void StationContext::onSocketServerCommandReceived(Server::Command command)
    {
        Client::Command updateCommand;
        Client::CountCommand countCommand;
        switch (command.commandType)
        {
            case Server::MESH_CONNECTION:
                startMeshCount();
                break;
            case Server::MESH_COUNTED:
                _meshLedCount = command.countedCommand.meshLedCount;
                Serial.printf("MESH_COUNT done with %d LEDs on the longest route.\n", _meshLedCount);
                Serial.println("Distributing result of count ...");

                countCommand = { _meshLedCount, LED_COUNT, 1 };
                updateCommand = { millis(), Client::MESH_UPDATE };
                updateCommand.countCommand = countCommand;
                _socketServer.broadcast(updateCommand);
                break;
            case Server::MESH_UPDATED:
                Serial.println("Mesh updated - Resetting state ...");
                currentState = std::make_shared<ReadModState>(_modManager);
                break;
            case Server::EXTERNAL_ANALYZER:
            case Server::MOD_DISTRIBUTED:
                break;
        }
    }

    LED& StationContext::getLed() { return _led; }
    Display& StationContext::getDisplay() { return _display; }
    uint32_t StationContext::getMeshLedCount() { return _meshLedCount; }
    SocketServer& StationContext::getSocketServer() { return _socketServer; }
}