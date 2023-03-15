#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "./lib/board.h"
#include "./lib/lib.h"
#include "buggy.h"
#include "./rtx/cmsis_os2.h"
#include "serial.h"

#define FORWARD 0
#define REVERSE 1
#define SPIN_LEFT 2
#define SPIN_RIGHT 3
#define STOP 4

#define MAX_COUNT 20
static int count;


volatile int current_state = FORWARD ;
volatile int next_state = FORWARD;

void timer_callback(void *arg)
{
    // states
    count ++;
    if (count == MAX_COUNT)
    {
        count = 0;
        next_state = (current_state+1) % 5;
    }
}


static void task1(void *arg)
{
    while(1)
    {
        
    // motor controller
    switch(current_state)
    {
        case FORWARD: buggy_move_forward(100);
                      //puts("FORWARD");
                      break;
        case REVERSE: buggy_move_reverse(100);
                      break;
        case SPIN_LEFT: buggy_spin_left();
                      break;
        case SPIN_RIGHT: buggy_spin_right();
                      break;
        case STOP: buggy_stop();
                      break;
    }
    

    // busy wait
    while (current_state == next_state)
        {
            continue;
        }
    puts("State Change");
    current_state = next_state;

    }

}


void app_main(void *arg)
{
    osTimerId_t timer_id;
    osThreadId_t tid1;

    tid1 = osThreadNew(task1, NULL, NULL);

    osThreadSetPriority(tid1, osPriorityNormal);

    timer_id = osTimerNew (timer_callback, osTimerPeriodic, NULL, NULL);
    osTimerStart (timer_id, 160);
}

int main(void)
{
    serial_init(40, 6);
    buggy_init();
    /* Initialize and start the kernel */
    osKernelInitialize();
    osThreadNew(app_main, NULL, NULL);
    
    osKernelStart();
    /* never returns */

    led_blink(2, 2, BLINK_FOREVER);// if the OosKernel start Doesn't work then this (2,2) led would blink
    return 0;
}