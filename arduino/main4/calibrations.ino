void calibration() {
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
