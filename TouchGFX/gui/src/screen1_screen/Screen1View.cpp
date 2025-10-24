#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();

    /* USER CODE BEGIN setupScreen */

    textArea1.setWildcard(tempBuffer); 
    textArea2.setWildcard(humBuffer);

    /* USER CODE END setupScreen */

}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

/* USER CODE BEGIN Screen1View */
void Screen1View::updateTemperature(float temp)
{
  // Goleste bufferul vechi
  Unicode::strncpy(tempBuffer, "", 6); 
  // Formateaza valoarea float în bufferul Unicode
  Unicode::snprintfFloat(tempBuffer, 6, "%.1f", temp); 
  // Invalideaza (forteaza redesenarea) zonei de text
  textArea1.invalidate(); 
}

void Screen1View::updateHumidity(float hum)
{
  Unicode::strncpy(humBuffer, "", 6);
  Unicode::snprintfFloat(humBuffer, 6, "%.1f", hum);
  textArea2.invalidate();
}
/* USER CODE END Screen1View */