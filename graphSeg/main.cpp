#include "graph.h"
#include "MLSmooth.h"
#include "cv.h"
#include "highgui.h"
int main()
{
	
	cvNamedWindow("graph", CV_WINDOW_AUTOSIZE);
	IplImage * im = cvLoadImage("input.ppm");


	GraphSeg(im);
	cvShowImage("graph", im);

	cvWaitKey(0);

	cvSaveImage("output.jpg", im);

	cvDestroyWindow("graph");
	cvReleaseImage(&im);
	return  0;
}
