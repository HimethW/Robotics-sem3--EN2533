/*
  Wheel.h
  EN2533 - Robot Design and Competition
  Nilakna Warushavithana, October 2024
*/

#ifndef Wheel_h
#define Wheel_h

#include "Arduino.h"

class Wheel {
  public:
    Wheel(byte in1, byte in2, byte enA);
    void setSpeed(int speed);
    void forward();
    bool backward();
    void stop();
    void brake();

  private:
    byte _in1, _in2, _enA;
};

typedef struct {
    byte controlPinA,
    byte controlPinB,
    byte speedPin
} Wheel_t;

#endif
