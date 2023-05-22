#include "Arduino.h"
#include "../inc/DfRobotRelay.h"
         

DfRobotRelay::DfRobotRelay(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;

  setRelay(LOW);
}

void DfRobotRelay::setRelay(int state)
{
  _state = state;
  digitalWrite(_pin,_state);
}

void DfRobotRelay::toggleRelay()
{
    digitalWrite(_pin,!_state);
}

int DfRobotRelay::getRelay()
{
  return _state;
}