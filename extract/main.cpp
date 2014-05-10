
#include <stdio.h>
#include <string>

#include "shared/CVWindow.h"
#include "shared/CVCapture.h"
#include "shared/Timer.h"

#include "FindContours.h"

#include "shared/FindEdges/FindEdgesCanny.h"
#include "shared/FindEdges/FindEdgesThreshold.h"
#include "shared/FindEdges/FindEdgesSobel.h"

#include "shared/BlurImage/BlurImagePyramids.h"
#include "shared/BlurImage/BlurImageGaussian.h"

#include "shared/Shared.h"

#include "shared/Video.h"

using namespace cv;
using namespace std;

struct Params
{
	std::string videoFile;
	int threshold1;
	int threshold2;
	int window;

	std::string database;
	bool useDatabase;
	bool showImages;
	bool save;

	Mat h;

	Params()
	{
		videoFile = "";
		threshold1 = 100;
		threshold2 = 150;
		database = "";
		useDatabase = false;

		showImages= false;
		save = true;
		window = 24;
	}

};

/*********************************************************************/

void printUsage()
{
	printf("This program extracts objects with canny edge detection followed by a contour extractor\n");
	printf("The objects are (optionally) stored in an sqlite database in the following hierarcy:\n");
	printf("  Video->Frames->Objects\n\n");

	printf("Usage: ./object_detection -v <path to video> -t1 <threshold1> -t2 <threshold2> -db <path to database> -w <window size> -h <homogrpahy xml matrix file> [-show <true|false>]\n\n");
	//printf("  if no database is specified, the results will not be stored\n\n");
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
		else if (strcmp(p, "-t1") == 0)
		{
			params->threshold1 = atoi(param);
		}
		else if (strcmp(p, "-t2") == 0)
		{
			params->threshold2 = atoi(param);
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
		else if (strcmp(p, "-show") == 0)
		{
			if (strcmp(param, "true") == 0)
				params->showImages = true;
		}
		else if (strcmp(p, "-save") == 0)
		{
			if (strcmp(param, "false") == 0)
				params->save = false;
		}
		else if (strcmp(p, "-w") == 0)
		{
			params->window = atoi(param);
		}
		else if (strcmp(p, "-h") == 0)
		{
			FileStorage fs(param, FileStorage::READ);
			string name;

			fs["homography"] >> params->h;
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
	Params params;
	tVideo tvideo;
	Video *video;

	Timer timer;
	unsigned long numFrames = 0;
	double fps = 0;
	std::ostringstream output;

	// int threshold1 = 100, threshold2 = 50;
	CVWindow window, window2;
	CVCapture capture;

	Mat image, imageBW, imageBlurred;

	vector<uchar> buf;

	Mat w(buf);

	FindContours *findContours;
	BlurImage *blurImage;

	if (!parseParams(&params, argc, argv))
	{
		printUsage();
		return 0;
	}

	if (!Shared::database.execSql("PRAGMA read_uncommitted = true"))
		exit(0);

	if (!Shared::database.execSql("PRAGMA synchronous = OFF"))
		exit(0);

	//if (!Shared::database.execSql("PRAGMA journal_mode = MEMORY"))
	//	exit(0);

	Shared::database.clear();

	Shared::database.beginTransaction();

	// if (!capture.openFile("/home/dupes/projects/classes/cs446/project/source/object_detection/videos/pacman1.avi"))
	if (!capture.openFile(params.videoFile.c_str()))
	{
		printf("failed to open vide file %s\n", params.videoFile.c_str());
		return -1;
	}

	window.createWindow("main", 0);
	window2.createWindow("main2", 0);

	image = capture.queryFrame();

	tvideo.name = params.videoFile;
	tvideo.cols = image.cols;
	tvideo.rows = image.rows;
	tvideo.threshold1 = params.threshold1;
	tvideo.threshold2 = params.threshold2;

	video = new Video(&Shared::database);

	if (!video->saveVideo(tvideo))
	{
		printf("failed to save video: %s\n", Shared::database.getErrorMessage());
		return 0;
	}

	printf("video ID: %d\n", tvideo.id);

	imageBW = image;

	// IplImage img(image);

	// findContours = new FindContours(new FindEdgesSobel(image, NULL));
	FindEdgesCanny *findEdges = new FindEdgesCanny(image, NULL, params.threshold1, params.threshold2, 3);
	// findContours = new FindContours(new FindEdgesCanny(image, NULL, 150, 100, 3));
	findContours = new FindContours(&Shared::database, findEdges);

	// findContours = new FindContours(new FindEdgesThreshold(image, NULL, 80, 255, CV_THRESH_BINARY));

	// blurImage = new BlurImagePyramids(image);
	blurImage = new BlurImageGaussian(image, 3, 3);

	timer.start();

	int wait = -1;
	int key = 0;

	time_t start, end;

	time(&start);

	while (1)
	{
		image = capture.queryFrame();

		if (image.cols == 0)
			break;

		// image.copyTo(destImage);
		warpPerspective(image, image, params.h, image.size());

		numFrames++;

		// printf("%ld\n", numFrames);

		cvtColor(image, imageBW, CV_BGR2GRAY);
		// cvtColor(destImage, imageBW, CV_BGR2GRAY);

		// imageBlurred = blurImage->blurImage(imageBW);
		// imageBlurred = blurImage->blurImage(imageBlurred);

		// Mat temp;

		// absdiff(imageBW, imageBlurred, temp);

		// temp = blurImage->blurImage(temp);

		//threshold(temp, temp, 30, 255, THRESH_BINARY);

		//add(imageBlurred, temp, temp);

		findContours->findContours(&tvideo, imageBW, image, params.window, numFrames);

		// findContours->findContours(&tvideo, temp, image, numFrames);
		// findContours->findContours(&tvideo, imageBW, image, numFrames);

		// findContours->findBoundingBoxes(imageBlurred, image);
		// findContours->findBoundingBoxes(imageBW, image);

		fps = (double)numFrames / (timer.getElapsedMS() / 1000.0);

		printf("%f\n", fps);

		output.str("");
		output << "FPS: " << fps;
		cv::putText(image, output.str(), cvPoint(10,30), FONT_HERSHEY_PLAIN, 2, cvScalar(200,200,250), 2, CV_AA);

		// window.showImage(image);

		//window.showImage(imageBlurred);
		// window2.showImage(temp);

		if (params.showImages)
			key = CVWindow::waitKey(wait);

		if (key == 'p')
		{
			if (wait == -1)
				wait = 10;
			else
				wait = -1;
		}
	}

	time(&end);

	printf("time: %d\n", end - start);

	Shared::database.commitTransaction();

	delete (video);

	delete (findContours);

	printf("frames: %ld\n", numFrames);
}
