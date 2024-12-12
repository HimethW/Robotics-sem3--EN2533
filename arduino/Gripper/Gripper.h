#ifndef Gripper_h
#define Gripper_h

#include "Arduino.h"
#include "Servo.h"

class Gripper {
    public:
    Gripper (byte controlPinA, byte controlPinB);

    void initialize();
    void lift (byte stopAngle); //For 0 < angle <= 90
    void lower ();
    void grab ();
    void release ();
    void push();

    private:
    Servo _servo1;
    Servo _servo2;
    byte _pinA, _pinB;
};

#endif
