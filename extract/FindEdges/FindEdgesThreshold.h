/*
 * FindEdgesThreshold.h
 *
 *  Created on: Nov 11, 2012
 *      Author: dupes
 */

#ifndef FINDEDGESTHRESHOLD_H_
#define FINDEDGESTHRESHOLD_H_

#include "FindEdges.h"

class FindEdgesThreshold: public FindEdges {
private:
	int m_threshold;
	int m_maxValue;
	int m_thresholdType;

public:
	FindEdgesThreshold(Mat &image, BlurImage *blurImage, int threshold, int maxValue, int thresholdType);
	virtual ~FindEdgesThreshold();

	Mat findEdges(Mat &image);
};

#endif /* FINDEDGESTHRESHOLD_H_ */
