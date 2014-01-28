/*
 * chapter5.cpp
 *
 *  Created on: 2014. 1. 14.
 *      Author: bong
 */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
//#include <iostream>

using namespace cv;
using namespace std;

/*
 * declares Chapter5 Feature Try Function
 */
void Try_Smoothing(IplImage *srcImg, IplImage *dstImg);

void Try_ErodeandDilate(Mat src);
void Dilation(int, void*);
void Erosion(int, void*);

void Try_StructKernel(IplImage *srcImg, IplImage *dstImg);
void Try_Morphologu(IplImage *srcImg, IplImage *dstImg);
void Try_FloodFill(IplImage *srcImg, IplImage *dstImg);
void Try_Resize(IplImage *srcImg, IplImage *dstImg);
void Try_Pyr(IplImage *srcImg, IplImage *dstImg);
void Try_Threshold(IplImage *srcImg, IplImage *dstImg);
void Try_AdaptiveThreshold(IplImage *srcImg, IplImage *dstImg);

/*
 * declare variable
 *
 */
char* erode_window = "Erosion Demo";
char* dilate_window = "Dilation Demo";
Mat src5, erosion_dst, dilation_dst;

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

/*
 * main Function
 */
//
int main(int argc, char** argv) {
	IplImage *srcImg = cvLoadImage(argv[1], CV_WINDOW_AUTOSIZE);
	src5 = imread(argv[1], 1);
	//Try_Smoothing(srcImg, cvCloneImage(srcImg));
	Try_ErodeandDilate(src5);
	return 0;
}

/*
 * implements Chapter4 Feature Try Function
 */

void Try_Smoothing(IplImage *srcImg, IplImage *dstImg) {
	/*
	 * dstImg 는 사용시 srcImg와 size와 depth가 같아야함  cvCloneImage 함수 사용!!
	 */
	cvSmooth(srcImg, dstImg, CV_BLUR);

	char* srcName = "src";
	char* dstName = "dst";

	cvNamedWindow(srcName);
	cvNamedWindow(dstName);

	cvShowImage(srcName, srcImg);
	cvShowImage(dstName, dstImg);
	cvWaitKey(0);

	cvReleaseImage(&dstImg);
	cvReleaseImage(&srcImg);

	cvDestroyAllWindows();
}
;
void Try_ErodeandDilate(Mat src) {

	namedWindow(erode_window, CV_WINDOW_AUTOSIZE);
	namedWindow(dilate_window, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(dilate_window, src.cols, 0);

	/// Create Erosion Trackbar
	createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse",
			"Erosion Demo", &erosion_elem, max_elem, Erosion);

	createTrackbar("Kernel size:\n 2n +1", "Erosion Demo", &erosion_size,
			max_kernel_size, Erosion);

	/// Create Dilation Trackbar
	createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse",
			"Dilation Demo", &dilation_elem, max_elem, Dilation);

	createTrackbar("Kernel size:\n 2n +1", "Dilation Demo", &dilation_size,
			max_kernel_size, Dilation);
}
;
/** @function Erosion */
void Erosion(int, void*) {
	int erosion_type;
	if (erosion_elem == 0) {
		erosion_type = MORPH_RECT;
	} else if (erosion_elem == 1) {
		erosion_type = MORPH_CROSS;
	} else if (erosion_elem == 2) {
		erosion_type = MORPH_ELLIPSE;
	}

	Mat element = getStructuringElement(erosion_type,
			Size(2 * erosion_size + 1, 2 * erosion_size + 1),
			Point(erosion_size, erosion_size));

	/// Apply the erosion operation
	erode(src5, erosion_dst, element);
	imshow("Erosion Demo", erosion_dst);
}

/** @function Dilation */
void Dilation(int, void*) {
	int dilation_type;
	if (dilation_elem == 0) {
		dilation_type = MORPH_RECT;
	} else if (dilation_elem == 1) {
		dilation_type = MORPH_CROSS;
	} else if (dilation_elem == 2) {
		dilation_type = MORPH_ELLIPSE;
	}

	Mat element = getStructuringElement(dilation_type,
			Size(2 * dilation_size + 1, 2 * dilation_size + 1),
			Point(dilation_size, dilation_size));
	/// Apply the dilation operation
	dilate(src5, dilation_dst, element);
	imshow("Dilation Demo", dilation_dst);
}

void Try_StructKernel(IplImage *srcImg, IplImage *dstImg) {

}
;
void Try_Morphologu(IplImage *srcImg, IplImage *dstImg) {

}
;
void Try_FloodFill(IplImage *srcImg, IplImage *dstImg) {

}
;
void Try_Resize(IplImage *srcImg, IplImage *dstImg) {

}
;
void Try_Pyr(IplImage *srcImg, IplImage *dstImg) {

}
;
void Try_Threshold(IplImage *srcImg, IplImage *dstImg) {

}
;
void Try_AdaptiveThreshold(IplImage *srcImg, IplImage *dstImg) {

}
;

