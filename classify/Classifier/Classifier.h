/*
 * Classifier.h
 *
 *  Created on: May 11, 2014
 *      Author: dupes
 */

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include "../shared/Object.h"

#include <map>
#include <list>

using namespace std;

class Classifier {
public:
	Classifier();
	virtual ~Classifier();

	virtual void train(map<int, tObject*> &objects, list<int> &positive, list<int> &negative, list<int> &test) = 0;

	virtual void load(string file) = 0;
	virtual void save(string file) = 0;
};

#endif /* CLASSIFIER_H_ */
