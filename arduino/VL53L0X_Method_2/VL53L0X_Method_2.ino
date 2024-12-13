#include "VL53L0X_Sensors.h"

// #define SHT_LOX1 30 // Sensor 1 lower
// #define SHT_LOX2 31 // Sensor 2 middle
// #define SHT_LOX3 33 // Sensor 3 upper
// #define SHT_LOX4 50 // Sensor 4 side

// 1 - lower 3cm
// 2 - middle 7cm
// 3 - high 13 cm

// Threshold to account for non-verticality of sensor tower
const int threshold;  // INITIALISE THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Upper and lower distnaces for sensor offset thresholds
int box_dist_upper = 150;
int box_dist_lower = 90;

// Sensor threshold for box height measurement (for lowest sensor 1)
int box_keep_dist_upper = 150;
int box_keep_dist_lower = 120;

// To determine when robot is at the box
bool start_height_measure = false;

// To determine which box is being measured in height order from smallest to largest
int box = 0;


void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(1); }

  initTofSensors();  // Initialize the sensors
  Serial.println("Inited");
  pinMode(A1, OUTPUT);
  digitalWrite(A1, HIGH);
}

int average_sensor_reading(int (*read_sensor)()) {
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += read_sensor();  // Call the provided sensor reading function
    delay(10);             // Small delay between readings for stability
  }
  return sum / 5;  // Return the average
}


void loop() {
  int distance_to_box = read_sensor1();  // Read (Lower) Sensor 1. Doubles as small box measure
  start_height_measure = (box_keep_dist_lower < distance_to_box && distance_to_box < box_keep_dist_upper);

  if (start_height_measure) {
    Serial.print("Box detected!!! ");
    Serial.print(distance_to_box);
    Serial.print(" Box No. ");
    

    // Take the average of 5 readings for sensor 2 and sensor 3
    int distance_mid_box = average_sensor_reading(read_sensor2);   // Read Sensor 2 with averaging
    int distance_tall_box = average_sensor_reading(read_sensor3);  // Read Sensor 3 with averaging

    // Determine the box type
    if (!(box_dist_lower < distance_mid_box && distance_mid_box < box_dist_upper) && !(box_dist_lower < distance_tall_box && distance_tall_box < box_dist_upper )) {
      box = 1;
    } else if ((box_dist_lower < distance_mid_box && distance_mid_box < box_dist_upper) && !(box_dist_lower < distance_tall_box && distance_tall_box < box_dist_upper)) {
      box = 2;
    } else if ((box_dist_lower < distance_mid_box && distance_mid_box < box_dist_upper) && (box_dist_lower < distance_tall_box && distance_tall_box < box_dist_upper)) {
      box = 3;
    }
  } else {
    Serial.print("No box. Box No ");
  }

  Serial.println(box);

  delay(100);
}
