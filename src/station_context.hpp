#ifndef STATIONCONTEXT_H
#define STATIONCONTEXT_H

#include <memory>
#include <led/led.hpp>
#include <network/mesh/glowfly_mesh.hpp>
#include <network/websocket/socket_server.hpp>
#include <network/webserver/glowfly_web.hpp>

#include "states/state.hpp"
#include "hardware/display/display.hpp"
#include "hardware/rom/rom_24LC32A.hpp"

namespace GlowFly
{
    class StationContext
    {
        public:
            StationContext();

            void setup();
            void loop();

            bool isModInserted() const;

            LED& getLed();
            Rom24LC32A& getRom();
            Display& getDisplay();
            uint32_t getMeshLedCount();
            SocketServer& getSocketServer();
            IPAddress getLocalIp();

            bool writeRequested = false;
            std::shared_ptr<State> currentState;

        private:
            void checkModuleInserted();
            void checkWriteRequest();

            void startMeshCount();
            void onSocketServerMeshConnection();
            void onSocketServerCommandReceived(Server::Command command);

            LED _led;
            GlowFlyWeb _web;
            GlowFlyMesh _mesh;
            Display _display;
            SocketServer _socketServer;
            Rom24LC32A _rom = Rom24LC32A(0x50);

            uint32_t _meshLedCount = LED_COUNT;
            std::shared_ptr<State> _noModState;
    };
}

#endif // STATIONCONTEXT_H