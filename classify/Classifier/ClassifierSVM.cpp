/*
 * ClassifierSVM.cpp
 *
 *  Created on: May 11, 2014
 *      Author: dupes
 */

#include "ClassifierSVM.h"

ClassifierSVM::ClassifierSVM()
{
	m_params.svm_type    = CvSVM::C_SVC;
	m_params.kernel_type = CvSVM::LINEAR;

	m_params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
}

/*********************************************************************/

ClassifierSVM::~ClassifierSVM()
{
}

/*********************************************************************/

void ClassifierSVM::train(map<int, tObject*> &objects, list<int> &positive, list<int> &negative, list<int> &test)
{
	// TODO: normalize to 0 mean and 1 stddev

	tObject *object = (*objects.begin()).second;

	int rows = object->object_image.rows;
	int cols = object->object_image.cols;

	int imageArea = rows * cols;

	Mat trainingData(positive.size() + negative.size(), imageArea, CV_32FC1);
	Mat labels(positive.size() + negative.size(), 1, CV_32FC1);

	/*Mat wtf(10, 10, CV_32FC1);

	// obj->object_image.

	printf("%d, %d, %d\n", rows, cols, imageArea);

	printf("%d, %d, %d, %d\n", trainingData.rows, trainingData.cols, trainingData.type(), trainingData.depth());

	printf("%d, %d, %d, %d\n", wtf.rows, wtf.cols, wtf.type(), wtf.depth());*/

	int sample = 0;

	for (list<int>::iterator itr = positive.begin(); itr != positive.end(); itr++)
	{
		Mat image;

		object = objects[(*itr)];

		object->object_image.copyTo(image);

		cvtColor(image, image, CV_BGR2GRAY);

		//for (int index = 0; index < rows; index++)
		//	image.row(index).copyTo(trainingData.row(sample)(Rect(0,0,0,0)));

		int ii = 0;
		for (int i = 0; i< image.rows; i++)
		{
		    for (int j = 0; j < image.cols; j++)
		    {
		        trainingData.at<float>(sample, ii++) = image.at<uchar>(i,j);
		    }
		}

		labels.at<float>(Point(0, sample)) = 1.0;

		sample++;
	}

	for (list<int>::iterator itr = negative.begin(); itr != negative.end(); itr++)
	{

		Mat image;

		object = objects[(*itr)];

		object->object_image.copyTo(image);

		cvtColor(image, image, CV_BGR2GRAY);

		int ii = 0;
		for (int i = 0; i< image.rows; i++)
		{
		    for (int j = 0; j < image.cols; j++)
		    {
		        trainingData.at<float>(sample, ii++) = image.at<uchar>(i,j);
		    }
		}

		labels.at<float>(Point(0, sample)) = -1.0;
		sample++;
	}

	// printf("%d, %d, %d, %d\n", trainingData.depth(), trainingData.rows, trainingData.cols, trainingData.type());

	m_svm.train(trainingData, labels, Mat(), Mat(), m_params);

	printf("test: %f\n", m_svm.predict(trainingData.row(0)));

	printf("test: %f\n", m_svm.predict(trainingData.row(sample-1)));

}

/*********************************************************************/

void ClassifierSVM::load(string file)
{
}

/*********************************************************************/

void ClassifierSVM::save(string file)
{
}
