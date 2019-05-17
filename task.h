/*
 * task.h
 *
 *  Created on: 2019年5月14日
 *      Author: colin
 */

#ifndef TASK_H_
#define TASK_H_
#include "noncopyable.h"
#include "ring_queue.h"
#include <pthread.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>

enum eTaskStaus
{
	taskRun  = 0,
	taskQuit = 1
};

const unsigned maxEvents = 1024;

class TaskManager : nocopyable
{
public:
	TaskManager()
		: quit_(taskRun)
	{

	}
	virtual ~TaskManager(){
	}

	virtual int push(Task&& task) = 0;
	virtual Task pop() = 0;
	virtual void stop() = 0;
	inline bool isEmpty(){return queue_.isEmpty();}
	inline bool isFull(){return queue_.isFull();}
	void reserve(int size){queue_.reverse(size);};

protected:
	unsigned quit_;//0表示不退出， 1表示退出
	RingQueue queue_;
};

class CondTask final : public TaskManager
{
public:
	CondTask();
	~CondTask();

	int push(Task&& );
	Task pop();
	void stop();

private:
	unsigned waitingThreads_;
	pthread_mutex_t mutex_;
	pthread_cond_t cond_;

};

class EventfdTask final : public TaskManager
{
public:
	EventfdTask();
	~EventfdTask();

	int push(Task&& );
	Task pop();
	void stop();

private:
	int epollfd_;
	struct epoll_event events_[maxEvents];
};


#endif /* TASK_H_ */
