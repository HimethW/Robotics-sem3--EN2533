void lineFollow() {
  // Simply line follow. Take a reading from the IR sensors, calculate the error, the PID signal and give it to the wheels.
  // Keep calling this continuously, to keep the robot following the line.

  // The corner-most sensors are different. Their outputs are significantly different from those of the array, so we treat them separately.
  byte leftmost = 1 - digitalRead(leftmostIR);
  float error = pravindu.getWeightedDigitalReading();
  byte rightmost = 1 - digitalRead(rightmostIR);
  error += (leftmostWeight * leftmost) + (rightmostWeight * rightmost);

  nilakna.drivePID(error);

  // nilakna.drive(FORWARD);
}

void equalizeEncoderCounts() {
  // Asks the robot to maintain its current heading, and go straight ahead, using encoder counts. We give the difference between the encoder ticks as the
  // error signal for PID control. Keep calling this continuously to get it to keep doing this.

  long error = rightWheel.encoderCount - leftWheel.encoderCount;
  nilakna.drivePID(error);

  // nilakna.drive(FORWARD);
}

void lineFollowAndEncoder(int K_line, int K_enc) {
  // Take inputs from both the IR sensors and the encoder ticks to try to make the robot both follow the line and maintain the heading.
  byte leftmost = 1 - digitalRead(leftmostIR);
  float error = pravindu.getWeightedDigitalReading();
  byte rightmost = 1 - digitalRead(rightmostIR);
  error += (leftmostWeight * leftmost) + (rightmostWeight * rightmost);
  error *= K_line;
  error += (rightWheel.encoderCount - leftWheel.encoderCount) * K_enc;

  nilakna.drivePID(error);

  // nilakna.drive(FORWARD);
}

void moveForwardEncoder(long numTicks) {
  // Makes the robot move forward until the encoder ticks a certain number of times. Ignores all other inputs. Function returns when the robot moves the
  // specified number of ticks. Function is blocking.

  leftWheel.resetEncoderCount();
  rightWheel.resetEncoderCount();
  nilakna.setBaseSpeed(80, 80);
  nilakna.drive(FORWARD);
  while (leftWheel.encoderCount < numTicks) {
    equalizeEncoderCounts();
    nilakna.drive(FORWARD);  // maybe we can remove this if we call drive forward inside the equalize function.
  }
}

void straighten(long numTicks) {
  // Keep line following until the encoders tick a certain number of times. Ignore all other inputs. Function returns when the robot moves the specified number
  // of ticks. Function is blocking.

  leftWheel.resetEncoderCount();
  rightWheel.resetEncoderCount();
  nilakna.setBaseSpeed(80, 80);
  nilakna.drive(FORWARD);
  while ((leftWheel.encoderCount < numTicks) || (rightWheel.encoderCount < numTicks)) {
    nilakna.drive(FORWARD);
    lineFollow();
  }
}

byte detectJunction() {
  // Use the cornermost sensors to look for a junction. Return a byte describing the junction found.

  byte leftmost = 1 - digitalRead(leftmostIR);
  byte rightmost = 1 - digitalRead(rightmostIR);

  if ((leftmost == 1) && (rightmost == 0)) {
    return LEFT;
  } else if ((leftmost == 0) && (rightmost == 1)) {
    return RIGHT;
  } else if ((leftmost == 1) && (rightmost == 1)) {
    return BOTH;
  }

  return NONE;
}

void escapeJunc() {
  // Keep moving forward until we escape a junction that we are currently at. If we are not at a junction, the function returns immediately.

  byte j = detectJunction();

  while (j != NONE) {
    nilakna.setBaseSpeed(80, 80);
    nilakna.drive(FORWARD);
    equalizeEncoderCounts();

    j = detectJunction();

    if (j == NONE) {
      break;
    }
  }
}

byte lineFollowTillJunc() {
  // Keep following the line and return when we find a junction. When done, returns the type of junction detected.

  nilakna.setBaseSpeed(80, 80);
  lineFollow();
  nilakna.drive(FORWARD);

  byte j = detectJunction();

  leftWheel.resetEncoderCount();
  rightWheel.resetEncoderCount();  // are these two needed?

  while (j == NONE) {
    nilakna.setBaseSpeed(80, 80);
    lineFollow();

    j = detectJunction();

    if (j != NONE) {
      break;
    }
  }

  return j;
}

byte equalizeEncoderTillJunc() {
  // Keep moving straight ahead until a junction is met. When done, returns the type of the junction detected.

  nilakna.setBaseSpeed(80, 80);
  lineFollow();
  nilakna.drive(FORWARD);

  byte j = detectJunction();

  leftWheel.resetEncoderCount();
  rightWheel.resetEncoderCount();

  while (j == NONE) {
    nilakna.setBaseSpeed(80, 80);
    equalizeEncoderCounts();

    j = detectJunction();

    if (j != NONE) {
      break;
    }
  }

  return j;
}

void turn(byte direction, int numTicks, int rightWheelSpeed, int leftWheelSpeed) {
  // Move forward a bit, brake and pause, and take the turn.

  nilakna.drive(FORWARD);
  moveForwardEncoder(550);  // We may need to change this if we can't go very far forward.

  nilakna.brake();
  delay(2000);

  nilakna.drive(FORWARD);

  leftWheel.resetEncoderCount();
  rightWheel.resetEncoderCount();

  if (direction == RIGHT) {
    while (rightWheel.encoderCount < numTicks) {
      nilakna.setBaseSpeed(rightWheelSpeed, leftWheelSpeed);
      rightWheel.backward();
      leftWheel.forward();
    }
  } else {
    while (leftWheel.encoderCount < numTicks) {
      nilakna.setBaseSpeed(rightWheelSpeed, leftWheelSpeed);
      leftWheel.backward();
      rightWheel.forward();
    }
  }

  nilakna.brake();
  delay(2000);
}

void turnWithoutForward(byte direction, int numTicks, int speed) {
  // Move forward a bit, brake and pause, and take the turn

  nilakna.drive(FORWARD);

  leftWheel.resetEncoderCount();
  rightWheel.resetEncoderCount();

  while (rightWheel.encoderCount < numTicks) {
    nilakna.setBaseSpeed(speed, speed);
    rightWheel.backward();
    leftWheel.forward();
  }

  nilakna.brake();
  delay(2000);
}
