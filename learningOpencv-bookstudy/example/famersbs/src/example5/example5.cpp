#include "example5.h"


#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//#include <opencv2/highgui/math.h>

using namespace cv;
using namespace std;

#include <stdio.h>
#include <iostream>

#include "../util.h"

void run_example5_1(){

	cvWindowManager winman;
	cvImageManager	imgman;

	IplImage* m_pImage 		= imgman.LoadImage("res/IMG_0191.jpg", CV_LOAD_IMAGE_COLOR );
	IplImage* m_pTempImage 	= imgman.createImage( m_pImage );
	IplImage* m_pTempImage2 = imgman.createImage( m_pImage );

	cvSmooth( m_pImage, m_pTempImage, CV_GAUSSIAN, 3, 3, 0, 0 );
	winman.printImage( m_pTempImage, "3x3" );
	cvSmooth( m_pImage, m_pTempImage, CV_GAUSSIAN, 5, 5, 0, 0 );
	winman.printImage( m_pTempImage, "5x5" );
	cvSmooth( m_pTempImage, m_pTempImage2, CV_GAUSSIAN, 5, 5, 0, 0 );
	winman.printImage( m_pTempImage2, "5x5*2" );

	cvSmooth( m_pImage, m_pTempImage, CV_GAUSSIAN, 9, 9, 0, 0 );
	winman.printImage( m_pTempImage, "9x9" );
	cvSmooth( m_pImage, m_pTempImage, CV_GAUSSIAN, 11, 11, 0, 0 );
	winman.printImage( m_pTempImage, "11x11" );

	printf("test example 5 \n" );

	pause();

}

/**
 * 100x100 으로 하면 이상하게 깨진다.
 */
void run_example5_2(){

	const int default_size = 100;

	cvWindowManager winman;
	cvImageManager	imgman;


	IplImage* m_pImage 	= imgman.createImage( cvSize(default_size,default_size), IPL_DEPTH_8U, 1);
	IplImage* m_temp 	= imgman.createImage( m_pImage );
	IplImage* m_temp5_2 = imgman.createImage( m_pImage );
	IplImage* m_temp9 	= imgman.createImage( m_pImage );

	cvSetZero( m_pImage );
	cvRectangle( m_pImage, cvPoint(default_size/2,default_size/2), cvPoint(default_size/2,default_size/2), cvScalar(255), CV_FILLED);

	cvSmooth( m_pImage, m_temp, CV_GAUSSIAN, 5, 5, 0, 0 );
	cvSmooth( m_temp, m_temp5_2, CV_GAUSSIAN, 5, 5, 0, 0 );
	cvSmooth( m_pImage, m_temp9, CV_GAUSSIAN, 5, 5, 0, 0 );

	winman.printImage( m_pImage, "original" );
	winman.printImage( m_temp, "5x5" );
	winman.printImage( m_temp5_2, "5x5*2" );
	winman.printImage( m_temp9, "9x9" );

	pause();
}

