/*
  BoxArrange.cpp
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 13 September, 2024
*/

#include "Arduino.h"
#include "Robot.h"
#include "BoxArrange.h"

// It is assumed that the robot starts from one of the box positions, facing the direction that the box 
// would be placed.

void boxToTemp(Robot *robot, byte startPos, byte endPos) {
  robot->turn(0, 180);  // Turn 180 degrees. These functions can be implemented in the Robot class later on.
                        // Maybe create a different function where it uses the IR sensors and the line to know
                        // if it turned 180 deg or something. 
  
  bool junctionMet = false; // Move forward until it reaches the junction
  while (junctionMet) {
    robot->lineFollower.lineFollow();
    junctionMet = robot->lineFollower.junctionMet();
  }

  if (endPos > startPos) {
    robot->turn(0, -90);  // If the end position is ahead of where it is now, turn left
                          // (+ve angles - right, -ve angles left)
  } else {
    robot->turn(0, 90);   // Turn right
  }

  byte junctionsMet = 0;   // Keep moving forward till it passes the relevant number of junctions to reach the
                           // end position
  while (junctionsMet < abs(endPos - startPos)) {
    robot->lineFollower.lineFollow();
    if (robot->lineFollower.junctionMet()) {
      junctionsMet++;
    }
  }

  if (endPos > startPos) {
    robot->turn(0, 90); // If we are approaching a temporary position from a lesser start position, we turn
                        // right
  } else {
    robot->turn(0, -90);   // Turn left
  }

  bool condition = true;  // Some condition to know we reached the end of the line. This could be based on
                          // encoders IG
  while (condition) {
    robot->lineFollower.lineFollow();
    condition = false;
  }
}

void tempToBox(Robot *robot, byte startPos, byte endPos) {
  robot->turn(0, 180);
  
  bool junctionMet = false; // Move forward until it reaches the junction
  while (junctionMet) {
    robot->lineFollower.lineFollow();
    junctionMet = robot->lineFollower.junctionMet();
  }

  if (endPos > startPos) {
    robot->turn(0, 90); // When approaching the junction from the temporary box side, we turn right here.
  } else {
    robot->turn(0, -90);
  }
  
  byte junctionsMet = 0;
  while (junctionsMet < abs(endPos - startPos)) {
    robot->lineFollower.lineFollow();
    if (robot->lineFollower.junctionMet()) {
      junctionsMet++;
    }
  }

  if (endPos > startPos) {
    robot->turn(0, -90); // Turn left here
  } else {
    robot->turn(0, 90); // Turn right
  }

  bool condition = true;  // Some condition to know we reached the end of the line
  while (condition) {
    robot->lineFollower.lineFollow();
    condition = false;
  }
}

void boxToBox(Robot *robot, byte startPos, byte endPos) {
  robot->turn(0, 180);
  
  bool junctionMet = false; // Move forward until it reaches the junction
  while (junctionMet) {
    robot->lineFollower.lineFollow();
    junctionMet = robot->lineFollower.junctionMet();
  }

  if (endPos > startPos) {
    robot->turn(0, -90);
  } else {
    robot->turn(0, 90);
  }
  
  byte junctionsMet = 0;
  while (junctionsMet < abs(endPos - startPos)) {
    robot->lineFollower.lineFollow();
    if (robot->lineFollower.junctionMet()) {
      junctionsMet++;
    }
  }

  if (endPos > startPos) {
    robot->turn(0, -90);
  } else {
    robot->turn(0, 90);
  }

  bool condition = true;  // Some condition to know we reached the end of the line
  while (condition) {
    robot->lineFollower.lineFollow();
    condition = false;
  }
}
