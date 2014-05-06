
#include "shared/CVCapture.h"
#include "shared/CVWindow.h"
#include "shared/CVImage.h"

#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

Size getContinuousSize( const Mat& m1, int widthScale=1 )
{
    return m1.isContinuous() ? Size(m1.cols*m1.rows*widthScale, 1) :
        Size(m1.cols*widthScale, m1.rows);
}

/**************************************************************/

Size getContinuousSize( const Mat& m1, const Mat& m2, int widthScale=1 )
{
    return (m1.flags & m2.flags & Mat::CONTINUOUS_FLAG) != 0 ?
        Size(m1.cols*m1.rows*widthScale, 1) : Size(m1.cols*widthScale, m1.rows);
}

/**************************************************************/

void test1()
{
	CVCapture capture;
	Mat image;

	Mat copy;

	Buffer buffer;

	CVWindow main;

	main.createWindow("main", 0);

	if (!capture.openFile("/home/dupes/projects/classes/cs446/project/source/object_detection/videos/pacman1.avi"))
	{
		printf("failed to open file\n");
		return;
	}

	image = capture.queryFrame();

	printf("dims: %d\n", image.dims);

	main.showImage(image);
	CVWindow::waitKey(-1);

	int length = -1;
	CVImage::image2dToBuffer(image, buffer);

	CVImage::bufferToImage2d(buffer, copy);

	printf("buffer len: %d\n", buffer.getNumBytes());

	main.showImage(copy);
	CVWindow::waitKey(-1);
}

/**************************************************************/

void test2()
{
	CVCapture capture;
	Mat image;
	Mat first;

	Mat copy;

	Buffer buffer;

	CVWindow main;

	main.createWindow("main", 0);

	if (!capture.openFile("/home/dupes/projects/classes/cs446/project/source/object_detection/videos/pacman1.avi"))
	{
		printf("failed to open file\n");
		return;
	}

	first = capture.queryFrame();

	Rect rect(20, 20, 40, 40);

	image = first(rect);

	printf("dims: %d\n", image.dims);

	main.showImage(image);
	CVWindow::waitKey(-1);

	int length = -1;
	CVImage::image2dToBuffer(image, buffer);

	CVImage::bufferToImage2d(buffer, copy);

	printf("buffer len: %d\n", buffer.getNumBytes());

	main.showImage(copy);
	CVWindow::waitKey(-1);

}

/**************************************************************/

int main()
{
	test2();

	return 0;
}
