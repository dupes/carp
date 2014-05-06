/*
 * FindEdges.h
 *
 *  Created on: Nov 11, 2012
 *      Author: dupes
 */

#ifndef FINDEDGES_H_
#define FINDEDGES_H_

#include <opencv/cv.h>

#include "../BlurImage/BlurImage.h"

using namespace cv;

class FindEdges {
private:

	BlurImage *m_blurImage;

protected:
	//IplImage *m_edges;
	Mat m_edges;

	//IplImage *m_tmp;
	//IplImage *m_pyramid;

	// bool m_usePyramids;

	IplImage *usePyramid(IplImage *image);

public:
	FindEdges(Mat &mat, BlurImage *blurImage = NULL);
	virtual ~FindEdges();

	virtual Mat findEdges(Mat &image) = 0;
};

#endif /* FINDEDGES_H_ */
