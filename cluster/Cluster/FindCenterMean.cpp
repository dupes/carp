/*
 * FindCenterMean.cpp
 *
 *  Created on: May 10, 2014
 *      Author: dupes
 */

#include "FindCenterMean.h"

#include "../shared/CVWindow.h"

FindCenterMean::FindCenterMean()
{
}

/*********************************************************************/

FindCenterMean::~FindCenterMean()
{
}

/*********************************************************************/

Mat FindCenterMean::findCenter(list<tObject*> objects, Distance *distance, int clusterID)
{
	Mat center;
	list<tObject*>::iterator itr, first;

	itr = objects.begin();

	// create float matrix to avoid overflow
	center = Mat((*itr)->object_image.cols, (*itr)->object_image.rows, CV_64FC3, 0.0);

	int count = 0;


	for (itr = objects.begin(); itr != objects.end(); itr++)
	{
		if ((*itr)->clusterID != clusterID)
			continue;

		//printf("found cluster: \n");

		//win.showImage((*itr)->object_image);
		//win.waitKey(-1);

		add(center, (*itr)->object_image, center, noArray(), CV_64FC3);

		count++;
	}

	printf("objects in cluster: %d\n", count);

	// CVWindow win;

	// win.createWindow("center", 0);
	//win.showImage(center);
	//win.waitKey(-1);

	//Mat result;

	itr = objects.begin();

	// the resulting center matrix should be the same type as the incoming image
	//result = Mat((*itr)->object_image.cols, (*itr)->object_image.rows, (*itr)->object_image.type(), 0.0);

	divide(center, count, center);

	center.convertTo(center, (*itr)->object_image.depth());

	// normalize(center,center,0,1,CV_MINMAX);

	//win.showImage(center);
	// win.waitKey(-1);

	return center;
}
