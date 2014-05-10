/*
 * FindClusters.cpp
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#include <limits.h>

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
	size_t numClusters = 2;

	vector<Mat> centers;

	list<tObject*>::iterator itr;

	int count = 0;

	centers.push_back(Mat());
	centers.push_back(Mat());

	for (itr = objects.begin(); itr != objects.end(); itr++)
	{
		(*itr)->clusterID = count % numClusters;

		count++;
	}

	centers = findCenter.findCenters(objects, &distance, 2);

	bool modified = false;

	while (1)
	{
		modified = false;

		for (itr = objects.begin(); itr != objects.end(); itr++)
		{
			double minDistance = 1;
			int objectClusterID = (*itr)->clusterID;

			for (size_t clusterID = 0; clusterID < numClusters; clusterID++)
			{
				double d = distance.distance(&centers[clusterID], (*itr));

				if (d < minDistance)
				{
					minDistance = d;
					objectClusterID = clusterID;
				}

				if ((*itr)->clusterID != objectClusterID)
				{
					(*itr)->clusterID = objectClusterID;
					modified = true;
				}
			}
		}

		if (!modified)
			break;
	}
}
