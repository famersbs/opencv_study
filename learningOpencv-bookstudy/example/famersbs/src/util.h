/*
 * util.h
 *
 *  Created on: 2014. 1. 28.
 *      Author: famersbs
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * Window Manager
 */
class cvWindowManager
{
	private:
		std::vector<std::string> m_windows;

	public:
		~cvWindowManager();

	public:
		void printImage( IplImage* img, const char* window_name = (char*) 0 );
};

/**
 * Image Manager
 */
class cvImageManager
{
private:
	std::vector<IplImage*> m_images;

public:
	~cvImageManager();
public:
	IplImage* LoadImage( const char* path, int type );

	IplImage* createImage( CvSize size, int depth, int channels );

	IplImage* createImage( IplImage* image);
};

//utils
void pause();


#endif /* UTIL_H_ */
