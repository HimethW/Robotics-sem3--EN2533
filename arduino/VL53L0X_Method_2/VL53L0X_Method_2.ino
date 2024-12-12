#include "VL53L0X_Sensors.h"

// #define SHT_LOX1 30 // Sensor 1
// #define SHT_LOX2 31 // Sensor 2
// #define SHT_LOX3 33 // Sensor 3

// 1 - lower 3cm
// 2 - middle 7cm
// 3 - high 13 cm

// Threshold to account for non-verticality of sensor tower
const int threshold; // INITIALISE THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Upper and lower distnaces to keep box at when measuring height
int box_dist_upper = 60;
int box_dist_lower = 45;

// To determine when robot is at the box
bool start_height_measure = false;

// To determine which box is being measured in height order from smallest to largest
int box = 0;


void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(1); }
    
    initSensors();  // Initialize the sensors
}

void loop() {
    int distance_to_box = read_sensor1();  // Read (Lower) Sensor 1. Doubles as small box measure
    start_height_measure = (box_dist_lower < distance_to_box && distance_to_box < box_dist_upper);

    if(start_height_measure){
      int distance_mid_box= read_sensor2();  // Read Sensor 2 Doubles as chamber measure
      int distance_tall_box = read_sensor3();  // Read Sensor 3

      if !(box_dist_lower < distance_mid_box && distance_mid_box < box_dist_upper) && 
         !(box_dist_lower < distance_tall_box && distance_tall_box < box_dist_upper){

        box = 1;
      }
      else if (box_dist_lower < distance_mid_box && distance_mid_box < box_dist_upper) && 
             !(box_dist_lower < distance_tall_box && distance_tall_box < box_dist_upper){
        box = 2;
      }
      else if (box_dist_lower < distance_mid_box && distance_mid_box < box_dist_upper) && 
              (box_dist_lower < distance_tall_box && distance_tall_box < box_dist_upper){
        box = 3;
      }


    }



    delay(100);
}