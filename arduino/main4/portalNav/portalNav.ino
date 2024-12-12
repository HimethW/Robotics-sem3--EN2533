// ROBOT EN2533 - task 5 - portal navigation
// Nilakna. 12-12-2024

#include <Robot.h>
#include <IRSensors.h>
#include <Wheels.h>
#include <math.h>
#include "VL53L0X_Sensors.h"
#include "Adafruit_VL53L0X.h"

const byte numIRSensors = 8;
const byte IRSensorPins[numIRSensors] = { A14, A13, A12, A11, A10, A9, A8, A7 };  // left to right
// const byte IRSensorPins[numIRSensors] = { 34, 35, 36, 37, 38, 39, 40, 41 };
const float weightsArray[numIRSensors] = { -20, -10, -6, -2, 2, 6, 10, 20 };  // furthest are +- 15
// const float rightTur

const byte leftmostIR = A15;
const byte rightmostIR = A6;
const float leftmostWeight = -40;  // -30
const float rightmostWeight = 40;  // 50

const Wheel leftWheel(24, 25, 4);
const byte leftWheelInterrupt = 18;
const Wheel rightWheel(28, 29, 5);
const byte rightWheelInterrupt = 19;

const float K_enc = 0.8;
const int gateClearance = 200;  // change this to the min distance to the gate

const Robot nilakna;
const IRSensorArray pravindu(IRSensorPins, numIRSensors);

// initialize the TOF sensors

// Barcode Variables

volatile long int white_start_time = 0;
bool white_detected = false;
int encoder_count_start = 0;
int encoder_count_end = 0;
int count_difference = 0;
const int THRESHOLD_COUNT = 115;
int resultArray[12];
int resultArrayIndex = 0;

void setup() {
  Serial.begin(115200);

  pinMode(13, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, LOW);
  digitalWrite(A2, LOW);
  pinMode(leftmostIR, INPUT);
  pinMode(rightmostIR, INPUT);

  Serial.println("set up");
  analogWrite(A1, 155);
  initTofSensors();
  Serial.println("tof initialized");
  leftWheel.setEncoderPins(leftWheelInterrupt, 22);
  rightWheel.setEncoderPins(rightWheelInterrupt, 26);

  attachInterrupt(digitalPinToInterrupt(rightWheelInterrupt), rightWheelCallback, CHANGE);
  attachInterrupt(digitalPinToInterrupt(leftWheelInterrupt), leftWheelCallback, CHANGE);

  pravindu.setWeights(weightsArray);

  // Find mean and standard deviation from all sensors, with the robot placed on a white line. This gives the typical "white-ish" reading from ALL sensors.
  // The thinking is that a reading significantly different from this is unlikely to be white.
  float overallMean = 0;
  float overallStdDev = 0;
  float sensorReadings[numIRSensors];
  pravindu.calibrate(&overallMean, &overallStdDev);

  // Find the variability in the reading of each individual sensor. This gives the typical "white-ish" reading for EACH INDIVIDUAL sensor.
  float lowerThreshes[numIRSensors], upperThreshes[numIRSensors];

  for (byte j = 0; j < numIRSensors; j++) {
    float mean = 0;
    float stdDev = 0;
    pravindu.calibrate(&mean, &stdDev, j);

    // Decide a way to find these values. This is just me putting ranges off the top of my head.
    lowerThreshes[j] = mean - (10 * overallStdDev + 2 * stdDev);
    upperThreshes[j] = mean + (10 * overallStdDev + 2 * stdDev);
  }

  // // Indicate the completion of calibration
  // digitalWrite(13, HIGH);
  // delay(1000);
  // digitalWrite(13, LOW);
  // delay(5000);

  // Give some time for setup
  delay(2000);

  pravindu.setSensorThresholds(WHT_LINE_BLK_BG, lowerThreshes, upperThreshes);

  nilakna.attachWheels(&leftWheel, &rightWheel);
  nilakna.setBaseSpeed(100, 100);  // forward speeds right left
  // nilakna.setBaseSpeed(90,90); // backward speeds
  nilakna.setMinMaxSpeeds(20, 150);
  nilakna.initPIDController(1, 0, 0);  // for line following P = 6.5, D = 20 work.

  analogWrite(A1, 0);
  nilakna.drive(FORWARD);
}

int a = 5;

void loop() {
  if (a == 5) {
    portalNav();
}
    a = 9;
  
  // nilakna.setBaseSpeed(80, 80);
  // nilakna.drive(FORWARD);
}

void portalNav() {
  // detect tof. if closed wait till open. start going forward with black line following on.
  // if open wait till closed. then wait till open. then start going with encoder errors. when line detected start line follwowing.
  Serial.println(gateClearance);
  int portalStartTime = millis();
  Serial.println(millis());
  int initDist;
  // while (millis() < (portalStartTime + 5000)) 
  while (1)
  {
    // go forward and stop near the gate while closed. just wait.
    initDist = read_sensor1();
    Serial.print("start time: ");
    Serial.print(millis());
    Serial.print(", read dist: ");
    Serial.println(initDist);
    //if we stop as soon as the line ends and in the white box the max clearance can be determined.
    if (initDist < gateClearance) {
      // gate should be within the range. otherwise gate is considered open: do nothing.
      // now go forward as near as possible.
      // break and do while the gate is closed.
      Serial.println("gateDetected");
      analogWrite(A1, 155);
      delay(1000);
      analogWrite(A1, 0);
      break;
    }
    // else: this means the gate is either open or the robot is too far away. so wait.
  }
  Serial.println("waiting...");
  analogWrite(A1, 155);
  while (read_sensor1() <= (initDist + 60)) {  // EDITTTTTTTTTTTTTT 10
  Serial.println(read_sensor1());
  }
  Serial.println("gate open. going. ");
  analogWrite(A1, 0);
  leftWheel.resetEncoderCount();

  while (leftWheel.encoderCount < 1000) {  // EDITTTTTTTTTTTTTTTT 200
    equalizeEncoderCounts();
    nilakna.drive(FORWARD);
  }
  Serial.println("stopped");
  nilakna.brake();  // stop when (black) line found
}

void equalizeEncoderCounts() {
  long error = rightWheel.encoderCount - leftWheel.encoderCount;
  nilakna.drivePID(error);
}

void lineFollowwithEnc() {
  byte leftmost = 1 - digitalRead(leftmostIR);
  float error = pravindu.getWeightedDigitalReading();
  byte rightmost = 1 - digitalRead(rightmostIR);
  error += (leftmostWeight * leftmost) + (rightmostWeight * rightmost);
  error += (rightWheel.encoderCount - leftWheel.encoderCount) * K_enc;  // for backward driving
  nilakna.drivePID(error);
}

void rightWheelCallback() {
  rightWheel.updateEncoderCount();
}

void leftWheelCallback() {
  leftWheel.updateEncoderCount();
}
