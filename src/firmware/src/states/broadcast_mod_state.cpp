#ifndef BROADCASTMODSTATE_H
#define BROADCASTMODSTATE_H

#include <visualization/script_context.hpp>
#include "run_mod_state.cpp"
#include "invalid_mod_state.cpp"
#include "state.hpp"
#include "../station_context.hpp"
#include "../views/icon_text_view.cpp"

namespace SyncBlink
{
    class BroadcastModState : public State
    {
        public:
            BroadcastModState(StationContext& context, std::string mod)
                : _context(context), _mod(mod)
            {
                _handleId = context.getSocketServer()
                    .serverCommandEvents
                    .addEventHandler([this](Server::Command command) 
                    { 
                        _modDistributed = command.commandType == Server::MOD_DISTRIBUTED; 
                    });
                _broadcastModView = std::make_shared<IconTextView>("Broadcasting MOD ...", u8g2_font_open_iconic_thing_2x_t, 74);
            }

            ~BroadcastModState()
            {
                _context.getSocketServer()
                    .serverCommandEvents
                    .removeEventHandler(_handleId);
            }

            void run(StationContext& context)
            {                         
                context.getLed().setAllLeds(SyncBlink::Yellow);
                context.getDisplay().setView(_broadcastModView);
                context.getDisplay().loop();

                if(!_broadcastStarted && context.getSocketServer().getClientsCount() > 0)
                {
                    context.getSocketServer().broadcastMod(_mod);
                    _broadcastStarted = true;
                }
                else if(context.getSocketServer().getClientsCount() == 0) _modDistributed = true;

                if(_modDistributed)
                {
                    auto scriptContext = std::make_shared<ScriptContext>(context.getLed(), _mod);
                    scriptContext->updateLedInfo(0, 0, context.getMeshLedCount());
                    scriptContext->init();

                    Client::SourceCommand sourceCommand = { scriptContext->source };
                    Client::Command command = { millis(), Client::SOURCE_UPDATE };
                    command.sourceCommand = sourceCommand;
                    context.getSocketServer().broadcast(command);

                    context.getLed().setAllLeds(SyncBlink::Black);
                    context.currentState = std::make_shared<RunModState>(context, scriptContext);

                    _modDistributed = false;
                    _broadcastStarted = false;
                }
            }

        private:
            StationContext& _context;
            std::string _mod;
            uint64_t _handleId;

            std::shared_ptr<IconTextView> _broadcastModView;
            bool _broadcastStarted = false;
            bool _modDistributed = false;
    };
}

#endif // BROADCASTMODSTATE_H