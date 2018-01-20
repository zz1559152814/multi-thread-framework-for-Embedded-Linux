/*************************************************************************
	> File Name: netsend.c
	> Author: 
	> Mail: 
	> Created Time: Thu 17 Mar 2016 02:23:59 PM CST
 ************************************************************************/

#include "../../include/includes.h"
#include <time.h>

int netsend_thread();
int senddata(void);  
void pack_data(char* buffer);
void close_nfp(int signo);
union sSenddata netsenddata;

/*global variable*/
int nfp;
int sfp;
pthread_t tid;

int netsend_thread()
{
    struct sockaddr_in s_add,c_add;  
    int sin_size;  
    unsigned short portnum= 0x8887; 
      
    printf("Hello,welcome to my server!\r\n");  
    sfp = socket(AF_INET, SOCK_STREAM, 0);  
    if(-1 == sfp)  
    {  
        printf("socket fail ! \r\n");  
        return -1;  
    }  
    printf("socket ok !\r\n");  
      
    bzero(&s_add,sizeof(struct sockaddr_in));  
    s_add.sin_family=AF_INET;  
    s_add.sin_addr.s_addr=htonl(INADDR_ANY);  
    s_add.sin_port=htons(portnum);  
    
    if(-1 == bind(sfp,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))  
    {  
        printf("bind fail !\r\n");  
        return -1;  
    }  
    printf("bind ok !\r\n");  
    
    if(-1 == listen(sfp,15))  
    {  
        printf("listen fail !\r\n");  
        return -1;  
    }  
    printf("listen ok\r\n");  
      
    while(1)  
    {  
        printf("netsend\n");
        sin_size = sizeof(struct sockaddr_in);  

        int err;
        void *res;
    
        nfp = accept(sfp, (struct sockaddr *)(&c_add), &sin_size);  
        printf("nfp:%d\n",nfp);
        if(-1 == nfp)  
        {  
            printf("accept fail !\r\n");  
            return -1;  
        }  
        printf("accept ok!\r\nServer start get connect from %#x : %#x\r\n",ntohl(c_add.sin_addr.s_addr),ntohs(c_add.sin_port));  
    
        err = pthread_create(&tid, NULL, (void*)senddata, NULL);
        if(err != 0)
        {
            printf("can't creat netsend %s\n", strerror(err));
            exit(1);
        }
        err = pthread_join(tid, &res);
        if(err != 0)
        {
            printf("can't join thread %s\n", strerror(err));
            exit(1);
        }

        usleep(5000);
    }

    close(sfp);  
    return 0;  
}

void close_nfp(int signo)
{
    printf("receviced %d",signo);
    pthread_exit(&tid);
    close(nfp);
    close(sfp);
    return;
}

int senddata(void)
{
    char buffer[1024];

    signal(SIGINT,close_nfp);
	time_t timep;
    while(1)
    {
    	time(&timep);
		printf("%s\n",asctime(gmtime(&timep)));
        pack_data(buffer);
        if(-1 == write(nfp,buffer,1024))  
        {  
            printf("write fail!\r\n");  
            return -1;  
        }
        usleep(500000);
    }
}

void pack_data(char* buffer)
{
    netsenddata.netsend.recvdata   = tmp_recv;
    netsenddata.netsend.senddata   = tmp_send;
    return;
}

