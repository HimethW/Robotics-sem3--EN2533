/*
  LineFollower.h
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 14 September, 2024
*/

#ifndef LineFollower_h
#define LineFollower_h

#include "Arduino.h"
#include "Robot.h"

void readIRSensors(float *readings); // change to int* if digital sensors are used
float calculateError(float *readings);
float getPID(float error, float *errorIntegral, float *errorDerivative);

#endif
