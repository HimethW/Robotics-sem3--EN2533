/*
  LineFollower.cpp
  EN2533 - Robot Design and Competition
  Nilakna Warushavithana, October 2024
*/

#include "Arduino.h"
#include "Wheel.h"

Wheel::Wheel(byte in1, byte in2, byte enA) {
    _in1 = in1;
    _in2 = in2;
    _enA = enA;
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    pinMode(_enA, OUTPUT);
}

void Wheel::setSpeed(int speed) {
    analogWrite(_enA, speed);
}

void Wheel::forward() {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
}

bool Wheel::backward() {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
}

void Wheel::stop() {
    analogWrite(_enA, 0);
}

void Wheel::brake() {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
}