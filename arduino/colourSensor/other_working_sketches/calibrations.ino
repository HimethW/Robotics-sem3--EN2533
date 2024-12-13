void initSensor(SoftwareWire& wire) {
  writeRegister(wire, TCS34725_ENABLE, 0x03); // Power ON and enable ADC
  writeRegister(wire, TCS34725_ATIME, 0xFF); // Set integration time (700ms)
  writeRegister(wire, TCS34725_CONTROL, 0x01); // Set gain (1x)
}

void setupColourSensors() {
  // Initialize both SoftwareWire instances
  RightSensor.begin();
  LeftSensor.begin();

  // Initialize both sensors
  Serial.println("Initializing Sensor 1...");
  initSensor(RightSensor);
  Serial.println("Sensor 1 initialized successfully");

  delay(3);

  Serial.println("Initializing Sensor 2...");
  initSensor(LeftSensor);
  Serial.println("Sensor 2 initialized successfully");

  delay(3);
}

void calibrateColourSensors() {
  uint16_t r1;
  uint16_t r2;
  byte numReadings = 10;

  Serial.println("PLACE ON RED");
  digitalWrite(13, LOW);
  delay(5000);
  digitalWrite(13, HIGH);
  Serial.println("Starting Calibration on RED");

  float redAvgRight_redBox = 0;
  float redAvgLeft_redBox = 0;
  for (byte i = 0; i < numReadings; i++) {
    r1 = readRegister16(RightSensor, TCS34725_CDATAL + 2);
    redAvgRight_redBox += r1;                                                        //
    r2 = readRegister16(LeftSensor, TCS34725_CDATAL + 2);
    redAvgLeft_redBox += r2;                                                           //
  }
  redAvgRight_redBox /= numReadings;
  redAvgLeft_redBox /= numReadings;
  
  Serial.println("PLACE ON BLUE");
  digitalWrite(13, LOW);
  delay(5000);
  digitalWrite(13, HIGH);
  Serial.println("Starting Calibration on BLUE");
  
  float redAvgRight_blueBox = 0;
  float redAvgLeft_blueBox = 0;
  for (byte i = 0; i < numReadings; i++) {
    r1 = readRegister16(RightSensor, TCS34725_CDATAL + 2);
    redAvgRight_blueBox += r1;
    r2 = readRegister16(LeftSensor, TCS34725_CDATAL + 2);
    redAvgLeft_blueBox += r2;
  }
  redAvgRight_blueBox /= numReadings;
  redAvgLeft_blueBox /= numReadings;

  Serial.println("PLACE ON BLACK");
  digitalWrite(13, LOW);
  delay(5000);
  digitalWrite(13, HIGH);
  Serial.println("Starting Calibration on BLACK");

  float redAvgRight_black = 0;
  float redAvgLeft_black = 0;
  for (byte i = 0; i < numReadings; i++) {
    r1 = readRegister16(RightSensor, TCS34725_CDATAL + 2);
    redAvgRight_black += r1;
    r2 = readRegister16(LeftSensor, TCS34725_CDATAL + 2);
    redAvgLeft_black += r2;
  }
  redAvgRight_black /= numReadings;
  redAvgLeft_black /= numReadings;

  digitalWrite(13, LOW);
  Serial.print("Calibration Complete");
  

  redAvgLeft_redblue = (redAvgLeft_blueBox + redAvgLeft_redBox) / 2;
  redAvgRight_redblue = (redAvgRight_blueBox + redAvgRight_redBox) / 2;

  redAvgLeft_blackblue = (redAvgLeft_blueBox + redAvgLeft_black) / 2;
  redAvgRight_blackblue = (redAvgRight_blueBox + redAvgRight_black) / 2;
}