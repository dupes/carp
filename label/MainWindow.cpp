/*
 * MainWindow.cpp
 *
 *  Created on: Nov 21, 2012
 *      Author: dupes
 */

#include "MainWindow.h"
#include "shared/Shared.h"

MainWindow::MainWindow()
	: m_video(&Shared::database)
{
	m_tframe = NULL;

	m_main.createWindow("main", 0);

	m_trackBarValue = 0;

	m_frameObjectsID = -1;

	m_object = NULL;

	// m_templateThresholdValue = 200000;
	m_templateThresholdValue = 10;

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

string MainWindow::getUserInput(string prompt)
{

	string value = "";

	while (1)
	{
		string output = prompt + value;

		Mat image = m_tframe->original_image.clone();

		putText(image, output.c_str(), cvPoint(10,30), FONT_HERSHEY_PLAIN, 2, CV_RGB(255, 0, 0), 2, CV_AA);

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

void MainWindow::findMatching()
{
	Mat image = m_tframe->original_image.clone();

	if (m_object == NULL)
	{
		printf("no object selected\n");
		return;
	}

	putText(image, "Searching.  pleas wait...", cvPoint(10,30), FONT_HERSHEY_PLAIN, 2, CV_RGB(255, 0, 0), 2, CV_AA);
	m_main.showImage(image);

	CVWindow::waitKey(5);

	printf("threshold: %d\n", m_templateThresholdValue);

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
	string label;

	if ((label = getUserInput("enter label: ")) == "\r")
	{
		printf("save canceled\n");
		return;
	}

	printf ("saving labels...\n");

	MatchTemplate::saveMatches(label, matches);

	printf("labels saved\n");
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
	printf(" f: set filter label\n");
	printf(" d: display labeled objects");
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
		// printf("message: %s\n", Shared::database.getErrorMessage());
		return;
	}

	numFramesLoaded = m_video.frames().size();

	printf("loaded frames: %d\n", numFramesLoaded);

	cvCreateTrackbar2("frames", "main", &m_trackBarValue, numFramesLoaded, trackBarCallback, this);
	cvCreateTrackbar2("threshold", "main", &m_templateThresholdValue, 10000000, thresholdTrackBarCallback, this);

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

			case 'f':
				setLabelFilter();
				break;

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
