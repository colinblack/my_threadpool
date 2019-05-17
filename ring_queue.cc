/*
 * ring_queue.cc
 *
 *  Created on: 2019年5月14日
 *      Author: colin
 */

#include "ring_queue.h"

RingQueue::RingQueue(int size)
	: head_(0), tail_(0), size_(0), capacity_(gQueueSize)
{
	task_.reserve(gQueueSize);
}


bool RingQueue::isFull()
{
	return size_ == capacity_;
}

bool RingQueue::isEmpty()
{
	return size_ == 0;
}

int RingQueue::pushTail(Task&& task)
{
	if(isFull())
		return -1;

	task_[tail_] = std::move(task);
	tail_ = (tail_ + 1) % capacity_;
	size_++;

	return 0;
}

Task RingQueue::popFront()
{
	if(isEmpty())
	{
		printf("queue is empty, file: %s, line: %u", __FILE__, __LINE__);
		return nullptr;
	}


	Task task = task_[head_];
	head_ = (head_ + 1) % capacity_;
	size_--;

	return task;
}
