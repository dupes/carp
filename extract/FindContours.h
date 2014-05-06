/*
 * FindContours.h
 *
 *  Created on: Nov 11, 2012
 *      Author: dupes
 */

#ifndef FINDCONTOURS_H_
#define FINDCONTOURS_H_

#include <opencv/cv.h>

#include "shared/CVWindow.h"
#include "FindEdges/FindEdges.h"

#include "shared/Frame.h"
#include "shared/Video.h"
#include "shared/Object.h"

using namespace cv;

class FindContours {

private:
	CVWindow m_boxes;
	CVWindow m_edges;

	CVWindow m_scratch;

	FindEdges *m_findEdges;

	CvMemStorage *m_storage;

	Mat m_boxImage;

	Frame m_frame;

	Object m_object;

public:
	FindContours(Database *db, FindEdges *findEdges);
	virtual ~FindContours();

	// void findBoundingBoxes(IplImage *image, IplImage *original);
	void findBoundingBoxes(Mat &image, Mat &original);

	void findContours(tVideo *video, Mat &image, Mat &original, int frameNumber);
};

#endif /* FINDCONTOURS_H_ */
