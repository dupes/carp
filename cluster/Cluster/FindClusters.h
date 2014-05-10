/*
 * FindClusters.h
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#ifndef FINDCLUSTERS_H_
#define FINDCLUSTERS_H_

#include "../shared/Object.h"

#include "Distance.h"
#include "FindCenter.h"

#include "../Cluster.h"

#include <list>

#include <vector>

using namespace std;

class FindClusters
{

public:
	FindClusters();
	virtual ~FindClusters();

	static void findClusters(list<tObject*> &objects, double clusterMaxDistance, Distance &distance, FindCenter &reCenter);
};

#endif /* FINDCLUSTERS_H_ */
