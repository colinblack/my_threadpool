/*
 * main.cc
 *
 *  Created on: 2019��5��13��
 *      Author: colin
 */


#include <iostream>
#include "thread_pool.h"
#include <sys/time.h>


unsigned count = 0;


void* test(void* arg)
{
	char* tid = static_cast<char*>(arg);
	printf("tid=%s, count=%u \n", tid, count++);
	return nullptr;
}

void testThreadPool()
{
    ThreadPool pool(condMode);

    if(pool.start(4, 65535) != 0)
    {
        printf("pool start failed \n");
        exit(0);
    }

    int ret = 0;
    while(count < 65535)
    {
        if((ret = pool.addTask(test)) !=0)
            printf("add task failed \n");
      //  usleep(100);
    }
}


int main()
{
    struct timeval begin;
    gettimeofday(&begin,NULL);


    testThreadPool();


    struct timeval end;
    gettimeofday(&end,NULL);


    printf("cost %d seconds \n", end.tv_sec - begin.tv_sec);
	return 0;
}




