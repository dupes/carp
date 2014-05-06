/*
 * main.cpp
 *
 *  Created on: Nov 24, 2012
 *      Author: dupes
 */

#include "shared/CVWindow.h"
#include "shared/Object.h"
#include "shared/Shared.h"

/*********************************************************************/

struct Params
{
	std::string database;
	int frameID;

	Params()
	{
		database = "";
		frameID = 1;
	}
};

/*********************************************************************/

void printUsage()
{
	printf("This program steps through each object found in a given frame\n\n");
	printf("Usage: ./view_object -d <path to database> -frame <frame number>\n\n");
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


		if (strcmp(p, "-db") == 0)
		{
			params->database = param;

			if (!Shared::database.open(params->database))
			{
				printf("failed to open database: %s, %s\n\n", params->database.c_str(), Shared::database.getErrorMessage());
				return false;
			}
		}
		else if (strcmp(p, "-frame") == 0)
		{
			params->frameID = atoi(param);
		}
		else
		{
			printf("invalid parameter: %s\n", param);
			return false;
		}

	}

	return true;
}

int main(int argc, char **argv)
{
	Object *object;
	tObject tobject;
	CVWindow main;

	Params params;

	if (!parseParams(&params, argc, argv))
	{
		printUsage();
		return -1;
	}

	object = new Object(&Shared::database);

	if (!object->findObjects(params.frameID))
	{
		printf("frame not found\n");
		return -1;
	}

	main.createWindow("main", 0);

	while (object->nextObject(tobject))
	{
		main.showImage(tobject.object_image);

		CVWindow::waitKey(-1);
	}

	return 0;
}



