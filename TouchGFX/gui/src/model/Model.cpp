/* USER CODE BEGIN Includes */
extern "C" {
  #include "cmsis_os2.h" // Header-ul FreeRTOS V2

  // Declaram cozile create în main.c
  extern osMessageQueueId_t tempQueueHandle;
  extern osMessageQueueId_t humQueueHandle;
}

#include <touchgfx/utils.hpp> // Necesar pentru formatarea textului
/* USER CODE END Includes */


#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
/* USER CODE BEGIN Model::tick */
  float temp, hum;
  osStatus_t status_temp, status_hum;

  // Verifica coada de temperatura (NON-BLOCANT - timeout 0U)
  status_temp = osMessageQueueGet(tempQueueHandle, &temp, NULL, 0U);
  if (status_temp == osOK) {
    // Avem o valoare noua. Spunem Presenter-ului sa actualizeze View-ul.
    modelListener->updateTemperature(temp);
  }

  // Verifica coada de umiditate
  status_hum = osMessageQueueGet(humQueueHandle, &hum, NULL, 0U);
  if (status_hum == osOK) {
    modelListener->updateHumidity(hum);
  }
/* USER CODE END Model::tick */
}
