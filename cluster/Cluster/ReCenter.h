/*
 * ReCenter.h
 *
 *  Created on: May 9, 2014
 *      Author: dupes
 */

#ifndef RECENTER_H_
#define RECENTER_H_

#include "../shared/Object.h"

using namespace std;

#include <list>

class ReCenter
{
private:

public:
	ReCenter();
	virtual ~ReCenter();

	virtual Mat* reCenter(list<tObject*> objects) = 0;
};

#endif /* RECENTER_H_ */
