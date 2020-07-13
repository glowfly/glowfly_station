#ifndef WRITERESULTSTATE_H
#define WRITERESULTSTATE_H

#include "no_mod_state.cpp"
#include "state.hpp"
#include "../station_context.hpp"
#include "../views/icon_text_view.cpp"

namespace GlowFly
{
    class WriteResultState : public State
    {
        public:
            WriteResultState(const bool success, const std::string text) : _success(success)
            { 
                if(success)
                {
                    _resultView = std::make_shared<IconTextView>(text, u8g2_font_open_iconic_check_2x_t, 65);
                }
                else
                {
                    _resultView = std::make_shared<IconTextView>(text, u8g2_font_open_iconic_check_2x_t, 66);
                }
            }

            void run(StationContext& context)
            {      
                context.getDisplay().setView(_resultView);
                
                if(_success) context.getLed().setAllLeds(GlowFly::Green);
                else context.getLed().setAllLeds(GlowFly::Red);

                if(_started + 2000 < millis())
                {
                    context.currentState = std::make_shared<NoModState>();
                }
            }
        
        private:
            const boolean _success;     
            std::shared_ptr<IconTextView> _resultView;
            uint64_t _started = millis();  
    };
}

#endif // WRITERESULTSTATE_H