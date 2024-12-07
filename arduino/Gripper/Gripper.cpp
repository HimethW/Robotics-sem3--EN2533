#include "Arduino.h"
#include "Servo.h"
#include "Gripper.h"

Gripper::Gripper (byte controlPinA, byte controlPinB) {
    _servo1.attach (controlPinA);
    _servo2.attach (controlPinB);
}

void Gripper::lift (byte stopAngle) {
    _servo1.write(stopAngle);
}

void Gripper::lower (byte stopAngle) {
    _servo1.write(stopAngle);
}

void Gripper::grab () {
    _servo2.write(160);
}

void Gripper::release () {
    _servo2.write(75);
}
