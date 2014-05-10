/*
 * FindeEdgesSobel.h
 *
 *  Created on: Nov 13, 2012
 *      Author: dupes
 */

#ifndef FINDEEDGESSOBEL_H_
#define FINDEEDGESSOBEL_H_

#include "FindEdges.h"

class FindEdgesSobel: public FindEdges {
public:
	FindEdgesSobel(Mat &image, BlurImage *blurImage);
	virtual ~FindEdgesSobel();

	Mat findEdges(Mat &image);
};

#endif /* FINDEEDGESSOBEL_H_ */
