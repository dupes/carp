/*
 * FindEdges.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: dupes
 */

#include "FindEdges.h"

/*********************************************************************/

FindEdges::FindEdges(Mat &mat, BlurImage *blurImage)
{
	// m_edges = mat; //cvCreateImage(cvGetSize(image), image->depth, 1);

	m_blurImage = blurImage;

	/*if (m_usePyramids)
	{
		m_tmp = cvCreateImage(cvGetSize(image), image->depth, 1);
		m_pyramid = cvCreateImage(cvSize(image->width / 2, image->height / 2), image->depth, 1);
	}*/
}

/*********************************************************************/

FindEdges::~FindEdges()
{
	// TODO Auto-generated destructor stub
}

/*********************************************************************/

IplImage *FindEdges::usePyramid(IplImage *image)
{
	/*if (m_usePyramids)
	{
		cvPyrDown(image, m_pyramid);
		cvPyrUp(m_pyramid, m_tmp);

		return m_tmp;
	}*/

	return image;
}

/*********************************************************************/
