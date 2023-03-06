#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "./lib/board.h"
#include "./lib/lib.h"

#include "./rtx/cmsis_os2.h"

uint8_t frame_buffer[LED_NUM_ROWS][LED_NUM_COLS];

void timer_callback(void *arg)
{
    static int first = 0;
    if (first == 0)
    {
        first = 1;

        printf("hello, timer!\n");

        frame_buffer[1][1] = 1;
        frame_buffer[1][3] = 1;
        frame_buffer[2][2] = 1;
        frame_buffer[3][1] = 1;
        frame_buffer[3][3] = 1;
    }

    frame_buffer[1][1] ^= 1;
    frame_buffer[1][2] ^= 1;
    frame_buffer[1][3] ^= 1;
    frame_buffer[2][1] ^= 1;
    frame_buffer[2][3] ^= 1;
    frame_buffer[3][1] ^= 1;
    frame_buffer[3][2] ^= 1;
    frame_buffer[3][3] ^= 1;
}

#define MAX_COUNT 100
static int count;

uint32_t r, c;
uint32_t r0, c0;

static void task1(void *arg)
{
    printf("hello, task1!\n");

    while (1)
    {
        frame_buffer[r][c] = 1;
        led_display(frame_buffer);
        frame_buffer[r][c] = 0;

        r += r0;
        c += c0;

        if ((r == 0) && (c == 0))
        {
            c0 = 1; r0 = 0;
        }
        else if ((r == 0) && ((c == (LED_NUM_COLS - 1))))
        {
            c0 = 0; r0 = 1;
        }
        else if ((r == (LED_NUM_ROWS - 1)) && ((c == (LED_NUM_COLS - 1))))
        {
            c0 = -1; r0 = 0;
        }
        else if ((r == (LED_NUM_ROWS - 1)) && (c == 0))
        {
            c0 = 0; r0 = -1;
        }

        count++;
        if (count == MAX_COUNT)
        {
            count = 0;
            osThreadYield();
        }
    }
}

static void task2(void *arg)
{
    printf("hello, task2!\n");

    while (1)
    {
        frame_buffer[r][c] = 1;
        led_display(frame_buffer);
        frame_buffer[r][c] = 0;

        r += r0;
        c += c0;

        if ((r == 0) && (c == 0))
        {
            r0 = 1; c0 = 0;
        }
        else if ((c == 0) && ((r == (LED_NUM_ROWS - 1))))
        {
            r0 = 0; c0 = 1;
        }
        else if ((c == (LED_NUM_COLS - 1)) && ((r == (LED_NUM_ROWS - 1))))
        {
            r0 = -1; c0 = 0;
        }
        else if ((c == (LED_NUM_COLS - 1)) && (r == 0))
        {
            r0 = 0; c0 = -1;
        }

        count++;
        if (count == MAX_COUNT)
        {
            count = 0;
            osThreadYield();
        }
    }
}

void led_x(void)
{
    int r, c;

    for (r = 0; r < LED_NUM_ROWS; r++)
        for (c = 0; c < LED_NUM_COLS; c++)
            frame_buffer[r][c] = ((r == c) || (r + c == LED_NUM_ROWS - 1));
}

void app_main(void *arg)
{
    osTimerId_t timer_id;
    osThreadId_t tid1, tid2;

    tid1 = osThreadNew(task1, NULL, NULL);
    tid2 = osThreadNew(task2, NULL, NULL);

    osThreadSetPriority(tid1, osPriorityNormal);
    osThreadSetPriority(tid2, osPriorityNormal);

    timer_id = osTimerNew (timer_callback, osTimerPeriodic, NULL, NULL);
    osTimerStart (timer_id, 500);
}

int main(void)
{
    /* Initialiazation */
    board_init();

    /* Greetings */
    puts("hello, world!\n");
    audio_sweep(100, 2000, 200);

    /* Initialize and start the kernel */
    osKernelInitialize();
    osThreadNew(app_main, NULL, NULL);
    // osTimerId_t timer_id;
    // osThreadId_t tid1, tid2;

    // tid1 = osThreadNew(task1, NULL, NULL);
    // tid2 = osThreadNew(task2, NULL, NULL);

    // osThreadSetPriority(tid1, osPriorityNormal);
    // osThreadSetPriority(tid2, osPriorityNormal);

    // timer_id = osTimerNew (timer_callback, osTimerPeriodic, NULL, NULL);
    // osTimerStart (timer_id, 500);

    osKernelStart();
    /* never returns */

    led_blink(2, 2, BLINK_FOREVER);// if the OosKernel start Doesn't work then this (2,2) led would blink

    return 0;
}
