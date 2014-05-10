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

class ReCenter
{
private:

public:
	ReCenter();
	virtual ~ReCenter();

	virtual vector<Mat> reCenter(list<tObject*> objects, Distance *distance, int numClusters) = 0;
};

#endif /* RECENTER_H_ */
