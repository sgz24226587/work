#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

int picture_count = 0 ;
int start = 0 ;
int test  = 0 ;

void *RoolJob(void *p)
{
	while(1)
	{

		if(start == 0 )
			continue ;

		picture_count ++ ;
		printf("RoolJob is running..........\n");

		printf("picture_count = %d ...\n",picture_count);

		sleep(1);
	}

	pthread_exit(NULL);
}

void *TestJob(void *p)
{
	while(1)
	{
		if(test == 0)
			continue ;

		printf("TestJob is running...........\n");
	}
	pthread_exit(NULL);
}


int init_job()
{
	pthread_t pthread_t_testjob ,pthread_t_rooljob ;
	int ret ; 

	ret = pthread_create(&pthread_t_testjob,NULL,TestJob,NULL);
	if(ret < 0)
	{
		perror("pthread_create TestJob failed");
		return -1 ;
	}

	ret = pthread_create(&pthread_t_rooljob,NULL,RoolJob,NULL);
	if(ret < 0)
	{
		perror("pthread_create RoolJob failed");
		return -1 ;
	}

	pthread_detach(pthread_t_testjob);
	pthread_detach(pthread_t_rooljob);

	return 0;
}
