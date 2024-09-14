/*
  Robot.h
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 14 September, 2024
*/

#ifndef Robot_h
#define Robot_h

#include "Arduino.h"
#include "Wheels.h"

class Robot {
  public:
    Robot();
    void attachWheels(Wheel_t *leftWheels, byte numLeftWheels, Wheel_t *rightWheels, byte numRightWheels);

    void setBaseSpeed(float speed);
    void setRightSpeed(float speed);
    void setLeftSpeed(float speed);
    
    void drive(byte direction);
    void brake();
    void stop();

    void attachGripper(Gripper *gripper);
  private:
    Wheel *_leftWheels;
    Wheel *_rightWheels;
    float _speed;
};

#endif
