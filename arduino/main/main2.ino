#include <Robot.h>
#include <IRSensors.h>
#include <Wheels.h>

const byte numIRSensors = 8;
const byte IRSensorPins[numIRSensors] = { A14, A13, A12, A11, A10, A9, A8, A7 };  // left to right
// const byte IRSensorPins[numIRSensors] = { 34, 35, 36, 37, 38, 39, 40, 41 };
const float weightsArray[numIRSensors] = { -20, -10, -6, -2, 2, 6, 10, 20 }; // furthest are +- 15

const byte leftmostIR = A15;
const byte rightmostIR = A6;
const float leftmostWeight = -40; // -30
const float rightmostWeight = 40; // 50

const Wheel leftWheel(24, 25, 4);
const byte leftWheelInterrupt = 18;
const Wheel rightWheel(28, 29, 5);
const byte rightWheelInterrupt = 19;

const Robot nilakna;                                       // LOL
const IRSensorArray pravindu(IRSensorPins, numIRSensors);  // LOL

void setup() {
  Serial.begin(115200);

  pinMode(13, OUTPUT);

  pinMode(leftmostIR, INPUT);
  pinMode(rightmostIR, INPUT);

  leftWheel.setEncoderPins(leftWheelInterrupt, 22);
  rightWheel.setEncoderPins(rightWheelInterrupt, 26);

  attachInterrupt(digitalPinToInterrupt(rightWheelInterrupt), rightWheelCallback, CHANGE);
  attachInterrupt(digitalPinToInterrupt(leftWheelInterrupt), leftWheelCallback, CHANGE);

  pravindu.setWeights(weightsArray);

  // Find mean from all sensors. Assuming equal noise to all, subtracting the mean from each reading must remove its effect. The standard deviation gives
  // how spread out the readings from the sensors are, when we are on the same color. We expect our sensors to keep reading such values.
  float overallMean = 0;
  float overallStdDev = 0;
  float sensorReadings[numIRSensors];
  pravindu.getAnalogReadingsArray(sensorReadings);
  pravindu.calibrate(&overallMean, &overallStdDev, sensorReadings);

  // Now we look at the variability of each sensor individually, when its on the same color. Again the effect of noise is assumed constant to all sensors,
  // and we only want to kind of find the variability of the sensor (which is unaffected really by the mean).
  float lowerThreshes[numIRSensors], upperThreshes[numIRSensors];
  for (byte j = 0; j < numIRSensors; j++) {
    float mean = 0;
    float stdDev = 0;
    pravindu.calibrateIndividual(&mean, &stdDev, j);

    lowerThreshes[j] = mean - (10 * overallStdDev + 2 * stdDev);
    upperThreshes[j] = mean + (10 * overallStdDev + 2 * stdDev);

    Serial.print(j);
    Serial.print(" ");
    Serial.print(lowerThreshes[j]);
    Serial.print(" & ");
    Serial.println(upperThreshes[j]);
  }

  digitalWrite(13, HIGH);
  delay(2000);

  pravindu.setSensorThresholds(WHT_LINE_BLK_BG, lowerThreshes, upperThreshes);

  nilakna.attachWheels(&leftWheel, &rightWheel);
  nilakna.setBaseSpeed(77, 77);  // 130, 90..corrected so that it goes forward.
  nilakna.setMinMaxSpeeds(20, 150);
  nilakna.initPIDController(6.5, 0, 20);
  nilakna.drive(FORWARD);
}

void loop() {
  lineFollow();
}

void lineFollow() {
  // check here if the sensors read a right angle. if so, make it turn and hold the turn for a fixed time, then go back to line follow below.
  // ig...
  byte leftmost = 1 - digitalRead(leftmostIR);
  // Serial.print(leftmost);
  // Serial.print(" ");
  float error = pravindu.getWeightedDigitalReading();
  byte rightmost = 1 - digitalRead(rightmostIR);
  // Serial.println(rightmost);
  error += (leftmostWeight * leftmost) + (rightmostWeight * rightmost);
  nilakna.drivePID(error);
}

void rightWheelCallback() {
  rightWheel.updateEncoderCount();
}

void leftWheelCallback() {
  leftWheel.updateEncoderCount();
}

void turn() {
  leftWheel.forward();
  rightWheel.backward();
  long startCount = leftWheel.encoderCount;
  while (leftWheel.encoderCount - startCount < 50) {
    ;
  }
  nilakna.brake();
}