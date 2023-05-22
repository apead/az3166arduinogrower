#ifndef Az3166Sensors_h
#define Az3166Sensors_h

#include "Arduino.h"

class Az3166Sensors
{
  public:
    Az3166Sensors();
    void Init();
    float ReadHumidity();
    float ReadTemperature();
    float ReadPressure();
    private:

    double Round(float var);
};

#endif
