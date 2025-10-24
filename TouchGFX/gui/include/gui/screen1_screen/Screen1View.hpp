#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

        /* USER CODE BEGIN Screen1View */
        void updateTemperature(float temp);
        void updateHumidity(float hum);

        protected:
        // Buffere pentru textul wildcard
        touchgfx::Unicode::UnicodeChar tempBuffer[6]; // "XX.X" + semn + null
        touchgfx::Unicode::UnicodeChar humBuffer[6];  // "YY.Y" + null
        /* USER CODE END Screen1View */

protected:
};

#endif // SCREEN1VIEW_HPP
