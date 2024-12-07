/*
  PIDController.cpp
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 14 September, 2024
*/

#include "Arduino.h"
#include "PIDController.h"

PIDController::PIDController(float K_P, float K_I, float K_D) {
  _K_P = K_P;
  _K_I = K_I;
  _K_D = K_D;

  _errorIntegral = 0;
  _prevError = 0;
}

float PIDController::getPID(float error) {
  float I = _errorIntegral + error;
  float D = error - _prevError;
  
  _prevError = error;
  _errorIntegral = I;
  
  return (_K_P * error) + (_K_I * I) + (_K_D * D);
}
