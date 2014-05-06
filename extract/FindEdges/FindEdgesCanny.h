/*
 * FindEdgesCanny.h
 *
 *  Created on: Nov 11, 2012
 *      Author: dupes
 */

#ifndef FINDEDGESCANNY_H_
#define FINDEDGESCANNY_H_

#include "FindEdges.h"

class FindEdgesCanny: public FindEdges {
private:
	int m_threshold1;
	int m_threshold2;
	int m_apertureSize;

public:
	FindEdgesCanny(Mat &image, BlurImage *blurImage, int threshold1, int threshold2, int apertureSize);
	virtual ~FindEdgesCanny();

	Mat findEdges(Mat &img);

	void setParameters(int threshold1, int threshold2, int apertureSize);
};

#endif /* FINDEDGESCANNY_H_ */
