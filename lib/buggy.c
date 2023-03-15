#include <stdint.h>
#include "pwm.h"
#include "buggy.h"


void buggy_init()
{
    pwm_init(LEFT_SERVO_PIN,CH1);
    pwm_init(RIGHT_SERVO_PIN,CH2);
}

// enter speed in percentage
void buggy_move_forward(int speed)
{
    // variables for speed control
    int left_count, right_count;
    left_count  = 1500 + speed * 2;
    right_count = 1500 - speed * 1.3;
    pwm_pulse5ms(left_count, CH1);
    pwm_pulse5ms(right_count, CH2);
}

void buggy_move_reverse(int speed)
{
    // variables for speed control
    int left_count, right_count;
    left_count  = STOP_COUNT - speed * 2;
    right_count = STOP_COUNT + speed * 1.3;
    pwm_pulse5ms(left_count, CH1);
    pwm_pulse5ms(right_count, CH2);
}

void buggy_spin_left()
{
    // variables for speed control
    int left_count, right_count;
    left_count  = STOP_COUNT;
    right_count = RIGHT_FWD_MAXCOUNT;
    pwm_pulse5ms(left_count, CH1);
    pwm_pulse5ms(right_count, CH2);
}

void buggy_spin_right()
{
    // variables for speed control
    int left_count, right_count;
    left_count  = LEFT_FWD_MAXCOUNT;
    right_count = STOP_COUNT;
    pwm_pulse5ms(left_count, CH1);
    pwm_pulse5ms(right_count, CH2);
}

void buggy_stop()
{
    pwm_off(CH1);
    pwm_off(CH2);
}