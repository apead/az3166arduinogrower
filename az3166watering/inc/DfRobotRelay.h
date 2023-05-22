#ifndef DfRobotRelay_h
#define DfRobotRelay_h

#include "Arduino.h"


class DfRobotRelay
{
  public:
    DfRobotRelay(int pin);
    void setRelay(int state);
    int getRelay();
    void toggleRelay();
 
  private:
    int _pin;
    int _state;
};

#endif
