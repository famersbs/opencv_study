/*
 * ex6.cpp
 *
 *  Created on: 2014. 2. 6.
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
using namespace std;

//global variable
Mat src, src_gray, detected_edges, dst;
int highThreshold;
std::string window_name[3] = { "1:1.5", "1:2.75", "1:4" };
float ratio_size[3] = { 1.5, 2.75, 4 };

void p1(); // ok
void p2(); // gaussian - no, kernel ok
void p3(Mat src);
void p4(); // ok
void p5(Mat src);
void p6(Mat src);

void p7(); //ok
void CannyThreshold1(int, void*);
void CannyThreshold2(int, void*);
void CannyThreshold3(int, void*);

void p8(); //ok
void p9(Mat src);
void p10(Mat src);
void p11(Mat src);
void p12();
void p13(Mat src);
void p14();
void p15(Mat src);
void p16();
void p17();

#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//int main(int argc, char **argv) {
	// Mat src = imread(argv[1], CV_GRAY2BGR);
//	IplImage* img = cvLoadImage(argv[1], CV_WINDOW_AUTOSIZE);
//	p1();
//	p2();
//	p4();
//	Mat C = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);

//	p7();
//	p8();
	//p12();
	//p14();
	//p16();
//	p17();
//	cout << "C = " << endl << " " << C << endl << endl;
//}

/*
 * filter2D �몴� 佚텢壹뒠漿뜎 houghLine 壹뱽 佚텢壹뒠欌릭佾연 60應즲 揶쏄낮猷꾤몴� 壹뵠�뙴�뫀�뮉 筌욊낯苑묒�뱽 筌≪뼔�뮉應뼄.
 */
void p1() {

	Mat src = imread("images/houghline_image.jpg", CV_GRAY2BGR);
	namedWindow("src", CV_WINDOW_AUTOSIZE);
	imshow("src", src);
	Mat dst, cdst;
	Mat kernel;
	int ddepth = -1;
	int delta = 0;
	Point anchor = Point(-1, -1);

	std::string window_name[] = { "2D 3*3", "2D 5*5", "2D 7*7", " 2D 9*9",
			"2D 11*11" };
	int kernel_size[5] = { 3, 5, 7, 9, 11 };
	Canny(src, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

	for (int i = 0; i < 5; i++) {
		kernel = Mat::ones(kernel_size[i], kernel_size[i], CV_32F)
				/ (float) (kernel_size[i] * kernel_size[i]);
		filter2D(src, dst, ddepth, kernel, anchor, delta, BORDER_DEFAULT);

		vector<Vec4i> lines;
		HoughLinesP(dst, lines, 1, CV_PI / 3, 250, 250, 10);

		cout << lines.size() << endl;
		for (size_t j = 0; j < lines.size(); j++) {
			Vec4i l = lines[j];
			line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255),
					3,
					CV_AA);
		}
		namedWindow(window_name[i], CV_WINDOW_AUTOSIZE);
		imshow(window_name[i], cdst);
	}
	waitKey(0);
}
;
void p2() {
	src = imread("images/a15.jpg");
	Mat dst;
	float b1[] = { 1. / 4., 2 / (float) 4, 1 / (float) 4 };

	float L45[3][3] = { { 1. / 16., 2. / 16., 1. / 16. }, { 2. / 16., 4. / 16.,
			2. / 16. }, { 1. / 16., 2. / 16., 1. / 16. } };
	Mat two_a = Mat(3, 3, CV_32FC1, L45);
	cout << "two_a = " << two_a << endl << "" << endl;

	filter2D(src, dst, -1, two_a, Point(-1, -1), 0);
	namedWindow("two_a");
	imshow("two_a", dst);

//筌띾슢諭띕�뮉 獄쎻뫖苡�
	Mat a = Mat(1, 3, CV_32FC1, b1);

	cout << "a = " << a << endl << "" << endl;

// 筌띾슢諭띕�뮉 獄쎻뫖苡� 2甕곕뜆�럮
	Mat b_kernel1 =
			(Mat_<double>(1, 3) << 1. / 4., 2 / (float) 4, 1 / (float) 4);
	cout << "b_kernel1 = " << endl << " " << b_kernel1 << endl << endl;
	Mat b_kernel2 =
			(Mat_<double>(3, 1) << 1. / 4., 2 / (float) 4, 1 / (float) 4);
	cout << "b_kernel2 = " << endl << " " << b_kernel2 << endl << endl;

	filter2D(src, dst, -1, b_kernel1, Point(-1, -1), 0, BORDER_DEFAULT);
//sepFilter2D(src, dst, CV_32F, b_kernel1.t(), b_kernel1);
	namedWindow("b_kernel1");
	imshow("b_kernel1", dst);

	filter2D(src, dst, -1, b_kernel2, Point(-1, -1), 0, BORDER_DEFAULT);
	namedWindow("b_kernel2");
	imshow("b_kernel2", dst);

	waitKey(0);

}
;
void p3(Mat src) {

}
;
void p4() {
	// original src
	Mat src = imread("images/picture4.jpg");
	Mat dst;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	int kernel_size[] = { 3, 5, 9, 13 };
	std::string kernel[] = { "3 * 3", "5 * 5", "9 * 9", "13 * 13" };

//	cvtColor(src,gsrc, CV_RGB2GRAY);

	namedWindow("src", CV_WINDOW_AUTOSIZE);
	imshow("src", src);

	for (int i = 0; i < 4; i++) {
		Sobel(src, dst, ddepth, 1, 1, kernel_size[i], scale, delta,
				BORDER_DEFAULT);
		namedWindow(kernel[i], CV_WINDOW_AUTOSIZE);
		imshow(kernel[i], dst);
	}

	waitKey();

}
;
void p5(Mat src) {
}
;
void p6(Mat src) {
}
;

