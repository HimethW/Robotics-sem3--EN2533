/*
  LineFollower.h
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 14 September, 2024
*/

#ifndef LineFollower_h
#define LineFollower_h

#include "Arduino.h"
#include "Robot.h"
#include "IRSensors.h"

class LineFollower {
  public:
    LineFollower(Robot *robot, IRSensorArray *irSensorArray);
    void setPIDConstants(float K_P, float K_I, float K_D);
    void lineFollow();

  private:
    Robot *_robot;

    float _K_P, _K_I, _K_D;
    float _prevError;
    float _errorIntegral;

    IRSensorArray _irSensorArray;

    float _getPID(float error);
};

#endif
