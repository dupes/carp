/*
 * Database.cpp
 *
 *  Created on: Nov 20, 2012
 *      Author: dupes
 */

#include "Database.h"

/*********************************************************************/

Database::Database()
{
	m_db = NULL;
	m_retval = 0;
}

/*********************************************************************/

Database::~Database()
{
}

/*********************************************************************/

void Database::captureError()
{

}

/*********************************************************************/

bool Database::open(std::string name)
{
	m_name = name;

	m_retval = sqlite3_open(m_name.c_str(), &m_db);

	if (m_retval)
		return false;

	return true;
}

/*********************************************************************/

const char *Database::getErrorMessage()
{
	return sqlite3_errmsg(m_db);
}

/*********************************************************************/

int Database::getRetval()
{
	return m_retval;
}

/*********************************************************************/

bool Database::addVideo(std::string name, int rows, int cols)
{
	return true;
}

/*********************************************************************/

bool Database::prepareStatement(sqlite3_stmt **statement, std::string sql)
{
	if (sqlite3_prepare(m_db, sql.c_str(), -1, statement, NULL))
		return false;

	return true;
}

/*********************************************************************/

bool Database::beginTransaction()
{
	sqlite3_exec(m_db, "BEGIN", NULL, NULL, NULL);

	return true;
}

/*********************************************************************/

bool Database::commitTransaction()
{
	sqlite3_exec(m_db, "COMMIT", NULL, NULL, NULL);

	return true;
}

/*********************************************************************/

bool Database::execSql(string sql)
{
	int retval;

	if ((retval = sqlite3_exec(m_db, sql.c_str(), NULL, NULL, NULL)) != 0)
	{
		printf("exec error: %d, %s\n", retval, getErrorMessage());
		return false;
	}

	return true;
}

/*********************************************************************/

bool Database::clear()
{
	if (!execSql("delete from videos"))
		return false;

	if (!execSql("delete from frames"))
		return false;

	if (!execSql("delete from objects"))
		return false;

	return true;
}
