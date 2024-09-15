/*
  BoxArrange.cpp
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 13 September, 2024
*/

#include "Arduino.h"
#include "Robot.h"
#include "LineFollower.h"
#include "BoxArrange.h"

// It is assumed that the robot starts from one of the box positions, facing the direction that the box 
// would be placed.

void driveTillJunction(Robot *robot, LineFollower *lineFollower) {
  while (lineFollower->junctionMet()) {
    lineFollower->lineFollow();
  }
  robot->brake();
}

void passNJunctions(Robot *robot, LineFollower *lineFollower, byte numJunctions) {
  byte junctionsMet = 0;   // Keep moving forward till it passes the relevant number of junctions to reach                              // the end position
  while (junctionsMet < numJunctions) {
    lineFollower->lineFollow();
    if (lineFollower->junctionMet()) {
      junctionsMet++;
    }
  }
  robot->brake();
}

void boxToTemp(Robot *robot, LineFollower *lineFollower, byte startPos, byte endPos) {
  robot->turn(0, 180);  // Turn 180 degrees.
                        // Maybe create a different function where it uses the IR sensors and the line to
                        // know if it turned 180 deg or something.
  
  driveTillJunction(lineFollower);
  
  if (endPos > startPos) {
    robot->turn(0, -90);  // If the end position is ahead of where it is now, turn left
                          // (+ve angles - right, -ve angles left)
  } else {
    robot->turn(0, 90);   // Turn right
  }

  passNJunctions(lineFollower, abs(endPos - startPos));

  if (endPos > startPos) {
    robot->turn(0, 90); // If we are approaching a temporary position from a lesser start position, we turn
                        // right
  } else {
    robot->turn(0, -90);   // Turn left
  }

  bool condition = true;  // Some condition to know we reached the end of the line. This could be based on
                          // encoders IG
  while (condition) {
    lineFollower->lineFollow();
    condition = false;
  }
}

void tempToBox(Robot *robot, LineFollower *lineFollower, byte startPos, byte endPos) {
  robot->turn(0, 180);
  
  driveTillJunction(lineFollower);

  if (endPos != startPos) {
    if (endPos > startPos) {
      robot->turn(0, 90); // When approaching the junction from the temporary box side, we turn right here.
    } else {
      robot->turn(0, -90);
    }
    
    passNJunctions(lineFollower, abs(endPos - startPos));
  
    if (endPos > startPos) {
      robot->turn(0, -90); // Turn left here
    } else {
      robot->turn(0, 90); // Turn right
    }
  } // If endPos == startPos, then you just drive straight ahead.

  bool condition = true;  // Some condition to know we reached the end of the line
  while (condition) {
    lineFollower->lineFollow();
    condition = false;
  }
}

void boxToBox(Robot *robot, LineFollower *lineFollower, byte startPos, byte endPos) {
  robot->turn(0, 180);
  
  driveTillJunction(lineFollower);

  if (endPos > startPos) {
    robot->turn(0, -90);
  } else {
    robot->turn(0, 90);
  }
  
  passNJunctions(lineFollower, abs(endPos - startPos));

  if (endPos > startPos) {
    robot->turn(0, -90);
  } else {
    robot->turn(0, 90);
  }

  bool condition = true;  // Some condition to know we reached the end of the line
  while (condition) {
    lineFollower->lineFollow();
    condition = false;
  }
}
