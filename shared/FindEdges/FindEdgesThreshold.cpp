/*
 * FindEdgesThreshold.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: dupes
 */

#include "FindEdgesThreshold.h"

FindEdgesThreshold::FindEdgesThreshold(Mat &image, BlurImage *blurImage, int threshold, int maxValue, int thresholdType)
	: FindEdges(image, blurImage)
{
	m_threshold = threshold;
	m_maxValue = maxValue;
	m_thresholdType = thresholdType;
}

/**************************************************************/

FindEdgesThreshold::~FindEdgesThreshold() {
	// TODO Auto-generated destructor stub
}

/**************************************************************/

Mat FindEdgesThreshold::findEdges(Mat &image)
{
	cv::threshold(image, m_edges, m_threshold, m_maxValue, m_thresholdType);

	// cvThreshold(usePyramid(image), m_edges, m_threshold, m_maxValue, m_thresholdType);
	// cvSobel(usePyramid(image), m_edges, 0, 1, -1);

	return m_edges;
}
