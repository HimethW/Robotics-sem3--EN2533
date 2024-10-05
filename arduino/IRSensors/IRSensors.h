/*
  IRSensors.h
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 5 October, 2024
*/

#ifndef IRSensors_h
#define IRSensors_h

#include "Arduino.h"

#define BLK_LINE_WHT_BG 0
#define WHT_LINE_BLK_BG 1
#define RED_LINE_BLK_BG 2
#define BLU_LINE_BLK_BG 3

class IRSensorArray {
    public:
        IRSensorArray(byte *pins, byte numPins);
        void setMode(byte mode);
        void setWeights(float *weightsArray);
        float getWeightedAnalogReading();
        float getWeightedDigitalReading();
        bool junctionMet();

    private:
        byte *_pins;
        byte _numPins;
        float *_weights;
};

#endif
