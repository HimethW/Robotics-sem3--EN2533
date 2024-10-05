/*
  BoxArrange.h
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 13 September, 2024
*/

#ifndef BoxArrange_h
#define BoxArrange_h

#include "Arduino.h"
#include "Robot.h"
#include "IRSensors.h"
#include "LineFollower.h"

void boxToTemp(Robot *robot, LineFollower *lineFollower, byte startPos, byte endPos);
void tempToBox(Robot *robot, LineFollower *lineFollower, byte startPos, byte endPos);
void boxToBox(Robot *robot, LineFollower *lineFollower, byte startPos, byte endPos);

void driveTillJunction(Robot *robot, LineFollower *lineFollower);
void passNJunctions(Robot *robot, LineFollower *lineFollower, byte numJunctions); // check what type abs(x) returns in Arduino

#endif
