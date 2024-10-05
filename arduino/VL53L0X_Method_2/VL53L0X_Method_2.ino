#include "VL53L0X_Sensors.h"

// Upper and lower distnaces to keep box at when measuring height
int box_dist_upper = 60;
int box_dist_lower = 45;

// Height of height measuring TOF from ground level
int tof_height = 180;

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(1); }
    
    initSensors();  // Initialize the sensors
}

void loop() {
    int distance1 = read_sensor1();  // Read Sensor 1
    int distance2 = read_sensor2();  // Read Sensor 2


    if (box_dist_lower < distance1 && distance1 < box_dist_upper){
      int height = tof_height - distance2;
      Serial.println(height);

    }

    
    
    delay(100);
}