#include <SoftwareWire.h> // Include the SoftwareWire library

// Define custom I²C pins
#define CUSTOM_SDA 44
#define CUSTOM_SCL 45

// Create a SoftwareWire instance
SoftwareWire myWire(CUSTOM_SDA, CUSTOM_SCL);

// TCS34725 Register Addresses
#define TCS34725_ADDRESS 0x29
#define TCS34725_ENABLE  0x00
#define TCS34725_ATIME   0x01
#define TCS34725_CONTROL 0x0F
#define TCS34725_CDATAL  0x14

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

void setup() {
  Serial.begin(9600);
  Serial.println("TCS34725 with SoftwareWire Example");

  // Initialize SoftwareWire
  myWire.begin();

  // Initialize the TCS34725 sensor
  writeRegister(TCS34725_ENABLE, 0x03); // Power ON and enable ADC
  writeRegister(TCS34725_ATIME, 0xFF); // Set integration time (700ms)
  writeRegister(TCS34725_CONTROL, 0x01); // Set gain (1x)

  delay(3); // Wait for initialization
  Serial.println("TCS34725 initialized successfully.");
}

void loop() {
  // Read RGB and Clear Data
  uint16_t c = readRegister16(TCS34725_CDATAL);
  uint16_t r = readRegister16(TCS34725_CDATAL + 2);
  uint16_t g = readRegister16(TCS34725_CDATAL + 4);
  uint16_t b = readRegister16(TCS34725_CDATAL + 6);

  // Print the results
  Serial.print("R: "); Serial.print(r);
  Serial.print(" G: "); Serial.print(g);
  Serial.print(" B: "); Serial.print(b);
  Serial.print(" C: "); Serial.println(c);

  delay(1000);
}
