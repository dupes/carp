/*
 * DetectObjects.h
 *
 *  Created on: Dec 2, 2012
 *      Author: dupes
 */

#ifndef DETECTOBJECTS_H_
#define DETECTOBJECTS_H_

#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "shared/CVWindow.h"

#include <map>

using namespace std;

using namespace cv;

struct tMatch
{
	Rect location;
	double confidence;
	int label;
};

class DetectObjects {
	Ptr<FaceRecognizer> m_model;

	map<string, tMatch> *m_previousMatches;
	map<string, tMatch> *m_currentMatches;

	CVWindow win;

	int m_pacmanCount;

public:
	DetectObjects(Ptr<FaceRecognizer> model);
	virtual ~DetectObjects();

	bool detectObjects(map<string, tMatch> &matches, Mat image, Mat imageBW, int threshold1, int threshold2, int aperature, int xoffset, int yoffset, tMatch &prev);

	void detectObjects2(Mat image, Mat imageBW, int threshold1, int threshold2, int aperature = 3);

	void printStats();
};

#endif /* DETECTOBJECTS_H_ */
