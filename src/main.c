/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Sun 20 Mar 2016 11:26:46 AM CST
	> two problem have:
		1.param_sched: can't find 
			struct forget
		2.pthread_creat: argument wrong
			add (void*) before function argument
 ************************************************************************/

// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include </usr/include/unistd.h>
// #include <sched.h>
// #include "../include/thread/netsend.h"
#include "../include/includes.h"

pthread_t emergency_tid;
pthread_t serialsend_tid;
pthread_t serialrecv_tid;
pthread_t realspeed_tid;
pthread_t handle_tid;
pthread_t netsend_tid;

int main(void)
{
	int err1,err2,err3,err4,err5,err6;
	void *res;

	pthread_attr_t emergency_attr; 
	pthread_attr_t serialrecv_attr; 
	pthread_attr_t serialsend_attr; 
	pthread_attr_t realspeed_attr; 
	pthread_attr_t handle_attr; 
	pthread_attr_t netsend_attr; 

	pthread_attr_init(&emergency_attr); 
	pthread_attr_init(&serialsend_attr); 
	pthread_attr_init(&serialrecv_attr); 
	pthread_attr_init(&realspeed_attr); 
	pthread_attr_init(&handle_attr); 
	pthread_attr_init(&netsend_attr); 

	int priority = 10; 
	struct sched_param param; 


	param.sched_priority = priority; 
	pthread_attr_setschedpolicy(&netsend_attr, SCHED_RR);
	pthread_attr_setschedparam(&netsend_attr, &param); 
	err1 = pthread_create(&netsend_tid, &netsend_attr, (void*)netsend_thread, NULL);
	if(err1 != 0)
	{
		printf("can't creat netsend %s\n", strerror(err1));
		exit(1);
	}
	err1 = pthread_join(netsend_tid, &res);
	if(err1 != 0)
	{
		printf("can't join thread %s\n", strerror(err1));
		exit(1);
	}

	param.sched_priority=priority; 
	pthread_attr_setschedpolicy(&emergency_attr, SCHED_RR);
	pthread_attr_setschedparam(&emergency_attr, &param); 
	err2 = pthread_create(&emergency_tid, &emergency_attr, (void*)emergency_thread, NULL);
	if(err2 != 0)
	{
		printf("can't creat emergency %s\n", strerror(err2));
		exit(1);
	}
	err2 = pthread_join(emergency_tid, &res);
	if(err1 != 0)
	{
		printf("can't join thread %s\n", strerror(err2));
		exit(1);
	}

	param.sched_priority=priority; 
	pthread_attr_setschedpolicy(&serialsend_attr, SCHED_RR);
	pthread_attr_setschedparam(&serialsend_attr, &param); 
	err5 = pthread_create(&serialsend_tid, &serialsend_attr, serialsend_thread, NULL);
	if(err5 != 0)
	{
		printf("can't creat serialsend thread %s\n", strerror(err5));
		exit(1);
	}
	err5 = pthread_join(serialsend_tid, &res);
	if(err5 != 0)
	{
		printf("can't join thread %s\n", strerror(err5));
		exit(1);
	}

	param.sched_priority=priority; 
	pthread_attr_setschedpolicy(&serialrecv_attr, SCHED_RR);
	pthread_attr_setschedparam(&serialrecv_attr, &param); 
	err3 = pthread_create(&serialrecv_tid, &serialrecv_attr, serialrecv_thread, NULL);
	if(err3 != 0)
	{
		printf("can't creat thread %s\n", strerror(err3));
		exit(1);
	}
	err3 = pthread_join(serialrecv_tid, &res);
	if(err3 != 0)
	{
		printf("can't join thread %s\n", strerror(err3));
		exit(1);
	}

	param.sched_priority=priority; 
	pthread_attr_setschedpolicy(&realspeed_attr, SCHED_RR);
	pthread_attr_setschedparam(&realspeed_attr, &param); 
	err4 = pthread_create(&realspeed_tid, &realspeed_attr, realspeed_thread, NULL);
	if(err4 != 0)
	{
		printf("can't creat thread %s\n", strerror(err4));
		exit(1);
	}
	err4 = pthread_join(realspeed_tid, &res);
	if(err4 != 0)
	{
		printf("can't join thread %s\n", strerror(err4));
		exit(1);
	}

	param.sched_priority=priority; 
	pthread_attr_setschedpolicy(&handle_attr, SCHED_RR);
	pthread_attr_setschedparam(&handle_attr, &param); 
	err6 = pthread_create(&handle_tid, &handle_attr, handle_thread,NULL);
	if(err6 != 0)
	{
		printf("can't creat thread %s\n", strerror(err6));
		exit(1);
	}
	err6 = pthread_join(handle_tid, &res);
	if(err6 != 0)
	{
		printf("can't join thread %s\n", strerror(err6));
		exit(1);
	}
}
