void f() {
  // Maintain existing heading till we escape the junction if we are on one, or do nothing if we are not on a junction. Then straighten, so we are now
  // aligned with the line. Then go straight till we find a junction.

  nilakna.drive(FORWARD);
  escapeJunc();
  straighten(300);
  equalizeEncoderTillJunc();
}

void reverse(float ticks) {
  float revEnc = leftWheel.encoderCount;
  nilakna.setBaseSpeed(80, 80);
  nilakna.drive(BACKWARD);
  while (leftWheel.encoderCount < revEnc + ticks) {
    byte leftmost = 1 - digitalRead(leftmostIR);
    byte rightmost = 1 - digitalRead(rightmostIR);
    float error = pravindu.getWeightedDigitalReading();
    error += (leftmostWeight * leftmost) + (rightmostWeight * rightmost);
    error *= K_line;
    error += (rightWheel.encoderCount - leftWheel.encoderCount) * K_enc;
    nilakna.drivePID(error);
  }
}

void b() {  // reverse
  ;
}

void pickup() {
  // Turns on an LED to indicate picking up the virtual box.

  ;
}

void drop() {
  // Turns off the LED to indicate dropping the virtual box.

  ;
}

void moveBox(byte barcodeResult) {
  // If the box is at position 0, pick it up and place it in front. Then reach the start of the first gate. Otherwise, reach the same position by going straight
  // ahead.

  if (barcodeResult == 0) {
    pickup();
    f();
    drop();
    b();
    turn(RIGHT, 530);
    f();
    turn(LEFT, 500);
    f();
  } else {
    f();
    turn(RIGHT, 530);
    f();
    turn(LEFT, 500);
  }
}

byte findGate() {
  // Find which gate is open by moving from the first to the second. This ends with the robot at the position of the second gate.
  byte gatePosition;
  
  // Code to check if the first gate is open. If open, set
  gatePosition = 1;

  turn(RIGHT, 530);
  f();
  f();
  turn(LEFT, 500);

  // Code to check if the second gate is open. If open, set
  gatePosition = 2;

  return gatePosition;
}

void solveMaze(byte barcodeResult, byte gatePosition) {
  switch (barcodeResult) {
    case 0:
      func0(gatePosition);
      break;
    case 1:
      func1(gatePosition);
      break;
    case 2:
      func2(gatePosition);
      break;
    case 3:
      func3(gatePosition);
      break;
    case 4:
      func4(gatePosition);
      break;
  }
}

void func0(byte gatePosition) {
  turn(LEFT, 500);
  f();
  f();
  f();
  pickup();
  b();
  if (gatePosition == 1) {
    drop();
    b();
    turn(LEFT, 500);
    f();
    turn(RIGHT, 530);
    f();
    turn(RIGHT, 530);
    f();
    pickup();
    f();
  } else {
    b();
    b();
    drop();
    b();
    turn(LEFT, 500);
    f();
    turn(RIGHT, 530);
    f();
    turn(RIGHT, 530);
    f();
    pickup();
    f();
  }
}

void func1(byte gatePosition) {
  if (gatePosition == 1) {
    turn(LEFT, 500);
    f();
    f();
    turn(LEFT, 500);
    f();
    pickup();
    b();
    b();
  } else {
    b();
    turn(LEFT, 500);
    f();
    f();
    pickup();
    b();
    b();
    drop();
    b();
    turn(RIGHT, 530);
    f();
    turn(LEFT, 500);
    f();
    turn(LEFT, 500);
    f();
    pickup();
    b();
    b();
  }
}

void func2(byte gatePosition) {
  b();
  turn(LEFT, 500);
  f();
  pickup();
  if (gatePosition == 1) {
    f();
    drop();
    b();
    turn(RIGHT, 530);
    f();
    turn(LEFT, 500);
    f();
    turn(LEFT, 500);
    f();
    pickup();
    b();
    b();
  } else {
    b();
    drop();
    b();
    turn(RIGHT, 530);
    f();
    turn(LEFT, 500);
    f();
    turn(LEFT, 500);
    f();
    pickup();
    b();
    b();
  }
}

void func3(byte gatePosition) {
  if (gatePosition == 1) {
    turn(RIGHT, 530);
    f();
    turn(RIGHT, 530);
    f();
    turn(RIGHT, 530);
    f();
    pickup();
    f();
    f();
    drop();
    b();
    turn(RIGHT, 530);
    f();
    turn(LEFT, 500);
    f();
    turn(LEFT, 500);
    f();
    pickup();
    b();
    b();
  } else {
    turn(RIGHT, 530);
    turn(RIGHT, 530);
    f();
    pickup();
    b();
    b();
  }
}

void func4(byte gatePosition) {
  turn(RIGHT, 530);
  turn(RIGHT, 530);
  f();
  turn(LEFT, 500);
  f();
  pickup();
  b();
  if (gatePosition == 1) {
    b();
    b();
  }
  drop();
  b();
  turn(LEFT, 500);
  f();
  turn(RIGHT, 530);
  f();
  turn(RIGHT, 530);
  f();
  pickup();
  b();
  b();
}