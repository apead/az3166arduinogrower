#include "Arduino.h"
#include "../inc/AutomationBit.h"



AutomationBit::AutomationBit(int state)
{
  //pinMode(pin, OUTPUT);
  _state = state;
  pinMode(38, OUTPUT);
  pinMode(ANALOG_ONE, INPUT);
  pinMode(ANALOG_TWO, INPUT);
  pinMode(ANALOG_THREE, INPUT);
}

 
void AutomationBit::setRelay(int state)
{
  digitalWrite(38, state);
}

int AutomationBit::getRelay()
{
  /*digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(250);*/
}

double AutomationBit::GetAnalog(int analog)
{
  int reading = analogRead(analog);
  return reading;
}

void AutomationBit::toggleRelay()
{
  /*digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(250);*/
}