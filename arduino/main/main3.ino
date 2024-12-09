#include <Robot.h>
#include <IRSensors.h>
#include <Wheels.h>
#include <math.h>

const byte numIRSensors = 8;
const byte IRSensorPins[numIRSensors] = { A14, A13, A12, A11, A10, A9, A8, A7 };  // left to right
// const byte IRSensorPins[numIRSensors] = { 34, 35, 36, 37, 38, 39, 40, 41 };
const float weightsArray[numIRSensors] = { -20, -10, -6, -2, 2, 6, 10, 20 };  // furthest are +- 15

const byte leftmostIR = A15;
const byte rightmostIR = A6;
const float leftmostWeight = -40;  // -30
const float rightmostWeight = 40;  // 50

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
const int THRESHOLD_COUNT = 115;
int resultArray[12];
int resultArrayIndex = 0;

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

    Serial.print(j);
    Serial.print(" ");
    Serial.print(lowerThreshes[j]);
    Serial.print(" & ");
    Serial.println(upperThreshes[j]);
  }

  // Indicate the completion of calibration
  digitalWrite(13, HIGH);

  // Give some time for setup
  delay(2000);

  pravindu.setSensorThresholds(WHT_LINE_BLK_BG, lowerThreshes, upperThreshes);

  nilakna.attachWheels(&leftWheel, &rightWheel);
  nilakna.setBaseSpeed(77, 77);  // 130, 90..corrected so that it goes forward.
  nilakna.setMinMaxSpeeds(20, 150);
  nilakna.initPIDController(6.2, 0, 20);  // for line following P = 6.5, D = 20 work.

  nilakna.drive(FORWARD);
}

int a = 0;

void loop() {
  if (a == 0) {
    barcodeRead();


    a = 5;
    Serial.println("DONE");
  }


}

int barcodeRead() {
  // volatile long int white_start_time = 0;
  // bool white_detected = false;
  // int encoder_count_start = 0;
  // int encoder_count_end = 0;
  // int count_difference = 0;
  // const int THRESHOLD_COUNT = 115;
  // int resultArray[12];
  // int resultArrayIndex = 0;

  volatile long int white_start_time = millis();

  nilakna.drive(FORWARD);
  // pid constants for encoder
  Serial.println("Start");

  int result;
  double binaryNumber = 0;

  while (1) {
    equalizeEncoderCounts();
    byte onesandzeros[numIRSensors];
    pravindu.getDigitalReadingsArray(onesandzeros);

    white_detected = false;

    // Update white_detected if the middle of the array detects white
    if ((onesandzeros[numIRSensors / 2] == 1) && (onesandzeros[numIRSensors / 2 + 1] == 1) && (onesandzeros[numIRSensors / 2 - 1] == 1) && (onesandzeros[numIRSensors / 2 + 2] == 1) && (onesandzeros[numIRSensors / 2 - 2] == 1)) {
      white_detected = true;
    }

    if (white_detected && white_start_time == 0) {
      // Start of a white line
      white_start_time = millis();
      encoder_count_start = leftWheel.encoderCount;  // Capture encoder count at the start of the white line
    }

    if (!white_detected && white_start_time != 0) {
      // End of a white line
      encoder_count_end = leftWheel.encoderCount;  // Capture encoder count at the end of the white line
      count_difference = encoder_count_end - encoder_count_start;
      Serial.println(count_difference);

      if (count_difference > THRESHOLD_COUNT) {
        resultArray[resultArrayIndex] = 1;
        Serial.println("Binary 1");
      } else {
        resultArray[resultArrayIndex] = 0;
        Serial.println("Binary 0");
      }
      // Reset timing and array index for next detection
      white_start_time = 0;
      resultArrayIndex++;
    }

    if(resultArrayIndex > 4 && resultArray[resultArrayIndex-1] == 0 && resultArray[resultArrayIndex-2] == 0 && resultArray[resultArrayIndex-3] == 0){
      // Stop Condition
      Serial.println("Stopping");
      nilakna.brake();

      for (int i = 1; i <= 8; i++) {
        binaryNumber = binaryNumber + resultArray[i] * pow(2, i-1);
        // Serial.println(resultArray[i] * pow(2, i-1));
      }
      resultArrayIndex++; // To prevent further looping within the while(1). do not remove without counter measures
      result = int(binaryNumber) % 5;


      break;
    }
  }
  Serial.print("BINARY NUM: ");
  Serial.println(binaryNumber);
  
  return result;

  delay(5);
}

void equalizeEncoderCounts() {
  long error = rightWheel.encoderCount - leftWheel.encoderCount;
  nilakna.drivePID(error);
}

void lineFollow() {
  // check here if the sensors read a right angle. if so, make it turn and hold the turn for a fixed time, then go back to line follow below.
  // ig...
  // The corner-most sensors are different. Their outputs are significantly different from those of the array, so we treat them separately.
  byte leftmost = 1 - digitalRead(leftmostIR);
  float error = pravindu.getWeightedDigitalReading();
  byte rightmost = 1 - digitalRead(rightmostIR);
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