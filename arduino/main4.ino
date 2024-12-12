#include <Robot.h>
#include <IRSensors.h>
#include <Wheels.h>

#define LEFT 0
#define RIGHT 1
#define NONE 2
#define BOTH 3

const byte numIRSensors = 8;
const byte IRSensorPins[numIRSensors] = { A14, A13, A12, A11, A10, A9, A8, A7 };  // left to right
const float weightsArray[numIRSensors] = { -20, -10, -6, -2, 2, 6, 10, 20 };      // furthest are +- 15

const byte leftmostIR = A15;
const byte rightmostIR = A6;
const float leftmostWeight = -40;
const float rightmostWeight = 40;

const Wheel leftWheel(24, 25, 4);
const byte leftWheelInterrupt = 18;
const Wheel rightWheel(28, 29, 5);
const byte rightWheelInterrupt = 19;

const Robot nilakna;
const IRSensorArray pravindu(IRSensorPins, numIRSensors);

// Barcode Variables
volatile long int white_start_time = 0;
bool white_detected = false;
int encoder_count_start = 0;
int encoder_count_end = 0;
int count_difference = 0;
const int THRESHOLD_COUNT = 100;
int resultArray[12];
int resultArrayIndex = 0;
double binaryNumber = 0;

const float K_enc = 0.9;
const float K_line = 1.2;

void setup() {
  Serial.begin(115200);

  pinMode(13, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(leftmostIR, INPUT);
  pinMode(rightmostIR, INPUT);

  digitalWrite(A3, 0); // GND LED

  leftWheel.setEncoderPins(leftWheelInterrupt, 22);
  rightWheel.setEncoderPins(rightWheelInterrupt, 26);

  attachInterrupt(digitalPinToInterrupt(rightWheelInterrupt), rightWheelCallback, CHANGE);
  attachInterrupt(digitalPinToInterrupt(leftWheelInterrupt), leftWheelCallback, CHANGE);

  pravindu.setWeights(weightsArray);

  analogWrite(A1, 155);
  calibration();
  analogWrite(A1, 0);

  nilakna.attachWheels(&leftWheel, &rightWheel);
  nilakna.setBaseSpeed(80, 80);  // 130, 90..corrected so that it goes forward.
  nilakna.setMinMaxSpeeds(20, 150);
  nilakna.initPIDController(10, 0, 10);  // for line following P = 6.5, D = 20 work.

  nilakna.drive(FORWARD);
}

unsigned long lastCheck = millis();
long stallPrevent_lastEncCount = 0;

void preventStalls() {
  unsigned long timeSpentStalling = millis() - lastCheck;
  // set speed to a constant times this time, if stalled.
  if ((leftWheel.encoderCount - stallPrevent_lastEncCount < 5) || (rightWheel.encoderCount - stallPrevent_lastEncCount < 5)) {
    ;
  }
}

int a = 0;

void loop() {
  byte result = barcodeRead();
  // Move to the end of the first junction
  equalizeEncoderTillJunc();
  // Turn
  turn(RIGHT, 500);
  // Correct the course and go forward using encoders till the first junction.
  f();
  // Now start Himeth's part
  moveBox(result);
  byte gatePos = findGate();
  solveMaze(result, gatePos);
}

void juncandturn() {
  byte j = equalizeEncoderTillJunc();

  if (j == LEFT) {
    turn(LEFT, 400);
  } else if (j == RIGHT) {
    turn(RIGHT, 500);
  }

  nilakna.brake();
  delay(2000);
}

void rightWheelCallback() {
  rightWheel.updateEncoderCount();
}

void leftWheelCallback() {
  leftWheel.updateEncoderCount();
}