/*
  IRSensors.cpp
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 5 October, 2024
*/

// Works as of now (1)

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

void IRSensorArray::setDigitalMode(byte mode) {
    _digitalMode = mode;
    // We set thresholds for each mode. e.g. WHT_LINE_BLK_BG = for this mode, we can set an interval of analog values
    // which the sensor reading should be within, or without, i.e., reading in [a, b] vs reading in (-infty, a] U [b, infty],
    // in order to consider the line to be detected.
}

void IRSensorArray::calibrate(float *mean, float *standardDeviation) {
    float rawSensorReadings[_numPins];
    getAnalogReadingsArray(rawSensorReadings);

    for (byte i = 0; i < _numPins; i++) {
        *mean += rawSensorReadings[i];
        *standardDeviation += rawSensorReadings[i] * rawSensorReadings[i];
    }

    *mean = *mean / _numPins;
    *standardDeviation = *standardDeviation / _numPins;
    *standardDeviation = sqrt(*standardDeviation - ((*mean) * (*mean)));
}

void IRSensorArray::calibrate(float *mean, float *standardDeviation, byte index) {
    byte numReadings = 10;

    for (byte j = 0; j < numReadings; j++) {
        float pinValue;
        pinValue = analogRead(_pins[index]);
        *mean += pinValue;
        *standardDeviation += pinValue * pinValue;
        delay(100);
    }
    
    *mean = *mean / numReadings;
    *standardDeviation = *standardDeviation / numReadings;
    *standardDeviation = sqrt(*standardDeviation - ((*mean) * (*mean)));
}

// Set the same threshold value for each sensor, for a specified mode
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

// Set different thresholds for each sensor, for a specified mode
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

// Finds the weighted average of the digital values obtained through getDigitalReadingsArray
float IRSensorArray::getWeightedDigitalReading(byte mode = USE_THRESHES) {
    float weightedReading = 0;
    
    byte digitalValues[_numPins];
    getDigitalReadingsArray(digitalValues, mode);

    for (byte i = 0; i < _numPins; i++) {
        weightedReading += (float)(digitalValues[i] * _weights[i]);
    }

    return weightedReading;
}

// Read each sensor pin based on the value of mode, and find the corresponding digital value
void IRSensorArray::getDigitalReadingsArray(byte *targetArray, byte mode = USE_THRESHES) {
    if (mode == DIRECT_DIGITAL_READ) {
        for (byte i = 0; i < _numPins; i++) {
            byte pinValue = digitalRead(_pins[i]) ^ _invert;
            targetArray[i] = pinValue;
        }
        return;
    }

    byte numReadings = 10;
    bool within = _upperThresholds[_numPins][_digitalMode];

    float rawSensorReadings[_numPins] = { 0 };
    getAnalogReadingsArray(rawSensorReadings);

    for (byte i = 0; i < _numPins; i++) {
        // Find upper and lower thresholds for the ith sensor
        float upperThresh = _upperThresholds[i][_digitalMode];
        float lowerThresh = _lowerThresholds[i][_digitalMode];

        // Find the analog reading from the ith sensor with respect to the middle one.
        // Assuming that equal amounts of noise have been added to each sensor, this subtraction should remove its effect
        float pinValue = rawSensorReadings[i];

        // If the denoised reading lies within the specified thresholds, set the digital reading to 1.
        bool digitalValue = false;
        if ((pinValue >= lowerThresh) && (pinValue <= upperThresh)) {
            digitalValue = true;
        }

        // XNOR digitalValue with within gives the required result, but ~ for some reason gives strange values
        digitalValue = digitalValue ^ within;
        if (digitalValue) {
            targetArray[i] = 0;
        } else {
            targetArray[i] = 1;
        }

        // Serial.print(targetArray[i]);
        // Serial.print(" : ");
    }
}

void IRSensorArray::getAnalogReadingsArray(float *targetArray) {
    // Read all the sensors more than one time
    byte numReadings = 5;

    for (byte i = 0; i < _numPins; i++) {
        float pinValue = 0;
        for (byte j = 0; j < numReadings; j++) {
            pinValue += analogRead(_pins[i]); // put a delay? increase num of readings?
        }
        targetArray[i] = (float)(pinValue / numReadings);
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

bool IRSensorArray::patternDetected(byte *patternArray, byte mode = USE_THRESHES) {
    byte digitalReadings[_numPins];
    getDigitalReadingsArray(digitalReadings, mode);

    for (byte i = 0; i < _numPins; i++) {
        if (digitalReadings[i] != patternArray[i]) {
            return false;
        }
    }

    return true;
}
