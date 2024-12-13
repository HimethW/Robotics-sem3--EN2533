#include "VL53L0X_Sensors.h"

// Create objects for the sensors
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();

// Measurement data structures
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
VL53L0X_RangingMeasurementData_t measure4;

// Sensor initialization function
void initTofSensors() {
    pinMode(SHT_LOX1, OUTPUT);
    pinMode(SHT_LOX2, OUTPUT);
    pinMode(SHT_LOX3, OUTPUT);
    pinMode(SHT_LOX4, OUTPUT);

    digitalWrite(SHT_LOX1, LOW);
    digitalWrite(SHT_LOX2, LOW);
    digitalWrite(SHT_LOX3, LOW);
    digitalWrite(SHT_LOX4, LOW);

    delay(10);

    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, HIGH);
    digitalWrite(SHT_LOX3, HIGH);
    digitalWrite(SHT_LOX4, HIGH);
    delay(10);

    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, LOW);
    digitalWrite(SHT_LOX3, LOW);
    digitalWrite(SHT_LOX4, LOW);

    if (!lox1.begin(LOX1_ADDRESS)) {
        Serial.println(F("Failed to boot first VL53L0X"));
        //while (1); // ADD THIS
    }
    else {
      Serial.println("Booted first");
    }

    delay(10);

    digitalWrite(SHT_LOX2, HIGH);
    delay(10);

    if (!lox2.begin(LOX2_ADDRESS)) {
        Serial.println(F("Failed to boot second VL53L0X"));
        // while (1); //ADD THIS
    }
    else{
      Serial.println("Booted 2nd");
    }

    delay(10);

    digitalWrite(SHT_LOX3, HIGH);
    delay(10);

    if (!lox3.begin(LOX3_ADDRESS)) {
        Serial.println(F("Failed to boot third VL53L0X"));
        //while (1);
    }
    else{
      Serial.println("Booted Third");
    }
        delay(10);

    digitalWrite(SHT_LOX4, HIGH);
    delay(10);

    if (!lox3.begin(LOX4_ADDRESS)) {
        Serial.println(F("Failed to boot fourth VL53L0X"));
        //while (1);
    }
    else{
      Serial.println("Booted Fourth");
    }
}

int read_sensor1() {
  
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
 

  // print sensor one reading
  if(measure1.RangeStatus != 4) {     // if not out of range
    int sensor1 = measure1.RangeMilliMeter;    
    return sensor1;   
  } else {
    //Serial.println("1 Out of range");
  }
}

int read_sensor2() {

  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  // print sensor two reading
  if(measure2.RangeStatus != 4) {
    int sensor2 = measure2.RangeMilliMeter;
    return sensor2;
  } else {
    //Serial.println("2 Out of range");
  }
  
}

int read_sensor3() {

  lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!

  // print sensor two reading
  if(measure3.RangeStatus != 4) {
    int sensor3 = measure3.RangeMilliMeter;
    return sensor3;
  } else {
    //Serial.println("3 Out of range");
  }
  
}

int read_sensor4() {

  lox4.rangingTest(&measure4, false); // pass in 'true' to get debug data printout!

  // print sensor two reading
  if(measure4
  .RangeStatus != 4) {
    int sensor4 = measure3.RangeMilliMeter;
    return sensor4;
  } else {
    //Serial.println("3 Out of range");
  }
  
}

