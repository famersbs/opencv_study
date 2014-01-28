/*
 * chapter6.cpp
 *
 *  Created on: 2014. 1. 28.
 *      Author: bong
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

void houghline(IplImage *image);
void fillter2d(IplImage* src);
void makeborder(Mat src);
void sobel(Mat src);
void laplace(Mat src);
void canny(Mat src);
void canny(int, void*);
void houghcircle(IplImage* image);
//mapping
void mapping(Mat src);
void update_map(void);

IplImage* image;
Mat src, src_gray;
Mat dst, detected_edges;

//canny variable
int lowThreshold;
int edgeThreshold = 1;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* canny_name = "Edge Map";

//makeborder variable
char* border_name = "copyMakeBorder Demo";

//sobel variable
char* sobel_name = "Sobel Demo";

// laplace variable
char* laplace_name = "Laplace Demo";

// remapping variable
Mat map_x, map_y;
char* remap_window = "Remap demo";
int ind = 0;

//int main(int argc, char** argv) {
////	IplImage *image = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
//	src = imread(argv[1], 1);
//	//makeborder(src);
//	//houghcircle(image);
//	//canny(src);
//	//sobel(src);
//	//laplace(src);
//	mapping(src);
//	return 0;
//}

//add makeborder
void makeborder(Mat src) {

	int c;
	int top, bottom, left, right;
	int borderType;
	Scalar value;
	RNG rng(12345);

	printf("\n \t copyMakeBorder Demo: \n");
	printf("\t -------------------- \n");
	printf(" ** Press 'c' to set the border to a random constant value \n");
	printf(" ** Press 'r' to set the border to be replicated \n");
	printf(" ** Press 'ESC' to exit the program \n");

	namedWindow(border_name, CV_WINDOW_AUTOSIZE);
	top = (int) (0.05 * src.rows);
	bottom = (int) (0.05 * src.rows);
	left = (int) (0.05 * src.cols);
	right = (int) (0.05 * src.cols);
	dst = src;

	imshow(border_name, dst);
	while (true) {
		c = waitKey(500);
		borderType = BORDER_DEFAULT;
		if ((char) c == 27) {
			break;
		} else if ((char) c == 'c') {
			borderType = BORDER_CONSTANT;
		} else if ((char) c == 'r') {
			borderType = BORDER_REPLICATE;
		}

		value = cvScalarAll(0);
//		value =Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
//				rng.uniform(0, 255));
		copyMakeBorder(src, dst, top, bottom, left, right, borderType);
		imshow(border_name, dst);
	}
}
;

//cannny!!!!!!!!!!!!
void canny(Mat src) {
	dst.create(src.size(), src.type());
	cvtColor(src, src_gray, CV_RGB2GRAY);
	namedWindow(canny_name, CV_WINDOW_AUTOSIZE);
	createTrackbar("Min Threshold:", canny_name, &lowThreshold,
			max_lowThreshold, canny);
	canny(0, 0);
	waitKey(0);
}

void canny(int, void*) {
	blur(src_gray, detected_edges, Size(3, 3));
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio,
			kernel_size);
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges);
	imshow(canny_name, dst);
}

//houghcircle!!!!!!
void houghcircle(IplImage* image) {
	CvMemStorage* storage = cvCreateMemStorage(0);
	cvSmooth(image, image, CV_GAUSSIAN, 5, 5);

	CvSeq* results = cvHoughCircles(image, storage, CV_HOUGH_GRADIENT, 2,
			image->width / 10);

	for (int i = 0; i < results->total; i++) {
		float* p = (float*) cvGetSeqElem(results, i);
		CvPoint pt = cvPoint(cvRound(p[0]), cvRound(p[1]));
		cvCircle(image, pt, cvRound(p[2]), CV_RGB(0xff, 0xff, 0xff));
	}
	cvNamedWindow("cvHoughCircles", 1);
	cvShowImage("cvHoughCircles", image);
	cvWaitKey(0);
	cvReleaseImage(&image);
	cvReleaseMemStorage(&storage);

}

void sobel(Mat src) {
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	cvtColor(src, src_gray, CV_RGB2GRAY);

	namedWindow(sobel_name, CV_WINDOW_AUTOSIZE);

	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	Sobel(src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	imshow(sobel_name, grad);
	waitKey(0);

}
;

void laplace(Mat src) {
	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	// Remove noise by blurring with a Gaussian filter
	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// Convert the image to grayscale
	cvtColor(src, src_gray, CV_RGB2GRAY);

	/// Create window
	namedWindow(laplace_name, CV_WINDOW_AUTOSIZE);

	/// Apply Laplace function
	Mat abs_dst;

	Laplacian(src_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(dst, abs_dst);
	/// Show what you got
	imshow(laplace_name, abs_dst);
	waitKey(0);
}
;
void mapping(Mat src) {
	dst.create(src.size(), src.type());
	map_x.create(src.size(), CV_32FC1);
	map_y.create(src.size(), CV_32FC1);

	namedWindow(remap_window, CV_WINDOW_AUTOSIZE);
	while (true) {
		int c = waitKey(1000);
		if ((char) c == 27) {
			break;
		}
		update_map();
		remap(src, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT,
				Scalar(0, 0, 0));
	}
}
;
void update_map(void) {
	ind = ind % 4;
	for (int j = 0; j < src.rows; j++) {
		for (int i = 0; i < src.cols; i++) {
			switch (ind) {
			case 0:
				if (i > src.cols * 0.25 && i < src.cols * 0.75
						&& j > src.rows * 0.25 && j < src.rows * 0.75) {
					map_x.at<float>(j, i) = 2 * (j - src.cols * 0.25) + 0.5;
					map_y.at<float>(j, i) = 2 * (j - src.rows * 0.25) + 0.5;
				} else {
					map_x.at<float>(j, i) = 0;
					map_y.at<float>(j, i) = 0;
				}
				break;
			case 1:
				map_x.at<float>(j, i) = i;
				map_y.at<float>(j, i) = src.rows - j;
				break;
			case 2:
				map_x.at<float>(j, i) = src.cols - i;
				map_y.at<float>(j, i) = j;
				break;
			case 3:
				map_x.at<float>(j, i) = src.cols - i;
				map_y.at<float>(j, i) = src.rows - j;
				break;
			}
		}
	}
}

