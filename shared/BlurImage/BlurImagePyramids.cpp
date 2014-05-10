/*
 * BlurImagePyramids.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: dupes
 */

#include "BlurImagePyramids.h"

BlurImagePyramids::BlurImagePyramids(Mat &image)
	: BlurImage(image)
{
	m_tmp = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
	m_pyr = cv::Mat::zeros(image.rows / 2, image.cols / 2, CV_8UC1);
}

/*********************************************************************/

BlurImagePyramids::~BlurImagePyramids() {
	// TODO Auto-generated destructor stub
}

/*********************************************************************/

Mat BlurImagePyramids::blurImage(Mat &image)
{
	pyrDown(image, m_pyr);
	pyrUp(m_pyr, m_tmp);

	return m_tmp;
}
