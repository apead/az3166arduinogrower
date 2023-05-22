#ifndef DfRobotMoistureSensor_h
#define DfRobotMoistureSensor_h

#include "Arduino.h"

const int AirValue = 734;   
const int WaterValue = 260;  

typedef enum MoistureDegree{  
  VeryWet,
  Wet,
  Dry
};

/*
soilMoistureValue = analogRead(A0);  //put Sensor insert into soil


*/

class DfRobotMoistureSensor
{
  public:
    DfRobotMoistureSensor(int pin);
    int GetSensorReading();
    MoistureDegree GetMoistureReading();
    int GetMoisturePercentage();
 
  private:
    int _pin;
};

#endif
