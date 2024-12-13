void IRSensorCalibration() {
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
}

void ColorSensorCalibration(){
  uint16_t r1;
  uint16_t r2;
  byte numReadings = 10;

  //Serial.println("PLACE ON RED");
  digitalWrite(13, LOW);
  delay(5000);
  digitalWrite(13, HIGH);
  //Serial.println("Starting Calibration on RED");

  float redAvgRight_redBox = 0;
  float redAvgLeft_redBox = 0;
  for (byte i = 0; i < numReadings; i++) {
    r1 = RightSensor.readRawValue();
    redAvgRight_redBox += r1;                                                        //
    r2 = LeftSensor.readRawValue();
    redAvgLeft_redBox += r2;                                                           //
  }
  redAvgRight_redBox /= numReadings;
  redAvgLeft_redBox /= numReadings;
  
  //Serial.println("PLACE ON BLUE");
  digitalWrite(13, LOW);
  delay(5000);
  digitalWrite(13, HIGH);
  //Serial.println("Starting Calibration on BLUE");
  
  float redAvgRight_blueBox = 0;
  float redAvgLeft_blueBox = 0;
  for (byte i = 0; i < numReadings; i++) {
    r1 = RightSensor.readRawValue();
    redAvgRight_blueBox += r1;
    r2 = LeftSensor.readRawValue();
    redAvgLeft_blueBox += r2;
  }
  redAvgRight_blueBox /= numReadings;
  redAvgLeft_blueBox /= numReadings;

  //Serial.println("PLACE ON BLACK");
  digitalWrite(13, LOW);
  delay(5000);
  digitalWrite(13, HIGH);
  Serial.println("Starting Calibration on BLACK");

  float redAvgRight_black = 0;
  float redAvgLeft_black = 0;
  for (byte i = 0; i < numReadings; i++) {
    r1 = RightSensor.readRawValue();
    redAvgRight_black += r1;
    r2 = LeftSensor.readRawValue();
    redAvgLeft_black += r2;
  }
  redAvgRight_black /= numReadings;
  redAvgLeft_black /= numReadings;

  digitalWrite(13, LOW);
  //Serial.print("Calibration Complete");
  

  redAvgLeft_redblue = (redAvgLeft_blueBox + redAvgLeft_redBox) / 2;
  redAvgRight_redblue = (redAvgRight_blueBox + redAvgRight_redBox) / 2;

  redAvgLeft_blackblue = (redAvgLeft_blueBox + redAvgLeft_black) / 2;
  redAvgRight_blackblue = (redAvgRight_blueBox + redAvgRight_black) / 2;

  RightSensor.setThresholds(redAvgRight_redblue,redAvgRight_blackblue);
  LeftSensor.setThresholds(redAvgLeft_redblue,redAvgLeft_blackblue);
  

}
