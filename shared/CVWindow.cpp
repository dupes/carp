/*
 * CVWindow.cpp
 *
 *  Created on: May 9, 2011
 *      Author: dupes
 */

#include "CVWindow.h"

CVWindow::CVWindow() {
	m_pszName = NULL;
}

/*********************************************************************/

CVWindow::~CVWindow() {
	destroyWindow();
}

/*********************************************************************/

int CVWindow::createWindow(const char *name, int flags) {
	m_name = name;

	m_pszName = m_name.c_str();

	return cvNamedWindow(name, flags);
}

/*********************************************************************/

void CVWindow::resizeWindow(int width, int height) {
	return cvResizeWindow(m_pszName, width, height);
}

/*********************************************************************/

const char *CVWindow::getWindowName() {
	return m_pszName;
}

/*********************************************************************/

void CVWindow::showImage(const CvArr *image) {
	return cvShowImage(m_pszName, image);
}

/*********************************************************************/

void CVWindow::showImage(InputArray mat)
{
	imshow(m_name, mat);
}

/*********************************************************************/

void CVWindow::destroyWindow() {
	if (m_pszName != NULL)
		cvDestroyWindow(m_pszName);
}

/*********************************************************************/

void CVWindow::moveWindow(int x, int y) {
	cvMoveWindow(m_pszName, x, y);
}

/*********************************************************************/

int CVWindow::waitKey(int delay) {
	return cvWaitKey(delay);
}

/*********************************************************************/

void CVWindow::destroyAllWindows() {
	cvDestroyAllWindows();
}

/*********************************************************************/

int CVWindow::initSystem(int argc, char **argv) {
	return cvInitSystem(argc, argv);
}
