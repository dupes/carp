/*
 * MainWindow.cpp
 *
 *  Created on: Nov 21, 2012
 *      Author: dupes
 */

#include "MainWindow.h"

#include <opencv2/core/core.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <X11/keysymdef.h>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>

#include "../classify/shared/Database.h"
#include "../classify/shared/Frame.h"
#include "../classify/shared/Object.h"
#include "../classify/shared/Shared.h"
//#include "shared/Shared.h"

MainWindow::MainWindow()
	: m_video(&Shared::database)
{
	m_tframe = NULL;

	m_main.createWindow("main", 0);

	m_trackBarValue = 0;

	m_frameObjectsID = -1;

	m_object = NULL;

	m_templateThresholdValue = 0.95;

	matches = NULL;
	allMatches = NULL;

	m_labelFilter = "";
	m_showLabeledObjects = true;
}

/*********************************************************************/

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

/*********************************************************************/

void MainWindow::setTrackbarPosition(int position)
{
	// m_video->findFrame(m_tvideo->id, position, m_tframe);

	if (position <= 1)
		position = 1;

	if (m_video.frames().find(position) != m_video.frames().end())
	{
		m_tframe = m_video.frames()[position];
	}

	displayFrame();
}

/*********************************************************************/

void MainWindow::displayFrame()
{
	Mat frame = m_tframe->original_image.clone();

	if (m_showLabeledObjects)
	{
		for (map<int, tObject*>::iterator itr = m_tframe->objects.begin(); itr != m_tframe->objects.end(); itr++)
		{
			std::string label = (*itr).second->label;

			if (label != "")
			{
				Rect rect = (*itr).second->boundingBox;
				CvScalar color;

				if ((*itr).second->verified)
					color = CV_RGB(255, 0, 0);
				else
					color = CV_RGB(240, 0, 255);

				rectangle(frame, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),
						color, 1, 0, 0);

				putText(frame, label.c_str(), cvPoint(rect.x, rect.y), FONT_HERSHEY_PLAIN, 1, color, 1);//, CV_AA);
			}
		}
	}

	m_main.showImage(frame);

	m_homographyPoints.clear();
}

/*********************************************************************/

void trackBarCallback(int position, void *data)
{
	MainWindow *window = (MainWindow *)data;

	window->setTrackbarPosition(position);
}

/*********************************************************************/

void thresholdTrackBarCallback(int position, void *data)
{

}

/*********************************************************************/

void MainWindow::handleMouseButtonClick(int event, int x, int y, int flags)
{
	// printf("mouse click: %d, %d, %d, %d\n", event, x, y, flags);

	if (event != CV_EVENT_LBUTTONUP)
		return;

	if (x < 0 || x > m_video.getVideo()->cols)
		return;

	if (y < 0 || y > m_video.getVideo()->rows)
		return;

	if (event == CV_EVENT_LBUTTONUP)
		findObject(x, y);
}

/*********************************************************************/

string MainWindow::getUserInput(string prompt, string startValue)
{

	string value = startValue;

	while (1)
	{
		string output = prompt + value;

		Mat image = m_tframe->original_image.clone();

		putText(image, output.c_str(), cvPoint(10,30), FONT_HERSHEY_PLAIN, 2, CV_RGB(255, 255, 255), 2, CV_AA);

		m_main.showImage(image);

		int key = CVWindow::waitKey(-1);

		if (key == 10)
		{
			break;
		}
		else if (key == XK_BackSpace)
		{
			if (value.length())
				value = value.substr(0, value.length() - 1);
		}
		else if (key == 27)
			return "\r";
		else
		{
			value = value + (char)key;
		}
	}

	return value;
}

/*********************************************************************/

void mouseButtonClickCallback(int event, int x, int y, int flags, void* param)
{
	MainWindow *window = (MainWindow *)param;

	window->handleMouseButtonClick(event, x, y, flags);
}

/*********************************************************************/

