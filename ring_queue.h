/*
 * ring_queue.h
 *
 *  Created on: 2019年5月14日
 *      Author: colin
 *  环形队列
 */

#ifndef RING_QUEUE_H_
#define RING_QUEUE_H_
#include <functional>
#include <vector>

typedef std::function<void* (void*)> Task;

const int gQueueSize = 655360;

struct RingQueue
{
public:
	RingQueue(int size = 0);
	bool isFull();
	int pushTail(Task&& );
	Task popFront();
	bool isEmpty();
	void reverse(int size){task_.reserve(size);}

public:
	int head_;
	int tail_;
	int size_;
	int capacity_;
	//Task* task_;
	std::vector<Task> task_;
};

#endif /* RING_QUEUE_H_ */
