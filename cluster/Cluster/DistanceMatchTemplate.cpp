/*
 * DistanceMatchTemplate.cpp
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#include "DistanceMatchTemplate.h"

#include "../shared/CVWindow.h"

DistanceMatchTemplate::DistanceMatchTemplate()
{
}

/*********************************************************************/

DistanceMatchTemplate::~DistanceMatchTemplate()
{
}

/*********************************************************************/

double DistanceMatchTemplate::distance(Mat *center, tObject *object)
{
	double minVal;
	double maxVal;

	Point minLoc;
	Point maxLoc;

	Point matchLoc;

	// CVWindow win;
	// win.showImage(*center);

	// win.waitKey(-1);

	// win.showImage(object->object_image);
	// win.waitKey(-1);

	if (m_match.size() != object->object_image.size())
		m_match.create(object->object_image.cols, object->object_image.rows, CV_32FC1);

	cv::matchTemplate(object->object_image, *center, m_match, CV_TM_CCOEFF_NORMED);

	minMaxLoc(m_match, &minVal, &maxVal, &minLoc, &maxLoc);

	/*if (maxVal >= 1.0)
	{
		printf("max found: %f\n", maxVal);

		win.createWindow("match", 0);
		win.showImage(*center);
		win.waitKey(-1);

		win.showImage(object->object_image);
		win.waitKey(-1);

	}*/

	return 1 - maxVal;
}
