#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <stdlib.h>
#include <stdio.h>

#include "net.h"

#define UDP_PC_PORT 12582

#define TYPE_ARM    1
#define TYPE_JOB    2

#define CMD_START 	1
#define CMD_SYSTIME 2 
#define CMD_SYSSTAT 3 

#define CMD_JOBCFG 	1
#define CMD_JOBOP  	2
#define CMD_JOBTEST 3	


extern int picture_count ;
extern int start ;

void * net_service_offline(void * p)
{
	printf("net_service_offline is running ...\n");
	pthread_exit(NULL) ;
}

int do_arm_cmd(char *buf ,int lenth ,struct sockaddr_in *paddr)
{
	NetMsg *netmsg = (NetMsg *)buf ;
	switch (netmsg -> cmd)
	{
	case CMD_START :
		picture_count = 0 ;
		start = 1 ;
		break ;
	case CMD_SYSTIME:
		break ;
	case CMD_SYSSTAT:
		break ;
	}
	return 0 ;

}

int do_job_cmd(char *buf , int lenth , struct sockaddr_in * paddr)
{	
	NetMsg *netmsg = (NetMsg *)buf ;
	switch (netmsg -> cmd)
	{
	case CMD_JOBCFG :
		printf("job config........\n");
		sleep(2);
		break;
	case CMD_JOBOP:
		printf("job oping .........\n");
		sleep(2);
		break ;
	default:
		break ;

	}

	return 0 ;

}


int net_do_cmd(char * buf , int lenth ,struct sockaddr_in *paddr)
{
	NetMsg *netmsg = (NetMsg *)buf ;
	switch (netmsg -> type)
	{
	case TYPE_ARM:
		do_arm_cmd(buf,lenth,paddr);
		break;
	case TYPE_JOB:
		do_job_cmd(buf,lenth,paddr);
		break ;
	default:
		break ;

	}

	return 0 ;
}


void * net_service_pc(void *p )
{
	int fd_pc ;
	int ret ;
	int recvlenth ;
	struct sockaddr_in local_addr ;
	struct sockaddr_in pc_addr;
	socklen_t socket_lenth ;
	char pbuf[1024];

	bzero(&pc_addr , sizeof(struct sockaddr_in));
	bzero(&local_addr , sizeof(struct sockaddr_in));
	local_addr.sin_family = AF_INET ;
	local_addr.sin_port   = htons(UDP_PC_PORT);
	local_addr.sin_addr.s_addr   =  INADDR_ANY ;

	printf("net_service_pc is running.........\n");


	fd_pc = socket(AF_INET,SOCK_DGRAM ,0 );

	if(fd_pc < 0)
	{
		perror("socket fd_pc failed ");
		pthread_exit(NULL);
	}

	ret = bind(fd_pc, (struct sockaddr *)&local_addr, sizeof(struct sockaddr));

	if(ret < 0)
	{
		perror("bind failed ");
		close(fd_pc);
		pthread_exit(NULL);
	}

	while(1)
	{
		recvlenth = recvfrom(fd_pc, pbuf ,sizeof(pbuf) , 0 ,(struct sockaddr *)&pc_addr, &socket_lenth);

		if(recvlenth < 0)
		{
			continue ; 
		}

		net_do_cmd(pbuf ,recvlenth , &pc_addr);

	}

	pthread_exit(NULL);

}




int init_net()
{
	pthread_t  pthread_id_pc , pthread_id_offline ;
	int ret ;

	ret = pthread_create(&pthread_id_pc , NULL , net_service_pc ,NULL);

	if(ret < 0)
	{
		perror("pthread_create net_service_pc failed ");
		return -1 ;
	}

	ret = pthread_create(&pthread_id_offline , NULL , net_service_offline ,NULL);

	if(ret < 0)
	{
		perror("pthread_create net_service_offline failed ");
		return -1 ;
	}


	pthread_detach(pthread_id_pc);
	pthread_detach(pthread_id_offline);

	return 0 ;
}
