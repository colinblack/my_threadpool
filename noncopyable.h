/*
 * noncopyable.h
 *
 *  Created on: 2019年5月13日
 *      Author: colin
 */

#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_

class nocopyable
{
public:
	nocopyable(const nocopyable&) = delete;
	nocopyable operator=(const nocopyable&) = delete;

protected:
	nocopyable() = default;
	~nocopyable() = default;
};


#endif /* NONCOPYABLE_H_ */
