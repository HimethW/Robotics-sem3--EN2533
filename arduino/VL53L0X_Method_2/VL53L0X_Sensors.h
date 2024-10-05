#ifndef VL53L0X_SENSORS_H
#define VL53L0X_SENSORS_H

#include "Adafruit_VL53L0X.h"

// Define sensor addresses
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// Define shutdown pins
#define SHT_LOX1 7
#define SHT_LOX2 6

// Function prototypes
void initSensors();
int read_sensor1();
int read_sensor2();

#endif
