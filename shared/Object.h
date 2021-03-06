/*
 * Object.h
 *
 *  Created on: Nov 20, 2012
 *      Author: dupes
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>
#include <opencv2/imgproc/imgproc.hpp>

#include "Database.h"

#include "CVImage.h"

using namespace std;
using namespace cv;

struct tObject
{
	int id;
	int frame_id;
	Mat object_image;
	vector<Point> contour;
	Rect boundingBox;
	int number;

	double area;

	string label;
	int clusterID;
	bool verified;

	tObject(){};

	tObject(const tObject &object)
	{
		// printf("copy construct\n");
		id = object.id;
		frame_id = object.frame_id;

		object.object_image.copyTo(object_image);

		boundingBox = object.boundingBox;
		number = object.number;

		label = object.label;
		clusterID = object.clusterID;
		verified = object.verified;
	}
};

class Object
{
private:
	sqlite3_stmt *m_insertObject;

	sqlite3_stmt *m_findObjectsByFrameID;
	sqlite3_stmt *m_findObjectsByVideoID;
	sqlite3_stmt *m_findObjectsByLabel;

	sqlite3_stmt *m_updateObject;

	sqlite3_stmt *m_currentSelect;

	Database *m_db;

	bool parseObject(tObject &object);

public:
	Object(Database *db);
	virtual ~Object();

	bool saveObject(tObject *object);
	bool updateObject(tObject *object);

	bool findObjects(int frameID);

	bool findByLabel(string label);

	bool findObjectsByVideoID(int videoID);

	bool nextObject(tObject &object);

	tObject *nextObject();
};

#endif /* OBJECT_H_ */
