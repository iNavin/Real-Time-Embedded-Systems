#include <stdint.h>
#include "buggy.h"

#define LEFT_SERVO_PIN           4
#define RIGHT_SERVO_PIN          3
#define RIGHT_FWD_MAXCOUNT       1000    // 1ms pulse
#define RIGHT_REV_MAXCOUNT       2000    // 2ms pulse
#define LEFT_REV_MAXCOUNT        1000    // 1ms pulse
#define LEFT_FWD_MAXCOUNT        2000    // 2ms pulse
#define STOP_COUNT               1500    // 1.5ms pulse

int main()
{
    buggy_init();
    buggy_move_forward(100);
    return 0;
}