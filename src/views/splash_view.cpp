#ifndef SPLASHVIEW_H
#define SPLASHVIEW_H

#include "hardware/display/view.hpp"
#include "hardware/display/display.hpp"

namespace GlowFly
{
    class SplashView : public View
    {
        public:
            void drawFrame(DisplayCtrl& display) override
            {
                display.setFont(u8g2_font_maniac_tf);
                drawVHCenteredStr(display, "GlowFly");
            }
    };
}

#endif // SPLASHVIEW_H