#ifndef Az3166PlantOled_h
#define Az3166PlantOled_h

#include "Arduino.h"
#include <OledDisplay.h>

class Az3166PlantOled
{
  public:
    Az3166PlantOled();
    void InvalidateDisplay();
    void SetMoisturePercentage(int percentage);
    void SetTemperature(int temperature);
    void SetHumidity(int humidity);
    void SetPressure(int pressure);
    void SetPlantState(int state);
 
  private:
    int _waterPercentage;
    int _temperature;
    int _humidity;
    int _pressure;
    int _plantState;
};

#endif
