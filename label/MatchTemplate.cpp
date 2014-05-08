/*
 * MatchTemplate.cpp
 *
 *  Created on: Nov 24, 2012
 *      Author: dupes
 */

#include "MatchTemplate.h"

/*********************************************************************/

CVWindow MatchTemplate::matchesWindow;

int MatchTemplate::rows;
int MatchTemplate::cols;
int MatchTemplate::type;

/*********************************************************************/

MatchTemplate::MatchTemplate()
{
}

/*********************************************************************/

MatchTemplate::~MatchTemplate()
{
}

/*********************************************************************/

vector<tMatch> *MatchTemplate::matchTemplate(Mat templ, double area, map<int, tFrame*> frames, int method, int startingFrame, int numFrames, int step, double threshold)
{
	map<int, tFrame*>::iterator itr;
	Mat match;

	vector<tMatch> *matches = new vector<tMatch>();

	matches->clear();

	rows = frames[1]->original_image.rows;
	cols = frames[1]->original_image.cols;
	type = frames[1]->original_image.type();

	printf("threshold: %f\n", threshold);

	for (int index = startingFrame; index < startingFrame + (numFrames*step); index+= step)
	{
		if (index > (int)frames.size())
			break;

		for (map<int, tObject*>::iterator itr = frames[index]->objects.begin(); itr != frames[index]->objects.end(); itr++)
		{
			// Mat *inputImage = &(*itr).second->original_image;
			// Mat *inputImage = &frames[index]->original_image;



			 // if (inputImage->rows <= templ.rows * 1.2 && inputImage->rows >= templ.rows &&
				//	inputImage->cols <= templ.cols * 1.2 && inputImage->cols >= templ.cols)
			if ((*itr).second->area >= area)
			{
				// printf("areas: %f, %f\n", (*itr).second->area, area);


				Mat *inputImage = &(*itr).second->object_image;

				match.create(inputImage->cols, inputImage->rows, CV_32FC1);//inputImage->type());

				cv::matchTemplate(*inputImage, templ, match, method);

				// normalize( match, match, 0, 1, NORM_MINMAX, -1, Mat() );

				// Localizing the best match with minMaxLoc
				double minVal;
				double maxVal;

				Point minLoc;
				Point maxLoc;

				Point matchLoc;

				minMaxLoc(match, &minVal, &maxVal, &minLoc, &maxLoc);

				// printf("min: %f, max: %f\n", minVal, maxVal);

				//if(method  == CV_TM_SQDIFF || method == CV_TM_SQDIFF_NORMED)
				//	matchLoc = minLoc;
				//else
					matchLoc = maxLoc;



				if (maxVal > threshold)
				{
					tMatch match;

					match.location = Rect(matchLoc.x, matchLoc.y, templ.cols, templ.rows);
					Mat newImage = (*inputImage)(match.location);

					// printf("result: %f, %f, frame: %d\n", minVal, maxVal, index);

					match.object = itr->second;
					match.threshold = minVal;

					matches->push_back(match);
				}
			}
		}
	}

	printf("found matches: %ld\n", matches->size());

	return matches;
}

/*********************************************************************/

vector<tMatch> *MatchTemplate::matchTemplate(vector<tMatch> *matches, map<int, tFrame*> frames, int method, int startingFrame, int numFrames, int step, double threshold)
{
	vector<tMatch> *newMatches;
	vector<tMatch> *allMatches = new vector<tMatch>();

	map<int, int> indexes;
	// allMatches->

	for (size_t count = 0; count < matches->size(); count++)
	{
		tMatch *match = &(*matches)[count];

		Mat templ = match->object->object_image(match->location);
		// Mat templ = match->object->object_image;

		newMatches = matchTemplate(templ, match->object->area, frames, method, startingFrame, numFrames, step, threshold);

		//displayMatches(newMatches);
		// CVWindow::waitKey(-1);

		//delete (newMatches);
		// allMatches->insert(allMatches->end(), newMatches->begin(), newMatches->end());
		for (vector<tMatch>::iterator itr = newMatches->begin(); itr != newMatches->end(); itr++)
		{
			int id = (*itr).object->id;

			if (indexes.find(id) == indexes.end())
			{
				indexes[id] = id;
				allMatches->push_back(*itr);
				// printf("not duplicate\n");
			}
			//else
			//	printf("duplicate\n");
		}

		delete (newMatches);
	}

	printf("done: %ld\n", allMatches->size());

	return allMatches;
}

/*********************************************************************/

void MatchTemplate::displayMatches(vector<tMatch> *matches)
{
	Mat result = Mat::zeros(rows, cols, type);
	size_t colCount;

	matchesWindow.createWindow("matches", 0);

	if (matches->size() == 0)
		return;

	colCount = cols / (*matches)[0].location.width;

	size_t maxImages = colCount * (rows / (*matches)[0].location.height);

	std::sort(matches->begin(), matches->end());

	//printf
	// for (vector<tMatch*>::iterator itr = matches.begin(); itr != matches.end(); itr++)
	for (size_t count = 0; count < matches->size(); count++)
	{
		tMatch *match = &(*matches)[count];

		// printf("threshold: %d\n", match->threshold);

		Mat tempImage = match->object->object_image(match->location);
		Rect location;

		location.width = tempImage.cols;
		location.height = tempImage.rows;
		location.x = (count % colCount) * tempImage.cols;
		location.y = (count / colCount) * tempImage.rows;

		// Mat dest = result(Rect(0 + count * templ.cols, 0, templ.cols + count * templ.cols, templ.rows));
		// Mat dest = result(Rect((count % colCount) * templ.cols, (count / colCount) * templ.rows, templ.cols, templ.rows));
		Mat dest = result(location);

		// itr->second->object_image.copyTo((dest));
		tempImage.copyTo(dest);

		if (count + 1 == maxImages)
		{
			printf("too many matches to display\n");
			break;
		}
	}

	matchesWindow.showImage(result);
	// CVWindow::waitKey(25);
}

/*********************************************************************/

void MatchTemplate::saveMatches(string label, vector<tMatch> *matches)
{
	Object object(&Shared::database);

	Shared::database.beginTransaction();

	for (size_t count = 0; count < matches->size(); count++)
	{
		tMatch *match = &(*matches)[count];

		if (match->object->label != label)
		{
			match->object->label = label;
			match->object->verified = true;

			if (!object.updateObject(match->object))
				printf("object update failed: %s\n", Shared::database.getErrorMessage());
		}
	}

	Shared::database.commitTransaction();
}

