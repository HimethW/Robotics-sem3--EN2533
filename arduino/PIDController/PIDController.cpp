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

void PIDController::reset() {
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

void PIDController::getExisting(float *targetArray) {
  targetArray[0] = _K_P;
  targetArray[1] = _K_I;
  targetArray[2] = _K_D;
  targetArray[3] = _prevError;
  targetArray[4] = _errorIntegral;
}

void PIDController::restoreTo(float *array) {
  _K_P = array[0];
  _K_I = array[1];
  _K_D = array[2];
  _prevError = array[3];
  _errorIntegral = array[4];
}

void PIDController::updateConstants(float K_P, float K_I, float K_D) {
  _K_P = K_P;
  _K_I = K_I;
  _K_D = K_D;
}
