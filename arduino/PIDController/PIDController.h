/*
  PIDController.h
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 14 September, 2024
*/

#ifndef PIDController_h
#define PIDController_h

#include "Arduino.h"

class PIDController {
  public:
    PIDController(float K_P, float K_I, float K_D);
    float getPID(float error);

  private:
    float _K_P, _K_I, _K_D;
    float _prevError;
    float _errorIntegral;
};

#endif
