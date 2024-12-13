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

#define USE_THRESHES 0
#define DIRECT_DIGITAL_READ 1

class IRSensorArray {
    public:
        IRSensorArray(byte *pins, byte numPins, bool invert = false);
        // If invert is set to true, invert the value read from digital pin before proceeding further
        void setDigitalMode(byte mode);

        void calibrate(float *mean, float *standardDeviation);
        void calibrate(float *mean, float *standardDeviation, byte index);
        
        void setSensorThresholds(byte mode, float lowerThresh, float upperThresh, bool within = true);
        void setSensorThresholds(byte mode, float* lowerThresh, float* upperThresh, bool within = true);
        void setWeights(float *weightsArray);
        
        void getAnalogReadingsArray(float *targetArray);
        float getWeightedAnalogReading();
        
        void getDigitalReadingsArray(byte *targetArray, byte mode = USE_THRESHES);
        float getWeightedDigitalReading(byte mode = USE_THRESHES);
        
        bool patternDetected(byte *patternArray, byte mode = USE_THRESHES);
        
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
