/*************************************************************************
	> File Name: netsend.c
	> Author: 
	> Mail: 
	> Created Time: Thu 17 Mar 2016 02:23:59 PM CST
 ************************************************************************/

#include "../../include/includes.h"

int netsend_thread()
{
    int nfp,sfp;
    struct sockaddr_in s_add,c_add;  
    int sin_size;  
    unsigned short portnum= 0x8887; /* 服务端使用端口 */  
      
    sfp = socket(AF_INET, SOCK_STREAM, 0);  
      
    bzero(&s_add,sizeof(struct sockaddr_in));  
    s_add.sin_family=AF_INET;  
    s_add.sin_addr.s_addr=htonl(INADDR_ANY); /* 这里地址使用全0，即所有 */  
    s_add.sin_port=htons(portnum);  
  
    bind(sfp,(struct sockaddr *)(&s_add), sizeof(struct sockaddr));

    while(1)  
    {  
        listen(sfp,15);
        sin_size = sizeof(struct sockaddr_in);  
        nfp = accept(sfp, (struct sockaddr *)(&c_add), &sin_size);          
    }

    close(sfp);  
    return 0;  
}
