/*
  BoxArrange.h
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 13 September, 2024
*/

#ifndef BoxArrange_h
#define BoxArrange_h

#include "Arduino.h"
#include "Robot.h"
#include "LineFollower.h"

void boxToTemp(Robot *robot, byte startPos, byte endPos);
void tempToBox(Robot *robot, byte startPos, byte endPos);
void boxToBox(Robot *robot, byte startPos, byte endPos);

#endif
