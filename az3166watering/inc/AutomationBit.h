#ifndef AutomationBit_h
#define AutomationBit_h

#include "Arduino.h"

typedef enum Analog{  
  One = 23,
  Two = 22,
  Three = 16
};

const int ANALOG_ONE = 23;
const int ANALOG_TWO = 22;
const int ANALOG_THREE = 16;

class AutomationBit
{
  public:
    AutomationBit(int state);
    void setRelay(int state);
    int getRelay();
    void toggleRelay();

    double GetAnalog(int analog);

  private:
    int _state;
};

#endif
