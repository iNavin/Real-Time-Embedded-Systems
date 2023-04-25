#include <stdint.h>
#include <string.h>
#include "../rtx/cmsis_os2.h"
#include "serial.h"
#include "buggy.h"

#define MSGQUEUE_LENGTH 10 


osMessageQueueId_t controller_MsgQueue;
 
void check_controllerMsg(void );
void add_controllerMsg(char* command);
void execute_driver(char* command);


typedef struct{
    char* command;
    int* values[];
}ControllerMsg;

int init_controller(void){
  puts("init controller\n\r");
     controller_MsgQueue = osMessageQueueNew(MSGQUEUE_LENGTH, sizeof(ControllerMsg), NULL);
  if (controller_MsgQueue == NULL) {
    puts("Message Queue object not created, handle failure\n\r");
  }

  return 0;
 
}

void check_controllerMsg(void){
    ControllerMsg msg;
    osStatus_t status;
    uint16_t msgCount = osMessageQueueGetCount(controller_MsgQueue);

    puts("in check_controllerMsg\n\r");
    for(int i =0 ; i<msgCount; i++){
      status = osMessageQueueGet(controller_MsgQueue, &msg, NULL, 0U); 
      if (status == osOK) {
        char* command = msg.command;
        puts("command : ");
        puts(command);
        puts("\n\r");
        execute_driver(command);
        
      }
    }
}

void add_controllerMsg(char* command){
  ControllerMsg msg;
  msg.command = command;
  puts("in add_controllerMsg\n\r");
  osMessageQueuePut(controller_MsgQueue, &msg, 0U, 0U);
  puts("added the message in the queue with command :");
  puts(command);
  puts("\n\r");
  
}

void execute_driver(char* command){
  if(strcmp(command,"FORWARD")==0){
    buggy_move_forward(100);
    puts("MOVING FORWARD");
  }else if(strcmp(command,"BACKWARD")==0){
    buggy_move_reverse(100);
    puts("MOVING BACKWARD");
  }else if(strcmp(command,"LEFT")==0){
    buggy_spin_left();
    puts("MOVING LEFT");
  }else if(strcmp(command,"RIGHT")==0){
    buggy_spin_right();
    puts("MOVING RIGHT");
  }else if(strcmp(command,"STOP")==0){
    buggy_stop();
    puts("STOPPING");
  }
          
}