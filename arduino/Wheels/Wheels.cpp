/*
  Wheels.cpp
  EN2533 - Robot Design and Competition
  Nilakna Warushavithana, October 2024
*/

#include "Arduino.h"
#include "Wheels.h"

Wheel::Wheel(byte controlPinA, byte controlPinB, byte speedPin) {
    _controlPinA = controlPinA;
    _controlPinB = controlPinB;
    _speedPin = speedPin;
    encoderCount = 0;

    pinMode(controlPinA, OUTPUT);
    pinMode(controlPinB, OUTPUT);
    pinMode(speedPin, OUTPUT);
}

void Wheel::setEncoderPins(byte encoderPin1, byte encoderPin2) {
    _encoderPin1 = encoderPin1;
    _encoderPin2 = encoderPin2;

    pinMode(encoderPin1, INPUT_PULLUP);
    pinMode(encoderPin2, INPUT_PULLUP);
}

void Wheel::updateEncoderCount() { // To be called when _encoderPin1 is interrupted
    encoderCount++;
}

void Wheel::resetEncoderCount() { // To be called when _encoderPin1 is interrupted
    encoderCount = 0;
}

void Wheel::setSpeed(int speed) {
    analogWrite(_speedPin, speed);
}

void Wheel::forward() {
    digitalWrite(_controlPinA, HIGH);
    digitalWrite(_controlPinB, LOW);
}

void Wheel::backward() {
    digitalWrite(_controlPinA, LOW);
    digitalWrite(_controlPinB, HIGH);
}

void Wheel::stop() {
    analogWrite(_speedPin, 0);
}

void Wheel::brake() {
    digitalWrite(_controlPinA, LOW);
    digitalWrite(_controlPinB, LOW);
}