void p7() {
	src = imread("images/building.jpg");

	int const max_highThreshold = 250;

	dst.create(src.size(), src.type());

	cvtColor(src, src_gray, CV_BGR2GRAY);

	namedWindow(window_name[0], CV_WINDOW_AUTOSIZE);
	imshow(window_name[0], src);

	namedWindow(window_name[1], CV_WINDOW_AUTOSIZE);
	imshow(window_name[1], src);

	namedWindow(window_name[2], CV_WINDOW_AUTOSIZE);
	imshow(window_name[2], src);

	createTrackbar("High Threshold", window_name[0], &highThreshold,
			max_highThreshold, CannyThreshold1);
	createTrackbar("High Threshold", window_name[1], &highThreshold,
			max_highThreshold, CannyThreshold2);
	createTrackbar("High Threshold", window_name[2], &highThreshold,
			max_highThreshold, CannyThreshold3);

	CannyThreshold1(0, 0);

	CannyThreshold2(0, 0);
	CannyThreshold3(0, 0);
	waitKey(0);

}
;
void CannyThreshold1(int, void*) {
	blur(src_gray, detected_edges, Size(3, 3));

	Canny(detected_edges, detected_edges, highThreshold / (float) ratio_size[0],
			highThreshold, 3);
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges);
	imshow(window_name[0], dst);
}
void CannyThreshold2(int, void*) {
	blur(src_gray, detected_edges, Size(3, 3));

	Canny(detected_edges, detected_edges, highThreshold / (float) ratio_size[1],
			highThreshold, 3);
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges);
	imshow(window_name[1], dst);
}

void CannyThreshold3(int, void*) {
	blur(src_gray, detected_edges, Size(3, 3));

	Canny(detected_edges, detected_edges, highThreshold / (float) ratio_size[2],
			highThreshold, 3);
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges);
	imshow(window_name[2], dst);
}

void p8() {
	src = imread("images/bicycle.jpg");

	if (!src.data) {
		return exit(-1);
	}
	Mat cdst;

	//Hough line Translate
	Canny(src, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI / 180, 100, 0, 0);
	for (size_t i = 0; i < lines.size(); i++) {
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);

	}
	namedWindow("src", CV_WINDOW_AUTOSIZE);
	namedWindow("detected lines", CV_WINDOW_AUTOSIZE);

	imshow("src", src);
	imshow("detected lines", cdst);

	//Hough Circle Translate
	Mat p8SrcGray;
	cvtColor(src, p8SrcGray, CV_RGB2GRAY);
	GaussianBlur(p8SrcGray, p8SrcGray, Size(9, 9), 2, 2);

	vector<Vec3f> circles;

	HoughCircles(p8SrcGray, circles, CV_HOUGH_GRADIENT, 1, p8SrcGray.rows / 8,
			200, 100, 0, 0);

	for (size_t i = 0; i < circles.size(); i++) {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);

		circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);

	}
	namedWindow("detected circles", CV_WINDOW_AUTOSIZE);
	imshow("detected circles", src);

	waitKey();
}
;
void p9(Mat src) {

}
;
void p10(Mat src) {
}
;
void p11(Mat src) {
}
;
void p12() {
	// create rectangle
	int w = 400;
	int r1_size  =100;
	int r2_size = 50;
	double angle = 45.0;
	double scale = 1.0;
	Point center;
	Rect r1 = cvRect(w /2 - (r1_size/2),w /2 - (r1_size/2),r1_size,r1_size);
	Rect r2 = cvRect(w /2 - (r2_size/2),w /2 - (r2_size/2),r2_size,r2_size);

	std::string window_name[4] = {"L", "L-R" , "M","M-R"};
	std::string logpolar_window_name[4] = {"L LogPolar", "L-R logPolar" , "M logPolar","M-R logPolar"};
	Mat window1 = Mat::zeros(w,w,CV_8UC3);
	Mat window2 = Mat::zeros(w,w,CV_8UC3);
	Mat window1_r,window2_r,rot_mat;

	rectangle(window1,r1,Scalar(0,0,255),-1,8);

	rectangle(window2,r2,Scalar(0,255,0),-1,8);

	center = Point(window1.cols/2,window1.rows/2);
	rot_mat = getRotationMatrix2D(center,angle,scale);

	warpAffine(window1,window1_r,rot_mat,window1.size());

	center = Point(window2.cols/2,window2.rows/2);
	rot_mat = getRotationMatrix2D(center,angle,scale);

	warpAffine(window2,window2_r,rot_mat,window2.size());

	imshow(window_name[0],window1);
	imshow(window_name[1],window1_r);
	imshow(window_name[2],window2);
	imshow(window_name[3],window2_r);

	Mat dst1(window1.size(),window1.type()),
	dst1_r(window1_r.size(),window1_r.type()),
	dst2(window2.size(),window2.type()),
	dst2_r(window2_r.size(),window2_r.type());
	IplImage c_src = window1, c_dst = dst1;

	cvLogPolar(&c_src,&c_dst,cvPoint2D32f(window1.rows /2, window1.cols /2),scale,CV_INTER_LINEAR);

	c_src = window1_r, c_dst = dst1_r;

	cvLogPolar(&c_src,&c_dst,cvPoint2D32f(window1.rows /2, window1.cols /2),scale,CV_INTER_LINEAR);

	c_src = window2, c_dst = dst2;

	cvLogPolar(&c_src,&c_dst,cvPoint2D32f(window1.rows /2, window1.cols /2),scale,CV_INTER_LINEAR);

	c_src = window2_r, c_dst = dst2_r;

	cvLogPolar(&c_src,&c_dst,cvPoint2D32f(window1.rows /2, window1.cols /2),scale,CV_INTER_LINEAR);

	imshow(logpolar_window_name[0],dst1);
	imshow(logpolar_window_name[1],dst1_r);
	imshow(logpolar_window_name[2],dst2);
	imshow(logpolar_window_name[3],dst2_r);

   waitKey(0);
}

