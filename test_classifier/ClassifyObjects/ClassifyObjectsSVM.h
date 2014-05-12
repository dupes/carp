/*
 * ClassifyObjectsSVM.h
 *
 *  Created on: May 10, 2014
 *      Author: dupes
 */

#ifndef CLASSIFYOBJECTSSVM_H_
#define CLASSIFYOBJECTSSVM_H_

#include "../shared/Object.h"

#include <opencv2/ml/ml.hpp>

#include <string>
#include <map>
#include <list>

using namespace std;

struct tMatch
{
	tObject *object;
	string label;

	tMatch(tObject *aobject, string alabel)
	{
		object = aobject;
		label = alabel;
	}
};

class ClassifyObjectsSVM
{
private:
	map<string, CvSVM*> m_svms;

public:
	ClassifyObjectsSVM();
	virtual ~ClassifyObjectsSVM();

	void loadClassifers(string name);

	void classifyObjects(list<tObject> &objects, list<tMatch> &matches);
};

#endif /* CLASSIFYOBJECTSSVM_H_ */
