/*
 * Cluster.h
 *
 *  Created on: May 8, 2014
 *      Author: dupes
 */

#ifndef CLUSTER_H_
#define CLUSTER_H_

#include "shared/Object.h"

#include <list>

using namespace std;

class Cluster
{
private:
	list<tObject*> m_objects;

	Mat *m_center;

public:
	Cluster();
	virtual ~Cluster();

	double distance(tObject *object);

	void addObject(tObject *object);

	double meanDistance();

	void initialize(tObject *object);

	void reCenter();
};

#endif /* CLUSTER_H_ */
