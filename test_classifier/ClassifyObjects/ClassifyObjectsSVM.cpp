/*
 * ClassifyObjectsSVM.cpp
 *
 *  Created on: May 10, 2014
 *      Author: dupes
 */

#include "ClassifyObjectsSVM.h"

#include <dirent.h>
#include <stdio.h>

ClassifyObjectsSVM::ClassifyObjectsSVM()
{
}

/*********************************************************************/

ClassifyObjectsSVM::~ClassifyObjectsSVM()
{
	for (map<string, CvSVM*>::iterator itr = m_svms.begin(); itr != m_svms.end(); itr++)
	{
		delete ((*itr).second);
	}
}

/*********************************************************************/

void ClassifyObjectsSVM::loadClassifers(string name)
{
	dirent *dir;
	DIR *dirp;

	string path = "../../data/classify/";

	dirp = opendir(path.c_str());


	while ((dir = readdir(dirp)) != NULL)
	{
		// printf("%s\n", dir->d_name);

		if (strstr(dir->d_name, name.c_str()) != NULL)
		{
			string fileName = path + dir->d_name;
			string classfierName(dir->d_name, strlen(dir->d_name) - 4);

			//classfierName.copy(dir->d_name, 0, strlen(dir->d_name) - 4);

			printf("loading classifier: %s\n", classfierName.c_str());

			CvSVM *svm = new CvSVM;

			svm->load(fileName.c_str(), classfierName.c_str());

			m_svms[classfierName] = svm;
		}

	}
}

/*********************************************************************/

#include "../shared/CVWindow.h"

void ClassifyObjectsSVM::classifyObjects(list<tObject> &objects, list<tMatch> &matches)
{
	// CVWindow win;

	// win.createWindow("found", 0);
	int objecsChecked = 0;

	for (list<tObject>::iterator itr = objects.begin(); itr != objects.end(); itr++)
	{
		Mat image;// = (*itr).object_image;

		Rect rect = (*itr).boundingBox;
		if (rect.width * rect.height < 150)
			continue;

		objecsChecked++;

		(*itr).object_image.copyTo(image);

		cvtColor(image, image, CV_BGR2GRAY);

		Mat flatened(1, image.rows*image.cols, CV_32FC1);

		int ii = 0;
		for (int i = 0; i< image.rows; i++)
		{
		    for (int j = 0; j < image.cols; j++)
		    {
		    	flatened.at<float>(0, ii++) = image.at<uchar>(i,j);
		    }
		}

		for (map<string, CvSVM*>::iterator svmItr = m_svms.begin(); svmItr != m_svms.end(); svmItr++)
		{
			float val = (*svmItr).second->predict(flatened);

			if (val == 1.0)
			{
				matches.push_back(tMatch(&(*itr), (*svmItr).first));

				//win.showImage((*itr).object_image);

				//win.waitKey(-1);
				break;
			}
		}
	}

	printf("checked: %d\n", objecsChecked);

}
