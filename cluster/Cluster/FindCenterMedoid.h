/*
 * ReCenterMedoid.h
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#ifndef RECENTERMEDOID_H_
#define RECENTERMEDOID_H_

#include "FindCenter.h"

class ReCenterMedoid : public FindCenter
{
private:

	virtual Mat doReCenter(list<tObject*> objects, Distance *distance, int clusterID);

public:
	ReCenterMedoid();
	virtual ~ReCenterMedoid();
};

#endif /* RECENTERMEDOID_H_ */
