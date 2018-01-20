/*************************************************************************
	> File Name: includes.h
	> Author: 
	> Mail: 
	> Created Time: Sun 20 Mar 2016 09:28:11 AM CST
 ************************************************************************/

#ifndef _INCLUDES_H
#define _INCLUDES_H
#define pi 3.1415926
// #include "MyFile/speed_test.h"

// #include "Route/GoCircle.h"
// #include "Route/EasyLine.h"
// #include "Route/GoLine.h"
// #include "Route/Keep.h"
// #include "Route/PointPath.h"
// #include "Route/PointPath.h"
#include "Route/mecanum.h"

// #include "general_func/MyMath.h"
// #include "general_func/MyString.h"
// #include "general_func/MyTypeDef.h"
// #include "general_func/PID.h"

#include "thread/emergency.h"
#include "thread/handle.h"
#include "thread/netsend.h"
#include "thread/realspeed.h"
#include "thread/serialrecv.h"
#include "thread/serialsend.h"
#include "thread/mydef.h"
#include "serial/serial.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <sched.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <math.h>
#include <sys/socket.h>  
#include <linux/in.h>  
#include <signal.h>

extern pthread_t emergency_tid;
extern pthread_t realspeed_tid;
extern pthread_t handle_tid;
extern pthread_t netsend_ti;

#endif
