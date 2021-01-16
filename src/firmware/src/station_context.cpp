#include "station_context.hpp"
#include "states/read_mod_state.cpp"
#include "views/splash_view.cpp"

namespace SyncBlink
{
    StationContext::StationContext() : _web(_wifi, _modManager)
    {
        resetState();
    }

    void StationContext::setup()
    {
        _display.init();
        _display.setView(std::make_shared<SyncBlink::SplashView>());
        _display.loop();
        _socketServer
            .serverMessageEvents
            .addEventHandler([this](Server::Message message) { onSocketServerCommandReceived(message); });
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

        std::unique_ptr<ReadModState> test(ReadModState);
    }

    void StationContext::resetState() 
    { 
        currentState = std::make_shared<ReadModState>(); 
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
            Client::CountMessage countMessage = { LED_COUNT, LED_COUNT, 1 };
            Client::Message message = { millis(), Client::MESH_COUNT_REQUEST };
            message.countMessage = countMessage;
            
            _socketServer.broadcast(message);
        }
    }

    void StationContext::onSocketServerMeshConnection()
    {
        startMeshCount();
    }

    void StationContext::onSocketServerCommandReceived(Server::Message message)
    {
        Client::Message updateMessage;
        Client::CountMessage countMessage;
        switch (updateMessage.messageType)
        {
            case Server::MESH_CONNECTION:
                startMeshCount();
                break;
            case Server::MESH_COUNTED:
                _meshLedCount = updateMessage.countMessage.meshLedCount;
                Serial.printf("MESH_COUNT done with %d LEDs on the longest route.\n", _meshLedCount);
                Serial.println("Distributing result of count ...");

                countMessage = { _meshLedCount, LED_COUNT, 1 };
                updateMessage = { millis(), Client::MESH_UPDATE };
                updateMessage.countMessage = countMessage;
                _socketServer.broadcast(updateMessage);
                break;
            case Server::MESH_UPDATED:
                Serial.println("Mesh updated - Resetting state ...");
                currentState = std::make_shared<ReadModState>();
                break;
            case Server::EXTERNAL_ANALYZER:
            case Server::MOD_DISTRIBUTED:
                break;
        }
    }

    LED& StationContext::getLed() { return _led; }
    Display& StationContext::getDisplay() { return _display; }
    ModManager& StationContext::getModManager() { return _modManager; }
    uint32_t StationContext::getMeshLedCount() { return _meshLedCount; }
    SocketServer& StationContext::getSocketServer() { return _socketServer; }
}