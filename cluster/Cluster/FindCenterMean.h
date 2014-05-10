/*
 * FindCenterMean.h
 *
 *  Created on: May 10, 2014
 *      Author: dupes
 */

#ifndef FINDCENTERMEAN_H_
#define FINDCENTERMEAN_H_

#include "FindCenter.h"

class FindCenterMean : public FindCenter
{
private:

	virtual Mat findCenter(list<tObject*> objects, Distance *distance, int clusterID);

public:
	FindCenterMean();
	virtual ~FindCenterMean();

};

#endif /* FINDCENTERMEAN_H_ */
