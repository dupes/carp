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
	string m_fileName;
	string m_svmName;

	CvSVM m_svm;
	CvSVMParams m_params;

	void prepareData(map<int, tObject*> &objects, list<int> &positive, list<int> &negative, Mat &trainingData, Mat &labels);

public:
	ClassifierSVM(string name, size_t clusterID);
	virtual ~ClassifierSVM();

	virtual void train(map<int, tObject*> &objects, list<int> &positive, list<int> &negative);

	virtual void test(map<int, tObject*> &objects, list<int> &positive, list<int> &negative);

	virtual void load(string file);
	virtual void save(string file);
};

#endif /* CLASSIFIERSVM_H_ */
