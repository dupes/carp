
#include <stdio.h>
#include <string>

#include "shared/CVWindow.h"
#include "shared/CVCapture.h"
#include "shared/Timer.h"

#include "shared/Shared.h"

#include "shared/Video.h"

#include "Cluster/FindClusters.h"

#include "Cluster/FindCenterMean.h"
#include "Cluster/DistanceMatchTemplate.h"

/*********************************************************************/

struct Params
{
	std::string label;
	std::string database;

	Distance *distance;
	FindCenter *findCenter;

	size_t numClusters;

	Params()
	{
		distance = NULL;
		findCenter = NULL;

		numClusters = 5;

		label ="";
		database = "";
	}
};

/*********************************************************************/

void printUsage()
{
	printf("This program creates clusters from labeled objects in the database\n");

	printf("Usage: ./cluster -db <path to database> -l <object label> -n <num clusters> \n\n");
}

/*********************************************************************/

bool parseParams(Params *params, int argc, char **argv)
{
	if (argc == 1)
		return false;

	for (int argIndex = 1; argIndex < argc; argIndex+= 2)
	{
		char *p = argv[argIndex];

		if (argIndex + 1 == argc)
			return false;

		char *param = argv[argIndex+1];

		if (strcmp(p, "-l") == 0)
		{
			params->label = param;
		}
		else if (strcmp(p, "-n") == 0)
		{
			params->numClusters = atoi(param);
		}
		else if (strcmp(p, "-db") == 0)
		{
			params->database = param;

			if (!Shared::database.open(params->database))
			{
				printf("failed to open database: %s, %s\n\n", params->database.c_str(), Shared::database.getErrorMessage());
				return false;
			}
		}
		else
		{
			printf("invalid parameter: %s\n", p);
			return false;
		}
	}
	return true;
}

/*********************************************************************/

int main(int argc, char **argv)
{
	Params params;
	Object *object;
	tObject *tobject;

	DistanceMatchTemplate distance;
	FindCenterMean findCenter;

	// map<int, tObject*> objects;
	list<tObject*> objects;

	if (!parseParams(&params, argc, argv))
	{
		printUsage();
		return 0;
	}

	object = new Object(&Shared::database);

	if (!object->findByLabel(params.label))
	{
		printf("failed to load objects: %s, %s\n", params.database.c_str(), Shared::database.getErrorMessage());
		return -1;
	}

	while ((tobject = object->nextObject()) != NULL)
		// objects[tobject->id] = tobject;
		objects.push_back(tobject);

	printf("loaded objects: %ld\n", objects.size());

	FindClusters::findClusters(objects, 0.0, distance, findCenter, params.numClusters);

	Shared::database.beginTransaction();

	for (list<tObject*>::iterator itr = objects.begin(); itr != objects.end(); itr++)
	{
		object->updateObject((*itr));
	}

	Shared::database.commitTransaction();

	delete (object);

	return 0;
}
