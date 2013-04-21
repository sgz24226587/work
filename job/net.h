#ifndef __JOB__H
#define __JOB__H


typedef struct NETMSG{
	char cmd ;
	char type ;
	char lenth ;
	char msg[512];
}NetMsg ;

extern int init_net();

#endif
