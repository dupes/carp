/*
 * BlurImageGaussian.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: dupes
 */

#include "BlurImageGaussian.h"

BlurImageGaussian::BlurImageGaussian(Mat &image, int kernelWidth, int kernelHeight)
	: BlurImage(image),
	m_kernelWidth(kernelWidth),
	m_kernelHeight(kernelHeight)

{
	m_image = cv::Mat::zeros(image.rows, image.cols, image.type());
}

/*********************************************************************/

BlurImageGaussian::~BlurImageGaussian() {
	// TODO Auto-generated destructor stub
}

/*********************************************************************/

Mat BlurImageGaussian::blurImage(Mat &image)
{
	GaussianBlur(image, m_image, Size(m_kernelWidth, m_kernelHeight), 0, 0, BORDER_CONSTANT);

	return m_image;
}
