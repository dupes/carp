/*
 * CVCapture.h
 *
 *  Created on: May 9, 2011
 *      Author: dupes
 */

#ifndef CVCAPTURE_H_
#define CVCAPTURE_H_

#include <opencv2/highgui/highgui.hpp>

class CVCapture {
private:
	CvCapture *m_capture;

public:
	CVCapture();

	virtual ~CVCapture();

	bool openDevice(int device);

	bool openFile(const char *file);

	int setResolution(double width, double height);

	void release();

	double getCaptureProperty(int propertyId);
	int setCaptureProperty(int propertyId, double value);

	IplImage *queryFrame();
};

#endif /* CVCAPTURE_H_ */