;
void p13(Mat src) {
}
;
void p14() {
	Mat src = imread("images/a15.jpg");
	Mat src_gray, sum, sqsum,tilted_sum;
	std:string window_name[5] = {"src","src_gray","sum","sqsum","tilted_sum"};

	cvtColor(src,src_gray,CV_RGB2GRAY);

	namedWindow(window_name[0],CV_WINDOW_AUTOSIZE);
	namedWindow(window_name[1],CV_WINDOW_AUTOSIZE);
	namedWindow(window_name[2],CV_WINDOW_AUTOSIZE);
	namedWindow(window_name[3],CV_WINDOW_AUTOSIZE);
	namedWindow(window_name[4],CV_WINDOW_AUTOSIZE);

	integral(src_gray,sum,sqsum,tilted_sum);

	imshow(window_name[0],src);
	imshow(window_name[1],src_gray);
	imshow(window_name[2],sum);
	imshow(window_name[3],sqsum);
	imshow(window_name[4],tilted_sum);

	waitKey(0);

}
;
void p15(Mat src) {




}
;
void p16() {
	Mat src = imread("images/a15.jpg"),dst;
	std:string window_name[2] ={"Source Image","Equalized Image"};

	cvtColor(src,src,CV_RGB2GRAY);

	equalizeHist(src,dst);

	namedWindow(window_name[0],CV_WINDOW_AUTOSIZE);
	namedWindow(window_name[1],CV_WINDOW_AUTOSIZE);

	imshow(window_name[0],src);
	imshow(window_name[1],dst);

	waitKey(0);

}
;
void p17() {
	Mat src = imread("images/a15.jpg");
	Mat dst(src.size(),src.type());
	Point2f srcQuad[4],dstQuad[4];
	Mat M = Mat::zeros(3,3,CV_32FC1);

	srcQuad[0].x = 0;
	srcQuad[0].y = 0;
	srcQuad[1].x = src.cols -1;
	srcQuad[1].y = 0;
	srcQuad[2].x = 0;
	srcQuad[2].y = src.rows -1;
	srcQuad[3].x = src.cols -1;
	srcQuad[3].y = src.rows -1;

	dstQuad[0].x = src.cols * 0.05;
	dstQuad[0].y = src.rows * 0.33;
	dstQuad[1].x = src.cols * 0.9;
	dstQuad[1].y = src.rows * 0.25;
	dstQuad[2].x = src.cols * 0.2;
	dstQuad[2].y = src.rows *  0.7;
	dstQuad[3].x = src.cols * 0.8;
	dstQuad[3].y = src.rows * 0.9;

	M = getPerspectiveTransform(srcQuad,dstQuad);

	warpPerspective(src,dst,M,src.size());
	namedWindow("src",CV_WINDOW_AUTOSIZE);
	namedWindow("dst",CV_WINDOW_AUTOSIZE);
	imshow("src",src);
	imshow("dst",dst);

	waitKey(0);

}
;
