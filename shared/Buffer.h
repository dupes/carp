/*
 * Buffer.h
 *
 *  Created on: Nov 21, 2012
 *      Author: dupes
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdlib.h>
#include <string.h>

class Buffer
{
private:
	unsigned char *m_buffer;
	int m_length;

	int m_numBytes;

public:
	Buffer();
	virtual ~Buffer();

	unsigned char *getBuffer();
	int getBufferLength();

	int getNumBytes();

	void realloc(int length);

	void memcpy(unsigned char *src, int length);

	void reset();

	void setNumBytes(int numBytes);
};

#endif /* BUFFER_H_ */
