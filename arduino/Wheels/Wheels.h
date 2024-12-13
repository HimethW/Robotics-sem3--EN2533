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

    void setEncoderPins(byte encoderPin1, byte encoderPin2);
    void updateEncoderCount();
    void resetEncoderCount();
    long encoderCount; // kinda ugly having this here tho ig. we may not need two counts if we are doing the direction thing.

    void setSpeed(int speed);
    void forward();
    void backward();
    void stop();
    void brake();

  private:
    byte _controlPinA, _controlPinB, _speedPin;
    byte _encoderPin1, _encoderPin2;
};

typedef struct {
    byte controlPinA;
    byte controlPinB;
    byte speedPin;
    byte encoderPin1 = -1;
    byte encoderPin2 = -1;
} Wheel_t;

#endif
