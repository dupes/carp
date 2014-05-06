/*
 * Object.cpp
 *
 *  Created on: Nov 20, 2012
 *      Author: dupes
 */

#include <stdio.h>

#include "Object.h"

Object::Object(Database *db)
{
	string sql;

	m_db = db;

	sql = "insert into objects (frame_id, object_image, x, y, width, height, number) values (?, ?, ?, ?, ?, ?, ?)";
	m_db->prepareStatement(&m_insertObject, sql);

	sql = "select id, frame_id, object_image, contour, x, y, width, height, number, label, verified from objects where frame_id = ?";
	m_db->prepareStatement(&m_findObjectsByFrameID, sql);

	sql = "select c.id, c.frame_id, c.object_image, c.contour, c.x, c.y, c.width, c.height, c.number, c.label, c.verified from ";
	sql += "videos a inner join frames b on a.id = b.video_id inner join objects c on b.id = c.frame_id where label is not null and label <> '' and verified == 1";
	// sql += " and height = 22 and width = 21 ";
	// sql += " and label = 'pacman'";

	// sql = "select id, frame_id, object_image, contour, x, y, width, height, number, label, verified from objects order by label desc limit 5000";
	// sql = "select id, frame_id, object_image, contour, x, y, width, height, number, label, verified from objects where height <= 50 and width <= 50";// and height >= 10 and width >= 10";

	m_db->prepareStatement(&m_findObjectsByVideoID, sql);

	sql = "update objects set label = ?, verified = ? where id = ?";
	m_db->prepareStatement(&m_updateObject, sql);

	m_currentSelect = NULL;
}

/*********************************************************************/

Object::~Object()
{
}

/*********************************************************************/

bool Object::saveObject(tObject *object)
{
	int retval;

	static Buffer objectImage;

	CVImage::image2dToBuffer(object->object_image, objectImage);

	sqlite3_bind_int(m_insertObject, 1, object->frame_id);
	sqlite3_bind_blob(m_insertObject, 2, objectImage.getBuffer(), objectImage.getNumBytes(), SQLITE_STATIC);
	// countour sqlite3_bind_blob(m_insertObject, 3, bwImageBuffer, bwImageBufferLength, SQLITE_STATIC);
	sqlite3_bind_int(m_insertObject, 3, object->boundingBox.x);
	sqlite3_bind_int(m_insertObject, 4, object->boundingBox.y);
	sqlite3_bind_int(m_insertObject, 5, object->boundingBox.width);
	sqlite3_bind_int(m_insertObject, 6, object->boundingBox.height);
	sqlite3_bind_int(m_insertObject, 7, object->number);

	retval = sqlite3_step(m_insertObject);

	sqlite3_reset(m_insertObject);
	sqlite3_clear_bindings(m_insertObject);

	if (retval != SQLITE_DONE)
	{
		printf("%d, %s\n", retval, m_db->getErrorMessage());

		exit(0);

		return false;
	}

	return true;
}

/*********************************************************************/

bool Object::updateObject(tObject *object)
{
	int retval;

	sqlite3_bind_text(m_updateObject, 1, object->label.c_str(), object->label.length(), SQLITE_STATIC);
	sqlite3_bind_int(m_updateObject, 2, (object->verified ? 1 : 0));
	sqlite3_bind_int(m_updateObject, 3, object->id);

	retval = sqlite3_step(m_updateObject);

	if (retval != SQLITE_DONE)
	{
		printf("%d, %s\n", retval, m_db->getErrorMessage());

		exit(0);

		return false;
	}

	sqlite3_reset(m_updateObject);
	sqlite3_clear_bindings(m_updateObject);

	return true;
}

/*********************************************************************/

bool Object::findObjects(int frameID)
{
	sqlite3_reset(m_findObjectsByFrameID);
	sqlite3_clear_bindings(m_findObjectsByFrameID);

	sqlite3_bind_int(m_findObjectsByFrameID, 1, frameID);

	m_currentSelect = m_findObjectsByFrameID;

	return true;
}

/*********************************************************************/

bool Object::findObjectsByVideoID(int videoID)
{
	sqlite3_reset(m_findObjectsByVideoID);
	sqlite3_clear_bindings(m_findObjectsByVideoID);

	// sqlite3_bind_int(m_findObjectsByVideoID, 1, videoID);

	m_currentSelect = m_findObjectsByVideoID;

	return true;
}

/*********************************************************************/

bool Object::parseObject(tObject &object)
{
	sqlite3_blob *blob;
	int size;

	static Buffer buffer;

	object.id = sqlite3_column_int(m_currentSelect, 0);
	object.frame_id = sqlite3_column_int(m_currentSelect, 1);

	blob = (sqlite3_blob *)sqlite3_column_blob(m_currentSelect, 2);
	// size = sqlite3_blob_bytes(blob);
	size = sqlite3_column_bytes(m_currentSelect, 2);
	buffer.memcpy((unsigned char *)blob, size);
	CVImage::bufferToImage2d(buffer, object.object_image);

	object.boundingBox = Rect(
		sqlite3_column_int(m_currentSelect, 4), // x
		sqlite3_column_int(m_currentSelect, 5), // y
		sqlite3_column_int(m_currentSelect, 6), // width
		sqlite3_column_int(m_currentSelect, 7)  // height
	);

	object.number = sqlite3_column_int(m_currentSelect, 8);

	if (sqlite3_column_text(m_currentSelect, 9) != NULL)
		object.label = (char *)sqlite3_column_text(m_currentSelect, 9);

	object.verified = (sqlite3_column_int(m_currentSelect, 10) == 0 ? false : true);

	return true;
}

/*********************************************************************/

bool Object::nextObject(tObject &object)
{
	int retval;

	if ((retval = sqlite3_step(m_currentSelect)) == SQLITE_DONE)
	{
		sqlite3_reset(m_currentSelect);
		sqlite3_clear_bindings(m_currentSelect);

		return false;
	}

	parseObject(object);

	return true;
}

/*********************************************************************/

tObject *Object::nextObject()
{
	int retval;
	tObject *object = NULL;

	if ((retval = sqlite3_step(m_currentSelect)) == SQLITE_DONE)
	{
		sqlite3_reset(m_currentSelect);
		sqlite3_clear_bindings(m_currentSelect);

		return NULL;
	}

	object = new tObject();

	parseObject(*object);

	return object;
}
