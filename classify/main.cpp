/*
 * main.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: dupes
 */

#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <string>

#include <iostream>

#include "shared/CVWindow.h"
#include "shared/CVCapture.h"
#include "shared/Timer.h"

#include "shared/Shared.h"

#include "shared/Video.h"

#include "Classifier/ClassifierSVM.h"

struct Params
{
	std::string videoFile;
	std::string database;
	std::string recognizerFile;

	std::string label;

	bool useDatabase;
	bool train;
	bool test;

	Params()
	{
		recognizerFile = "recognizer.xml";

		train = false;
		test = true;

		database = "";
		useDatabase = false;
	}
};

/*********************************************************************/

void printUsage()
{
	printf("This program aids the user in finding interesting objects to track\n");
	printf("After the user selects and labels objects, the program creates clusters\n");
	printf("  that are later used in classification.  The clustering data is saved to\n");
	printf("  the database\n\n");

	printf("Usage: ./classify -v <video name in database> -db <path to database> -f <recognizer file>  -l <label> --train <true or false>\n\n");

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
			params->recognizerFile = param;
		}
		else if (strcmp(p, "-l") == 0)
		{
			params->label = param;
		}
		else if (strcmp(p, "--train") == 0)
		{
			if (strcmp(param, "true") == 0)
				params->train = true;
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

static Mat norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    // Create and return normalized image:
    Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

/*********************************************************************/

void train(Ptr<FaceRecognizer> model, vector<Mat> &images, vector<int> &labels, string fileName)
{
	model->train(images, labels);



    model->save(fileName);

    printf("training complete\n");

    return;
/*
	Mat eigenvalues = model->getMat("eigenvalues");
    // And we can do the same to display the Eigenvectors (read Eigenfaces):
    Mat W = model->getMat("eigenvectors");

	Mat mean = model->getMat("mean");
	    // Display or save:

	int height = images[0].rows;

	imshow("mean", norm_0_255(mean.reshape(1, images[0].rows)));

	for (int i = 0; i < min(16, W.cols); i++)
	{
		string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
		cout << msg << endl;

		// get eigenvector #i
		Mat ev = W.col(i).clone();

		// Reshape to original size & normalize to [0...255] for imshow.
		Mat grayscale = norm_0_255(ev.reshape(1, height));
		// Show the image & apply a Bone colormap for better sensing.

		Mat cgrayscale;
		applyColorMap(grayscale, cgrayscale, COLORMAP_BONE);

		namedWindow(format("fisherface_%d", i), 0);
		imshow(format("fisherface_%d", i), cgrayscale);
	}

	printf("training complete\n");
*/
    /*for(int num_components = min(W.cols, 10); num_components < min(W.cols, 300); num_components+=15) {
        // slice the eigenvectors from the model
        Mat evs = Mat(W, Range::all(), Range(0, num_components));
        Mat projection = subspaceProject(evs, mean, images[0].reshape(1,1));
        Mat reconstruction = subspaceReconstruct(evs, mean, projection);
        // Normalize the result:
        reconstruction = norm_0_255(reconstruction.reshape(1, images[0].rows));

        // Display or save:
        namedWindow(format("eigenface_reconstruction_%d", num_components), 0);
        imshow(format("eigenface_reconstruction_%d", num_components), reconstruction);
    }*/
}

/*********************************************************************/

void test(Ptr<FaceRecognizer> model, map<int, tObject*> &objects, vector<Mat> imagesToClassify, vector<int> imagesToClassifyID)
{
	map<int, string> labelNames;
	Object object(&Shared::database);

	labelNames[0] = "pacman";
	labelNames[1] = "ghost";

	/*labelNames[1] = "pink ghost";
	labelNames[2] = "blue ghost";
	labelNames[3] = "red ghost";
	labelNames[4] = "yellow ghost";
	labelNames[5] = "edible ghost";
	*/

	Shared::database.beginTransaction();

	for (size_t x = 0; x < imagesToClassify.size(); x++)
	{
		Mat image = imagesToClassify[x];
		double confidence = 0.0;
		int label = -1;
		int objectID = imagesToClassifyID[x];

		tObject *tobject = objects[objectID];

		model->predict(image, label, confidence);


		if (label >= 0 && confidence <= 1.0)
		{
			printf("predicted label: %d, %f\n", label, confidence);

			tobject->label = labelNames[label];
			tobject->verified = false;

			object.updateObject(tobject);
		}
	}

	Shared::database.commitTransaction();
}

/*********************************************************************/

void test(Ptr<FaceRecognizer> model, vector<Mat> &images, vector<int> &labels)
{
	// for (vector<Mat>::iterator itr = images.begin(); itr != images.end(); itr++)
	int numCorrect = 0;
	int numIncorrect = 0;

	for (size_t x = 0; x < images.size(); x++)
	{
		Mat image = images[x];
		int expectedLabel = labels[x];
		double confidence = 0.0;
		int label = -1;

		model->predict(image, label, confidence);

		if (expectedLabel == label)
			numCorrect++;
		else
			numIncorrect++;

		// printf("image: %ld, expected: %d, actual: %d, confidence: %f\n", x, expectedLabel, label, confidence);
	}

	printf("results: correct: %d, incorrect: %d\n", numCorrect, numIncorrect);
}

/*********************************************************************/

void loadObjects(map<int, tObject*> &objects, vector<Mat> &images, vector<int> &labels,	vector<Mat> &imagesToClassify, vector<int> &imagesToClassifyID)
{
	map<string, int> labelNames;

	// labelNames["pacman"] = 0;
	// labelNames["ghost"] = 1;

	/*
	labelNames["pink ghost"] = 1;
	labelNames["blue ghost"] = 1;
	labelNames["red ghost"] = 1;
	labelNames["yellow ghost"] = 1;
	labelNames["edible ghost"] = 1;
	 */

	//int maxRows = 50;
	//int maxCols = 50;

	/*for (map<int, tObject*>::iterator itr = objects.begin(); itr != objects.end(); itr++)
	{
		Mat image = (*itr).second->object_image;// (Rect(1, 2, 3, 4));

		// image.resize()

		if (image.cols > maxCols)
			maxCols = image.cols;

		if (image.rows > maxRows)
			maxRows = image.rows;
	}*/

	// Rect = objects[0]->bo
	for (map<int, tObject*>::iterator itr = objects.begin(); itr != objects.end(); itr++)
	{
		tObject *object = (*itr).second;

		// if (!(object->object_image.rows <= maxRows && object->object_image.cols <= maxCols))
		if (object->label == "")
			continue;

		//Mat image = Mat::zeros(maxRows, maxCols, object->object_image.type()); //= (*itr)->object_image.clone();// (Rect(1, 2, 3, 4));

		// printf("dim: %d, %d, image: %d, %d\n", (*itr)->object_image.cols, (*itr)->object_image.rows, image.cols, image.rows);

		//int x = (maxCols - object->object_image.cols) / 2;
		//int y = (maxRows - object->object_image.rows) / 2;

		//Mat dest = image(Rect(x, y, object->object_image.cols, object->object_image.rows));

		Mat image;

		object->object_image.copyTo(image);

		cvtColor(image, image, CV_BGR2GRAY);

		// imshow("template", image);
		// cvWaitKey(-1);
		// printf("next\n");

		if (object->verified)
		{
			images.push_back(image);

			stringstream label;

			label << object->label << object->clusterID;

			if (labelNames.find(label.str()) == labelNames.end())
			{
				labelNames[label.str()] = labelNames.size();
			}

			int id = labelNames[label.str()];

			labels.push_back(id);
		}
		else
		{
			if (object->object_image.cols >= 20 && object->object_image.rows >= 20)
			{
				imagesToClassify.push_back(image);
				imagesToClassifyID.push_back(object->id);
			}
		}
	}

	printf("training: %ld, classify: %ld\n", images.size(), imagesToClassify.size());
}

/*********************************************************************/

void initSamples(map<int, tObject*> objects, list<int> &positive, list<int> &negative, list<int> &testpos, list<int> &testneg, string label, int clusterID)
{
	// TODO: select random set of positive samples for testing
	for (map<int, tObject*>::iterator itr = objects.begin(); itr != objects.end(); itr++)
	{
		int id = (*itr).first;
		tObject *obj = (*itr).second;

		if (obj->label == label)
		{
			if (obj->clusterID == clusterID)
			{
				if (rand() % 100 < 20)
				{
					testpos.push_back(id);
				}
				else
				{
					positive.push_back(id);
				}
			}
		}
		else
		{
			if (rand() % 100 < 20)
			{
				testneg.push_back(id);
			}
			else
			{
				negative.push_back(id);
			}
		}
	}

	printf("positive samples: %lu\nnegative samples: %lu\n", positive.size(), negative.size());
	printf("positive test samples: %lu\nnegative test samples: %lu\n", testpos.size(), testneg.size());

}

/*********************************************************************/

int main(int argc, char **argv)
{
	Params params;
	int videoID;
	Video *video;
	Object *object;
	tObject *tobject;

	map<int, tObject*> objects;

	vector<Mat> images;
	vector<int> labels;
	vector<Mat> imagesToClassify;
	vector<int> imagesToClassifyID;

	srand (time(NULL));
	if (!parseParams(&params, argc, argv))
	{
		printUsage();
		return 0;
	}

	video = new Video(&Shared::database);
	object = new Object(&Shared::database);

	// if (!m_video.loadVideo(videoName, numFrames, true))
	if (!video->getID(params.videoFile.c_str(), videoID))
	{
		printf("failed to load video: %s, %s\n", params.videoFile.c_str(), Shared::database.getErrorMessage());
		return -1;
	}

	printf("found ID for video: %s, %d\n", params.videoFile.c_str(), videoID);

	if (!object->findObjectsByVideoID(videoID))
	{
		printf("failed to load objects: %s, %s\n", params.videoFile.c_str(), Shared::database.getErrorMessage());
		return -1;
	}

	map<int, int> clusterIDs;

	while ((tobject = object->nextObject()) != NULL)
	{
		objects[tobject->id] = tobject;

		if (tobject->label == params.label)
		{
			if (clusterIDs.find(tobject->clusterID) == clusterIDs.end())
				clusterIDs[tobject->clusterID] = tobject->clusterID;
		}
	}

	printf("loaded objects: %ld\n", objects.size());

	for (map<int, int>::iterator itr = clusterIDs.begin(); itr != clusterIDs.end(); itr++)
	{
		list<int> positive;
		list<int> negative;
		list<int> testpos;
		list<int> testneg;

		int clusterID = (*itr).first;

		ClassifierSVM svm(params.label, clusterID);

		initSamples(objects, positive, negative, testpos, testneg, params.label, clusterID);

		svm.train(objects, positive, negative);

		svm.test(objects, testpos, testneg);

		svm.save("");
	}

	/*
	loadObjects(objects, images, labels, imagesToClassify, imagesToClassifyID);

	// Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
	Ptr<FaceRecognizer> model = createEigenFaceRecognizer(80);
	// Ptr<FaceRecognizer> model = createLBPHFaceRecognizer();

	if (params.train)
		train(model, images, labels, params.recognizerFile);
	else
		model->load(params.recognizerFile);

	// test(model, objects);
	test(model, images, labels);

	// test(model, objects, imagesToClassify, imagesToClassifyID);

	printf("program complete\n");

	cvWaitKey(0);
*/

	delete (video);
	delete (object);

	return 0;
}
