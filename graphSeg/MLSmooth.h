#ifndef ML_SMOOTH__
#define ML_SMOOTH__

#include "cv.h"
#include "highgui.h"
#include <vector>
#include <cmath>
#define GAUSSIAN_WIDTH 4.0

#define GAUSSIAN_SIGMA 0.5 

void norm(std::vector<float> & mask)
{
	int len = mask.size();
	float sum = 0;
	for(int i = 1; i < len; i ++)
	{
		sum += fabs(mask[i]);
	}
	sum = 2*sum + fabs(mask[0]);

	for(int i =0; i < len; i ++)
	{
		mask[i] /= sum;
	}
};

IplImage * gaussianSmooth(IplImage * im)
{
	int width = im ->width;
	int height = im ->height;
	int step = im ->widthStep;


	int len = (int) ceil(GAUSSIAN_SIGMA *  GAUSSIAN_WIDTH) + 1;
	std::vector<float> mask(len);

	for(int i = 0; i< len; i ++)
	{
		mask[i] = exp(-0.5 *((i/GAUSSIAN_SIGMA)* (i/GAUSSIAN_SIGMA)));
	}

	norm(mask);
	

	IplImage * tmp = cvCreateImage(cvSize( height, width), 32, 3);
	IplImage * output = cvCreateImage(cvSize( width,height ), 32, 3);


	uchar * sptr ;//= (uchar*)im ->imageData;
	float * dptr ;//= (uchar*)tmp ->imageData;


	for(int y = 0; y < height; y ++)
	{
		sptr = (uchar*) (y * im ->widthStep + im ->imageData);
		for(int x = 0; x < width; x ++)//,sptr += 3)
		{

			float sumR = mask[0] *(float) sptr[x*3 +0];
			float sumG = mask[0] *(float) sptr[x*3 +1];
			float sumB = mask[0] *(float) sptr[x*3 +2];

			
			for(int i =1; i < len; i ++)
			{
				
				int leftOffset = std::max(x - i, 0);
				int rightOffset = std::min(x + i, width -1);

				uchar * left = (uchar*) (im ->imageData + y*im ->widthStep);// + leftOffset *3);
				uchar * right =(uchar*) ( im -> imageData +  y* im ->widthStep);//  + rightOffset *3);

				sumR += (float) (mask[i] * (left[leftOffset *3 +0] +right[rightOffset *3 +0]));
				sumG += (float) (mask[i] * (left[leftOffset *3 +1] +right[rightOffset *3 +1]));
				sumB += (float) (mask[i] * (left[leftOffset *3 +2] +right[rightOffset *3 +2]));
				
			}

			
			dptr = (float*)  (tmp ->imageData + x * tmp->widthStep);// + y *3);

			 dptr[y*3 + 0] = sumR;
			 dptr[y*3 + 1] = sumG;
			 dptr[y*3 + 2] = sumB;

		}
	}

	//cvConvertScale(tmp, tmp, 255);
	//cvShowImage("graph", tmp);
	//cvWaitKey(0);
	
	float * ddptr = NULL;
	for(int y = 0; y < width; y ++)
	{
		dptr = (float *) (y * tmp ->widthStep + tmp ->imageData);
		for(int x = 0; x < height; x ++)//, dptr += 3)
		{
			float sumR =  mask[0] * (float)dptr[x*3 +0];
			float sumG =  mask[0] * (float)dptr[x*3 +1];
			float sumB =  mask[0] * (float)dptr[x*3 +2];

			for(int i = 1; i < len; i ++)
			{
				
				int leftOffset = std::max(0, x -i);
				int rightOffset = std::min(height -1, x +i);

				float* left = (float*) (tmp ->imageData +y * tmp->widthStep);// + leftOffset *3);
				float* right =(float*) ( tmp ->imageData + y* tmp ->widthStep);// + rightOffset*3);
				sumR +=(float)  (mask[i] * ((float)left[leftOffset *3 +0] + (float)right[rightOffset *3 +0]));
				sumG += (float) (mask[i] * ((float)left[leftOffset *3 +1] + (float)right[rightOffset *3 +1]));
				sumB += (float) (mask[i] * ((float)left[leftOffset *3 +2] + (float)right[rightOffset *3 +2]));
			}

			
			ddptr =(float*) (output ->imageData + x * output ->widthStep);// + y *3);

			ddptr[y*3 + 0] = (float)sumR;
			ddptr[y*3 + 1] = (float)sumG;
			ddptr[y*3 + 2] = (float)sumB;
		}
	}

	cvReleaseImage(&tmp);
	//cvNamedWindow("hehe");
	//cvConvertScale(output, output, 255, 0);
//	cvShowImage("graph", output);

	//cv::imshow("graph",cv::cvarrToMat(output));
	//cv::waitKey(0);

	//cvWaitKey(0);

//	output = im;
	return output;
	
};


IplImage * MLSmooth(IplImage *im)
{


	return gaussianSmooth(im);


	//return 0;
	
};

#endif
