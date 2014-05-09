/*
 * Distance.h
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#ifndef DISTANCE_H_
#define DISTANCE_H_

#include "../shared/Object.h"

class Distance
{
private:

public:
	Distance();
	virtual ~Distance();

	virtual double distance(Mat *center, tObject *object) = 0;
};

#endif /* DISTANCE_H_ */
