// Functions for reading/writing to a register
void writeRegister(SoftwareWire& wire, uint8_t reg, uint8_t value) {
  wire.beginTransmission(TCS34725_ADDRESS);
  wire.write(0x80 | reg); // Command bit + register address
  wire.write(value);
  wire.endTransmission();
}

uint16_t readRegister16(SoftwareWire& wire, uint8_t reg) {
  wire.beginTransmission(TCS34725_ADDRESS);
  wire.write(0x80 | reg); // Command bit + register address
  wire.endTransmission();

  wire.requestFrom(TCS34725_ADDRESS, 2);
  uint16_t value = wire.read();
  value |= (wire.read() << 8);
  return value;
}