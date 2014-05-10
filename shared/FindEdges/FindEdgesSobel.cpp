/*
 * FindeEdgesSobel.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: dupes
 */

#include "FindEdgesSobel.h"

FindEdgesSobel::FindEdgesSobel(Mat &image, BlurImage *blurImage)
	: FindEdges(image, blurImage)
{
}

FindEdgesSobel::~FindEdgesSobel() {
	// TODO Auto-generated destructor stub
}

Mat FindEdgesSobel::findEdges(Mat &image)
{
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	// m_edges = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);

	Sobel(image, m_edges, CV_8UC1, 0, 1, 3, 1, 0, BORDER_DEFAULT );

	/*Sobel(image, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );

	Sobel(image, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, m_edges);*/

	return m_edges;
}
