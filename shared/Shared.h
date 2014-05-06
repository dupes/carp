/*
 * Shared.h
 *
 *  Created on: Nov 20, 2012
 *      Author: dupes
 */

#ifndef SHARED_H_
#define SHARED_H_

#include "Database.h"

class Shared
{

private:
	Shared();

public:
	virtual ~Shared();

	static Database database;
};

#endif /* SHARED_H_ */
