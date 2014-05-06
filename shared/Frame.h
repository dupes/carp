/*
 * Frame.h
 *
 *  Created on: Nov 20, 2012
 *      Author: dupes
 */

#ifndef FRAME_H_
#define FRAME_H_

#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <map>

#include "Database.h"

#include "Object.h"

#include "CVImage.h"

using namespace std;
using namespace cv;

struct tFrame
{
	int id;
	int video_id;
	int num_objects;
	Mat original_image;
	Mat bw_image;
	Mat contour_image;
	Mat reduced_image;
	int number;

	map<int, tObject*> objects;

	bool objectsLoaded;
};


class Frame
{
private:
	sqlite3_stmt *m_insertFrame;
	sqlite3_stmt *m_getFrameID;
	sqlite3_stmt *m_findFramesByVideoID;
	sqlite3_stmt *m_findFramesByVideoIDLimit;
	sqlite3_stmt *m_findFrameByVideoIDAndNumber;

	Database *m_db;

	Object m_objects;

	bool parseFrame(tFrame &frame, sqlite3_stmt *statement);

public:
	Frame(Database *db);
	virtual ~Frame();

	bool saveFrame(tFrame *frame);

	bool getID(int videoID, int number, int &id);

	bool findFrames(int videoID, int numFrames);

	bool findFrame(int videoID, int number, tFrame &frame);

	bool nextFrame(tFrame &frame);

	tFrame *nextFrame();

	bool findObjects(int frameID);

	bool nextObject(tObject &object);

	tObject *nextObject();
};

#endif /* FRAME_H_ */
