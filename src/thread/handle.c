/*************************************************************************
	> File Name: handle.c
	> Author: 
	> Mail: 
	> Created Time: Thu 17 Mar 2016 02:23:59 PM CST
 ************************************************************************/

#include "../../include/includes.h"

#define BUTTON(X) Buttons&(1<<X)
unsigned int another_way = 1;
/*--------------------------------*/
	
int handle_thread(void* parameter)
{
	float  	Handle_Speed_X;
	float  	Handle_Speed_Y;
	float  	Handle_Speed_R;
	int 	Buttons;
	while(1)
	{
		Handle_Speed_X = Recv_Data1.recvdata.handlevaule.HandleData_X;
		Handle_Speed_Y = Recv_Data1.recvdata.handlevaule.HandleData_Y;
		Handle_Speed_R = Recv_Data1.recvdata.handlevaule.HandleData_R;

		SetSpeed(Handle_Speed_X, Handle_Speed_Y, Handle_Speed_R);

		if(BUTTON(1))
		{

		}

		if(BUTTON(2))
		{

		}

		if(BUTTON(3))
		{

		}

		if(BUTTON(4))
		{
			
		}

		if(BUTTON(5))
		{
			
		}

		usleep(5000);
	}
}
