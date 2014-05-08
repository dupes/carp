/*
 * MatchTemplate.h
 *
 *  Created on: Nov 24, 2012
 *      Author: dupes
 */

#ifndef MATCHTEMPLATE_H_
#define MATCHTEMPLATE_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

#include "shared/Video.h"
#include "shared/CVWindow.h"
#include "shared/Shared.h"

using namespace std;
using namespace cv;

struct tMatch
{
	tObject *object;
	int threshold;
	Rect location;

	bool operator < (const tMatch &match) const
	{
		return threshold < match.threshold;
	}
};

class MatchTemplate
{
private:
	//static CVWindow matches;
	//static void matchTemplate(Mat image, Mat templ, Mat result, int method);
	static CVWindow matchesWindow;

	// static vector<tMatch> matches;

	static int rows;
	static int cols;
	static int type;

public:
	MatchTemplate();
	virtual ~MatchTemplate();

	static vector<tMatch> *matchTemplate(Mat templ, double area, map<int, tFrame*> frames, int method, int startingFrame, int numFrames, int step, double threshold = 200000);
	static vector<tMatch> *matchTemplate(vector<tMatch> *matches, map<int, tFrame*> frames, int method, int startingFrame, int numFrames, int step, double threshold = 30000);

	static void displayMatches(vector<tMatch> *matches);

	static void saveMatches(string label, vector<tMatch> *matches);
};

#endif /* MATCHTEMPLATE_H_ */
