/*
 * DistanceMatchTemplate.h
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#ifndef DISTANCEMATCHTEMPLATE_H_
#define DISTANCEMATCHTEMPLATE_H_

#include "Distance.h"

class DistanceMatchTemplate : public Distance
{
private:
	Mat m_match;

public:
	DistanceMatchTemplate();
	virtual ~DistanceMatchTemplate();

	virtual double distance(Mat *center, tObject *object);
};

#endif /* DISTANCEMATCHTEMPLATE_H_ */