void MainWindow::findObject(int x, int y)
{
	map<int, tObject*>::iterator itr;

	Mat image = m_tframe->original_image.clone();

	m_video.findObjects(*m_tframe);

	Rect match(0, 0, m_tframe->original_image.cols, m_tframe->original_image.rows);

	printf("find object: %d, %d\n", x, y);

	for (itr = m_tframe->objects.begin(); itr != m_tframe->objects.end(); itr++)
	{
		Rect rect = (*itr).second->boundingBox;

		if (x > rect.x && x < rect.x + rect.width &&
				y > rect.y && y < rect.y + rect.height)
		{
			// printf("match found\n");
			rectangle(image, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),
					CV_RGB(255, 0, 0), 1, 0, 0);

			if (rect.area() < match.area())
			{
				m_object =(*itr).second;
				match = rect;
			}

		}
	}

	if (match.area() != 0)
	{
		// Mat image = m_tframe->original_image.clone();

		rectangle(image, cvPoint(match.x, match.y), cvPoint(match.x + match.width, match.y + match.height),
				CV_RGB(0, 0, 255), 2, 0, 0);

		m_main.showImage(image);
		/*CVWindow win;

		win.createWindow("templ", 0);
		win.showImage(m_object->object_image);*/
	}

}

/*********************************************************************/

void MainWindow::findHomography()
{

	printf("find h\n");

	m_homographyPoints.push_back(m_object);

	if (m_homographyPoints.size() < 4)
		return;

	// printf("4 points selected\n");

	Point2f src[4];
	Point2f dest[4];

	double minx = INT_MAX, maxx = 0, miny = INT_MAX, maxy = 0;

	for (int index = 0; index < 4; index++)
	{
		src[index] = Point(m_homographyPoints[index]->boundingBox.x, m_homographyPoints[index]->boundingBox.y);

		if (m_homographyPoints[index]->boundingBox.x > maxx)
			maxx = m_homographyPoints[index]->boundingBox.x;

		if (m_homographyPoints[index]->boundingBox.x < minx)
			minx = m_homographyPoints[index]->boundingBox.x;

		if (m_homographyPoints[index]->boundingBox.y > maxy)
			maxy = m_homographyPoints[index]->boundingBox.y;

		if (m_homographyPoints[index]->boundingBox.y < minx)
			miny = m_homographyPoints[index]->boundingBox.y;
	}

	dest[0] = Point(minx, miny);

	dest[1] = Point(maxx, miny);

	dest[2] = Point(minx, maxy);

	dest[3] = Point(maxx, maxy);

	Mat h = getPerspectiveTransform(src, dest);

	Mat destImage;

	// destImage.create(m_tframe->original_image.cols, m_tframe->original_image.rows, CV_32FC1);

	m_tframe->original_image.copyTo(destImage);

	// perspectiveTransform(m_tframe->original_image, destImage, h);

	warpPerspective(m_tframe->original_image, destImage, h, destImage.size());

	m_homographyPoints.clear();

	m_main.showImage(destImage);

	FileStorage fs("../../homography/homography.xml", FileStorage::WRITE);

	fs << "homography" << h;
}

/*********************************************************************/

void MainWindow::findMatching()
{
	Mat image = m_tframe->original_image.clone();

	if (m_object == NULL)
	{
		printf("no object selected\n");
		return;
	}

	putText(image, "Searching.  please wait...", cvPoint(10,30), FONT_HERSHEY_PLAIN, 2, CV_RGB(255, 0, 0), 2, CV_AA);
	m_main.showImage(image);

	CVWindow::waitKey(5);

	// Mat templ, map<int, tFrame*> frames, int method, int startingFrame, int numFrames, int step
	// CV_TM_SQDIFF_NORMED, CV_TM_SQDIFF
	if (matches != NULL)
		delete (matches);

	matches = MatchTemplate::matchTemplate(m_object->object_image, m_object->area, m_video.frames(), CV_TM_CCOEFF_NORMED, 1, m_video.frames().size(), 1, m_templateThresholdValue);
	MatchTemplate::displayMatches(matches);
//	CVWindow::waitKey(-1);

/*	if (allMatches != NULL)
		delete (allMatches);

	allMatches = MatchTemplate::matchTemplate(matches, m_video.frames(), CV_TM_SQDIFF, 1, m_video.frames().size(), 1, m_templateThresholdValue);
	MatchTemplate::displayMatches(allMatches);
*/

	m_main.showImage(m_tframe->original_image);
}

