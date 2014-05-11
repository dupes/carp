/*
 * ClassifierSVM.h
 *
 *  Created on: May 11, 2014
 *      Author: dupes
 */

#ifndef CLASSIFIERSVM_H_
#define CLASSIFIERSVM_H_

#include "Classifier.h"


class ClassifierSVM : public Classifier
{

private:
	CvSVM m_svm;
	CvSVMParams m_params;

public:
	ClassifierSVM();
	virtual ~ClassifierSVM();

	virtual void train(map<int, tObject*> &objects, list<int> &positive, list<int> &negative, list<int> &test);

	virtual void load(string file);
	virtual void save(string file);
};

#endif /* CLASSIFIERSVM_H_ */
