#include <stdint.h>
#include <string.h>
#include "pwm.h"

// #ifndef IOREG32
// #define IOREG32(base_addr, offset) (*(volatile unsigned long *) (base_addr + offset ))
// #endif

//base address
#define CH1_BASE 0x4001C000
#define CH2_BASE 0x40021000
#define CH3_BASE 0x40022000
#define CH4_BASE 0x4002D000

//channel number
#define CH1 1
#define CH2 2
#define CH3 3
#define CH4 4


#define PWM_STOP_OFFSET       		0x004     // stop pwm
#define PWM_SEQ0START_OFFSET  		0x008     // start sequence 0
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

#define   PRESCALER_DIV32   5     //  divide by 32
#define PWM_CLK 500000

//#define PWM_PSEL            IOREG32(0x560)     
//TODO: pin select we need the PWN_SEL is according to the channel

static uint16_t s_sequence[2];


volatile unsigned long IOREG32(unsigned long offset, int channel){
    switch (channel)
    {
    case 1: return (*(volatile unsigned long *) (CH1_BASE + offset ));
    case 2: return (*(volatile unsigned long *) (CH2_BASE + offset ));
    case 3: return (*(volatile unsigned long *) (CH3_BASE + offset ));
    case 4: return (*(volatile unsigned long *) (CH4_BASE + offset ));

    }
    
}

struct channel
{
    volatile unsigned long PWM_STOP;
    volatile unsigned long PWM_SEQ0START;
    volatile unsigned long PWM_EVENTS_STOPPED;
    volatile unsigned long PWM_EVENTS_SEQ0END;
    volatile unsigned long PWM_EVENTS_PWMEND;
    volatile unsigned long PWM_ENABLE;
    volatile unsigned long PWM_COUNTERTOP;
    volatile unsigned long PWM_PRESCALER;
    volatile unsigned long PWM_DECODER;
    volatile unsigned long PWM_SEQ0PTR;
    volatile unsigned long PWM_SEQ0CNT;
    volatile unsigned long PWM_SEQ0REFRESH;
    volatile unsigned long PWM_SEQ0ENDDELAY;
};

void init_channel_registers(void){
    
    struct channel *ch[4];

    for(int i=1;i<=4;i++){
        ch[i]-> PWM_STOP = IOREG32(PWM_STOP_OFFSET,i);
        ch[i]-> PWM_SEQ0START = IOREG32(PWM_SEQ0START_OFFSET,i);
        ch[i]-> PWM_EVENTS_STOPPED = IOREG32(PWM_EVENTS_STOPPED_OFFSET,i);
        ch[i]-> PWM_EVENTS_SEQ0END = IOREG32(PWM_EVENTS_SEQ0END_OFFSET,i);
        ch[i]-> PWM_EVENTS_PWMEND = IOREG32(PWM_EVENTS_PWMEND_OFFSET,i);
        ch[i]-> PWM_ENABLE = IOREG32(PWM_ENABLE_OFFSET,i);
        ch[i]-> PWM_COUNTERTOP = IOREG32(PWM_COUNTERTOP_OFFSET,i);
        ch[i]-> PWM_PRESCALER = IOREG32(PWM_PRESCALER_OFFSET,i);
        ch[i]-> PWM_DECODER = IOREG32(PWM_DECODER_OFFSET,i);
        ch[i]-> PWM_SEQ0PTR = IOREG32(PWM_SEQ0PTR_OFFSET,i);
        ch[i]-> PWM_SEQ0CNT = IOREG32(PWM_SEQ0CNT_OFFSET,i);
        ch[i]-> PWM_SEQ0REFRESH = IOREG32(PWM_SEQ0REFRESH_OFFSET,i);
        ch[i]-> PWM_SEQ0ENDDELAY = IOREG32(PWM_SEQ0ENDDELAY_OFFSET,i);

    }


}

