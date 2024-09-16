#include <Robot.h>
#include <Wheels.h>
#include <Gripper.h>
#include <LineFollower.h>
#include <BoxArrange.h>

#define ASCENDING 0
#define DESCENDING 1

Robot nilakna(); // LOL
Gripper amoda(); // LOL

Wheel_t rearLeft = {1, 2, 3}; // controlPinA, controlPinB, speedPin
Wheel_t forwardLeft = {4, 5, 6};
Wheel_t forwardRight = {7, 8, 9};
Wheel_t rearRight = {10, 11, 12};

LineFollower lineFollower(&nilakna, 8);

int shortBoxHeight = 1;
int midBoxHeight = 2;
int tallBoxHeight = 3;
int boxHeightTolerance = 1;

void setup() {
  nilakna.attachWheels(rearLeft, forwardLeft, forwardRight, rearRight);
  nilakna.attachGripper(&amoda); // maybe we dont need to attach the gripper to the robot and it could                                         // function on its own. doesn't matter either way, trying to kinda stick
                                 // OOP concepts
  nilakna.setBaseSpeed(1.0);

  lineFollower.setPIDConstants(1, 0, 1);
}

void loop() {
  byte arrangeOrder = ASCENDING; // This variable should be set based on whatever happens above
  // Assuming we start at the place to arrange the boxes
  
  driveTillJunction(&nilakna, &lineFollower);  // Drive to the junction for the first box
  nilakna.turn(0, -90);  // Turn left towards the box
  while (closeEnoughToBox) { // implement closeEnoughToBox with sensors
    lineFollower.lineFollow();
  }
  nilakna.brake();

  // We are now right in front of the first box, facing it, i.e., position index = 0
  int pos0BoxHeight = 0;  // Measure its height
  boxToBox(&nilakna, &lineFollower, 0, 1); // Go to the next position
  int pos1BoxHeight = 1; // Measure the height of the box there

  if (withinTolerance(pos1BoxHeight, midBoxHeight, boxHeightTolerance)) { // Mid-height box is in the right                                                                             // place
    if (
        ((arrangeOrder == ASCENDING) && withinTolerance(pos0BoxHeight, tallBoxHeight, boxHeightTolerance))
        ||
        ((arrangeOrder == DESCENDING) && withinTolerance(pos0BoxHeight, shortBoxHeight, boxHeightTolerance))
    ) {
      // We need only swap the box at position index 0 with what is at position index 2
      boxToBox(&nilakna, &lineFollower, 1, 0); // Go back to position index 0
      amoda.grip(); // Pick up the box there
      boxToTemp(&nilanka, &lineFollower, 0, 0); // Take it to the temporary location at the same position
      amoda.release(); // Drop the box there
      tempToBox(&nilakna, &lineFollower, 0, 2); // Go to the very end now
      amoda.grip(); // Pick up the box there
      boxToBox(&nilakna, &lineFollower, 2, 0); // Take it all the way to the first position
      amoda.release(); // Drop it there
      boxToTemp(&nilakna, &lineFollower, 0, 0); // Go to the temporary location that now holds the box                                                        // originally at position index 0
      amoda.grip();
      tempToBox(&nilakna, &lineFollower, 0, 2);
      amoda.release();
      // We are done!
    }
    // If the above if statement does not evaluate to true, then the boxes are already arranged. We do not        // need to do anything
  } else if (withinTolerance(pos1BoxHeight, shortBoxHeight, boxHeightTolerance)) {
    // The shortest box is in the middle.
    // ahh i realize im basically hard coding everything. kinda have another idea, ill think about it a     // little and implement it 
  }
}

bool withinTolerance(int value, int baseValue, int tolerance) {
  return ((value <= baseValue + tolerance) && (value >= baseValue - tolerance));
}
