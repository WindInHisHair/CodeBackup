//author: Baiyu Xi
//date: Oct 2011
//descriptions: the code the implement the algorithm for the paper "Efficient graph-based image segmentation" IJCV 2004
//Tips: there are already the implemented cod for this algorithm, this code just for testing my idea, and prepare for the video graph-based segmentation algorithms implementation 
#ifndef  GRAPH_SEGMENT_H_INCLUDE
#define GRAPH_SEGMENT_H_INCLUDE
#include <stdlib.h>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include "cv.h"
#include "highgui.h"

#include "image.h"
#include "filter.h"

#include "segment-graph.h"
#include "graph.h"
#include "MLSmooth.h"
//#include "disjoint-set.h"
struct node
{
	float r;
	float g;
	float b;
};
float  var(IplImage *im,  int x, int y, int nx, int ny)
{
	float * sptr = (float*) (im  -> imageData+ y * im ->widthStep);//+ x*3);

//	cv::Mat src = cv::cvarrToMat(im);
//        uchar * sptr = src.data;
	//sptr +=(uchar*) ( y * im ->widthStep+ x*3);
	float s1, s2, s3;
	s1 = sptr[x *3 +0];
	s2 = sptr[x *3 +1];
	s3 = sptr[x *3 +2];

	float t1, t2, t3;
	float* dptr =(float*) ( im -> imageData + ny * im ->widthStep);//+ nx*3);
	
//	uchar *dptr = src.data;
	//dptr +=  (uchar*) (ny * im ->widthStep+ nx*3);
	t1 = dptr[nx *3 +0];
	t2 = dptr[nx *3 +1];
	t3 = dptr[nx *3 +2];
	//return sqrt(tab[s1 - t1 + 255])  +sqrt(tab[ s2 - t2 + 255]) + sqrt(tab[s3 - t3 + 255]);
	//return sqrt(tab[s1-t1+ 255] + tab[s2 -t2 + 255] + tab[s3 - t3 + 255]);
	return sqrt((t1-s1)*(t1-s1) + (t2-s2)*(t2-s2) + (t3-s3) * (t3-s3));
};


void GraphSeg(IplImage *input,int sigma = 0.5, int k = 500, int mini = 20)
{
	int width = input -> width;
	int height = input -> height;
	int step = input -> widthStep;

	//smooth

	IplImage * im = MLSmooth(input);

//	cvConvertScale(im ,im, 256.0);
//	cvShowImage("graph", im);
//	cvWaitKey(0);

	//initialize the egdes
	//
	int num = 0;
	edge *edges = new edge[width * height * 4];
	for(int y = 0 ; y < height; y ++)
	{
		for (int x = 0 ; x < width ; x ++)
		{
			if(x < width-1)
			{
				edges[num].a = y * width + x;
				edges[num].b = y *width + x + 1;
				edges[num].w = var(im, x, y, x+1, y);
				num ++;
			}
			if(y < height -1)
			{
				edges[num].a = y*width + x;	
				edges[num].b = (y+1) * width + x;
				edges[num].w = var(im, x, y, x,y+1 );
				num ++;
			}
			if((x < width - 1) && (y < height -1))
			{
				edges[num].a = y*width + x;	
				edges[num].b = (y+1) * width + (x + 1);
				edges[num].w = var(im, x ,y, x+ 1, y+1);
				num ++;
			}
			if((x < width - 1) && (y > 0))
			{

				edges[num].a = y*width + x;	
				edges[num].b = (y -1) * width + x+ 1;
				edges[num].w = var(im, x, y, x+ 1, y -1);
				num ++;
			}
		}// for widht
	}//for height


	//segmentation
	universe * univ = segment_graph(width *height, num, edges, k);
	//handle the small size region
	
	for(int i = 0; i < num; i ++)
	{
		int a = univ ->find(edges[i].a);
		int b = univ ->find(edges[i].b);

		if( (a != b) && ((univ->size(a) <mini)||(univ ->size(b) < mini) ))
			univ ->join(a,b);
	}

	//pick random color 
	struct node *colorTab = new struct node[width *height];
	for(int i = 0; i < height *width; i ++)
	{
		colorTab[i].r = random();
		colorTab[i].g = random();
		colorTab[i].b = random();
	}


//	char * ptr = im -> imageData;
	//cv::Mat src = cv::cvarrToMat(im);
	//uchar * ptr = src.data;

	for(int j = 0; j < height; j ++)
	{
		uchar * ptr =(uchar*) ( input ->imageData + j * input ->widthStep);
		for(int i = 0; i< width; i ++)//, ptr += 3)
		{
			int comp = univ -> find(j*width +i);
			ptr[i *3 +0] = colorTab[comp].r;
			ptr[i *3 +1] = colorTab[comp].g;
			ptr[i *3 +2] = colorTab[comp].b;
		}
	}
	
};
#endif
