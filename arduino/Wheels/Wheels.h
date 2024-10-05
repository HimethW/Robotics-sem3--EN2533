/*
  Wheels.h
  EN2533 - Robot Design and Competition
  Nilakna Warushavithana, October 2024
*/

#ifndef Wheels_h
#define Wheels_h

#include "Arduino.h"

class Wheel {
  public:
    Wheel(byte controlPinA, byte controlPinB, byte speedPin);
    void setSpeed(int speed);
    void forward();
    bool backward();
    void stop();
    void brake();

  private:
    byte _controlPinA, _controlPinB, _speedPin;
};

typedef struct {
    byte controlPinA,
    byte controlPinB,
    byte speedPin
} Wheel_t;

#endif