void run_example5_3(){

	cvWindowManager winman;
	cvImageManager	imgman;

	// 3.a
	IplImage* m_pImage 		= imgman.LoadImage("res/IMG_0191.jpg", CV_LOAD_IMAGE_COLOR );
	IplImage* img_3a_1 		= imgman.createImage(m_pImage);
	IplImage* img_3a_4 		= imgman.createImage(m_pImage);
	IplImage* img_3a_6 		= imgman.createImage(m_pImage);

	cvSmooth( m_pImage, img_3a_1, CV_GAUSSIAN, 9, 9, 1, 0 );
	cvSmooth( m_pImage, img_3a_4, CV_GAUSSIAN, 9, 9, 4, 0 );
	cvSmooth( m_pImage, img_3a_6, CV_GAUSSIAN, 9, 9, 6, 0 );

	winman.printImage( img_3a_1, "3a_1");
	winman.printImage( img_3a_4, "3a_4");
	winman.printImage( img_3a_6, "3a_6");


	IplImage* img_3c_019 	= imgman.createImage(m_pImage);
	cvSmooth( m_pImage, img_3c_019, CV_GAUSSIAN, 0, 0, 1, 9 );
	winman.printImage( img_3c_019, "3c_019");

	IplImage* img_3d_091 	= imgman.createImage(m_pImage);
	cvSmooth( m_pImage, img_3d_091, CV_GAUSSIAN, 0, 0, 9, 1 );
	winman.printImage( img_3d_091, "3d_091");

	IplImage* img_3e 	= imgman.createImage(m_pImage);
	cvSmooth( img_3c_019, img_3e, CV_GAUSSIAN, 0, 0, 9, 1 );
	winman.printImage( img_3e, "3e");


	IplImage* img_3f_o = imgman.createImage(m_pImage);
	IplImage* img_3f_l = imgman.createImage(m_pImage);

	cvSmooth( m_pImage, img_3f_o, CV_GAUSSIAN, 9, 9, 0, 0 );
	cvSmooth( m_pImage, img_3f_l, CV_GAUSSIAN, 9, 9, 9, 9 );

	winman.printImage( img_3f_o, "3f_o");
	winman.printImage( img_3f_l, "3f_l");

	pause();
}

void run_example5_4(){

	cvWindowManager winman;
	cvImageManager	imgman;

	IplImage* m_pImage1		= imgman.LoadImage("res/5_4_1.jpg", CV_LOAD_IMAGE_COLOR );
	IplImage* m_pImage2		= imgman.LoadImage("res/5_4_2.jpg", CV_LOAD_IMAGE_COLOR );
	IplImage* m_temp		= imgman.createImage( m_pImage1 );

	IplImage* m_temp_1		= imgman.createImage( m_pImage1 );
	IplImage* m_temp_2		= imgman.createImage( m_pImage1 );

	cvSub( m_pImage1, m_pImage1, m_temp );
	winman.printImage( m_temp, "temp_same" );
	cvSub( m_pImage1, m_pImage2, m_temp_1 );
	cvAbs( m_temp_1, m_temp );
	winman.printImage( m_temp, "temp_diff" );


	cvErode(m_temp, m_temp_1);
	cvDilate(m_temp_1, m_temp_2);
	winman.printImage( m_temp_2, "clean_diff");


	cvDilate(m_temp, m_temp_1);
	cvErode(m_temp_1, m_temp_2);
	winman.printImage( m_temp_2, "dirty_diff");

	pause();

}

void run_example5_5(){


	cvWindowManager winman;
	cvImageManager	imgman;

	IplImage* m_pImage1		= imgman.LoadImage("res/5_5_1.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	IplImage* m_pImage2		= imgman.LoadImage("res/5_5_2.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	IplImage* m_temp		= imgman.createImage( m_pImage1 );
	IplImage* m_temp_1		= imgman.createImage( m_pImage1 );
	//IplImage* m_temp_2		= imgman.createImage( m_pImage1 );

	winman.printImage( m_pImage1, "origin 1" );
	winman.printImage( m_pImage2, "origin 2" );

	cvSub( m_pImage1, m_pImage2, m_temp_1 );
	cvAbs( m_temp_1, m_temp );
	winman.printImage( m_temp, "stapler check" );

	// 그냥 됬음... 스테이플러 라서 그런가?

	pause();

}

void run_example5(){

	int number = 0;

	cout << "input problem number : " << endl; // prints !!!Hello World!!!

	cin >> number ;

	cout << "your selected example number is : " << number << endl;

	getchar();

	switch( number ){
	case 1:
		run_example5_1();
		break;
	case 2:
		run_example5_2();
		break;
	case 3:
		run_example5_3();
		break;
	case 4:
		run_example5_4();
		break;
	case 5:
		run_example5_5();
		break;
	default:
		cout << "not found Example number " << number << endl;
		break;
	}
}
