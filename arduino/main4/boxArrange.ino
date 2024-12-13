int ticks_until_box = 500;

byte findDestination(byte color) {
  bool tof1 = true;
  bool tof2 = true;

  if (color == RED) {  // Descending
    if (tof1) {
      return 1;  // Largest
    } else if (!tof1 && tof2) {
      return 2;
    } else {
      return 3;  // Smallest
    }
  } else {  // Ascending
    if (tof1) {
      return 3;  // Largest
    } else if (!tof1 && tof2) {
      return 2;
    } else {
      return 1;  // Smallest
    }
  }
}

void moveToDestinationJunc(int junc_count) {
  for (int i = 0; i < abs(junc_count); i++) {
    if (junc_count > 0) {
      f();
    } else {
      b();
    }
  }
}

void liftBox();

void lowerBox();

int pickProcedure(byte color) {
  turn(LEFT, 400, 0, 180);
  straighten(ticks_until_box);
  
  byte endjunc = findDestination(color);
  
  liftBox(); // Amoda

  b();
  turn(RIGHT, 500, 130, 0);

  return endjunc;
}

void placeProcedure() {
  turn(RIGHT, 500, 130, 0);
  straighten(ticks_until_box);

  lowerBox();

  b();
  turn(LEFT, 400, 0, 180);
}


void boxArrange(byte color) {
  // We start from a square

  int endJunc = 0;
  int startJunc = 1;

  f();  // Move forward till we reach the first junction.

  for (byte i = 1; i <= 3; i++) {
    endJunc = pickProcedure(color);
    moveToDestinationJunc(endJunc - startJunc);
    placeProcedure();
    startJunc++;
    // if (startJunc == 4) {
    //   break;
    // }
    moveToDestinationJunc(startJunc - endJunc);
  }
}
