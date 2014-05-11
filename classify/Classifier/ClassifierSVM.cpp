/*
 * ClassifierSVM.cpp
 *
 *  Created on: May 11, 2014
 *      Author: dupes
 */

#include "ClassifierSVM.h"

ClassifierSVM::ClassifierSVM(string name, size_t clusterID)
{
	m_params.svm_type    = CvSVM::C_SVC;
	m_params.kernel_type = CvSVM::LINEAR;

	m_params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	stringstream s;

	s << name << clusterID;

	m_name = name;
}

/*********************************************************************/

ClassifierSVM::~ClassifierSVM()
{
}

/*********************************************************************/


void ClassifierSVM::prepareData(map<int, tObject*> &objects, list<int> &positive, list<int> &negative, Mat &trainingData, Mat &labels)
{
	tObject *object = (*objects.begin()).second;

	int rows = object->object_image.rows;
	int cols = object->object_image.cols;

	int imageArea = rows * cols;

	trainingData = Mat(positive.size() + negative.size(), imageArea, CV_32FC1);
	labels = Mat(positive.size() + negative.size(), 1, CV_32FC1);

	int sample = 0;

	for (list<int>::iterator itr = positive.begin(); itr != positive.end(); itr++)
	{
		Mat image;

		object = objects[(*itr)];

		object->object_image.copyTo(image);

		cvtColor(image, image, CV_BGR2GRAY);

		// stack overflow: convert image to vector
		// http://stackoverflow.com/questions/14694810/using-opencv-and-svm-with-images
		// should be replaced
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

}

/*********************************************************************/

void ClassifierSVM::train(map<int, tObject*> &objects, list<int> &positive, list<int> &negative)
{
	// TODO: normalize to 0 mean and 1 stddev
	Mat trainingData;//(positive.size() + negative.size(), imageArea, CV_32FC1);
	Mat labels;//(positive.size() + negative.size(), 1, CV_32FC1);

	prepareData(objects, positive, negative, trainingData, labels);

	m_svm.train(trainingData, labels, Mat(), Mat(), m_params);

	int sample = positive.size() + negative.size();

	// printf("test: %f\n", m_svm.predict(trainingData.row(0)));
	// printf("test: %f\n", m_svm.predict(trainingData.row(sample-1)));
}

/*********************************************************************/

void ClassifierSVM::test(map<int, tObject*> &objects, list<int> &positive, list<int> &negative)
{
	Mat testingData;
	Mat labels;
	Mat results(positive.size() + negative.size(), 1, CV_32FC1);

	prepareData(objects, positive, negative, testingData, labels);

	m_svm.predict(testingData, results);

	for (size_t index = 0; index < positive.size() + negative.size(); index++)
	{
		if (labels.at<float>(index, 0) != results.at<float>(index, 0))
			printf("mismatch: %lu, %f\n", index, results.at<float>(index, 0));
	}
}

/*********************************************************************/

void ClassifierSVM::load(string file)
{
	m_svm.load(file.c_str(), m_name.c_str());
}

/*********************************************************************/

void ClassifierSVM::save(string file)
{
	m_svm.save(file.c_str(), m_name.c_str());
}
