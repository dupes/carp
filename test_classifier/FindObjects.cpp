/*
 * FindObjects.cpp
 *
 *  Created on: May 10, 2014
 *      Author: dupes
 */

#include "FindObjects.h"

FindObjects::FindObjects(FindEdges *findEdges)
{
	m_findEdges = findEdges;
}

/**************************************************************/

FindObjects::~FindObjects()
{
}

/**************************************************************/

void FindObjects::findObjects(Mat &image, Mat &imageBW, list<tObject> &objects, int window)
{

	vector<vector<Point> > contours;

	Mat edges = m_findEdges->findEdges(imageBW);

	tObject obj;

	findContours(edges, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	for(size_t i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(contours[i]);

		if (rect.width <= window && rect.height <= window && rect.x + window < image.cols && rect.y + window < image.rows)
		{
			obj.area = rect.area();

			obj.boundingBox = rect;

			obj.object_image = Mat::zeros(window, window, image.type());

			image(rect).copyTo(obj.object_image(Rect(0,0,rect.width, rect.height)));

			objects.push_back(obj);

			//rectangle(image, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),
			//		CV_RGB(255, 0, 0), 1, 0, 0);
		}


	}

}
