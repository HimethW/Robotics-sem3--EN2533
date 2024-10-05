#include "GA12_N20.h"

Motor motor(8, 9, 12, 2, 3);

volatile long int time = 0;
volatile long int current_time = 0;
const int active_time = 4;

void setup() {
    Serial.begin(9600);
    delay(1000);
}

void loop() {
    motor.motor_fwd(100);

    time = millis();

    if (time - current_time > active_time * 1000) {
        motor.motor_stop();

        Serial.print(active_time);
        Serial.print(": ");
        Serial.print(motor.get_encoder1_count());
        Serial.print(" -- ");
        Serial.println(motor.get_encoder2_count());

        motor.reset_counts();
        current_time = time;
    }
}
