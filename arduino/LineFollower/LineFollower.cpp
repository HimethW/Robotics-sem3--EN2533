/*
  LineFollower.cpp
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 14 September, 2024
*/

#include "Arduino.h"
#include "Robot.h"
#include "IRSensors.h"
#include "LineFollower.h"

LineFollower::LineFollower(Robot *robot, IRSensorArray *irSensorArray) {
  _robot = robot;
  _irSensorArray = irSensorArray;
}

LineFollower::setPIDConstants(float K_P, float K_I, float K_D) {
  _K_P = K_P;
  _K_I = K_I;
  _K_D = K_D;
}

float LineFollower::_getPID(float error) {
  float I = _errorIntegral + error;
  float D = error - _prevError;
  
  _prevError = error;
  _errorIntegral = I;
  
  return (_K_P * error) + (_K_I * I) + (_K_D * D);
}

void LineFollower::lineFollow() {
  _readIRSensors();
  float error = _irSensorArray->getWeightedAnalogReading();
  float PID = _getPID(error);

  float k = 1.0;
  
  _robot->setRightSpeed(_robot->spd + PID * k);
  _robot->setLeftSpeed(_robot->spd - PID * k);
}
