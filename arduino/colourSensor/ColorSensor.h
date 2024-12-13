/*
  ColorSensor.h
  EN2533 - Robot Design and Competition
  Himeth Walgampaya, December 2024
*/

#ifndef ColorSensor_h
#define ColorSensor_h

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_TCS34725.h"
#include "SoftwareWire.h"

#define RED 0
#define BLUE 1
#define BLACK 2

#define TCS34725_ADDRESS 0x29
#define TCS34725_ENABLE  0x00
#define TCS34725_ATIME   0x01
#define TCS34725_CONTROL 0x0F
#define TCS34725_CDATAL  0x14

class ColorSensor {
  public:
    ColorSensor(byte SCL_pin, byte SDA_pin);

    uint16_t readRawValue();
    void setThresholds(float red_blue_TH, float blue_black_TH);
    void initialize();
    byte detectColor();


  private:
    SoftwareWire _softI2CObj;
    float _redAvg_redblue, _redAvg_blackblue;
    void _writeRegister(uint8_t reg, uint8_t value);
    uint16_t _readRegister16(uint8_t reg);

};

#endif
