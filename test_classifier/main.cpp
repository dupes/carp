#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <string>

#include <iostream>

#include "DetectObjects.h"

#include "shared/CVWindow.h"
#include "shared/CVCapture.h"
#include "shared/Timer.h"

#include "shared/Shared.h"

#include "shared/Video.h"

#include "FindObjects.h"

#include "shared/FindEdges/FindEdgesCanny.h"

/*********************************************************************/

struct Params
{
	std::string videoFile;
	std::string database;
	std::string recognizerFile;

	bool useDatabase;
	bool train;
	bool test;

	Mat homography;

	Params()
	{
		recognizerFile = "recognizer.xml";

		train = false;
		test = true;

		database = "";
		useDatabase = false;
	}
};

/*********************************************************************/

void printUsage()
{
	printf("This program aids the user in finding interesting objects to track\n");
	printf("After the user selects and labels objects, the program creates clusters\n");
	printf("  that are later used in classification.  The clustering data is saved to\n");
	printf("  the database\n\n");

	printf("Usage: ./test_classifier -v <video name in database> -f <recognizer file> -h <homography file> \n\n");

}

/*********************************************************************/

bool parseParams(Params *params, int argc, char **argv)
{
	if (argc == 1)
		return false;

	for (int argIndex = 1; argIndex < argc; argIndex+= 2)
	{
		char *p = argv[argIndex];

		if (argIndex + 1 == argc)
			return false;

		char *param = argv[argIndex+1];

		if (strcmp(p, "-v") == 0)
		{
			params->videoFile = param;
		}
		else if (strcmp(p, "-db") == 0)
		{
			params->database = param;

			if (!Shared::database.open(params->database))
			{
				printf("failed to open database: %s, %s\n\n", params->database.c_str(), Shared::database.getErrorMessage());
				return false;
			}

			params->useDatabase = true;
		}
		else if (strcmp(p, "-f") == 0)
		{
			params->recognizerFile = param;
		}
		else if (strcmp(p, "--train") == 0)
		{
			if (strcmp(param, "true") == 0)
				params->train = true;
		}
		else if (strcmp(p, "-h") == 0)
		{
			FileStorage fs(param, FileStorage::READ);
			string name;

			fs["homography"] >> params->homography;
		}
		else
		{
			printf("invalid parameter: %s\n", p);
			return false;
		}
	}
	return true;
}

/*********************************************************************/

int main(int argc, char **argv)
{
	Timer timer;
	list<tObject> objects;

	Params params;
	// Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
	Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
	// Ptr<FaceRecognizer> model = createLBPHFaceRecognizer();

	CVCapture capture;
	Mat image, imageBW;
	CVWindow window;
	DetectObjects *detectObjects;

	FindEdgesCanny *findEdges = new FindEdgesCanny(image, NULL, 100, 300, 3);

	FindObjects *findObjects = new FindObjects(findEdges);

	if (!parseParams(&params, argc, argv))
	{
		printUsage();
		return -1;
	}

	model->load(params.recognizerFile);

	detectObjects = new DetectObjects(model);


	window.createWindow("main", 0);

	bool play = true;

	int key;
	int numFrames = 0;
	double fps;
	stringstream s;

	timer.start();

	if (!capture.openFile(params.videoFile.c_str()))
	// if (!capture.openDevice(CV_CAP_ANY))
	{
		printf("failed to open vide file %s\n", params.videoFile.c_str());
		return -1;
	}

	// capture.setResolution(320, 240);

	while (1)
	{

		image = capture.queryFrame();

		if (image.cols == 0)
			break;

		numFrames++;

		fps = numFrames / (timer.getElapsedMS() / 1000.0);

		s.str("");
		s << "fps: " << fps;

		printf("%s\n", s.str().c_str());

		warpPerspective(image, image, params.homography, image.size());

		cvtColor(image, imageBW, CV_BGR2GRAY);

		findObjects->findObjects(image, imageBW, objects, 28);

		// detectObjects->detectObjects2(image, imageBW, 50, 150, 3);

		objects.clear();

		// putText(image, s.str().c_str(), cvPoint(0,10), FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 0, 0), 1);
		// putText(frame, label.c_str(), cvPoint(rect.x, rect.y), FONT_HERSHEY_PLAIN, 1, color, 1)

		// printf("detect complete\n");

		window.showImage(image);

		if (!play)
			key = CVWindow::waitKey(-1);
		else
			key = CVWindow::waitKey(1);

		switch (key)
		{
		case 'p':
			play = !play;

			break;
		}
	}

	detectObjects->printStats();

	return 0;
}
