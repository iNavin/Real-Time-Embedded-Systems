
#define LEFT_SERVO_PIN           4
#define RIGHT_SERVO_PIN          3
#define RIGHT_FWD_MAXCOUNT       1000    // 1ms pulse
#define RIGHT_REV_MAXCOUNT       2000    // 2ms pulse
#define LEFT_REV_MAXCOUNT        1000    // 1ms pulse
#define LEFT_FWD_MAXCOUNT        2000    // 2ms pulse
#define STOP_COUNT               1500    // 1.5ms pulse


void buggy_init();
void buggy_move_forward(int speed);
void buggy_move_reverse(int speed);
void buggy_spin_left();
void buggy_spin_right();
void buggy_stop();