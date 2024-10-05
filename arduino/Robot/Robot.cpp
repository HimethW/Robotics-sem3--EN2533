/*
  Robot.cpp
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 14 September, 2024
*/

#include "Arduino.h"
#include "Robot.h"
#include "Wheels.h"

Robot::Robot() {
  spd = 0;
}

void Robot::attachWheels(Wheel_t *leftWheels, byte numLeftWheels, Wheel_t *rightWheels, byte numRightWheels) {
  _leftWheels = (Wheel*)malloc(sizeof(Wheel) * numLeftWheels);
  _numLeftWheels = numLeftWheels;
  for (byte i = 0; i < numLeftWheels; i++) {
    _leftWheels[i] = Wheel(leftWheels[i].controlPinA, leftWheels[i].controlPinB, leftWheels[i].speedPin);
  }
  
  _rightWheels = (Wheel*)malloc(sizeof(Wheel) * numRightWheels);
  _numRightWheels = numRightWheels;
  for (byte i = 0; i < numRightWheels; i++) {
    _rightWheels[i] = Wheel(rightWheels[i].controlPinA, rightWheels[i].controlPinB, rightWheels[i].speedPin);
  }
}

void Robot::setLeftSpeed(float speed) {
  for (byte i = 0; i < _numLeftWheels; i++) {
    _leftWheels[i].setSpeed(speed);
  }
}

void Robot::setRightSpeed(float speed) {
  for (byte i = 0; i < _numRightWheels; i++) {
    _rightWheels[i].setSpeed(speed);
  }
}

void Robot::setBaseSpeed(float speed) {
  spd = speed;
  setRightSpeed(speed);
  setLeftSpeed(speed);
}

void Robot::drive(byte direction) {
  if (direction == FORWARD) {
    for (byte i = 0; i < _numLeftWheels; i++) {
      _leftWheels[i].forward();
    }
    for (byte i = 0; i < _numRightWheels; i++) {
      _rightWheels[i].forward();
    }
  } else if (direction == BACKWARD) {
    for (byte i = 0; i < _numLeftWheels; i++) {
      _leftWheels[i].backward();
    }
    for (byte i = 0; i < _numRightWheels; i++) {
      _rightWheels[i].backward();
    }
  }
}

void Robot::brake() {
  for (byte i = 0; i < _numLeftWheels; i++) {
    _leftWheels[i].brake();
  }
  for (byte i = 0; i < _numRightWheels; i++) {
    _rightWheels[i].brake();
  }
}

void Robot::driveDistance(byte direction, int distance) {
  drive(direction);
  bool condition = true;
  while (condition) {
    condition = false; // keep reading encoders here an updating condition to see if the distance has been moved
  }
  brake();
}

void Robot::turn(int radius, int angle) {
  // don't know how to do this yet. maybe keep turning, reading the compass, to make sure we've turned through the angle
}
