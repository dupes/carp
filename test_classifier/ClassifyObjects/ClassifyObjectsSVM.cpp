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
