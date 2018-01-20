/*************************************************************************
	> File Name: serialsend.c
	> Author: 
	> Mail: 
	> Created Time: Thu 17 Mar 2016 02:28:38 PM CST
 ************************************************************************/

#include "../../include/includes.h"

union uSendData Send_Data1; 
struct SendData tmp_send;

int setvaule(struct SendData tmp_send);

int serialsend_thread()
{
    int nread,i;    
    int fd;

    while(1)
    {
        printf("serialsend\n");
        setvaule(tmp_send);
        Send_Data1.senddata = tmp_send;
    
        if((fd=open_port(fd,0))<0)
        {
            printf("serialsend_open_port error\n");
            return;
        }
        if((i=set_opt(fd,115200,8,'N',1))<0)
        {
            printf("serialsend_set_opt error\n");
            return;
        }
        printf("fd=%d\n",fd);

        if(write(fd,Send_Data1.buff,sizeof(&Send_Data1)) <= sizeof(&Send_Data1));
        {
        	printf("all data have sended is %d", sizeof(&Send_Data1));
        	return;
        }
		close(fd);

		usleep(5000);
    }
    return;
}

int setvaule(struct SendData tmp)
{
    tmp.Switch_8 = 1;
    tmp.motor[0] = 1;
    tmp.motor[1] = 1;
    tmp.motor[2] = 1;
    tmp.wheelmotor.fr_speed = fr_speed;
    tmp.wheelmotor.br_speed = br_speed;
    tmp.wheelmotor.bl_speed = bl_speed;
    tmp.wheelmotor.fl_speed = fl_speed;
}
