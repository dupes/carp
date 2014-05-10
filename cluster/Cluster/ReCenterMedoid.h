/*
 * ReCenterMedoid.h
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#ifndef RECENTERMEDOID_H_
#define RECENTERMEDOID_H_

#include "ReCenter.h"

class ReCenterMedoid : public ReCenter
{
private:

	Mat doReCenter(list<tObject*> objects, Distance *distance, int clusterID);

public:
	ReCenterMedoid();
	virtual ~ReCenterMedoid();

	virtual vector<Mat> reCenter(list<tObject*> objects, Distance *distance, int numClusters);
};

#endif /* RECENTERMEDOID_H_ */
