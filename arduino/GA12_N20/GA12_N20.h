#ifndef GA12_N20_H
#define GA12_N20_H

#include <PinChangeInterrupt.h>

class Motor {
public:
    Motor(int fwd_pin, int bwd_pin, int pwm_pin, int enc1_pin, int enc2_pin);
    void motor_fwd(int pwm);
    void motor_bwd(int pwm);
    void motor_stop();
    int get_encoder1_count();
    int get_encoder2_count();
    void reset_counts();

    // Interrupt Service Routines
    static void updateEncoder1();
    static void updateEncoder2();

private:
    int _fwd_pin, _bwd_pin, _pwm_pin, _enc1_pin, _enc2_pin;
    volatile long int _count1, _count2; // Counts for 2 encoders
    static Motor* instance; // Static pointer to the current instance
};

#endif
