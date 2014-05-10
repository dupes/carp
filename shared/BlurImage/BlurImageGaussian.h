/*
 * BlurImageGaussian.h
 *
 *  Created on: Nov 13, 2012
 *      Author: dupes
 */

#ifndef BLURIMAGEGAUSSIAN_H_
#define BLURIMAGEGAUSSIAN_H_

#include "BlurImage.h"

using namespace cv;

class BlurImageGaussian : public BlurImage
{
private:
	Mat m_image;

	int m_kernelWidth;
	int m_kernelHeight;

public:
	/**
	 * kernelWidth and kernelHeight must be positive odd integers
	 */
	BlurImageGaussian(Mat &image, int kernelWidth, int kernelHeight);
	virtual ~BlurImageGaussian();

	Mat blurImage(Mat &image);
};

#endif /* BLURIMAGEGAUSSIAN_H_ */
