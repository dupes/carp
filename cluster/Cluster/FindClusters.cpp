/*
 * FindClusters.cpp
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#include "FindClusters.h"

FindClusters::FindClusters()
{
}

/*********************************************************************/

FindClusters::~FindClusters()
{
}

/*********************************************************************/

void FindClusters::findClusters(list<tObject*> &objects, double clusterMaxDistance, Distance &distance, FindCenter &findCenter)
{
	vector<Mat> centers;

	list<tObject*>::iterator itr;

	int count = 0;

	centers.push_back(Mat());
	centers.push_back(Mat());

	for (itr = objects.begin(); itr != objects.end(); itr++)
	{
		(*itr)->clusterID = count % 2;

		count++;
	}

	centers = findCenter.findCenters(objects, &distance, 2);

	bool modified = false;

	while (1)
	{
		for (itr = objects.begin(); itr != objects.end(); itr++)
		{

		}

		if (!modified)
			break;
	}
}
