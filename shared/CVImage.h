/*
 * CVImage.h
 *
 *  Created on: Nov 12, 2012
 *      Author: dupes
 */

#ifndef CVIMAGE_H_
#define CVIMAGE_H_

#include <opencv2/imgproc/imgproc.hpp>

#include "Buffer.h"

using namespace cv;

struct tImageHeader
{
	int rows;
	int cols;
	int type;
};

class CVImage
{
public:
	CVImage();
	virtual ~CVImage();

	void setImage(IplImage *image);

	void bgr2gray();

	static int image2dToBuffer(Mat mat, Buffer &buffer);
	static bool bufferToImage2d(Buffer &buffer, Mat &mat);
};

#endif /* CVIMAGE_H_ */
