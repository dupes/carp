/*
 * FindObjects.h
 *
 *  Created on: May 10, 2014
 *      Author: dupes
 */

#ifndef FINDOBJECTS_H_
#define FINDOBJECTS_H_

#include "../shared/FindEdges/FindEdges.h"

#include <list>

#include "shared/Object.h"

class FindObjects
{
private:
	FindEdges *m_findEdges;

public:
	FindObjects(FindEdges *findEdges);
	virtual ~FindObjects();

	void findObjects(Mat &image, Mat &imageBW, list<tObject> &objects, int window);
};

#endif /* FINDOBJECTS_H_ */
