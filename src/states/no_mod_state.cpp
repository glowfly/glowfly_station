#ifndef NOMODSTATE_H
#define NOMODSTATE_H

#include <memory>
#include <led/patterns/led_fade_pattern.cpp>

#include "state.hpp"
#include "read_mod_state.cpp"
#include "../station_context.hpp"
#include "../views/insert_mod_view.cpp"

namespace SyncBlink
{
    class NoModState : public State
    {
        public:
            NoModState()
            {
                _insertModView = std::make_shared<InsertModView>();
                _fadePattern = std::make_shared<LedFadePattern>(SyncBlink::Yellow);
            }

            void run(StationContext& context)
            {
                if(!_modRemovedSent)
                {
                    Client::Command modRemovedCommand = { millis(), Client::MOD_REMOVED };
                    context.getSocketServer().broadcast(modRemovedCommand);
                    _modRemovedSent = true;
                }
                
                context.getLed().setPattern(_fadePattern);
                context.getDisplay().setView(_insertModView);
                if(context.isModInserted())
                {
                    _modRemovedSent = false;
                    context.getLed().clearGroups();
                    context.getLed().setPattern(nullptr);
                    context.currentState = std::make_shared<ReadModState>();
                }
            }

        private:
            bool _modRemovedSent = false;
            std::shared_ptr<LedFadePattern> _fadePattern;
            std::shared_ptr<InsertModView> _insertModView;
    };
}

#endif // NOMODSTATE_H