/*
 * main.cc
 *
 *  Created on: 2019��5��13��
 *      Author: colin
 */


#include <iostream>
#include "thread_pool.h"

unsigned count = 0;


void* test(void* arg)
{
	char* tid = static_cast<char*>(arg);
	printf("tid=%s, count=%u \n", tid, count++);
	return nullptr;
}

int main()
{
	ThreadPool pool(enventfdMode);

	if(pool.start(2, 100) != 0)
	{
		printf("pool start failed \n");
		exit(0);
	}

	while(count < 10)
	{
		pool.addTask(test);
		sleep(1);
	}


	return 0;
}




