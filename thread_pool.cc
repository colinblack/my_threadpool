/*
 * thread_pool.cc
 *
 *  Created on: 2019年5月13日
 *      Author: colin
 */

#include "thread_pool.h"
#include "task.h"
#include <cstdio>


ThreadPool::ThreadPool(int mode)
	: tsize_(0), bStarted_(false)
{
	if(mode == condMode)
	{
		std::unique_ptr<CondTask> tmp(new CondTask); //此处一定记得要初始化
		task_ = std::move(tmp);
	}
	else if(mode == enventfdMode)
	{
		std::unique_ptr<EventfdTask> tmp(new EventfdTask);
		task_ = std::move(tmp);
	}
}


ThreadPool::~ThreadPool()
{
	if(bStarted_)
	{
		task_->stop();
		bStarted_ = false;
		for(int i = threads_.size()-1; i >= 0; --i)
		{
			if(threads_[i]->join() != 0)
			{
				printf("join thread fail, file: %s, line: %u \n",  __FILE__, __LINE__);
			}
		}
	}
}

void* ThreadPool::impl(void* arg)
{
	while(bStarted_)
	{
		Task task(task_->pop());
		if(task)
		{
			task(arg);
		}
	}
	printf("thread %s exit,  %d, file: %s, line: %u \n", const_cast<char*>((const char*)arg), bStarted_, __FILE__, __LINE__);

	return nullptr;
}

int ThreadPool::start(int threadSize, int queueSize)
{
	if(threadSize <= 0 || threadSize > gThreadMaxNum || queueSize <= 0 || queueSize > gQueueSize)
	{
		printf("param is invalid, file: %s, line: %u \n",  __FILE__, __LINE__);
		return -1;
	}

	tsize_ = threadSize;
	threads_.reserve(threadSize);
	bStarted_ = true;
	char tmp[10];
	for(int  i = 0; i < threadSize; ++i)
	{
		snprintf(tmp, sizeof(tmp), "t%d", i);
		threads_.emplace_back(new Thread(std::bind(&ThreadPool::impl, this,  std::placeholders::_1), tmp));
		if(threads_.back()->start() != 0)
		{
			printf("pool start failed file: %s, line: %u \n",  __FILE__, __LINE__);
			return -1;
		}
	}
	if(queueSize < gQueueSize)
		task_->reserve(queueSize);


	return 0;
}

int ThreadPool::addTask(Task task)
{
	if(!bStarted_)
	{
		printf("threadpool not start , file: %s, line: %u \n",  __FILE__, __LINE__);
		return -1;
	}

	return task_->push(std::move(task));
}


