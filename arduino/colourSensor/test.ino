#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <SoftwareWire.h>

#define LEFT 0
#define RIGHT 1
#define RED 0
#define BLUE 1
#define BLACK 2

// Define custom I²C pins
#define CUSTOM_SDA 44
#define CUSTOM_SCL 45

// TCS34725 Register Addresses
#define TCS34725_ADDRESS 0x29
#define TCS34725_ENABLE  0x00
#define TCS34725_ATIME   0x01
#define TCS34725_CONTROL 0x0F
#define TCS34725_CDATAL  0x14

SoftwareWire leftColorSensor(CUSTOM_SDA, CUSTOM_SCL);
Adafruit_TCS34725 rightColorSensor(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

float redAvgLeft;
float redAvgRight;
float redAvgLeft_blackblue;
float redAvgRight_blackblue;

// Write to left sensor register
void writeRegister(uint8_t reg, uint8_t value) {
  leftColorSensor.beginTransmission(TCS34725_ADDRESS);
  leftColorSensor.write(0x80 | reg); // Command bit + register address
  leftColorSensor.write(value);
  leftColorSensor.endTransmission();
}

// Read a 16-bit register value from left sensor
uint16_t readRegister16(uint8_t reg) {
  leftColorSensor.beginTransmission(TCS34725_ADDRESS);
  leftColorSensor.write(0x80 | reg); // Command bit + register address
  leftColorSensor.endTransmission();

  leftColorSensor.requestFrom(TCS34725_ADDRESS, 2);
  uint16_t value = leftColorSensor.read();
  value |= (leftColorSensor.read() << 8);
  return value;
}

void setupColourSensors() {
  rightColorSensor.begin();
  
  leftColorSensor.begin();
  // Initialize the left sensor
  writeRegister(TCS34725_ENABLE, 0x03); // Power ON and enable ADC
  writeRegister(TCS34725_ATIME, 0xFF); // Set integration time (700ms)
  writeRegister(TCS34725_CONTROL, 0x01); // Set gain (1x)

  delay(3); // Wait for initialization
}

void calibrateColourSensors() {
  uint16_t r1;
  uint16_t r2, g2, b2, c2;
  byte numReadings = 10;

  Serial.println("PLACE ON RED");
  digitalWrite(13, LOW);
  delay(5000);
  digitalWrite(13, HIGH);
  Serial.println("Starting Calibration on RED");

  float redAvgRight_redBox = 0;
  float redAvgLeft_redBox = 0;
  for (byte i = 0; i < numReadings; i++) {
    r1 = readRegister16(TCS34725_CDATAL + 2);
    redAvgLeft_redBox += r1;
    rightColorSensor.getRawData(&r2, &g2, &b2, &c2);
    redAvgRight_redBox += r2;
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
    r1 = readRegister16(TCS34725_CDATAL + 2);
    redAvgLeft_blueBox += r1;
    rightColorSensor.getRawData(&r2, &g2, &b2, &c2);
    redAvgRight_blueBox += r2;
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
    r1 = readRegister16(TCS34725_CDATAL + 2);
    redAvgLeft_black += r1;
    rightColorSensor.getRawData(&r2, &g2, &b2, &c2);
    redAvgRight_black += r2;
  }
  redAvgRight_black /= numReadings;
  redAvgLeft_black /= numReadings;

  digitalWrite(13, LOW);
  Serial.print("Calibration Complete");
  

  redAvgLeft = (redAvgLeft_blueBox + redAvgLeft_redBox) / 2;
  redAvgRight = (redAvgRight_blueBox + redAvgRight_redBox) / 2;

  redAvgLeft_blackblue = (redAvgLeft_blueBox + redAvgLeft_black) / 2;
  redAvgRight_blackblue = (redAvgRight_blueBox + redAvgRight_black) / 2;
}

// bool isRed(byte side) {
//   uint16_t c = readRegister16(TCS34725_CDATAL);
//   uint16_t r = readRegister16(TCS34725_CDATAL + 2);
//   uint16_t g = readRegister16(TCS34725_CDATAL + 4);
//   uint16_t b = readRegister16(TCS34725_CDATAL + 6);
//   // Do we really need c, g, b? Probably they can be set to like empty variables and passed to getRawData.
  
//   if (side == RIGHT) {
//     rightColorSensor.getRawData(&r, &g, &b, &c);
//     return (r > redAvgRight);
//   } else {
//     return (r > redAvgLeft);
//   }
// }


//i'm editing this part. see if this is okay. your original code is commented out above
byte isColour(byte side) {
  uint16_t c;
  uint16_t r;
  uint16_t g;
  uint16_t b;
  // Do we really need c, g, b? Probably they can be set to like empty variables and passed to getRawData.
  
  if (side == RIGHT) 
  {
    rightColorSensor.getRawData(&r, &g, &b, &c);
    if(r > redAvgRight)
    {
      return RED;
    }
    else if(r > redAvgRight_blackblue)
    {
      return BLUE;
    }
    else
    {
      return BLACK;
    }
  }
  else
  {
    r = readRegister16(TCS34725_CDATAL + 2); //if the side is LEFT we need to read the left sensor manually. cannot use the previously read value
    if(r > redAvgLeft)
    {
      return RED;
    }
    else if(r > redAvgLeft_blackblue)
    {
      return BLUE;
    }
    else
    {
      return BLACK;
    }
  }
  
}
