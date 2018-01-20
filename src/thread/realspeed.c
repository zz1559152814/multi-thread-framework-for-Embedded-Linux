/*************************************************************************
	> File Name: realspeed.c
	> Author: 
	> Mail: 
	> Created Time: Thu 17 Mar 2016 02:36:51 PM CST
 ************************************************************************/

#include "../../include/includes.h"
	
float  realspe,realspe_x,realspe_y;//really_speed//
float  pos[5][2]={0};
float  speed[5][2]={0};

int realspeed_thread();

int realspeed_thread()
{
	int n;
	while(1)
	{
		// printf("realspeed_thread");
		n=n%5;	

 		pos[n][0]=Recv_Data1.recvdata.GPS.position.x;
		pos[n][1]=Recv_Data1.recvdata.GPS.position.y;  
			
		if(n!=0)
		{
			speed[n][0]=sqrt(pow((pos[n-1][0]-pos[n][0]),2))/0.005;
			speed[n][1]=sqrt(pow((pos[n-1][1]-pos[n][1]),2))/0.005;
		}
		else
		{
			speed[n][0]=sqrt(pow((pos[4][0]-pos[n][0]),2))/0.005;
			speed[n][1]=sqrt(pow((pos[4][1]-pos[n][1]),2))/0.005;
		}	
			
		if(n==0)
		{
			realspe_x=(speed[4][0]+speed[3][0]+speed[2][0]+speed[0][0])/4;
			realspe_y=(speed[4][1]+speed[3][1]+speed[2][1]+speed[0][1])/4;
			realspe=sqrt(pow(realspe_x,2)+pow(realspe_y,2));
		}
		else if(n==1)
		{
			realspe_x=(speed[4][0]+speed[3][0]+speed[1][0]+speed[0][0])/4;
			realspe_y=(speed[4][1]+speed[3][1]+speed[1][1]+speed[0][1])/4;
			realspe=sqrt(pow(realspe_x,2)+pow(realspe_y,2));
		}
		else if(n==2)
		{
			realspe_x=(speed[4][0]+speed[1][0]+speed[2][0]+speed[0][0])/4;
			realspe_y=(speed[4][1]+speed[1][1]+speed[2][1]+speed[0][1])/4;
			realspe=sqrt(pow(realspe_x,2)+pow(realspe_y,2));
		}
		else if(n==3)
		{
			realspe_x=(speed[1][0]+speed[3][0]+speed[2][0]+speed[0][0])/4;
			realspe_y=(speed[1][1]+speed[3][1]+speed[2][1]+speed[0][1])/4;
			realspe=sqrt(pow(realspe_x,2)+pow(realspe_y,2));
		}		
		else if(n==4)
		{
			realspe_x=(speed[4][0]+speed[3][0]+speed[2][0]+speed[1][0])/4;
			realspe_y=(speed[4][1]+speed[3][1]+speed[2][1]+speed[1][1])/4;
			realspe=sqrt(pow(realspe_x,2)+pow(realspe_y,2));
		}	
		n++;
	}
	usleep(5000);    
}