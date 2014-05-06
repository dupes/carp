/*
 * DetectObjects.cpp

 *
 *  Created on: Dec 2, 2012
 *      Author: dupes
 */
#include <stdio.h>

#include "DetectObjects.h"

#include "shared/CVWindow.h"

DetectObjects::DetectObjects(Ptr<FaceRecognizer> model)
{
	m_model = model;

	m_previousMatches = new map<string, tMatch>();
	m_currentMatches = new map<string, tMatch>();

	win.createWindow("edges", 0);

	m_pacmanCount = 0;
}

/*********************************************************************/

DetectObjects::~DetectObjects()
{
}

/*********************************************************************/

bool DetectObjects::detectObjects(map<string, tMatch> &matches, Mat image, Mat imageBW, int threshold1, int threshold2, int aperature, int xoffset, int yoffset, tMatch &prev)
{
	Mat edges;
	vector<vector<Point> > contours;
	int count = 0;
	bool found = false;;

	cv::Canny(imageBW, edges, threshold1, threshold2, aperature, false);

	cv::findContours(edges, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	win.showImage(edges);

	// CVWindow::waitKey(-1);

	for(size_t i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(contours[i]);

		if (rect.width <= 50 && rect.width >= 20 && rect.height <= 50 && rect.height >= 20)
		{
			count++;
			Mat newImage = Mat::zeros(50, 50, imageBW.type());

			int x = (50 - rect.width) / 2;
			int y = (50 - rect.height) / 2;

			Mat dest = newImage(Rect(x, y, rect.width, rect.height));

			Mat tempImage = imageBW(rect); //(Rect(x + rect.x, y + rect.y, 50, 50));
			tempImage.copyTo(dest);

			double confidence = 0.0;
			int label = -1;

			// win.showImage(newImage);

			m_model->predict(newImage, label, confidence);

			if (confidence <= 0.01)
			{
				tMatch match;

				if (label == prev.label)
					found = true;

				match.location = rect;
				match.location.x += xoffset;
				match.location.y += yoffset;

				match.confidence = confidence;
				match.label = label;

				stringstream index;
				index << match.location.x << "," << match.location.y;

				if (matches.find(index.str()) == matches.end())
				{
					if (label == 0)
						m_pacmanCount++;

					matches[index.str()] = match;
				}
				// return;
				// printf("match\n");

				rectangle(image, cvPoint(match.location.x, match.location.y), cvPoint(match.location.x + match.location.width, match.location.y + match.location.height),
						CV_RGB(255, 0, 0), 1, 0, 0);

				// putText(image, label.c_str(), cvPoint(rect.x, rect.y), FONT_HERSHEY_PLAIN, 1, color, 1);//, CV_AA);
			}

			// CVWindow::waitKey(-1);
		}
	}

	return found;
	// printf("checked objects: %d\n", count);
}

/*********************************************************************/

void DetectObjects::detectObjects2(Mat image, Mat imageBW, int threshold1, int threshold2, int aperature)
{
	map<string, tMatch> *tempMatches;
	tMatch match;

	//if (m_previousMatches->size() > 5)
	//	m_previousMatches->clear();

	if (m_previousMatches->empty())
	{
		detectObjects(*m_previousMatches, image, imageBW, threshold1, threshold2, aperature, 0, 0, match);
		return;
	}

	printf("checking matches: %ld\n", m_previousMatches->size());

	// for (size_t x = 0; x < m_previousMatches->size(); x++)
	for (map<string, tMatch>::iterator itr = m_previousMatches->begin(); itr != m_previousMatches->end(); itr++)
	{
		match = (*itr).second;

		Rect roi = match.location;

		roi.x = (roi.x > roi.width ? roi.x - roi.width : 0);
		roi.y = (roi.y > roi.height ? roi.y - roi.height : 0);

		roi.width = (roi.width*4 + roi.x < imageBW.cols ? roi.width*4 : roi.width*4 - (roi.width*4 + roi.x - imageBW.cols));
		roi.height = (roi.height*4 + roi.y < imageBW.rows ? roi.height*4 : roi.height*4 - (roi.height*4 + roi.y - imageBW.rows));

		Mat imageROI = imageBW(roi);

		detectObjects(*m_currentMatches, image, imageROI, threshold1, threshold2, aperature, roi.x, roi.y, match);
	}

	tempMatches = m_previousMatches;
	m_previousMatches = m_currentMatches;

	m_currentMatches = tempMatches;
	m_currentMatches->clear();

	// return detectObjects(*m_previousMatches, image, imageBW, threshold1, threshold2, aperature);
}

/*********************************************************************/

void DetectObjects::printStats()
{
	printf("pacman count: %d\n", m_pacmanCount);
}
