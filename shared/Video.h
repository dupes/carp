/*
 * Video.h
 *
 *  Created on: Nov 20, 2012
 *      Author: dupes
 */

#ifndef VIDEO_H_
#define VIDEO_H_

#include <string>
#include <map>

#include <opencv2/imgproc/imgproc.hpp>

#include "Database.h"
#include "Frame.h"


using namespace std;
using namespace cv;



struct tVideo
{
	int id;
	std::string name;
	int rows;
	int cols;

	int threshold1;
	int threshold2;

	map<int, tFrame*> frames;
};

class Video {
private:
	Database *m_db;

	tVideo m_video;

	Frame m_frame;

	bool findFrames(tVideo &video, int numFrames = -1, bool loadObjects = false);

public:
	Video(Database *db);
	virtual ~Video();

	bool saveVideo(tVideo &video);

	bool loadVideo(string name, int numFrames = -1, bool loadObjects = false);
	tVideo *getVideo();

	bool findVideo(string name, tVideo &video);
	bool findVideo(int id, tVideo &video);

	bool getID(string name, int &id);

	bool findObjects(tFrame &frame);

	bool nextFrame(tFrame &frame);

	bool findFrame(int videoID, int frameNumber, tFrame &frame);

	bool countFrames(tVideo &video, int &numFrames);

	bool findObjects(int frameID);

	bool nextObject(tObject &object);

	map<int, tFrame*> frames();
};

#endif /* VIDEO_H_ */
