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

void IRSensorArray::setDigitalMode(byte mode) {
    _digitalMode = mode;
    // We set thresholds for each mode. e.g. WHT_LINE_BLK_BG = for this mode, we can set an interval of analog values
    // which the sensor reading should be within, or without, i.e., reading in [a, b] vs reading in (-infty, a] U [b, infty],
    // in order to consider the line to be detected.
}

void IRSensorArray::calibrate(float *mean, float *standardDeviation, float *analogReadings) {
    for (byte i = 0; i < _numPins; i++) {
        *mean += analogReadings[i];
        *standardDeviation += analogReadings[i] * analogReadings[i];
    }

    *mean = *mean / _numPins;
    *standardDeviation = *standardDeviation / _numPins;
    *standardDeviation = sqrt(*standardDeviation - ((*mean) * (*mean)));
}

void IRSensorArray::calibrateIndividual(float *mean, float *standardDeviation, byte index) {
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

// use threshed values, multiply by weights and output. 
float IRSensorArray::getWeightedDigitalReading() {
    float weightedReading = 0;
    
    byte digitalValues[_numPins];
    getDigitalReadingsArray(digitalValues);

    for (byte i = 0; i < _numPins; i++) {
        weightedReading += (float)(digitalValues[i] * _weights[i]);
    }

    return weightedReading;
}

// use direct digitalreads.
float IRSensorArray::getWeightedDigitalReading1() {
    float weightedReading = 0;

    for (byte i = 0; i < _numPins; i++) {
        byte pinValue = digitalRead(_pins[i]) ^ _invert; // again is there a type casting thing?
        weightedReading += pinValue * _weights[i];
    }

    return weightedReading;
}

void IRSensorArray::getDigitalReadingsArray(byte *targetArray) {
    byte numReadings = 10;
    bool within = _upperThresholds[_numPins][_digitalMode];

    float sensorReadings[_numPins] = {0};
    getAnalogReadingsArray(sensorReadings);
    float mean = 0;
    float stdDev = 0;
    calibrate(&mean, &stdDev, sensorReadings);

    for (byte i = 0; i < _numPins; i++) {

        float upperThresh = _upperThresholds[i][_digitalMode];
        float lowerThresh = _lowerThresholds[i][_digitalMode];

        float pinValue = sensorReadings[i]; //- mean;

        bool digitalValue = false;
        if ((pinValue >= lowerThresh) && (pinValue <= upperThresh)) {
            digitalValue = true;
        }

        digitalValue = digitalValue ^ within; // XNOR gives the required result
        if (digitalValue) {
            targetArray[i] = 0;
        } else {
            targetArray[i] = 1;
        }
        // Serial.print(targetArray[i]);
        // Serial.print(" : ");
    }
    /*Serial.print(sensorReadings[2]);
    Serial.print(" - mean = ");
    Serial.print(sensorReadings[2] - mean);
    Serial.print(", B/W ");
    Serial.print(_lowerThresholds[2][_digitalMode]);
    Serial.print(" & ");
    Serial.print(_upperThresholds[2][_digitalMode]);
    Serial.print("? : ");
    Serial.println(targetArray[2]);*/
}

// Digital read each pin directly.
void IRSensorArray::getDigitalReadingsArray1(byte *targetArray) {
    for (byte i = 0; i < _numPins; i++) {
        byte pinValue = digitalRead(_pins[i]) ^ _invert; // again is there a type casting thing?
        targetArray[i] = pinValue;
    }
}

void IRSensorArray::getDigitalReadingsArray2(byte *targetArray) {
    float differences[_numPins - 1];
    for (byte i = 0; i < _numPins - 1; i++) {
        float left = analogRead(_pins[i]);
        float leftNext = analogRead(_pins[i + 1]);
        differences[i] = abs(leftNext - left);
    }

}

void IRSensorArray::getAnalogReadingsArray(float *targetArray) {
    byte numReadings = 5;
    for (byte i = 0; i < _numPins; i++) {
        float pinValue = 0;
        for (byte j = 0; j < numReadings; j++) {
            pinValue += analogRead(_pins[i]); // put a delay? increase num of readings?
        }
        targetArray[i] = pinValue / numReadings;
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

bool IRSensorArray::patternDetected1(byte *patternArray) {
    for (byte i = 0; i < _numPins; i++) {
        if ((digitalRead(_pins[i]) ^ _invert) != patternArray[i]) {
            return false;
        }
    }
    return true;
}

bool IRSensorArray::patternDetected(byte *patternArray) {
    byte digitalReadings[_numPins];
    getDigitalReadingsArray(digitalReadings);
    for (byte i = 0; i < _numPins; i++) {
        if (digitalReadings[i] != patternArray[i]) {
            return false;
        }
    }
    return true;
}
