#include "OdapUtil.h"

#include <iostream>

using namespace std;

/*
 * 희지 않은 부분은 모두 검다라는 공식을 적용
 */
void setAllBlack( IplImage* img ){

	//cout << img->widthStep << " " << (int) (unsigned char)img->imageData[1] << endl;

	for( int i =  0 ; i < img->width ; ++ i ){
		for( int j = 0 ; j < img->height; ++ j ){

			int curdata = (int) (unsigned char) img->imageData[j*img->widthStep+i];

			if( curdata < 250 )
			{
				// 흰색이 아니면 무조건 검은색이지~~!
				img->imageData[j*img->widthStep+i] = (unsigned char)1;

			}else{

				img->imageData[j*img->widthStep+i] = (unsigned char)255;

			}
		}
	}
}

/**
 * 팽창 연산
 */
IplImage* FilterMax(IplImage *src,int nX, int nY)
{
    int nWidth = src->width;
    int nHeight = src->height;

    int widthStep = src->widthStep;

    IplImage* tmp;
    tmp = cvCreateImage( cvGetSize( src ), 8, 1 );
    cvSetZero(tmp);

    int nMax;

    for(int j = 0; j < nHeight; j++ )
    for(int i = 0; i < nWidth; i++ )
    {
        nMax = (uchar)src->imageData[j * widthStep + i];

        for(int n = j - nY; n < j + nY; n++ )
        for(int m = i - nX; m < i + nX; m++ )
        {
            if( n > -1 && n < nHeight && m > -1 && m < nWidth )
            {
                if( nMax < (uchar)src->imageData[n * widthStep + m] )
                   nMax = (uchar)src->imageData[n * widthStep + m];
                }
            }

        tmp->imageData[j * widthStep + i] = (uchar)nMax;
    }

    return tmp;
}



/**
 * 이미지 경로 받아서 템플릿 분석 진행
 */
CvPoint templateMatching( const char* src, const char* t ){

	IplImage* image    = cvLoadImage(src, 1);
	IplImage* templat  = cvLoadImage(t, 1);
	int percent =100;// declare a destination IplImage object with correct size, depth and  channels
	IplImage* image3 = cvCreateImage( cvSize((int)((image->width*percent)/100) ,
	(int)((image->height*percent)/100) ),image->depth, image->nChannels );

	//use cvResize to resize source to a destination image
	cvResize(image, image3);
	IplImage* image2   = cvCreateImage(cvSize(image3->width, image3->height),
	IPL_DEPTH_8U,   1);
	IplImage* templat2 = cvCreateImage(cvSize(templat->width,
	templat->height),   IPL_DEPTH_8U, 1);

	cvCvtColor(image3, image2, CV_BGR2GRAY);
	cvCvtColor(templat, templat2, CV_BGR2GRAY);


	int w = image3->width - templat->width + 1;
	int h = image3->height - templat->height + 1;
	IplImage* result = cvCreateImage(cvSize(w, h), IPL_DEPTH_32F, 1);
	cvMatchTemplate(image2, templat2, result, CV_TM_CCORR_NORMED);

	double min_val, max_val;
	CvPoint min_loc, max_loc;
	cvMinMaxLoc(result, &min_val, &max_val, &min_loc, &max_loc);

	return max_loc;

}

/**
 * 이미지 보여주기 디버깅 유틸
 */
void showImage( IplImage* image, const char* window_name, int ispause ){
	cvShowImage( window_name, image);

	// 사용자 입력 대기
	while( ispause == 1 ){
		//haar를 이용해 얼굴 영역 검출
		if( cvWaitKey(50) == 27 ) break;
	}
}
