/*
 * MainWindow.h
 *
 *  Created on: Nov 21, 2012
 *      Author: dupes
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <vector>
#include <map>
#include "shared/CVWindow.h"
#include "shared/Video.h"

#include "MatchTemplate.h"

// #include <keysymdef.h>
#include <X11/keysym.h>

class MainWindow
{
private:
	CVWindow m_main;
	// CVWindow m_objectWindow;
	int m_frameObjectsID;
	vector<tMatch> *matches;
	vector<tMatch> *allMatches;

	std::string m_labelFilter;
	bool m_showLabeledObjects;

	Video m_video;

	tFrame *m_tframe;

	tObject *m_object;
	vector<tObject*> m_homographyPoints;

	int m_trackBarValue;

	double m_templateThresholdValue;

	string getUserInput(string prompt);

	void setTrackbarPosition(int position);
	void handleMouseButtonClick(int event, int x, int y, int flags);

	void findObject(int x, int y);
	void findHomography();

	void findMatching();
	void saveLabels();
	void setLabelFilter();

	void getThreshold();

	void displayFrame();

	friend void trackBarCallback(int position, void *data);

	friend void thresholdTrackBarCallback(int position, void *data);

	friend void mouseButtonClickCallback(int event, int x, int y, int flags, void* param);

	void printCommands();

public:
	MainWindow();
	virtual ~MainWindow();

	void loop(string videoName, int numFrames = -1);
};

#endif /* MAINWINDOW_H_ */
