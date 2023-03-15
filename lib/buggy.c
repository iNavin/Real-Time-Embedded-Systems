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
    left_count  = 1500 + speed * 5;
    right_count = 1500 - speed * 5;
    pwm_pulse5ms(left_count, CH1);
    pwm_pulse5ms(right_count, CH2);
}