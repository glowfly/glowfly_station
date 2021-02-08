#ifndef INVALIDMODSTATE_H
#define INVALIDMODSTATE_H

#include "state.hpp"
#include "../station_context.hpp"
#include "../views/icon_text_view.cpp"

namespace SyncBlink
{
    class InvalidModState : public State
    {
        public:
            InvalidModState(StationContext& context) :
                _context(context),
                _invalidModView(std::make_shared<IconTextView>("Invalid MOD ...", u8g2_font_open_iconic_check_2x_t, 66))
            {
                _modEventHandleId = context.getModManager()
                    .activeModChangedEvents
                    .addEventHandler([this]() 
                    {
                        _activeModChanged = true;
                    });
            }

            ~InvalidModState()
            {
                _context.getModManager()
                    .activeModChangedEvents
                    .removeEventHandler(_modEventHandleId);
            }

            void run(StationContext& context)
            {
                context.getLed().setAllLeds(SyncBlink::Red);
                context.getDisplay().setView(_invalidModView);
                
                if(_activeModChanged) context.resetState();
            }

        private:
            StationContext& _context;
            std::shared_ptr<IconTextView> _invalidModView;            
            uint64_t _modEventHandleId = 0;
            bool _activeModChanged = false;
    };
}

#endif // INVALIDMODSTATE_H