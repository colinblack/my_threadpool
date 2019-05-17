/*
 * thread.cc
 *
 *  Created on: 2019年5月13日
 *      Author: colin
 */

#include "thread.h"


int Thread::start()
{
	if(pthread_create(&tid_, nullptr, &Thread::run, this))
	{
		printf("create thread faild: %d, file: %s, line: %u \n", errno,  __FILE__, __LINE__);
		return -1;
	}

	return 0;
}

int Thread::join()
{
//	printf("join pthread: %p %p\n", pthread_self(), tid_);
	return pthread_join(tid_, nullptr);
}

