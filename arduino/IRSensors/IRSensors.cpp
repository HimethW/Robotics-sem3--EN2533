/*
  IRSensors.cpp
  EN2533 - Robot Design and Competition
  Pulasthi Udugamasooriya, 5 October, 2024
*/

#include "Arduino.h"
#include "IRSensors.h"

IRSensorArray::IRSensorArray(byte *pins, byte numPins) {
    _pins = (byte*)malloc(sizeof(byte) * numPins);
    _numPins = numPins;

    for (byte i = 0; i < numPins; i++) {
        _pins[i] = pins[i];
        pinMode(pins[i], INPUT);
    }
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

float IRSensorArray::getWeightedDigitalReading() {
    float weightedReading;

    for (byte i = 0; i < _numPins; i++) {
        float pinValue = digitalRead(_pins[i]);
        weightedReading += pinValue * _weights[i];
    }

    return weightedReading;
}

bool IRSensorArray::junctionMet() {
    for (byte i = 0; i < _numPins; i++) {
        if (digitalRead(_pins[i]) == 0) {
            return false;
        }
    }
    return true;
}