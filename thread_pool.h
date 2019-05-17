/*
 * thread_pool.h
 *
 *  Created on: 2019年5月13日
 *      Author: colin
 */

#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "noncopyable.h"
#include "thread.h"
#include "task.h"

#include <memory>
#include <vector>
#include <cstdlib>
#include <pthread.h>

const int gThreadMaxNum = 128;


enum threadPoolMod
{
	condMode		= 0,
	enventfdMode 	= 1
};


class ThreadPool : nocopyable
{
public:
	ThreadPool(int mode = condMode);
	~ThreadPool();
	int start(int, int);
	int addTask(Task task);

private:
	void* impl(void*);

private:
	int tsize_;  //线程数量
	bool bStarted_; //是否启动
	std::vector<std::unique_ptr<Thread>> threads_;
	std::unique_ptr<TaskManager> task_;
};

#endif /* THREAD_POOL_H_ */
