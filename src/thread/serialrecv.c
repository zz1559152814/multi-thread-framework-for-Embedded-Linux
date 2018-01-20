/*************************************************************************
	> File Name: serialrecv.c
	> Author: 
	> Mail: 
	> Created Time: Thu 17 Mar 2016 02:23:59 PM CST
 ************************************************************************/

#include "../../include/includes.h"


union uRecvData Recv_Data1; 
struct RecvData tmp_recv;

int getvaule(struct RecvData tmp_recv);

int serialrecv_thread()
{
    int nread,i;    
    int fd;
	
	while(1)
    {
        printf("serialrecv\n");
        getvaule(tmp_recv);
        Recv_Data1.recvdata = tmp_recv;
        // printf("Recv_Data1.recvdata.HandleData_X:%d",Recv_Data1.recvdata.handlevaule.HandleData_X);

    	if((fd=open_port(fd,0))<0)
        {
            printf("serialrecv_open_port error\n");
            return;
        }
        if((i=set_opt(fd,115200,8,'N',1))<0)
        {
            printf("serialrecv_set_opt error\n");
            return;
        }
        printf("fd=%d\n",fd);

		if(read(fd,Recv_Data1.buff,sizeof(struct RecvData*)) <= sizeof(struct RecvData*));
        {
        	printf("all data have recviced is %d", sizeof(struct RecvData*));
        	return;
        }

		close(fd);

		usleep(5000);
    }
    return;
}


int getvaule(struct RecvData tmp)
{
    tmp.GPS.position.x = 1;
    tmp.GPS.position.y = 1;
    tmp.GPS.radian     = 1;
    tmp.handlevaule.HandleData_X = 1; 
    tmp.handlevaule.HandleData_Y = 1;
    tmp.handlevaule.HandleData_R = 1;
    tmp.handlevaule.Buttons = 1;
}
