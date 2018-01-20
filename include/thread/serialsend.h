#ifndef __SERIALSEND_H
#define __SERIALSEND_H
#include "../includes.h"


/*
Motor1 : beat motor
Motor2 : Undefined
Motor3 : Undefined

Switch_8 : for proximity sensor and electron magnetic valve

wheelmotor : set speed of four wheels

handlevaule : save the vaule from handle
*/

extern union uSendData Send_Data1;
extern struct SendData tmp_send;

int serialsend_thread();

#endif