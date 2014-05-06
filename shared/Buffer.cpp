/*
 * Buffer.cpp
 *
 *  Created on: Nov 21, 2012
 *      Author: dupes
 */

#include "Buffer.h"

Buffer::Buffer()
{
	m_buffer = NULL;
	m_length = -1;

	m_numBytes = -1;
}

/*********************************************************************/

Buffer::~Buffer()
{
	if (m_buffer != NULL)
		free (m_buffer);
}

/*********************************************************************/

unsigned char *Buffer::getBuffer()
{
	return m_buffer;
}

/*********************************************************************/

int Buffer::getBufferLength()
{
	return m_length;
}

/*********************************************************************/

int Buffer::getNumBytes()
{
	return m_numBytes;
}

/*********************************************************************/

void Buffer::realloc(int length)
{
	if (length > m_length)
	{
		m_buffer = (unsigned char *)::realloc(m_buffer, length);
		m_length = length;
	}
}

/*********************************************************************/

void Buffer::memcpy(unsigned char *src, int length)
{
	if (length > m_length)
		realloc(length);

	::memcpy(m_buffer, src, length);

	m_numBytes = length;
}

/*********************************************************************/

void Buffer::reset()
{
	m_numBytes = 0;
}

/*********************************************************************/

void Buffer::setNumBytes(int numBytes)
{
	m_numBytes = numBytes;
}

/*********************************************************************/
