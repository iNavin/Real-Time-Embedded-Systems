#ifndef PWM_H
#define PWM_H
#include <stdint.h>

//base address
#define CH1_BASE 0x4001C000
#define CH2_BASE 0x40021000
#define CH3_BASE 0x40022000
#define CH4_BASE 0x4002D000

#define NO_OF_CHANNEL 2

//channel number
#define CH1 1
#define CH2 2
#define CH3 3
#define CH4 4

void pwm_init(uint32_t pin, int channel_number);
void pwm_out(int freq, int duty, int duration_ms, int channel_number);
volatile unsigned long IOREG32(unsigned long offset, int channel);

#endif  /* PWM_H */
