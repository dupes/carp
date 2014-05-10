/*
 * ReCenter.h
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#ifndef RECENTER_H_
#define RECENTER_H_

#include "../shared/Object.h"

#include "Distance.h"

using namespace std;

#include <list>
#include <vector>

class FindCenter
{
private:

private:
	virtual Mat findCenter(list<tObject*> objects, Distance *distance, int clusterID) = 0;

public:
	FindCenter();
	virtual ~FindCenter();

	virtual vector<Mat> findCenters(list<tObject*> objects, Distance *distance, int numClusters) = 0;
};

#endif /* RECENTER_H_ */
