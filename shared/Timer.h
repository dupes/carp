/*
 * Timer.h
 *
 *  Created on: May 9, 2011
 *      Author: dupes
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <sys/time.h>

#include <stdio.h>

class Timer {
private:
	struct timeval m_tv;
	struct timezone m_tz;

public:
	Timer();
	virtual ~Timer();

	int start();

	unsigned long getElapsedMS();
};

#endif /* TIMER_H_ */
