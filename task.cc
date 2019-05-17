/*
 * task.cc
 *
 *  Created on: 2019年5月14日
 *      Author: colin
 */

#include "task.h"


CondTask::CondTask()
	: waitingThreads_(0)
{
	pthread_mutex_init(&mutex_, nullptr);
	pthread_cond_init(&cond_, nullptr);
}


CondTask::~CondTask()
{
	pthread_mutex_destroy(&mutex_);
	pthread_cond_destroy(&cond_);
}

int CondTask::push(Task&& task)
{
	if(queue_.isFull())
	{
		printf("queue is full, file: %s, line: %u \n",  __FILE__, __LINE__);
		return -1;
	}

	pthread_mutex_lock(&mutex_);
	queue_.pushTail(std::move(task));
	if(waitingThreads_ > 0)
		pthread_cond_signal(&cond_);
	pthread_mutex_unlock(&mutex_);

	return 0;
}

Task CondTask::pop()
{
	pthread_mutex_lock(&mutex_);
	waitingThreads_++;
	while(queue_.isEmpty() && quit_ == taskRun)
	{
		pthread_cond_wait(&cond_, &mutex_);
	}

	Task task;
	if(!queue_.isEmpty())
		task = queue_.popFront();
	waitingThreads_--;

	pthread_mutex_unlock(&mutex_);

	return task;
}

void CondTask::stop()
{
	pthread_mutex_lock(&mutex_);
	quit_ = taskQuit;
	pthread_cond_broadcast(&cond_);
	pthread_mutex_unlock(&mutex_);

}


EventfdTask::EventfdTask()
{
	epollfd_ = epoll_create1(0);
	if(epollfd_ == -1)
	{
		printf("epoll create failed: %s, file: %s, line: %u", strerror(errno),  __FILE__, __LINE__);
	}

}

EventfdTask::~EventfdTask()
{
	close(epollfd_);
}


int EventfdTask::push(Task&& task)
{
	unsigned seg = 0xffffffff;
	if(!queue_.isFull())
	{
		queue_.pushTail(std::move(task));

		int efd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
		if(efd == -1)
		{
			printf("eventfd failed: %s, file: %s, line: %u", strerror(errno),  __FILE__, __LINE__);
			return -1;
		}
		struct epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = efd;

		if(epoll_ctl(epollfd_, EPOLL_CTL_ADD, efd, &event) == -1)
		{
			printf("epoll_ctl failed: %s, file: %s, line: %u", strerror(errno),  __FILE__, __LINE__);
			return -1;
		}

		return write(efd, &seg, sizeof(seg));
	}

	return -1;
}


Task EventfdTask::pop()
{
	int num = epoll_wait(epollfd_, events_, maxEvents, -1);
	if(num == -1)
	{
		printf("epoll wait error: %s, file: %s, line: %u", strerror(errno),  __FILE__, __LINE__);
		return nullptr;
	}
	unsigned seg = 0;
	if(read(events_[0].data.fd, &seg, sizeof(seg)) == -1)
	{
		printf("read failed: %s, file: %s, line: %u", strerror(errno),  __FILE__, __LINE__);
		return nullptr;
	}

	close(events_[0].data.fd);

	return queue_.popFront();
}

void EventfdTask::stop()
{
	return;
}

