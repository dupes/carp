/*
 * BlurImagePyramids.h
 *
 *  Created on: Nov 13, 2012
 *      Author: dupes
 */

#ifndef BLURIMAGEPYRAMIDS_H_
#define BLURIMAGEPYRAMIDS_H_

#include "BlurImage.h"

class BlurImagePyramids: public BlurImage
{
private:
	Mat m_tmp;
	Mat m_pyr;

public:
	BlurImagePyramids(Mat &image);
	virtual ~BlurImagePyramids();

	Mat blurImage(Mat &image);
};

#endif /* BLURIMAGEPYRAMIDS_H_ */
