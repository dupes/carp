/*
 * Database.h
 *
 *  Created on: Nov 20, 2012
 *      Author: dupes
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <sqlite3.h>

#include <string>

#include <stdio.h>

using namespace std;

class Database
{
private:

	std::string m_name;

	sqlite3 *m_db;
	int m_retval;

	void captureError();

public:
	Database();
	virtual ~Database();

	const char *getErrorMessage();
	int getRetval();

	bool open(std::string name);

	bool addVideo(std::string name, int rows, int cols);

	bool prepareStatement(sqlite3_stmt **statement, string sql);

	bool beginTransaction();
	bool commitTransaction();

	bool execSql(string sql);

	bool clear();
};

#endif /* DATABASE_H_ */
