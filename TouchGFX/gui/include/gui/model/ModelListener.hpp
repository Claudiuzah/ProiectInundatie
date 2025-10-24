#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    /* USER CODE BEGIN ModelListener */
    virtual void updateTemperature(float temp) {}
    virtual void updateHumidity(float hum) {}
    /* USER CODE END ModelListener */

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP

