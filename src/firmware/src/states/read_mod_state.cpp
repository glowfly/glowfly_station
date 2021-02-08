#ifndef READMODSTATE_H
#define READMODSTATE_H

#include <blinkscript/blink_script.hpp>

#include "state.hpp"
#include "broadcast_mod_state.cpp"
#include "invalid_mod_state.cpp"
#include "station_context.hpp"
#include "views/icon_text_view.cpp"
#include "mod/mod_manager.hpp"
#include "mod/mod.hpp"

namespace SyncBlink
{
    class ReadModState : public State
    {
        public:
            ReadModState()
            {
                _readModView = std::make_shared<IconTextView>("Reading MOD ...", u8g2_font_open_iconic_thing_2x_t, 74);
            }

            void run(StationContext& context)
            {                         
                context.getLed().setAllLeds(SyncBlink::Yellow);
                context.getLed().loop();
                context.getDisplay().setView(_readModView);
                context.getDisplay().loop();

                std::string activeModName = context.getModManager().getActiveModName();
                Mod activeMod = context.getModManager().get(activeModName);

                if(activeMod.Exists)
                {
                    BlinkScript blinkScript = BlinkScript(context.getLed(), activeMod.Content);                    
                    if(!blinkScript.isFaulted()) context.currentState = std::make_shared<BroadcastModState>(context, activeMod.Content);
                    else context.currentState = std::make_shared<InvalidModState>(context);
                }
                else context.currentState = std::make_shared<InvalidModState>(context);     
            }

        private:
            bool isEmptyMod(const std::vector<uint8_t>& modChars) const
            {
                // A factory ROM will return the same char for the
                // whole memory - if the mod is empty, it is invalid
                bool isEmptyMod = true;
                for (uint32_t i = 1; i < modChars.size(); i++)
                {
                    if (modChars[i] != modChars[0])
                    {
                        isEmptyMod = false;
                        break;
                    }
                }
                return isEmptyMod;
            }
            
            std::shared_ptr<IconTextView> _readModView;
    };
}

#endif // READMODSTATE_H