/*********************************************************************/

void MainWindow::saveLabels()
{
	static string label;

	if ((label = getUserInput("enter label: ", label)) == "\r")
	{
		printf("save canceled\n");
		return;
	}

	printf ("saving labels...\n");

	MatchTemplate::saveMatches(label, matches);

	printf("labels saved\n");
}

/*********************************************************************/

void MainWindow::getThreshold()
{
	string value;

	if ((value = getUserInput("enter threshold (0 - 1): ")) == "\r")
		return;

	m_templateThresholdValue = atof(value.c_str());

	printf("new threshold: %f\n", m_templateThresholdValue);
}

/*********************************************************************/

void MainWindow::setLabelFilter()
{
	m_labelFilter = getUserInput("set label filter: ");
}

/*********************************************************************/

void MainWindow::printCommands()
{
	printf("Commands: \n");
	printf(" p: play/pause\n");
	printf(" z: backward one frame\n");
	printf(" x: forward one frame\n");
	printf(" left arrow key: backward 10 frames\n");
	printf(" right arrow key: forward 10 frames\n");
	printf(" m: find matching objects\n");
	printf(" s: save matching objects; will be prompted for label\n");
	printf(" f: save homography point\n");
	printf(" d: display labeled objects\n");
}

/*********************************************************************/

void MainWindow::loop(string videoName, int numFrames)
{
	int numFramesLoaded;
	bool pause = true;
	int delay = 67;


	printf("loading video: %s...\n", videoName.c_str());

	if (!m_video.loadVideo(videoName, numFrames, true))
	{
		printf("failed to load video: %s, %s\n", videoName.c_str(), Shared::database.getErrorMessage());
		// printf("message: %s\n", Shared::datam_objectbase.getErrorMessage());
		return;
	}

	numFramesLoaded = m_video.frames().size();

	printf("loaded frames: %d\n", numFramesLoaded);

	cvCreateTrackbar2("frames", "main", &m_trackBarValue, numFramesLoaded, trackBarCallback, this);

	cvSetMouseCallback("main", mouseButtonClickCallback, this);

	cvSetTrackbarPos("frames", "main", 1);

	printCommands();

	while (1)
	{
		int key = CVWindow::waitKey(delay);

		if (!pause)
			cvSetTrackbarPos("frames", "main", m_trackBarValue + 1);

		// printf("key: %d\n", key);

		switch (key)
		{
			case XK_Right: //65363: 	// right
				// m_trackBarValue += 10;
				cvSetTrackbarPos("frames", "main", m_trackBarValue + 9);
				break;

			case XK_Left: //65361:		// left
				// m_trackBarValue -= 10;
				cvSetTrackbarPos("frames", "main", m_trackBarValue - 9);
				break;

			case 'z':
				cvSetTrackbarPos("frames", "main", m_trackBarValue - 1);
				break;

			case 'x':
				cvSetTrackbarPos("frames", "main", m_trackBarValue + 1);
				break;

			case 's':
				saveLabels();
				break;

			case 't':
				getThreshold();
				break;
			/*case 'f':
				setLabelFilter();
				break;
			*/
			case 'd':
				if (m_showLabeledObjects)
				{
					m_showLabeledObjects = false;
					printf("display labels off\n");
				}
				else
				{
					m_showLabeledObjects = true;
					printf("display labels on\n");
				}

				break;

			case 'p':
				if (pause)
				{
					pause = false;
					delay = 67;
				}
				else
				{
					pause = true;
					delay = -1;
				}

				break;

			case 'f':
				findHomography();
				break;

			case 'm':
				findMatching();
				break;

			case 'h':
				printCommands();
				break;

			case -1:
				break;

			default:
				printf("key: %d, %d\n", key, key & 0xff);
				break;
		}
	}
}
