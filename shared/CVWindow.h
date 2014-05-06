/*
 * CVWindow.h
 *
 *  Created on: May 9, 2011
 *      Author: dupes
 */

#ifndef CVWINDOW_H_
#define CVWINDOW_H_

#include <opencv2/highgui/highgui.hpp>

#include <string>

using namespace cv;

class CVWindow {
private:
	std::string m_name;
	const char *m_pszName;

public:
	CVWindow();
	virtual ~CVWindow();

	int createWindow(const char *name, int flags);

	void resizeWindow(int width, int height);

	const char *getWindowName();

	void showImage(const CvArr *image);

	void showImage(InputArray mat);

	void destroyWindow();

	void moveWindow(int x, int y);

	// virtual void mouseCallback();
	// virtual void trackbarCallback(int position);

	static int waitKey(int delay = 0);

	static void destroyAllWindows();
	static int initSystem(int argc, char **argv);
};

#endif /* CVWINDOW_H_ */
