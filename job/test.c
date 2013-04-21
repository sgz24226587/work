#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct NETMSG{
	char cmd ;
	char type ;
	char lenth ;
	char msg[512];
}NetMsg ;

#define TYPE_ARM    1
#define TYPE_JOB    2

#define CMD_START 	1
#define CMD_SYSTIME 2 
#define CMD_SYSSTAT 3 

#define CMD_JOBCFG 	1
#define CMD_JOBOP  	2
#define CMD_JOBTEST 3	

int  main( )
{
	int fd_pc ;
	int ret ;
	int recvlenth ;
	struct sockaddr_in server_addr ;
	struct sockaddr_in pc_addr;
	socklen_t socket_lenth ;
	char pbuf;
	NetMsg sbuf;

	bzero(&pbuf ,sizeof(pbuf));
	bzero(&sbuf ,sizeof(sbuf));

	bzero(&pc_addr , sizeof(struct sockaddr_in));
	bzero(&server_addr , sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET ;
	server_addr.sin_port   = htons(12582);
	server_addr.sin_addr.s_addr   =  inet_addr("127.1") ;


	fd_pc = socket(AF_INET,SOCK_DGRAM ,0 );

	if(fd_pc < 0)
	{
		perror("socket fd_pc failed ");
		pthread_exit(NULL);
	}


	while(1)
	{
		printf(">");
		pbuf = getchar();
		getchar();

		switch (pbuf)
		{
		case '1':
			sbuf.type = TYPE_JOB ;
			sbuf.cmd  = CMD_JOBCFG ;
			break ;
		case '2':
			sbuf.type = TYPE_JOB ;
			sbuf.cmd  = CMD_JOBOP ;
			break ;
		case '3':
			sbuf.type = TYPE_ARM ;
			sbuf.cmd  = CMD_START ;
			break ;
		default:
			printf("wrong cmd .....\n");
			break ;
			
		}



		sendto(fd_pc,&sbuf,sizeof(sbuf),0,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));

	}

	exit(EXIT_SUCCESS);

}
