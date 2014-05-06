/*
 * Timer.cpp
 *
 *  Created on: May 9, 2011
 *      Author: dupes
 */

#include "Timer.h"

Timer::Timer() {
	gettimeofday(&m_tv, &m_tz);
}

/*********************************************************************/

Timer::~Timer() {
}

/*********************************************************************/

int Timer::start() {
	return gettimeofday(&m_tv, &m_tz);
}

/*********************************************************************/

unsigned long Timer::getElapsedMS() {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (unsigned long)(((double)(tv.tv_sec - m_tv.tv_sec) * 1000000.0 + (double)(tv.tv_usec - m_tv.tv_usec)) / 1000.0);
}
