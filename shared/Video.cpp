/*
 * Video.cpp
 *
 *  Created on: Nov 20, 2012
 *      Author: dupes
 */

#include <stdio.h>

#include "Video.h"

Video::Video(Database *db)
	: m_frame(db)
{
	m_db = db;
}

/*********************************************************************/

Video::~Video()
{
}

/*********************************************************************/

bool Video::saveVideo(tVideo &video)
{
	std::string sql;
	sqlite3_stmt *statement;
	int retval;

	sql = "Insert into Videos (name, rows, cols, threshold1, threshold2) values (?, ?, ?, ?, ?)";

	if (!m_db->prepareStatement(&statement, sql))
		return false;

	sqlite3_bind_text(statement, 1, video.name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(statement, 2, video.rows);
	sqlite3_bind_int(statement, 3, video.cols);
	sqlite3_bind_int(statement, 4, video.threshold1);
	sqlite3_bind_int(statement, 5, video.threshold2);

	retval = sqlite3_step(statement);

	sqlite3_finalize(statement);

	if (retval != SQLITE_DONE)
		return false;

	return getID(video.name, video.id);
}

/*********************************************************************/

bool Video::getID(string name, int &id)
{
	string sql = "select id from videos where name = ?";
	sqlite3_stmt *statement;
	int retval;

	if (!m_db->prepareStatement(&statement, sql))
		return false;

	sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);

	retval = sqlite3_step(statement);

	if (retval == SQLITE_DONE)
	{
		sqlite3_finalize(statement);
		return false;
	}

	id = sqlite3_column_int(statement, 0);

	sqlite3_finalize(statement);

	return true;
}

/*********************************************************************/

bool Video::loadVideo(string name, int numFrames, bool loadObjects)
{
	if (!findVideo(name, m_video))
		return false;

	return findFrames(m_video, numFrames, loadObjects);
}

/*********************************************************************/

tVideo *Video::getVideo()
{
	return &m_video;
}

/*********************************************************************/

bool Video::findVideo(string name, tVideo &video)
{
	//tVideo *video = NULL; //= new tVideo();
	string sql = "select id, name, rows, cols, threshold1, threshold2 from videos where name = ?";
	sqlite3_stmt *statement;
	int retval;

	if (!m_db->prepareStatement(&statement, sql))
		return false;

	sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);

	retval = sqlite3_step(statement);

	if (retval == SQLITE_DONE)
	{
		sqlite3_finalize(statement);
		return false;
	}

	video.id = sqlite3_column_int(statement, 0);
	video.name = (char *)sqlite3_column_text(statement, 1);
	video.rows = sqlite3_column_int(statement, 2);
	video.cols = sqlite3_column_int(statement, 3);
	video.threshold1 = sqlite3_column_int(statement, 4);
	video.threshold2 = sqlite3_column_int(statement, 5);

	sqlite3_finalize(statement);

	return true;
}


/*********************************************************************/
/*
bool Video::findVideo(int id, tVideo &video)
{
	return false;
}
*/
/*********************************************************************/

bool Video::findFrames(tVideo &video, int numFrames, bool loadObjects)
{
	if (!m_frame.findFrames(video.id, numFrames))
		return false;

	tFrame *frame;

	while ((frame = m_frame.nextFrame()) != NULL)
	{
		m_video.frames[frame->number] = frame;

		if (loadObjects)
			findObjects(*frame);
	}

	return true;
}

/*********************************************************************/

bool Video::findObjects(tFrame &frame)
{
	if (frame.objectsLoaded)
		return true;

	if (!m_frame.findObjects(frame.id))
		return false;

	tObject *object;

	while ((object = m_frame.nextObject()) != NULL)
	{
		frame.objects[object->number] = object;
	}

	frame.objectsLoaded = true;

	return true;
}

/*********************************************************************/

bool Video::nextFrame(tFrame &frame)
{
	return m_frame.nextFrame(frame);
}

/*********************************************************************/

bool Video::countFrames(tVideo &video, int &numFrames)
{
	string sql = "select count(*) from frames where video_id = ?";
	sqlite3_stmt *statement;
	int retval;

	if (!m_db->prepareStatement(&statement, sql))
		return false;

	sqlite3_bind_int(statement, 1, video.id);

	retval = sqlite3_step(statement);

	if (retval == SQLITE_DONE)
	{
		sqlite3_finalize(statement);
		return false;
	}

	numFrames = sqlite3_column_int(statement, 0);

	sqlite3_finalize(statement);

	return true;
}

/*********************************************************************/

bool Video::findFrame(int videoID, int frameNumber, tFrame &frame)
{
	return m_frame.findFrame(videoID, frameNumber, frame);
}

/*********************************************************************/

bool Video::findObjects(int frameID)
{
	return m_frame.findObjects(frameID);
}

/*********************************************************************/

bool Video::nextObject(tObject &object)
{
	return m_frame.nextObject(object);
}

/*********************************************************************/

map<int, tFrame*> Video::frames()
{
	return m_video.frames;
}
