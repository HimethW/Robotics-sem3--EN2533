#include "Arduino.h"
#include "Servo.h"
#include "Gripper.h"

Gripper::Gripper (byte controlPinA, byte controlPinB) {
    _pinA = controlPinA;
    _pinB = controlPinB;
}

void Gripper::initialize() {
    _servo1.attach (_pinA);
    _servo2.attach (_pinB);
}

void Gripper::lift (byte stopAngle) {
    _servo1.write(stopAngle);
}

void Gripper::lower () {
    _servo1.write(0);
}

void Gripper::grab () {
    _servo2.write(160);
}

void Gripper::release () {
    _servo2.write(75);
}

void Gripper::push () {
    _servo2.write(170);
}
