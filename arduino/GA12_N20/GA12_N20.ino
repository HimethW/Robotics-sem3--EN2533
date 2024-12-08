#include "GA12_N20.h"

Motor motor(8, 9, 12, 2, 3);

volatile long int time = 0;
volatile long int current_time = 0;
volatile long int white_start_time = 0;
const int active_time = 4;
bool white_detected = false;
int encoder_count_start = 0;

void setup() {
    Serial.begin(9600);
    delay(1000);

    // Initialize your IR sensor pin here, e.g., pinMode(IR_PIN, INPUT);
}

void loop() {
    motor.motor_fwd(100);  // Start the motor at speed 100
    time = millis();

    // Assume `white_detected` is set to `true` when the IR sensor detects white and `false` otherwise.
    // Replace `IR_PIN` with the actual pin connected to the IR sensor.
    white_detected = digitalRead(IR_PIN) == HIGH;  // Example condition

    if (white_detected && white_start_time == 0) {
        // Start of a white line
        white_start_time = time;
        encoder_count_start = motor.get_encoder1_count();  // Capture encoder count at the start of the white line
    }

    if (!white_detected && white_start_time != 0) {
        // End of a white line
        int encoder_count_end = motor.get_encoder1_count();  // Capture encoder count at the end of the white line
        int count_difference = encoder_count_end - encoder_count_start;

        // Calculate the duration of the white line
        long int white_duration = time - white_start_time;

        // Display results
        Serial.print("White line detected - Duration (ms): ");
        Serial.print(white_duration);
        Serial.print(" - Encoder Count: ");
        Serial.println(count_difference);

        // Determine if this is a thick or thin bar
        if (count_difference > THRESHOLD_COUNT) {
            Serial.println("Thick bar detected.");
        } else {
            Serial.println("Thin bar detected.");
        }

        // Reset timing and encoder count for next detection
        white_start_time = 0;
        encoder_count_start = 0;
    }

}
