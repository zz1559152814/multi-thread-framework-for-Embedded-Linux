/*************************************************************************
	> File Name: emergency.c
	> Author: 
	> Mail: 
	> Created Time: Thu 17 Mar 2016 02:30:33 PM CST
 ************************************************************************/
/*按键测试程序*/

#include "../../include/includes.h"
	
#define STOPALL 
int emergency_thread();
int killall(void);
int stopall(void);

int emergency_thread()
{
    int i; /*键盘输出时用到的循环变量*/
	int buttons_fd; /*buttons设备号*/
	int key_value[6]; /*四个按键的取值*/

	/*打开键盘设备文件*/
	buttons_fd = open("/dev/buttons_my",0); /*以0方式打开*/
	/*打开出错处理*/
	if(buttons_fd < 0) /*打开出错就会返回一个负值*/
	{
		perror("open device buttons"); /*perror函数?*/
	   	// exit(1); /*返回1*/
	}

	/*for无限循环,等待用户输入*/
	/*这是很典型的程序执行方式*/
	for(;;)
	{
		fd_set rds; /*fd_set是types.h中定义的类型,实质上是int型*/ /*rds用来存储设备号*/
		int ret; /*for循环内定义的局部变量ret*/
		FD_ZERO(&rds); /*rds初始化*/
		/*FD_ZERO是哪里定义的呢?*/
	  	FD_SET(buttons_fd,&rds); /*将buttons设备号赋给rds*/
		/*FD_SET是哪里定义的呢?*/
	   	/*使用系统调用select检查是否能够从/dev/buttons设备读取数据*/
		/*select函数是干什么的呢?*/
	   	ret = select(buttons_fd + 1,&rds,NULL,NULL,NULL);
	   	/*对ret的处理*/
		if(ret < 0) /*当ret小于0*/
		{
			perror("select");
			exit(1);
		}    

		if(ret == 0) /*当ret等于0*/
		{
			printf("Timeout.\n");
		}
		else /*能够读到数据*/
		{
			if(FD_ISSET(buttons_fd,&rds)) /*??*/
			{
				/*读取键盘驱动发出的数据*/
				/*key_value和键盘驱动中定义一致*/
				int ret = read(buttons_fd,key_value,sizeof(key_value)); /*注意此处的ret和前面的ret有何不同*/
				/*注意键盘设备读取的特点*/
			    /*对ret的处理*/
				if(ret != sizeof(key_value)) /*没有接收够*/
				{
					if(errno != EAGAIN)   /*???*/
					perror("read buttons\n");
					continue;
				}
				else /*正确接收,则打印到标准终端*/
				{
					for(i = 0;i < 6;i++) /*最开始定义的循环变量i*/
						printf("K%d %s, key value = 0x%02x\n",i,(key_value[i] & 0x80) ? "released" : key_value[i] ? "pressed down" : "",key_value[i]);
					stopall();
					killall();
					/*这一连串的输出,要注意格式*/
				}
			}
		}
		usleep(5000);
	}
	/*关闭设备*/
	close(buttons_fd);
	return 0; /*主函数返回*/
}

int stopall(void)
{
	Send_Data1.senddata.wheelmotor.fl_speed = 0;
	Send_Data1.senddata.wheelmotor.bl_speed = 0;
	Send_Data1.senddata.wheelmotor.fr_speed = 0;
	Send_Data1.senddata.wheelmotor.br_speed = 0;
}

int killall(void)
{
	int kill_rc;

	pthread_t emergency_tid;
	pthread_t serialsend_tid;
	pthread_t serialrecv_tid;
	pthread_t realspeed_tid;
	pthread_t handle_tid;
	pthread_t netsend_tid;
	

	kill_rc = pthread_kill(serialsend_tid, 0);
	if(kill_rc == ESRCH)
		printf("the serialsend thread did not exists or already quit\n"); 

	kill_rc = pthread_kill(serialrecv_tid, 0);
	if(kill_rc == ESRCH)
		printf("the serialrecv thread did not exists or already quit\n"); 

	kill_rc = pthread_kill(realspeed_tid, 0);
	if(kill_rc == ESRCH)
		printf("the realspeed thread did not exists or already quit\n"); 

	kill_rc = pthread_kill(handle_tid, 0);
	if(kill_rc == ESRCH)
		printf("the handle thread did not exists or already quit\n"); 

	kill_rc = pthread_kill(netsend_tid, 0);
	if(kill_rc == ESRCH)
		printf("the netsend thread did not exists or already quit\n"); 

	kill_rc = pthread_kill(emergency_tid, 0);
	if(kill_rc == ESRCH)
		printf("the emergency thread did not exists or already quit\n"); 
}
