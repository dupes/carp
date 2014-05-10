/*
 * DistanceMatchTemplate.cpp
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#include "DistanceMatchTemplate.h"

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

	if (m_match.size() != object->object_image.size())
		m_match.create(object->object_image.cols, object->object_image.rows, CV_32FC1);

	cv::matchTemplate(object->object_image, *center, m_match, CV_TM_CCOEFF_NORMED);

	minMaxLoc(m_match, &minVal, &maxVal, &minLoc, &maxLoc);

	return 1 - maxVal;
}