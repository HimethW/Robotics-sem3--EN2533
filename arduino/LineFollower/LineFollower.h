/*
  LineFollower.h
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 14 September, 2024
*/

#ifndef LineFollower_h
#define LineFollower_h

#include "Arduino.h"
#include "Robot.h"

class LineFollower {
  public:
    LineFollower(Robot *robot, byte numIRSensors);
    void setPIDConstants(float K_P, float K_I, float K_D);
    void lineFollow();
    bool junctionDetected();
  private:
    Robot *_robot;

    float _K_P, _K_I, _K_D;
    float _prevError;
    float _errorIntegral;

    byte _numIRSensors;
    float *_readings;

    void _readIRSensors();
    float _calculateError();
    float _getPID(float error);
};

#endif
