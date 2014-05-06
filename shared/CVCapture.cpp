/*
 * CVCapture.cpp
 *
 *  Created on: May 9, 2011
 *      Author: dupes
 */

#include "CVCapture.h"

CVCapture::CVCapture() {
	m_capture = NULL;
}

/*********************************************************************/

CVCapture::~CVCapture() {
}

/*********************************************************************/

bool CVCapture::openDevice(int device)
{
	if ((m_capture = cvCaptureFromCAM(device)) != NULL)
		return true;

	return false;
}

/*********************************************************************/

bool CVCapture::openFile(const char *file)
{
	if ((m_capture = cvCaptureFromFile(file)) != NULL)
		return true;

	return false;
}

/*********************************************************************/

int CVCapture::setResolution(double width, double height)
{
	setCaptureProperty(CV_CAP_PROP_FRAME_WIDTH, width);
	setCaptureProperty(CV_CAP_PROP_FRAME_HEIGHT, height);

	return 1;
}

/*********************************************************************/

void CVCapture::release()
{
	cvReleaseCapture(&m_capture);
}

/*********************************************************************/

double CVCapture::getCaptureProperty(int propertyId)
{
	return cvGetCaptureProperty(m_capture, propertyId);
}

/*********************************************************************/

int CVCapture::setCaptureProperty(int propertyId, double value)
{
	return cvSetCaptureProperty(m_capture, propertyId, value);
}

/*********************************************************************/

IplImage *CVCapture::queryFrame()
{
	return cvQueryFrame(m_capture);
}
