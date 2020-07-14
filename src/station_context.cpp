#include "station_context.hpp"
#include "states/no_mod_state.cpp"
#include "states/receive_length_state.cpp"
#include "views/splash_view.cpp"

namespace GlowFly
{
    StationContext::StationContext()
    {
        _noModState = std::make_shared<NoModState>();
        currentState = _noModState;
    }

    void StationContext::setup()
    {
        _display.init();
        _display.setView(std::make_shared<GlowFly::SplashView>());
        _display.loop();
        
        _socketServer
            .serverCommandEvents
            .addEventHandler([this](Server::Command command) { onSocketServerCommandReceived(command); });
        _socketServer
            .meshConnectionEvents
            .addEventHandler([this]() { onSocketServerMeshConnection(); });

        WiFi.disconnect();
        _web.connectNetwork();
        _mesh.startMesh();
    }

    void StationContext::loop()
    {
        checkModuleInserted();
        checkWriteRequest();
        
        _display.setLeftStatus("");
        _display.setRightStatus(getLocalIp().toString().c_str());
        
        currentState->run(*this);
        _socketServer.loop();
        _led.loop();
        _web.loop();
        _display.loop();
    }

    bool StationContext::isModInserted() const
    {
        return digitalRead(D6) == HIGH;
    }

    void StationContext::checkModuleInserted()
    {
        if(!isModInserted()) currentState = _noModState;
    }

    void StationContext::checkWriteRequest()
    {
        if(Serial.available() > 0
            && !writeRequested
            && isModInserted())
        {
            int incomingByte = Serial.read();
            if(incomingByte == 30)
            {
                Serial.write(30);
                writeRequested = true;
                currentState = std::make_shared<ReceiveLengthState>();
            }
        }
    }

    void StationContext::startMeshCount()
    {
        Serial.println("Connection state changed. Starting MESH_COUNT_REQUEST ...");
        if(_socketServer.getClientsCount() == 0)
        {
            Serial.println("No nodes connected!");
            Serial.printf("GlowFly Station alone with %d LEDs :(\n", LED_COUNT);
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
                currentState = std::make_shared<NoModState>();
                break;
            case Server::EXTERNAL_ANALYZER:
            case Server::MOD_DISTRIBUTED:
                break;
        }
    }

    LED& StationContext::getLed() { return _led; }
    Rom24LC32A& StationContext::getRom() { return _rom; }
    Display& StationContext::getDisplay() { return _display; }
    uint32_t StationContext::getMeshLedCount() { return _meshLedCount; }
    SocketServer& StationContext::getSocketServer() { return _socketServer; }
    IPAddress StationContext::getLocalIp() { return _mesh.getLocalIp(); }
}