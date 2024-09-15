#include <Robot.h>
#include <Wheels.h>
#include <Gripper.h>
#include <LineFollower.h>
#include <BoxArrange.h>

Robot nilakna(); // LOL
Gripper amoda(); // LOL

Wheel_t rearLeft = {1, 2, 3}; // controlPinA, controlPinB, speedPin
Wheel_t forwardLeft = {4, 5, 6};
Wheel_t forwardRight = {7, 8, 9};
Wheel_t rearRight = {10, 11, 12};

LineFollower lineFollower(&nilakna, 8);

void setup() {
  nilakna.attachWheels(rearLeft, forwardLeft, forwardRight, rearRight);
  nilakna.attachGripper(&amoda); // maybe we dont need to attach the gripper to the robot and it could                                         // function on its own. doesn't matter either way, trying to kinda stick
                                 // OOP concepts
  nilakna.setBaseSpeed(1.0);

  lineFollower.setPIDConstants(1, 0, 1);
}

void loop() {
  // Assuming we start at the place to arrange the boxes
  
  driveTillJunction(&nilakna, &lineFollower);  // Drive to the junction for the first box
  nilakna.turn(0, -90);  // Turn left towards the box
  while (closeEnoughToBox) { // implement closeEnoughToBox with sensors
    lineFollower.lineFollow();
  }
  nilakna.brake();

  // We are now right in front of the first box, facing it, i.e., position index = 0
  int height = 0;  // Measure its height
  if (height < 0) {  // Suppose this is true if the smallest box _is_ here.
                     // Change this so that if we are supposed to arrange in the descending order, here we
                     // check for the tallest box. In either case, we leave this box as is.
    boxToBox(&nilakna, &lineFollower, 0, 1); // Do nothing and go to the next box.

    height = 0.5; // Measure the height here again.
    if (height < 2) { // Suppose this means that the tallest box is here.
      // In this case we need only swap this box and the one in front, and we are done.
      amoda.grip();
      boxToTemp(&nilakna, &lineFollower, 1, 2);
      amoda.release();
      tempToBox(&nilakna, &lineFollower, 2, 2);
      amoda.grip();
      boxToBox(&nilakna, &lineFollower, 2, 1);
      amoda.release();
      boxToTemp(&nilakna, &lineFollower, 1, 2);
      amoda.grip();
      tempToBox(&nilakna, &lineFollower, 2, 2);
      amoda.release();
      // We are done!
    } // If not, the mid-height box is here and not arrangement is necessary. Leave the box thing here.
      // No need to check the third box.
  } else {
    // The box already here is not the one that is supposed to be here.
    byte tempEndPos = 1;
    if (height < 2) { // Suppose this being true means that the box here is the tallest box
                      // (i.e., if we are arranging in the ascending order; otherwise this being true means
                      // that this is the shortest box)
      tempEndPos = 2; // In each case, we temporarily place this box at position index = 2
    }
    // If the above if condition were not true, the box here must be the mid-height box and must be taken
    // to position index = 1, and tempEndPos is 1 by default.
    amoda.grip(); // grab the box
    boxToTemp(&nilakna, &lineFollower, 0, tempEndPos);
    amoda.release();
    tempToBox(&nilakna, &lineFollower, tempEndPos, tempEndPos);

    height = 0.5; // Measure the height here again
    
  }
}
