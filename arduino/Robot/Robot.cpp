/*
  Robot.cpp
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 14 September, 2024
*/

#include "Arduino.h"
#include "Robot.h"
#include "Wheels.h"
#include "IRSensors.h"
#include "PIDController.h"

Robot::Robot() {
  rightBaseSpeed = 0;
  leftBaseSpeed = 0;
}

void Robot::initPIDController(float K_P, float K_I, float K_D) {
  pidController = new PIDController(K_P, K_I, K_D);
}

// Work on these methods. We are not using them for this code so no worries, but work on them. 
// This type casting thing, and whether we are gonna store pointers etc.
void Robot::attachWheels(Wheel *leftWheels, byte numLeftWheels, Wheel *rightWheels, byte numRightWheels) {
  _leftWheels = (Wheel*)malloc(sizeof(Wheel) * numLeftWheels);
  _numLeftWheels = numLeftWheels;
  for (byte i = 0; i < numLeftWheels; i++) {
    _leftWheels[i] = leftWheels[i];
  }
  
  _rightWheels = (Wheel*)malloc(sizeof(Wheel) * numRightWheels);
  _numRightWheels = numRightWheels;
  for (byte i = 0; i < numRightWheels; i++) {
    _rightWheels[i] = rightWheels[i];
  }
}

void Robot::attachWheels(Wheel *leftWheel, Wheel *rightWheel) {
  _numLeftWheels = 1;
  _leftWheels = (Wheel*)malloc(sizeof(Wheel) * _numLeftWheels);
  _leftWheels[0] = *leftWheel;
  _numRightWheels = 1;
  _rightWheels = (Wheel*)malloc(sizeof(Wheel) * _numRightWheels);
  _rightWheels[0] = *rightWheel;
}

void Robot::setLeftSpeed(float speed) {
  if (speed < _minSpeed) {
    speed = _minSpeed;
  }
  if (speed > _maxSpeed) {
    speed = _maxSpeed;
  }
  for (byte i = 0; i < _numLeftWheels; i++) {
    _leftWheels[i].setSpeed(speed);
  }
}

void Robot::setRightSpeed(float speed) {
  if (speed < _minSpeed) {
    speed = _minSpeed;
  }
  if (speed > _maxSpeed) {
    speed = _maxSpeed;
  }
  for (byte i = 0; i < _numRightWheels; i++) {
    _rightWheels[i].setSpeed(speed);
  }
}

void Robot::setMinMaxSpeeds(float minSpeed, float maxSpeed) {
  _minSpeed = minSpeed;
  _maxSpeed = maxSpeed;
}

void Robot::setBaseSpeed(float right, float left) {
  rightBaseSpeed = right;
  leftBaseSpeed = left;
  setRightSpeed(right);
  setLeftSpeed(left);
}

void Robot::drivePID(float error) {
  float PID = pidController->getPID(error);

  if (_driveDirection == FORWARD) {
    setRightSpeed(rightBaseSpeed - PID);
    setLeftSpeed(leftBaseSpeed + PID);
    /*Serial.print("ERROR IS ");
    Serial.print(error);
    Serial.print(" WRITING ");
    Serial.print(leftBaseSpeed + PID);
    Serial.print(" AND ");
    Serial.println(rightBaseSpeed - PID);*/
  } else {
    setRightSpeed(rightBaseSpeed - PID); // I hope I got this right.
    setLeftSpeed(leftBaseSpeed + PID);
  }
}

// should we keep encoders disabled when different speeds? or reset them, if distance moved is what we are after?

void Robot::drive(byte direction) {
  _driveDirection = direction;

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

  int distanceMoved = 0;

  int rightStartEncoderCounts[_numRightWheels];
  int leftStartEncoderCounts[_numLeftWheels];

  for (byte i = 0; i < _numRightWheels; i++) {
    rightStartEncoderCounts[i] = _rightWheels[i].encoderCount;
  }

  for (byte i = 0; i < _numLeftWheels; i++) {
    leftStartEncoderCounts[i] = _leftWheels[i].encoderCount;
  }
  
  while (distanceMoved < distance) {
    int rightWheelDistance = 0;
    for (byte i = 0; i < _numRightWheels; i++) {
      rightWheelDistance += _rightWheels[i].encoderCount - rightStartEncoderCounts[i];
    }
    rightWheelDistance /= _numRightWheels;

    int leftWheelDistance = 0;
    for (byte i = 0; i < _numLeftWheels; i++) {
      leftWheelDistance += _leftWheels[i].encoderCount - leftStartEncoderCounts[i];
    }
    leftWheelDistance /= _numLeftWheels;

    distanceMoved = (leftWheelDistance + rightWheelDistance) / 2;
    Serial.println(distanceMoved);
  }

  brake();
}

void Robot::turn(int radius, int angle) {
  // don't know how to do this yet. maybe keep turning, reading the compass, to make sure we've turned through the angle
}
