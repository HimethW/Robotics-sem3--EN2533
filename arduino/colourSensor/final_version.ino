#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <SoftwareWire.h> // Include the SoftwareWire library

///custom I2C is sensor 2 and original I2C sensor is sensor 1


// Define custom I²C pins
#define CUSTOM_SDA 44
#define CUSTOM_SCL 45

//blue colour B 7 and R 6
//blue colour B 7 and R 9

// Create a SoftwareWire instance
SoftwareWire myWire(CUSTOM_SDA, CUSTOM_SCL);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

int redColour[2];
float rightI2CRed;
float leftI2CRed;

// TCS34725 Register Addresses
#define TCS34725_ADDRESS 0x29
#define TCS34725_ENABLE  0x00
#define TCS34725_ATIME   0x01
#define TCS34725_CONTROL 0x0F
#define TCS34725_CDATAL  0x14

//define thresholds
// #define customI2CRed 7.5
// #define originalI2CRed 470

// Function to write to a register
void writeRegister(uint8_t reg, uint8_t value) {
  myWire.beginTransmission(TCS34725_ADDRESS);
  myWire.write(0x80 | reg); // Command bit + register address
  myWire.write(value);
  myWire.endTransmission();
}

// Function to read a 16-bit register value
uint16_t readRegister16(uint8_t reg) {
  myWire.beginTransmission(TCS34725_ADDRESS);
  myWire.write(0x80 | reg); // Command bit + register address
  myWire.endTransmission();

  myWire.requestFrom(TCS34725_ADDRESS, 2);
  uint16_t value = myWire.read();
  value |= (myWire.read() << 8);
  return value;
}

void setupColourSensors(){
  Serial.begin(9600);
    if (tcs.begin()) {
    Serial.println("Found sensor 1");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  // Initialize SoftwareWire
  myWire.begin();

  // Initialize the TCS34725 sensor
  writeRegister(TCS34725_ENABLE, 0x03); // Power ON and enable ADC
  writeRegister(TCS34725_ATIME, 0xFF); // Set integration time (700ms)
  writeRegister(TCS34725_CONTROL, 0x01); // Set gain (1x)

  delay(3); // Wait for initialization
  Serial.println("Found sensor 2");
}

void calibrateColourSensor(){

  uint16_t r1;
  uint16_t r2; uint16_t g2; uint16_t b2; uint16_t c2;

  Serial.println("place in redbox");
  digitalWrite(13,HIGH);
  delay(3000);
  digitalWrite(13,LOW);
  

  //calc redbox
  float totalredleft_redbox = 0;
  float totalredright_redbox = 0;
  for(int i =0; i < 10; i++){
    r1 = readRegister16(TCS34725_CDATAL + 2);
    totalredleft_redbox += r1;
    tcs.getRawData(&r2, &g2, &b2, &c2);
    totalredright_redbox += r2;
  }
  totalredleft_redbox = totalredleft_redbox/10;
  totalredright_redbox = totalredright_redbox/10;

  Serial.println("place in bluebox");
  digitalWrite(13,HIGH);
  delay(3000);
  digitalWrite(13,LOW);
  
  //calc bluebox
  float totalredleft_bluebox = 0;
  float totalredright_bluebox = 0;
  for(int i =0; i < 10; i++){
    r1 = readRegister16(TCS34725_CDATAL + 2);
    totalredleft_bluebox += r1;
    tcs.getRawData(&r2, &g2, &b2, &c2);
    totalredright_bluebox += r2;
  }
  totalredleft_bluebox = totalredleft_bluebox/10;
  totalredright_bluebox = totalredright_bluebox/10;

  digitalWrite(13,HIGH);
  delay(3000);
  digitalWrite(13,LOW);

  rightI2CRed = (totalredright_redbox + totalredright_bluebox)/2;
  leftI2CRed = (totalredleft_redbox + totalredleft_bluebox)/2;


}

void readColourSensorData(){
  // Read RGB and Clear Data
  uint16_t c = readRegister16(TCS34725_CDATAL);
  uint16_t r = readRegister16(TCS34725_CDATAL + 2);
  uint16_t g = readRegister16(TCS34725_CDATAL + 4);
  uint16_t b = readRegister16(TCS34725_CDATAL + 6);

  // Print the results
  // Serial.print("custom I2C readings(sensor2) : ");
  // Serial.print("R: "); Serial.print(r);
  // Serial.print(" G: "); Serial.print(g);
  // Serial.print(" B: "); Serial.print(b);
  // Serial.print(" C: "); Serial.println(c);
  bool isRed = r > leftI2CRed;
  // if(isRed){
  //   Serial.print("left is rathu     ");
  // }
  // else{
  //   Serial.print("left is not rathu     ");
  // }
  if(isRed){
    redColour[0] = 1;
  }else{
    redColour[0] = 0;
  }

  ////////////////////////////////////////////////////////
    uint16_t colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  // Serial.print("original I2C readings(sensor1) : ");
  // Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  // Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  // Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  // Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  // Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  // Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  // Serial.println(" ");
  isRed = r > rightI2CRed;
  // if(isRed){
  //   Serial.println("right is rathu");
  // }
  // else{
  //   Serial.println("right is not rathu");
  // }
  if(isRed){
    redColour[1] = 1;
  }else{
    redColour[1] = 0;
  }
  /////////////////////////////////////////////////////////////////////////

  delay(1000);
}

void setup() {
  pinMode(13,OUTPUT);
  setupColourSensors();
  Serial.println("starting calibration");
  calibrateColourSensor();
  Serial.println("calibration complete");
}

void loop() {
  readColourSensorData();
  Serial.print(redColour[0]);Serial.print(" ");Serial.println(redColour[1]);
}
