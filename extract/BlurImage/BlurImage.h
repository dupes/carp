/*
 * BlurImage.h
 *
 *  Created on: Nov 13, 2012
 *      Author: dupes
 */

#ifndef BLURIMAGE_H_
#define BLURIMAGE_H_

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class BlurImage
{
private:

public:
	BlurImage(Mat &image);
	virtual ~BlurImage();

	/**
	 * kernelWidth and kernelHeight must be positive odd integers
	 */
	virtual Mat blurImage(Mat &image) = 0;
};

#endif /* BLURIMAGE_H_ */
