/*
 * FindContours.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: dupes
 */

#include "FindContours.h"

#include "shared/Shared.h"

FindContours::FindContours(Database *db, FindEdges *findEdges)
	: m_frame(db), m_object(db)
{
	m_boxes.createWindow("boxes", CV_WINDOW_NORMAL);
	m_edges.createWindow("edges", CV_WINDOW_NORMAL);

	m_scratch.createWindow("scratch", CV_WINDOW_NORMAL);

	m_findEdges = findEdges;

	m_storage = cvCreateMemStorage(0);

	//m_frame.beginTransation();
}

/**************************************************************/

FindContours::~FindContours()
{
	//m_frame.commitTransaction();
}

/**************************************************************/

void FindContours::findContours(tVideo *video, Mat &image, Mat &original, int window, int frameNumber)
{
	Mat boxImage;
	Mat scratch;

	tFrame tframe;
	tObject tobject;

	Mat edges = m_findEdges->findEdges(image);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	boxImage = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
	scratch = cv::Mat::zeros(image.rows, image.cols, original.type());


	// cv::findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	cv::findContours(edges, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	int count = 0;

	// tframe.id = 10;
	tframe.video_id = video->id;
	tframe.num_objects = contours.size();
	tframe.original_image = original;
	tframe.bw_image = image;
	tframe.number = frameNumber;

	if (!m_frame.saveFrame(&tframe))
	{
		printf("failed to save frame: %s\n", Shared::database.getErrorMessage());
		exit(0);
	}

	original.copyTo(scratch);

	// for (contour = firstContour; contour != NULL; contour = (CvContour *)contour->h_next)
	for(size_t i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(contours[i]);

		if (rect.width <= window && rect.height <= window && rect.x + window < image.cols && rect.y + window < image.rows)
		{
			tobject.area = rect.area();

			// rect.width = window;

			// rect.height = window;

			// drawContours(boxImage, contours, i, CV_RGB(count+10, 0, 255), CV_FILLED);//, CV_FILLED, hierarchy, 0, Point() );

			rectangle(scratch, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),
					CV_RGB(255, 0, 0), 1, 0, 0);

			tobject.boundingBox = rect;
			tobject.frame_id = tframe.id;
			// tobject.object_image = original(rect);

			tobject.object_image = Mat::zeros(window, window, original.type());
			// original(rect).copyTo(tobject.object_image);

			original(rect).copyTo(tobject.object_image(Rect(0,0,rect.width, rect.height)));

			// m_edges.showImage(tobject.object_image);
			// CVWindow::waitKey(-1);

			tobject.number = i;

			if (!m_object.saveObject(&tobject))
			{
				//printf("failed to save object: %s\n", m_frame.getDatabase()->getErrorMessage());
				exit(0);
			}
		}
	}

	// original.copyTo(scratch, boxImage);

	m_boxes.showImage(scratch);

	m_edges.showImage(edges);
//	m_edges.showImage(*edges);
}

/**************************************************************/

void FindContours::findBoundingBoxes(Mat &image, Mat &original)
{
	Mat boxImage;
	Mat scratch;
	static int imageNo = 0;

	// IplImage img(image);
	Mat edges = m_findEdges->findEdges(image);
	vector<vector<Point> > contours;
	// vector<Vec4i> hierarchy;

	// Mat mat(*edges);
	// boxImage = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
	boxImage = original.clone();

	// cv::findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	cv::findContours(edges, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	scratch = cv::Mat::zeros(original.rows, original.cols, original.type());

	printf("contours: %ld\n", contours.size());

	for(size_t i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(contours[i]);

		// double area = rect.height * rect.width;

		boxImage = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);

		// if (area >= 40)
		rectangle(edges, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),
				CV_RGB(0, 0, 255), 1, 0, 0);

		//rectangle(boxImage, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),
		//		CV_RGB(0, 0, 255), CV_FILLED);

		//original.copyTo(scratch, boxImage);

		// extract the object
		scratch = original(rect);

		std::ostringstream fileName;

		fileName << "../../output/image." << imageNo++ << ".jpg";

		// imwrite(fileName.str(), scratch);

		//cv::se
		//SetImageROI();

		//m_edges.showImage(boxImage);

		//cvWaitKey(-1);
	}

	//original.copyTo(scratch, boxImage);

	m_boxes.showImage(scratch);

//	m_boxes.showImage(scratch);
	m_edges.showImage(edges);

}

/**************************************************************/
