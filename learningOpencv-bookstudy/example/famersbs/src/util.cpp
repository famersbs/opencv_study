/*
 * util.cpp
 *
 *  Created on: 2014. 1. 28.
 *      Author: famersbs
 */

#include "util.h"

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


// windows manager
cvWindowManager::~cvWindowManager(){
	std::vector<std::string>::iterator i = m_windows.begin();

	while( i != m_windows.end() ){
		cvDestroyWindow( i->c_str() );
		cout << "destory window : " << i->c_str() << endl;
		++ i;
	}
}

void cvWindowManager::printImage( IplImage* img, const char* window_name ){
	static int uniq_name = 0;
	std::string name;

	if( 0 == window_name ){
		char temp[100] = {0,};
		sprintf( temp, "%d_tmp", uniq_name );
		++ uniq_name;
		name = temp;
	}else{
		name = window_name;
	}

	cvNamedWindow( name.c_str(), 0 );
	cvShowImage(name.c_str(), img);
	m_windows.push_back( name );
}

// image manager
cvImageManager::~cvImageManager(){
	std::vector<IplImage*>::iterator i;

	i = m_images.begin();

	while( i != m_images.end() ){
		IplImage* target = *i;
		cvReleaseImage(&target);
		i ++;
	}
}
IplImage* cvImageManager::LoadImage( const char* path, int type ){
	IplImage* ret = cvLoadImage( path, type );
	m_images.push_back( ret );
	return ret;
}

IplImage* cvImageManager::createImage( CvSize size, int depth, int channels ){
	IplImage* ret = cvCreateImage( size, depth, channels );
	m_images.push_back( ret );
	return ret;
}

IplImage* cvImageManager::createImage( IplImage* image){
	return createImage( cvGetSize(image), image->depth, image->nChannels );
}

// util func
void pause(){
	// 사용자 입력 대기
	while( true ){
		//haar를 이용해 얼굴 영역 검출
		if( cvWaitKey(50) == 27 ) break;
	}
}
