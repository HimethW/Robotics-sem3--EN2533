#include <Robot.h>
#include <Wheels.h>
#include <Gripper.h>
#include <IRSensors.h>
#include <LineFollower.h>
#include <BoxArrange.h>

#define ASCENDING 0
#define DESCENDING 1

const Wheel_t rearLeft = {1, 2, 3}; // controlPinA, controlPinB, speedPin
const Wheel_t forwardLeft = {4, 5, 6};
const Wheel_t forwardRight = {7, 8, 9};
const Wheel_t rearRight = {10, 11, 12};

const Wheel_t leftWheels[2] = {rearLeft, forwardLeft};
const Wheel_t rightWheels[2] = {rearRight, forwardRight};

const byte numIRSensors = 6;
const byte IRSensorPins[numIRSensors] = {13, 14, 15, 16, 17, 18}; // PWM pins

Robot nilakna; // LOL
Gripper amoda; // LOL
IRSensorArray pravindu(IRSensorPins, numIRSensors); // LOL

LineFollower lineFollower(&nilakna, &pravindu);

const int shortBoxHeight = 1;
const int midBoxHeight = 2;
const int tallBoxHeight = 3;
const int boxHeightTolerance = 1;

void setup() {
  nilakna.attachWheels(leftWheels, 2, rightWheels, 2);
  nilakna.setBaseSpeed(1.0);

  lineFollower.setPIDConstants(1, 0, 1);
}

void loop() {
  byte arrangeOrder = ASCENDING; // This variable should be set based on whatever happens above

  int expectedBoxHeightAt[3] = {shortBoxHeight, midBoxHeight, tallBoxHeight};
  if (arrangeOrder == DESCENDING) {
    expectedBoxHeightAt[0] = tallBoxHeight;
    expectedBoxHeightAt[3] = shortBoxHeight;
  }

  int boxPlacements = 0b0000111; // The first three digits represent the temporary locations. First of the three denotes                                      // position index 2. The next three digits represent the actual box locations, again the                                     // first one denoting position index 2. 1 denotes the presence of a box and 0 denotes the                                    // absence of a box. The middle bit is always a 0, separating the first and last set of                                      // three digits.
  
  // Assuming we start at the place to arrange the boxes
  
  driveTillJunction(&nilakna, &lineFollower);  // Drive to the junction for the first box
  nilakna.turn(0, -90);  // Turn left towards the box
  while (closeEnoughToBox) { // implement closeEnoughToBox with sensors
    lineFollower.lineFollow();
  }
  nilakna.brake();
  // We are now right in front of the first box, facing it, i.e., position index = 0
  
  int currentPos = 0b0000001; // The current location of the robot is indicated using the same binary format described                                     // above, except a 1 is placed at the location that the robot is at. This time the 1s and 0s                                 // have nothing to do with boxes.
  while (1) {
    int boxHeight = 0; // Use sensors to find the box height here
    
    if (withinTolerance(boxHeight, expectedBoxHeightAt[(currentPos >> 1) & 0b111], boxHeightTolerance)) {
      // The box here is the correct box. Consider the next position
      if (currentPos << 1 == 0b0000100) { // If the next position you are being asked to go to is the last position, don't.
        break;
      }
      
      // If not, go to the next position, and start over.
      boxToBox(&nilakna, &lineFollower, (currentPos >> 1) & 0b111, currentPos & 0b111);
      currentPos = currentPos << 1;
      continue;
    } else {
      // The box here is not the correct box.
      int nextPos = 0b0000001; // Assume that it must be taken to position index 0.
      
      if (withinTolerance(boxHeight, expectedBoxHeightAt[1], boxHeightTolerance)) {
        // If instead the box is actually the one that is expected to be at 1, update nextPos accordingly
        nextPos = 0b0000010;
      } else if (withinTolerance(boxHeight, expectedBoxHeightAt[2], boxHeightTolerance)) {
        // Same as above. There must be a 'nicer' way of doing this without using so many ifs, but it's only two ifs here, so
        // it's probably alright.
        nextPos = 0b0000100;
      }

      if (boxPlacements & nextPos) {
        // Above is a bitwise and, which 'filters' out what boxPlacements has at the position given by nextPos. If that is 1,
        // there is a box there, and the above evaluates to true. Hence, we take the current box to the temporary location
        // at nextPos.
        amoda.grab();
        boxPlacements = boxPlacements & (~currentPos & 0b1111111); // Put a zero in boxPlacements in the place corresponding                                                                    // to currentPos, because the box is being taken away                                                                        // from there.
        boxToTemp(&nilakna, &lineFollower, (currentPos >> 1) & 0b111, (nextPos >> 1) & 0b111);
        amoda.release();
        boxPlacements = boxPlacements | (nextPos << 4); // Put a 1 in boxPlacements in the temporary location side at the                                                            // appropriate location, because the box we are taking ends up                                                               // there.
        // Now go to the box at the same position.
        tempToBox(&nilakna, &lineFollower, (nextPos >> 1) & 0b111), (nextPos >> 1) & 0b111));
        currentPos = nextPos;
      } else {
        // In this case, the destination is empty, and the box can actually be taken there.
        amoda.grab();
        boxPlacements = boxPlacements & (~currentPos & 0b1111111);
        boxToBox(&nilakna, &lineFollower, (currentPos >> 1) & 0b111, (nextPos >> 1) & 0b111);
        amoda.release();
        boxPlacements = boxPlacements | nextPos;
        currentPos = nextPos;

        // By now, all boxes must be in order, except some may be in the temporary locations. All that remains is to check
        // boxPlacements to see at which temporary positions there still are boxes, and move them to the corresponding
        // destinations, and we will be done.

        if (boxPlacements & 0b0100000) {
          boxToTemp(&nilakna, &lineFollower, (currentPos >> 1) & 0b111, 1);
          amoda.grab();
          tempToBox(&nilakna, &lineFollower, 1, 1);
          amoda.release();
          currentPos = 0b0000010;
        }

        if (boxPlacements & 0b1000000) {
          boxToTemp(&nilakna, &lineFollower, (currentPos >> 1) & 0b111, 2);
          amoda.grab();
          tempToBox(&nilakna, &lineFollower, 2, 2);
          amoda.release();
          currentPos = 0b0000100;
        }

        // Make the robot come to a particular place now, if necessary; maybe to the junction at position index 2, and face
        // forward, for the next task
        break;
      }
    }
  }

  // Rest of the tasks can continue
}

bool withinTolerance(int value, int baseValue, int tolerance) {
  return ((value <= baseValue + tolerance) && (value >= baseValue - tolerance));
}
