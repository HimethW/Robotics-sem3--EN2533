#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <SoftwareWire.h>

#define LEFT 0
#define RIGHT 1
#define RED 0
#define BLUE 1
#define BLACK 2

// Define custom I²C pins
#define CUSTOM_SDA_1 44 // right
#define CUSTOM_SCL_1 45
#define CUSTOM_SDA_2 46 // left
#define CUSTOM_SCL_2 47

// TCS34725 Register Addresses
#define TCS34725_ADDRESS 0x29
#define TCS34725_ENABLE  0x00
#define TCS34725_ATIME   0x01
#define TCS34725_CONTROL 0x0F
#define TCS34725_CDATAL  0x14

// Create SoftwareWire instances for both sensors
SoftwareWire RightSensor = SoftwareWire(CUSTOM_SDA_1, CUSTOM_SCL_1);
SoftwareWire LeftSensor(CUSTOM_SDA_2, CUSTOM_SCL_2);

float redAvgLeft_redblue;
float redAvgRight_redblue;
float redAvgLeft_blackblue;
float redAvgRight_blackblue;

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
  uint16_t r;
  // Do we really need c, g, b? Probably they can be set to like empty variables and passed to getRawData.
  
  if (side == RIGHT) 
  {
    r = readRegister16(RightSensor, TCS34725_CDATAL + 2);
    if(r > redAvgRight_redblue)
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
    r = readRegister16(LeftSensor, TCS34725_CDATAL + 2);
    if(r > redAvgLeft_redblue)
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

void setup() {

}

void loop() {

}