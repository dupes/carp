/*
 * FindCenterMean.cpp
 *
 *  Created on: May 10, 2014
 *      Author: dupes
 */

#include "FindCenterMean.h"

FindCenterMean::FindCenterMean()
{
}

/*********************************************************************/

FindCenterMean::~FindCenterMean()
{
}

/*********************************************************************/

Mat FindCenterMean::doReCenter(list<tObject*> objects, Distance *distance, int clusterID)
{
	Mat center;
	list<tObject*>::iterator itr;

	itr = objects.begin();

	center = Mat((*itr)->object_image.cols, (*itr)->object_image.rows, CV_32FC1, 0.0);

	int count = 0;

	for (itr = objects.begin(); itr != objects.end(); itr++)
	{
		if ((*itr)->clusterID != clusterID)
			continue;

		add(center, (*itr)->object_image, center);

		count++;
	}

	divide(count, center, center);

	return center;
}
