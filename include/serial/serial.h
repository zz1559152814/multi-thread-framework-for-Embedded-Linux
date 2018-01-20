/*************************************************************************
	> File Name: serial.h
	> Author: 
	> Mail: 
	> Created Time: Sun 20 Mar 2016 11:38:41 AM CST
 ************************************************************************/

#ifndef _SERIAL_H
#define _SERIAL_H

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
int open_port(int fd,int comport);

#endif
