/*
  Robot.h
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 14 September, 2024
*/

#ifndef Robot_h
#define Robot_h

#include "Arduino.h"
#include "Wheels.h"
#include "IRSensors.h"
#include "PIDController.h"

#define FORWARD 0
#define BACKWARD 1

class Robot {
  public:
    Robot();
    float rightBaseSpeed, leftBaseSpeed; // why are these public? i probably needed to access these from outside...idr
    PIDController *pidController; // should this be public also?

    void attachWheels(Wheel *leftWheels, byte numLeftWheels, Wheel *rightWheels, byte numRightWheels);
    void attachWheels(Wheel *leftWheel, Wheel *rightWheel);

    void initPIDController(float K_P, float K_I, float K_D);

    void setBaseSpeed(float right, float left);
    void setRightSpeed(float speed);
    void setLeftSpeed(float speed);
    void setMinMaxSpeeds(float minSpeed, float maxSpeed);
    
    void drivePID(float error);
    void drive(byte direction);
    void driveDistance(byte direction, int distance);
    void brake();
    void turn(int radius, int angle); // int or float? will it even be possible to get it to turn with a given radius?

  private:
    Wheel *_leftWheels;
    Wheel *_rightWheels;
    byte _numLeftWheels, _numRightWheels;
    int _minSpeed, _maxSpeed;
    byte _driveDirection;
};

#endif
