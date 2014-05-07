#include <stdio.h>
#include <string>

#include "shared/CVWindow.h"
#include "shared/CVCapture.h"
#include "shared/Timer.h"

#include "shared/Shared.h"

#include "shared/Video.h"

#include "MainWindow.h"

using namespace std;

struct Params
{
	std::string videoFile;
	std::string database;
	bool useDatabase;
	int numFrames;

	Params()
	{
		videoFile = "";
		database = "";
		useDatabase = false;
		numFrames = -1;
	}
};

/*********************************************************************/

void printUsage()
{
	printf("This program aids the user in finding interesting objects to track\n");
	printf("After the user selects and labels objects, the program creates clusters\n");
	printf("  that are later used in classification.  The clustering data is saved to\n");
	printf("  the database\n\n");

	printf("Usage: ./label -v <video name in database> -db <path to database>\n\n");

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

		if (strcmp(p, "-v") == 0)
		{
			params->videoFile = param;
		}
		else if (strcmp(p, "-db") == 0)
		{
			params->database = param;

			if (!Shared::database.open(params->database))
			{
				printf("failed to open database: %s, %s\n\n", params->database.c_str(), Shared::database.getErrorMessage());
				return false;
			}

			params->useDatabase = true;
		}
		else if (strcmp(p, "-f") == 0)
		{
			params->numFrames = atoi(param);
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
	MainWindow *window;

	if (!parseParams(&params, argc, argv))
	{
		printUsage();
		return -1;
	}

	// window = new MainWindow(video, &tvideo);
	window = new MainWindow();

	window->loop(params.videoFile, params.numFrames);

	return 0;
}
