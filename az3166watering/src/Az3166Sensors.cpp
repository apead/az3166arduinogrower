#include "Arduino.h"
#include "../inc/Az3166Sensors.h"
#include "Sensor.h"

static HTS221Sensor *tempSensor;
static LPS22HBSensor *pressureSensor;
static DevI2C *ext_i2c;


Az3166Sensors::Az3166Sensors()
{
	
}

void Az3166Sensors::Init()
{
  ext_i2c = new DevI2C(D14, D15);

  tempSensor = new HTS221Sensor(*ext_i2c);
  tempSensor->init(NULL);
  tempSensor->reset();
  
  pressureSensor = new LPS22HBSensor(*ext_i2c);
  pressureSensor->init(NULL);
 
}

double Az3166Sensors::Round(float var) 
{ 
    double value = (int)(var * 10 + .5); 
    return (double)value / 10; 
} 

float Az3166Sensors::ReadHumidity()
{
    float h = 0;
    bool valueRead = false;

    tempSensor->enable();
    while (! valueRead) {
        valueRead = tempSensor->getHumidity(&h) == 0;
        if (! valueRead) {
            tempSensor->reset();
        }
    }
    tempSensor->disable();
    tempSensor->reset();

    return Round(h);
}

float Az3166Sensors::ReadTemperature()
{
    float t = 0;
    bool valueRead = false;

    tempSensor->enable();
    while (! valueRead) {
        valueRead = tempSensor->getTemperature(&t) == 0;
        if (! valueRead != 0) {
            tempSensor->reset();
        }
    }
    tempSensor->disable();
    tempSensor->reset();

    return Round(t);
}

float Az3166Sensors::ReadPressure()
{
    float p = 0;
    bool valueRead = false;

    while (! valueRead) {
        valueRead = pressureSensor->getPressure(&p) == 0;
    }

    return Round(p);
}

