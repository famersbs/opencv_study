#pragma once

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

class Thinning
{
public:
	Thinning(void);
public:
	~Thinning(void);
public:	
	void	DeleteA(char* img, char* timg,int cx,int cy, int widthStep) ;
	int		nays(char* img, int i, int j, int widthStep) ;
	int		Connect(char* img, int i, int j, int widthStep);
	IplImage*	Thin(IplImage* InImage) ;
	
};


