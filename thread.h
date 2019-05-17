/*
 * thread.h
 *
 *  Created on: 2019年5月13日
 *      Author: colin
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "noncopyable.h"
#include <functional>
#include <string>
#include <pthread.h>
#include <errno.h>

class Thread : nocopyable
{
public:
	typedef std::function<void* (void*)> ThreadFunc;

	explicit Thread(ThreadFunc func, const std::string& name = std::string())
		: tid_(0), func_(std::move(func)), name_(name)
	{}

	~Thread(){};

	int start();
	int join();

	static void* run(void* argc)
	{
		Thread* t = static_cast<Thread*>(argc);
		//printf("in thread %p %p \n", pthread_self(), t->tid_);
		return t->func_(const_cast<char*>(t->name_.c_str()));
	}

private:
	pthread_t  tid_;
	ThreadFunc func_;
	std::string name_;
};

#endif /* THREAD_H_ */
