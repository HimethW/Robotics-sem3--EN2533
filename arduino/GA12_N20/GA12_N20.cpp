#include "GA12_N20.h"
#include <Arduino.h>

// Initialize static member
Motor* Motor::instance = nullptr;


// Init Motor

Motor::Motor(int fwd_pin, int bwd_pin, int pwm_pin, int enc1_pin, int enc2_pin)
            :_fwd_pin(fwd_pin), _bwd_pin(bwd_pin), _pwm_pin(pwm_pin), _enc1_pin(enc1_pin), _enc2_pin(enc2_pin){

            instance = this;

              pinMode(_fwd_pin, OUTPUT);
              pinMode(_bwd_pin, OUTPUT);
              pinMode(_pwm_pin, OUTPUT);

              pinMode(_enc1_pin, INPUT_PULLUP);
              pinMode(_enc2_pin, INPUT_PULLUP);

              // Attach interrupts using the static methods
              attachInterrupt(digitalPinToInterrupt(_enc1_pin), updateEncoder1, RISING);
              attachInterrupt(digitalPinToInterrupt(_enc2_pin), updateEncoder2, RISING);

              _count1 = 0;
              _count2 = 0;
            
            }

// Motor rotate forwards
void Motor::motor_fwd(int pwm){
  digitalWrite(_fwd_pin, HIGH);
  digitalWrite(_bwd_pin, LOW);
  analogWrite(_pwm_pin, pwm);
}

// Motor Rotate Backwards
void Motor::motor_bwd(int pwm){
  digitalWrite(_fwd_pin, LOW);
  digitalWrite(_bwd_pin, HIGH);
  analogWrite(_pwm_pin, pwm);
}

// Stop motor
void Motor::motor_stop() {
    analogWrite(_pwm_pin, 0);
}

// Get encoder 1 count
int Motor::get_encoder1_count(){
  return _count1;
}

// Get encoder 2 count
int Motor::get_encoder2_count(){
  return _count2;
}

void Motor::reset_counts(){
  _count1 = 0;
  _count2 = 0;
  
}


// ISR for encoder 1
void Motor::updateEncoder1() {
    if (instance) {
        instance->_count1++; // Accessing count via instance pointer
    }
}

// ISR for encoder 2
void Motor::updateEncoder2() {
    if (instance) {
        instance->_count2++; // Accessing count via instance pointer
    }
}

