#include "Arduino.h"
#include "../inc/DfRobotMoistureSensor.h"
         

DfRobotMoistureSensor::DfRobotMoistureSensor(int pin)
{
  pinMode(pin, INPUT);
  _pin = pin;
}

int DfRobotMoistureSensor::GetSensorReading()
{
  int reading = analogRead(_pin);
  return reading;
}

int DfRobotMoistureSensor::GetMoisturePercentage()
{
  int soilMoistureValue = GetSensorReading();

  int moisture1_value = map(soilMoistureValue, AirValue, WaterValue, 0, 100); 

  if (moisture1_value < 0)
    moisture1_value = 0;

  if (moisture1_value > 100)
    moisture1_value = 100;

  return moisture1_value;
}

MoistureDegree DfRobotMoistureSensor::GetMoistureReading()
{
  int intervals = (AirValue - WaterValue) / 3;

  MoistureDegree moisture;
  int soilMoistureValue = GetSensorReading();

  if (soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
  {
    Serial.println("Very Wet");
  }
  else if (soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (AirValue - intervals))
  {
    Serial.println("Wet");
  }
  else if (soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals))
  {
    Serial.println("Dry");
  }

  return moisture;
}
