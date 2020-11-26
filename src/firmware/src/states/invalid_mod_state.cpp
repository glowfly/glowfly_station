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
            InvalidModState()
            {
                _invalidModView = std::make_shared<IconTextView>("Invalid MOD ...", u8g2_font_open_iconic_check_2x_t, 66);
            }

            void run(StationContext& context)
            {
                context.getLed().setAllLeds(SyncBlink::Red);
                context.getDisplay().setView(_invalidModView);
            }

        private:
            std::shared_ptr<IconTextView> _invalidModView;
    };
}

#endif // INVALIDMODSTATE_H