/*
  ColorSensor.cpp
  EN2533 - Robot Design and Competition
  Himeth Walgampaya, December 2024
*/

#include "Arduino.h"
#include "ColorSensor.h"

ColorSensor::ColorSensor(byte SCL_pin, byte SDA_pin) {
    _softI2CObj = SoftwareWire(SDA_pin, SCL_pin);
}

void ColorSensor::initialize() {
    _softI2CObj.begin();
    _writeRegister(TCS34725_ENABLE, 0x03); // Power ON and enable ADC
    _writeRegister(TCS34725_ATIME, 0xFF); // Set integration time (700ms)
    _writeRegister(TCS34725_CONTROL, 0x01); // Set gain (1x)
    delay(3);
}

uint16_t ColorSensor::readRawValue() {
    return _readRegister16(TCS34725_CDATAL + 2);
}

void ColorSensor::setThresholds(float red_blue_TH, float blue_black_TH){
    _redAvg_redblue = red_blue_TH;
    _redAvg_blackblue = blue_black_TH;
}

byte ColorSensor::detectColor(){
    uint16_t r = _readRegister16(TCS34725_CDATAL + 2);
    if(r > _redAvg_redblue){
        return RED;
    }
    else if(r > _redAvg_blackblue){
        return BLUE;
    }
    else{
        return BLACK;
    }
}

void ColorSensor::_writeRegister(uint8_t reg, uint8_t value) {
    _softI2CObj.beginTransmission(TCS34725_ADDRESS);
    _softI2CObj.write(0x80 | reg); // Command bit + register address
    _softI2CObj.write(value);
    _softI2CObj.endTransmission();
}

uint16_t ColorSensor::_readRegister16(uint8_t reg) {
  _softI2CObj.beginTransmission(TCS34725_ADDRESS);
  _softI2CObj.write(0x80 | reg); // Command bit + register address
  _softI2CObj.endTransmission();

  _softI2CObj.requestFrom(TCS34725_ADDRESS, 2);
  uint16_t value = _softI2CObj.read();
  value |= (_softI2CObj.read() << 8);
  return value;
}

