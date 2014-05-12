/*
 * FindClusters.cpp
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#include <limits.h>
#include <cstdlib>

#include "FindClusters.h"

#include "../shared/CVWindow.h"


FindClusters::FindClusters()
{
}

/*********************************************************************/

FindClusters::~FindClusters()
{
}

/*********************************************************************/

void FindClusters::findClusters(list<tObject*> &objects, double clusterMaxDistance, Distance &distance, FindCenter &findCenter, size_t numClusters)
{
	CVWindow win;

	vector<Mat> centers;

	list<tObject*>::iterator itr;

	int count = 0;

	win.createWindow("centers", 0);

	//for (size_t clusterID = 0; clusterID < numClusters; clusterID++)
	//		centers.push_back(Mat());

	for (itr = objects.begin(); itr != objects.end(); itr++)
	{
		(*itr)->clusterID = rand() % numClusters;

		count++;
	}

	centers = findCenter.findCenters(objects, &distance, numClusters);

	bool modified = false;

	count = 0;
	int numChanged = 0;

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
			}

			if ((*itr)->clusterID != objectClusterID)
			{
				(*itr)->clusterID = objectClusterID;
				modified = true;

				numChanged++;
			}
		}

		printf("end of round: %d, changed: %d\n", count+1, numChanged);

		count++;

		numChanged = 0;

		if (!modified)
			break;

		modified = false;
		centers = findCenter.findCenters(objects, &distance, numClusters);
	}

	Mat result = Mat::zeros(300, 300, ((*objects.begin())->object_image.type()));

	int colCount = 10;

	for (size_t clusterID = 0; clusterID < numClusters; clusterID++)
	{
		Mat tempImage = centers[clusterID];

		Rect location;

		location.width = tempImage.cols;
		location.height = tempImage.rows;
		location.x = (clusterID % colCount) * tempImage.cols;
		location.y = (clusterID / colCount) * tempImage.rows;

		// Mat dest = result(Rect(0 + count * templ.cols, 0, templ.cols + count * templ.cols, templ.rows));
		// Mat dest = result(Rect((count % colCount) * templ.cols, (count / colCount) * templ.rows, templ.cols, templ.rows));
		Mat dest = result(location);

		// itr->second->object_image.copyTo((dest));
		tempImage.copyTo(dest);
	}

	// win.showImage(centers[clusterID]);
	win.showImage(result);
	win.waitKey(-1);

	printf("done\n");
}
