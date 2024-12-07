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

// we could probably ignore the digital pins on the sensor array and always read only the analog pins.
// the code can be made to generate a digital output if necessary then. we check if the analog
// reading is within some threshold, and if so, generate a 1. otherwise generate a zero.
// some "modes" are defined above, and we can make it so that the digital output is 1 when the
// color corresponding to the line is detected.

class IRSensorArray {
    public:
        IRSensorArray(byte *pins, byte numPins, bool invert = false);
        // if invert is set to true, invert the value read from digitalpin before proceeding further.
        // this is not really relevant in the analog -> digital situation. corresponds to a within = false 
        // situation. now that i think of it, maybe the inversion thing is nicer than the within = true/false?
        // anyway currently im not gonna put the effect of invert into the analog one.
        void setDigitalMode(byte mode);

        void calibrate(float *mean, float *standardDeviation, float *analogReadings);
        void calibrateIndividual(float *mean, float *standardDeviation, byte index);
        
        void setSensorThresholds(byte mode, float lowerThresh, float upperThresh, bool within = true);
        void setSensorThresholds(byte mode, float* lowerThresh, float* upperThresh, bool within = true);
        void setWeights(float *weightsArray);
        
        float getWeightedAnalogReading();
        float getWeightedDigitalReading();
        float getWeightedDigitalReading1();
        float getWeightedDigitalReading2();

        void getAnalogReadingsArray(float *targetArray);
        void getDigitalReadingsArray(byte *targetArray);
        void getDigitalReadingsArray1(byte *targetArray);
        void getDigitalReadingsArray2(byte *targetArray);
        
        bool patternDetected(byte *patternArray);
        bool patternDetected1(byte *patternArray);

    private:
        byte _digitalMode;
        byte _invert;
        byte *_pins;
        byte _numPins;
        float *_weights;
        float (*_upperThresholds)[4]; // 4 modess
        float (*_lowerThresholds)[4];
};

#endif
