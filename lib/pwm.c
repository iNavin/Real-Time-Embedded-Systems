#include <stdint.h>
#include <string.h>
#include "pwm.h"

#define PWM_STOP_OFFSET       	   0x004     // stop pwm
#define PWM_SEQ0START_OFFSET  	   0x008     // start sequence 0
#define PWM_EVENTS_STOPPED_OFFSET  0x104     // event to indicate PWM stopped
#define PWM_EVENTS_SEQ0END_OFFSET  0x110     // event to indicate sequnce 0 ended
#define PWM_EVENTS_PWMEND_OFFSET   0x118     // event to indicate pwm period ended
#define PWM_ENABLE_OFFSET          0x500     // enable / disable pwm
#define PWM_COUNTERTOP_OFFSET      0x508     // to decide PWM frequncy                                               //   countertop = pwm clock / pwm freq
#define PWM_PRESCALER_OFFSET       0x50C     // divisor for PWM clock (2 power n)     
#define PWM_DECODER_OFFSET         0x510     // to set indivudial duty cycles of all 4 PWM channels we will use common configuration to set all 4 PWM to same duty cycle
#define PWM_SEQ0PTR_OFFSET         0x520     // address where the sequence is stored
#define PWM_SEQ0CNT_OFFSET         0x524     // number of values in the sequence
#define PWM_SEQ0REFRESH_OFFSET     0x528     // additional pwm cycles between two  values of the sequence 
#define PWM_SEQ0ENDDELAY_OFFSET    0x52C     // additional pwm cycles at the end

#define PRESCALER_DIV32   5     //  divide by 32
#define PRESCALER_DIV16   4     //  divide by 16
#define PWM_CLK 500000

static uint16_t s_sequence[2];

static unsigned int PWM_PSEL_OFFSET[] = {0x560, 0x564 , 0x568 , 0x56C}; 

//creating an array of channel structures
struct
{

    volatile unsigned long * PWM_STOP ;
    volatile unsigned long * PWM_SEQ0START;
    volatile unsigned long * PWM_EVENTS_STOPPED;
    volatile unsigned long * PWM_EVENTS_SEQ0END;
    volatile unsigned long * PWM_EVENTS_PWMEND;
    volatile unsigned long * PWM_ENABLE;
    volatile unsigned long * PWM_COUNTERTOP;
    volatile unsigned long * PWM_PRESCALER;
    volatile unsigned long * PWM_DECODER;
    volatile unsigned long * PWM_SEQ0PTR;
    volatile unsigned long * PWM_SEQ0CNT;
    volatile unsigned long * PWM_SEQ0REFRESH;
    volatile unsigned long * PWM_SEQ0ENDDELAY;
    volatile unsigned long * PWM_PSEL;

}channel[4];



volatile unsigned long* IOREG32(unsigned long offset, int channel){
    switch (channel)
    {
    case 1: return (volatile unsigned long *)(CH2_BASE + offset);
    case 2: return (volatile unsigned long *)(CH3_BASE + offset);
    case 3: return (volatile unsigned long *)(CH4_BASE + offset);
    case 4: return (volatile unsigned long *)(CH1_BASE + offset);
    default : return (volatile unsigned long *)(CH1_BASE + offset);

    }
    
    
}

void init_channel_registers(int i){
        channel[i].PWM_STOP = IOREG32(PWM_STOP_OFFSET,i);
        channel[i].PWM_SEQ0START = IOREG32(PWM_SEQ0START_OFFSET,i);
        channel[i].PWM_EVENTS_STOPPED = IOREG32(PWM_EVENTS_STOPPED_OFFSET,i);
        channel[i].PWM_EVENTS_SEQ0END = IOREG32(PWM_EVENTS_SEQ0END_OFFSET,i);
        channel[i].PWM_EVENTS_PWMEND = IOREG32(PWM_EVENTS_PWMEND_OFFSET,i);
        channel[i].PWM_ENABLE = IOREG32(PWM_ENABLE_OFFSET,i);
        channel[i].PWM_COUNTERTOP = IOREG32(PWM_COUNTERTOP_OFFSET,i);
        channel[i].PWM_PRESCALER = IOREG32(PWM_PRESCALER_OFFSET,i);
        channel[i].PWM_DECODER = IOREG32(PWM_DECODER_OFFSET,i);
        channel[i].PWM_SEQ0PTR = IOREG32(PWM_SEQ0PTR_OFFSET,i);
        channel[i].PWM_SEQ0CNT = IOREG32(PWM_SEQ0CNT_OFFSET,i);
        channel[i].PWM_SEQ0REFRESH = IOREG32(PWM_SEQ0REFRESH_OFFSET,i);
        channel[i].PWM_SEQ0ENDDELAY = IOREG32(PWM_SEQ0ENDDELAY_OFFSET,i);
        channel[i].PWM_PSEL = IOREG32(PWM_PSEL_OFFSET[i],i);
}

void pwm_init(uint32_t pin,int channel_number)
{
    init_channel_registers(channel_number);
    // PWM clock is 16 MHz. Hence, Prescaler divisor = 32 for 500 kHz clock 
    *(channel[channel_number].PWM_PRESCALER) = PRESCALER_DIV32;
    // Connect the port to the pin s
    *(channel[channel_number].PWM_PSEL) = pin;
    //Enable PWM
    *(channel[channel_number].PWM_ENABLE) = 1;
}

void pwm_out (int freq, int duty, int duration_ms, int channel_number)
{
    //int n_pwm_cycles = (freq * duration_ms) / 1000; // there is a diff. b/w this pwm.c and prev. assignments's pmw.c
    uint16_t countertop;

    countertop = PWM_CLK / freq;
    *(channel[channel_number].PWM_COUNTERTOP) = countertop;
    
     /* Set count values for specified duty cycle */
    s_sequence[0] = (countertop * duty) / 100;
    s_sequence[1] = (countertop * duty) / 100;

    *(channel[channel_number].PWM_SEQ0PTR) = (uint32_t) &s_sequence;
    *(channel[channel_number].PWM_SEQ0CNT) = 1;
    
    /* Start sequence 0 */
    *(channel[channel_number].PWM_SEQ0START) = 1;

}

void pwm_off (int channel_number)
{
    // Stop PWM
    *(channel[channel_number].PWM_STOP) = 1;
    while (channel[channel_number].PWM_EVENTS_STOPPED == 0)
    ;
    *(channel[channel_number].PWM_EVENTS_STOPPED) = 0;
}

// frequency = 200Hz
void pwm_pulse5ms(int pulse_duration_ms, int channel_number)
{
    uint16_t countertop;
    // setting 1Mhz clock
    *(channel[channel_number].PWM_PRESCALER) = PRESCALER_DIV16;
    countertop = 5000;

    s_sequence[0] = countertop - pulse_duration_ms;
    *(channel[channel_number].PWM_COUNTERTOP)  = countertop;  
    *(channel[channel_number].PWM_SEQ0PTR)     = (uint32_t) &s_sequence;
    *(channel[channel_number].PWM_SEQ0CNT)     = 1;
    *(channel[channel_number].PWM_SEQ0REFRESH) = 0;

    /* Start sequence 0 */
    *(channel[channel_number].PWM_SEQ0START)   = 1;    
 
}
