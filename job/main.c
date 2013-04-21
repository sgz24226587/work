#include "net.h"
#include <stdio.h>
#include <stdlib.h>
//#include "serial.h"
#include "job.h"


void check_task()
{
	return ;

}

void check_power()
{
	return ;
}

void init()
{
	check_task();
	init_net();
//	init_serial();
	init_job();
}

int main()
{
	init();
	while(1)
	{
		check_power();
		sleep(1);
	}

	exit(EXIT_SUCCESS);
}



