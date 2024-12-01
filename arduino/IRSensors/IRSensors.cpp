/*
  IRSensors.cpp
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 5 October, 2024
*/

#include "Arduino.h"
#include "IRSensors.h"

IRSensorArray::IRSensorArray(byte *pins, byte numPins, bool invert = false) {
    _pins = (byte*)malloc(sizeof(byte) * numPins);
    _numPins = numPins;
    _invert = invert;

    for (byte i = 0; i < numPins; i++) {
        _pins[i] = pins[i];
        pinMode(pins[i], INPUT);
    }

    _upperThresholds = nullptr;
    _lowerThresholds = nullptr;
    _digitalMode = WHT_LINE_BLK_BG;
}

void IRSensorArray::setWeights(float *weightsArray) {
    _weights = (float*)malloc(sizeof(float) * _numPins);

    for (byte i = 0; i < _numPins; i++) {
        _weights[i] = weightsArray[i];
    }
}

float IRSensorArray::getWeightedAnalogReading() {
    float weightedReading;

    for (byte i = 0; i < _numPins; i++) {
        float pinValue = analogRead(_pins[i]);
        weightedReading += pinValue * _weights[i];
    }

    return weightedReading;
}

void IRSensorArray::setDigitalMode(byte mode) {
    _digitalMode = mode;
    // Modes are so that we can set different thresholds for the sensor to regard the line as present or absent.
    // 4 modes are there defined in the header file.
    // We set thresholds for each mode. e.g. WHT_LINE_BLK_BG = for this mode, we can set an interval of analog values
    // which the sensor reading should be within, or without, i.e., reading in [a, b] vs reading in (-infty, a] U [b, infty],
    // in order to consider the line to be detected.
}

// Allows to set the same threshold values for each sensor, corresponding to a specified mode
void IRSensorArray::setSensorThresholds(byte mode, float lowerThresh, float upperThresh, bool within = true) {
    if (!_upperThresholds) {
        _upperThresholds = new float[_numPins + 1][4];
    }
    
    if (!_lowerThresholds) {
        _lowerThresholds = new float[_numPins][4];
    }

    for (byte i = 0; i < _numPins; i++) {
        _upperThresholds[i][mode] = upperThresh;
        _lowerThresholds[i][mode] = lowerThresh;
    }
    _upperThresholds[_numPins][mode] = within; // only _upperThresholds will store the within value
}

// Allows to set different thresholds for each sensor, corresponding to a given mode
void IRSensorArray::setSensorThresholds(byte mode, float* lowerThresh, float* upperThresh, bool within = true) {
    if (!_upperThresholds) {
        _upperThresholds = new float[_numPins + 1][4];
    }
    
    if (!_lowerThresholds) {
        _lowerThresholds = new float[_numPins][4];
    }

    for (byte i = 0; i < _numPins; i++) {
        _upperThresholds[i][mode] = upperThresh[i];
        _lowerThresholds[i][mode] = lowerThresh[i];
    }
    _upperThresholds[_numPins][mode] = within;
}

float IRSensorArray::getWeightedDigitalReading() {
    float weightedReading;
    bool within = _upperThresholds[_numPins][_digitalMode];

    for (byte i = 0; i < _numPins; i++) {
        float pinValue = analogRead(_pins[i]);

        float upperThresh = _upperThresholds[i][_digitalMode];
        float lowerThresh = _lowerThresholds[i][_digitalMode];

        byte digitalValue = 0;
        if ((pinValue >= lowerThresh) && (pinValue <= upperThresh)) {
            digitalValue = 1;
        }

        digitalValue = ~(digitalValue ^ within); // XNOR gives the required result

        weightedReading += digitalValue * _weights[i];
    }

    return weightedReading;
}

float IRSensorArray::getWeightedDigitalReading1() {
    float weightedReading;

    for (byte i = 0; i < _numPins; i++) {
        byte pinValue = digitalRead(_pins[i]) ^ _invert; // again is there a type casting thing?
        weightedReading += pinValue * _weights[i];
    }

    return weightedReading;
}

void IRSensorArray::getAnalogReadingsArray(float *targetArray) {
    for (byte i = 0; i < _numPins; i++) {
        float pinValue = analogRead(_pins[i]);
        targetArray[i] = pinValue;
    }
}

// so probably in here, get analog readings, then use some threshold thing to get ones and zeros
// rather than doing a digitalread...
void IRSensorArray::getDigitalReadingsArray(byte *targetArray) {
    for (byte i = 0; i < _numPins; i++) {
        byte pinValue = digitalRead(_pins[i]) ^ _invert; // again is there a type casting thing?
        targetArray[i] = pinValue;
    }
}

bool IRSensorArray::patternDetected(byte *patternArray) {
    for (byte i = 0; i < _numPins; i++) {
        if ((digitalRead(_pins[i]) ^ _invert) != patternArray[i]) {
            return false;
        }
    }
    return true;
}
