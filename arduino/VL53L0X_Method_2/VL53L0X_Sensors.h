#ifndef VL53L0X_SENSORS_H
#define VL53L0X_SENSORS_H

#include "Adafruit_VL53L0X.h"

// Define sensor addresses
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32

// Define shutdown pins
#define SHT_LOX1 30
#define SHT_LOX2 31
#define SHT_LOX3 33

// Function prototypes
void initTofSensors();
int read_sensor1();
int read_sensor2();
int read_sensor3();

#endif
