#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{

}

void Screen1Presenter::activate()
{

}

void Screen1Presenter::deactivate()
{

}

/* USER CODE BEGIN Screen1Presenter */

void Screen1Presenter::updateTemperature(float temp)
{
  view.updateTemperature(temp); // Trimite mai departe la View
}

void Screen1Presenter::updateHumidity(float hum)
{
  view.updateHumidity(hum); // Trimite mai departe la View
}

/* USER CODE END Screen1Presenter */