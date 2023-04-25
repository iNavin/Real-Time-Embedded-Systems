#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "./lib/board.h"
#include "./lib/lib.h"
#include "./rtx/cmsis_os2.h"
#include "serial.h"
#include "./lib/controller.c"

int main(void)
{
    
    serial_init(40, 6);
    puts("serial monitor up and running\n\r");
    buggy_init();
    
    /* Initialize and start the kernel */
    osKernelInitialize();
    //osThreadNew(check_controller, NULL, NULL);
    init_controller();
    add_controllerMsg("FORWARD");
    //add_controllerMsg("BA CKWARD");
    check_controllerMsg();
    
    osKernelStart();
    /* never returns */

    led_blink(2, 2, BLINK_FOREVER);// if the OosKernel start Doesn't work then this (2,2) led would blink
    return 0;
}