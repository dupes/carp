/*
 * ReCenter.cpp
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#include "FindCenter.h"

FindCenter::FindCenter()
{
}

FindCenter::~FindCenter()
{
}

/*********************************************************************/

vector<Mat> FindCenter::findCenters(list<tObject*> objects, Distance *distance, int numClusters)
{
	vector<Mat> centers;

	for (int clusterID = 0; clusterID < numClusters; clusterID++)
		centers.push_back(findCenter(objects, distance, clusterID));

	return centers;
}
