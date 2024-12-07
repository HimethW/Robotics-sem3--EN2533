#ifndef Gripper_h
#define Gripper_h

#include "Arduino.h"
#include "Servo.h"

class Gripper {
    public:
    Gripper (byte controlPinA, byte controlPinB);

    void lift (byte stopAngle); //For 0 < angles <= 90
    void lower (byte stopAngle); //For 0 < angles <= 90
    void grab ();
    void release ();

    private:
    Servo _servo1;
    Servo _servo2;
};

#endif
