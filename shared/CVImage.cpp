/*
 * CVImage.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: dupes
 */

#include "CVImage.h"
#include <stdio.h>

CVImage::CVImage()
{
}

/*********************************************************************/

CVImage::~CVImage()
{
}

/*********************************************************************/

/*
 * adapted from: modules/core/src/copyTo
 * this should be modified to avoid the extra mat.copyTo();
 */
int CVImage::image2dToBuffer(Mat mat, Buffer &buffer)
{
	int numBytes = mat.cols * mat.rows * mat.elemSize();
	int bufferSize = numBytes + sizeof(mat.cols)*2 + sizeof(mat.type());
	tImageHeader *header;

	Mat copy;

	mat.copyTo(copy);

	//unsigned char *buffer;

/*	if (bufferSize < len && len != -1)
	{
		delete [] (&buffer);
		len = -1;

		printf("realloc buffer\n");
	}

	if (len == -1)
	{
		(*buffer) = new unsigned char[bufferSize];
		len = bufferSize;
	}
*/
	buffer.realloc(bufferSize);

	header = (tImageHeader *)(buffer.getBuffer());

	header->rows = mat.rows;
	header->cols = mat.cols;
	header->type = mat.type();

	// memcpy((buffer.getBuffer()) + sizeof(tImageHeader), mat.data, numBytes);
	memcpy((buffer.getBuffer()) + sizeof(tImageHeader), copy.data, numBytes);

	buffer.setNumBytes(bufferSize);

	// return buffer;
	return bufferSize;
}

/*********************************************************************/

bool CVImage::bufferToImage2d(Buffer &buffer, Mat &mat)
{
	tImageHeader *header;
	size_t numBytes;

	// buffer.realloc(sizeof(tImageHeader));

	header = (tImageHeader *)buffer.getBuffer();

	// printf("header: %d, %d, %d\n", header->rows, header->cols, header->type);

	mat.create(header->rows, header->cols, header->type);

	numBytes = mat.cols * mat.rows * mat.elemSize();

	// buffer.realloc(numBytes + sizeof(tImageHeader));

	memcpy(mat.data, (buffer.getBuffer()) + sizeof(tImageHeader), numBytes);

	buffer.setNumBytes(numBytes + sizeof(tImageHeader));

	return true;
}
