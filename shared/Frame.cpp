/*
 * Frame.cpp
 *
 *  Created on: Nov 20, 2012
 *      Author: dupes
 */

#include <stdio.h>

#include "Frame.h"

Frame::Frame(Database *db)
	: m_objects(db)
{
	std::string sql;

	m_db = db;

	sql = "Insert into frames (video_id, num_objects, original_image, bw_image, number) values (?, ?, ?, ?, ?)";
	m_db->prepareStatement(&m_insertFrame, sql);

	sql = "Select id from frames where video_id = ? and number = ?";
	m_db->prepareStatement(&m_getFrameID, sql);

	sql = "Select id, video_id, num_objects, original_image, bw_image, contour_image, reduced_image, number from frames where video_id = ?";
	m_db->prepareStatement(&m_findFramesByVideoID, sql);

	sql = "Select id, video_id, num_objects, original_image, bw_image, contour_image, reduced_image, number from frames where video_id = ? order by number, number limit ?";
	m_db->prepareStatement(&m_findFramesByVideoIDLimit, sql);

	sql = "Select id, video_id, num_objects, original_image, bw_image, contour_image, reduced_image, number from frames where video_id = ? and number = ?";
	m_db->prepareStatement(&m_findFrameByVideoIDAndNumber, sql);
}

/*********************************************************************/

Frame::~Frame()
{
	sqlite3_finalize(m_insertFrame);
	sqlite3_finalize(m_getFrameID);
	sqlite3_finalize(m_findFramesByVideoID);
	sqlite3_finalize(m_findFramesByVideoIDLimit);
	sqlite3_finalize(m_findFrameByVideoIDAndNumber);
}

/*********************************************************************/

bool Frame::saveFrame(tFrame *frame)
{
	int retval;

	/*static unsigned char *originalImageBuffer = NULL;
	static int originalImageBufferLength = -1;

	static unsigned char *bwImageBuffer = NULL;
	static int bwImageBufferLength = -1;*/
	static Buffer originalImage;
	static Buffer bwImage;

	// static unsigned char *contourImageBuffer = NULL;
	// static int contourImageBufferLength = -1;

	CVImage::image2dToBuffer(frame->original_image, originalImage);
	CVImage::image2dToBuffer(frame->bw_image, bwImage);
	// contourImageBufferLength = CVImage::image2dToBuffer(frame->contour_image, contourImageBuffer, contourImageBufferLength);

	sqlite3_bind_int(m_insertFrame, 1, frame->video_id);
	sqlite3_bind_int(m_insertFrame, 2, frame->num_objects);
	sqlite3_bind_blob(m_insertFrame, 3, originalImage.getBuffer(), originalImage.getNumBytes(), SQLITE_STATIC);
	sqlite3_bind_blob(m_insertFrame, 4, bwImage.getBuffer(), bwImage.getNumBytes(), SQLITE_STATIC);
	// sqlite3_bind_blob(statement, 5, contourImageBuffer, contourImageBufferLength, SQLITE_STATIC);
	sqlite3_bind_int(m_insertFrame, 5, frame->number);

	retval = sqlite3_step(m_insertFrame);

	sqlite3_reset(m_insertFrame);
	sqlite3_clear_bindings(m_insertFrame);

	if (retval != SQLITE_DONE)
		return false;

	getID(frame->video_id, frame->number, frame->id);

	return true;
}

/*********************************************************************/

bool Frame::getID(int videoID, int number, int &id)
{
	int retval;

	id = -1;

	sqlite3_bind_int(m_getFrameID, 1, videoID);
	sqlite3_bind_int(m_getFrameID, 2, number);

	if ((retval = sqlite3_step(m_getFrameID)) == SQLITE_DONE)
	{
		printf("getID: %d, %s\n\n", retval, m_db->getErrorMessage());
		return false;
	}

	id = sqlite3_column_int(m_getFrameID, 0);

	sqlite3_reset(m_getFrameID);
	sqlite3_clear_bindings(m_getFrameID);

	return true;
}

/*********************************************************************/

bool Frame::parseFrame(tFrame &frame, sqlite3_stmt *statement)
{
	// int retval;
	sqlite3_blob *blob;
	int size;

	static Buffer buffer;

	frame.id = sqlite3_column_int(statement, 0);
	frame.video_id = sqlite3_column_int(statement, 1);
	frame.num_objects = sqlite3_column_int(statement, 2);

	blob = (sqlite3_blob *)sqlite3_column_blob(statement, 3);
	// size = sqlite3_blob_bytes(blob);
	size = sqlite3_column_bytes(statement, 3);
	buffer.memcpy((unsigned char *)blob, size);

	frame.number = sqlite3_column_int(statement, 7);

	CVImage::bufferToImage2d(buffer, frame.original_image);

	return true;
}

/*********************************************************************/

bool Frame::findFrame(int videoID, int number, tFrame &frame)
{
	int retval;
	bool success = true;

	sqlite3_bind_int(m_findFrameByVideoIDAndNumber, 1, videoID);
	sqlite3_bind_int(m_findFrameByVideoIDAndNumber, 2, number);

	if ((retval = sqlite3_step(m_findFrameByVideoIDAndNumber)) == SQLITE_DONE)
	{
		// printf("getID: %d, %s\n\n", retval, m_db->getErrorMessage());
		success = false;
	}

	else
		parseFrame(frame, m_findFrameByVideoIDAndNumber);

	sqlite3_reset(m_findFrameByVideoIDAndNumber);
	sqlite3_clear_bindings(m_findFrameByVideoIDAndNumber);

	return success;
}

/*********************************************************************/

bool Frame::findFrames(int videoID, int numFrames)
{
	sqlite3_bind_int(m_findFramesByVideoIDLimit, 1, videoID);
	sqlite3_bind_int(m_findFramesByVideoIDLimit, 2, numFrames);

	return true;
}

/*********************************************************************/

bool Frame::nextFrame(tFrame &frame)
{
	int retval;

	if ((retval = sqlite3_step(m_findFramesByVideoID)) == SQLITE_DONE)
	{
		sqlite3_reset(m_findFramesByVideoID);
		sqlite3_clear_bindings(m_findFramesByVideoID);

		return false;
	}

	return parseFrame(frame, m_findFramesByVideoID);
}

/*********************************************************************/

tFrame *Frame::nextFrame()
{
	tFrame *frame = NULL;

	int retval;

	if ((retval = sqlite3_step(m_findFramesByVideoIDLimit)) == SQLITE_DONE)
	{
		sqlite3_reset(m_findFramesByVideoIDLimit);
		sqlite3_clear_bindings(m_findFramesByVideoIDLimit);

		return NULL;
	}

	frame = new tFrame();

	if (!parseFrame(*frame, m_findFramesByVideoIDLimit))
	{
		delete (frame);
		return NULL;
	}

	frame->objectsLoaded = false;

	return frame;
}

/*********************************************************************/

bool Frame::findObjects(int frameID)
{
	return m_objects.findObjects(frameID);
}

/*********************************************************************/

bool Frame::nextObject(tObject &object)
{
	return m_objects.nextObject(object);
}

/*********************************************************************/

tObject *Frame::nextObject()
{
	return m_objects.nextObject();
}
