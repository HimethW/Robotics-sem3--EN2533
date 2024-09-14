/*
  LineFollower.cpp
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 14 September, 2024
*/

#include "Arduino.h"
#include "Robot.h"
#include "LineFollower.h"

LineFollower::LineFollower(Robot *robot, byte numIRSensors) {
  _robot = robot;
  _numIRSensors = numIRSensors;
  _readings = (float*)malloc(sizeof(float) * numIRSensors);
}

LineFollower::setPIDConstants(float K_P, float K_I, float K_D) {
  _K_P = K_P;
  _K_I = K_I;
  _K_D = K_D;
}

void LineFollower::_readIRSensors() {
  // Write code here to read the IR sensors
  // I am expecting there to be some library that can do this, or we can write one, or we can even hardcode IG, idk.
  // Somehow, _readings must be set to the values read
  for (byte i = 0; i < _numIRSensors; i++) {
    _readings[i] = 0;
  }
}

float LineFollower::_calculateError() {
  float error = 0;
  for (byte i = 0; i < _numIRSensors; i++) {
    error += _readings[i] * (i - (_numIRSensors / 2));  // or define an array with custom weights for each sensor, or implement a                                                                 // suitable way of calculating an error signal based on the sensor readings
  }
  return error;
}

float LineFollower::_getPID(float error) {
  float I = _errorIntegral + error;
  float D = error - _prevError;
  
  _prevError = error;
  _errorIntegral = I;
  
  return (_K_P * error) + (_K_I * I) + (_K_D * D);
}

bool LineFollower::junctionDetected() {
  for (byte i = 0; i < _numIRSensors; i++) {    // This is just checking if all sensors detect the line. May need improving.
    if (_readings[i] < 0.5) {                   // Decide a threshold or something maybe idk. consider defining 0.5 or whatever as                                                        // variable.
      return false;
    }
  }
  return true;
}

void LineFollower::lineFollow() {
  _readIRSensors();
  float error = _calculateError();
  float PID = _getPID(error);

  float k = 1.0;
  
  _robot->setRightSpeed(_robot->_speed + PID * k);
  _robot->setLeftSpeed(_robot->_speed - PID * k);
}
