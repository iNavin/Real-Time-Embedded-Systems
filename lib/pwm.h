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
#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3

void pwm_init(uint32_t pin, int channel_number);
void pwm_out(int freq, int duty, int duration_ms, int channel_number);
volatile unsigned long* IOREG32(unsigned long offset, int channel);
void pwm_pulse5ms(int pulse_duration_ms, int channel_number);
#endif  /* PWM_H */
