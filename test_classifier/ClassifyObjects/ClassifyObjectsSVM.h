/*
 * ClassifyObjectsSVM.h
 *
 *  Created on: May 10, 2014
 *      Author: dupes
 */

#ifndef CLASSIFYOBJECTSSVM_H_
#define CLASSIFYOBJECTSSVM_H_

#include <opencv2/ml/ml.hpp>

#include <string>
#include <map>

using namespace std;

class ClassifyObjectsSVM
{
private:
	map<string, CvSVM*> m_svms;

public:
	ClassifyObjectsSVM();
	virtual ~ClassifyObjectsSVM();

	void loadClassifers(string name);
};

#endif /* CLASSIFYOBJECTSSVM_H_ */
