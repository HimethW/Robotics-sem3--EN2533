/*
  Robot.h
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 14 September, 2024
*/

#ifndef Robot_h
#define Robot_h

#include "Arduino.h"
#include "Wheels.h"

#define FORWARD 0
#define BACKWARD 1

class Robot {
  public:
    Robot();
    void attachWheels(Wheel_t *leftWheels, byte numLeftWheels, Wheel_t *rightWheels, byte numRightWheels);

    void setBaseSpeed(float speed);
    void setRightSpeed(float speed);
    void setLeftSpeed(float speed);
    
    void drive(byte direction);
    void driveDistance(byte direction, int distance);
    void brake();
    void turn(int radius, int angle); // int or float? will it even be possible to get it to turn with a given radius?

    void attachGripper(Gripper *gripper);
  private:
    Wheel *_leftWheels;
    Wheel *_rightWheels;
    byte _numLeftWheels, _numRightWheels;
    float _speed;
};

#endif
