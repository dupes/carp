/*
 * FindEdgesCanny.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: dupes
 */

#include "FindEdgesCanny.h"

FindEdgesCanny::FindEdgesCanny(Mat &image, BlurImage *blurImage, int threshold1, int threshold2, int apertureSize)
	: FindEdges(image, blurImage)
{
	m_threshold1 = threshold1;
	m_threshold2 = threshold2;
	m_apertureSize = apertureSize;
}

/**************************************************************/

FindEdgesCanny::~FindEdgesCanny() {
	// TODO Auto-generated destructor stub
}

/**************************************************************/

Mat FindEdgesCanny::findEdges(Mat &image)
{
	// cvCanny(usePyramid(image), m_edges, m_threshold1, m_threshold2, m_apertureSize);
	// cvCanny(image, m_edges, m_threshold1, m_threshold2, m_apertureSize);

	cv::Canny(image, m_edges, m_threshold1, m_threshold2, 3, true);

	// cv::Canny()
	return m_edges;
}

/**************************************************************/

void FindEdgesCanny::setParameters(int threshold1, int threshold2, int apertureSize)
{
	m_threshold1 = threshold1;
	m_threshold2 = threshold2;
	m_apertureSize = apertureSize;
}
