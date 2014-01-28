//============================================================================
// Name        : opencv_proc.cpp
// Author      : vdfsdf
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;


int facedetect_test_img_binary( const char* path ){

	int threshold = 128; // 임계값(Threshold) 설정
	IplImage* output = 0;
	IplImage* gray = 0;

	char origin_path[1024] = {0,};
	char save_path[1024] = {0,};
	sprintf(origin_path, "%s/%s", path, "origin.png" );
	sprintf(save_path, "%s/%s", path, "binary.png" );


	IplImage* image = cvLoadImage( origin_path  );
	if(!image){
		printf("Could not load image file: %s\n", origin_path);
		exit(0);
	}

	gray = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1); // 흑백 이미지 생성
	output = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1); // 흑백 이미지 생성
	cvCvtColor(image, gray, CV_RGB2GRAY); // 컬러를 흑백으로 변환


	// 영상의 각 픽셀(x,y) 값이 threshold 값의 초과는 255 로, 그 이하는 0 으로 변환
	cvThreshold(gray, output, threshold, 255, CV_THRESH_BINARY);
	output->origin = image->origin; // 방향이 뒤집어 진것을 바로 잡아줌


	//이미지 저장
	cvSaveImage(save_path, output);

	cvReleaseImage( &gray );
	cvReleaseImage( &output );
	cvReleaseImage( &image );

	return 0;

}

int facedetect_test( const char* path )  {

	const char *classifer = "./res/haarcascade_frontalface_alt_tree.xml";
	//const char *classifer = "C:/Users/Administrator/Desktop/OpenCV_스터디/project/opencv_proc/res/haarcascade_frontalface_alt_tree.xml";

	char origin_path[1024] = {0,};
	char save_path[1024] = {0,};

	sprintf(origin_path, "%s/%s", path, "origin.png" );
	sprintf(save_path, "%s/%s", path, "facedect.png" );

	CvHaarClassifierCascade* cascade = 0;
	cascade = (CvHaarClassifierCascade*) cvLoad(classifer, 0, 0, 0 );
	if(!cascade){
		std::cerr<<"error: cascade error!!"<<std::endl;
		return -1;
	}

	CvMemStorage* storage = 0;
	storage = cvCreateMemStorage(0);
	if(!storage){
		std::cerr<<"error: storage error!!"<<std::endl;
		return -2;
	}

	IplImage* m_pImage =cvLoadImage( origin_path  );
	if(!m_pImage){
		printf("Could not load image file: %s\n", origin_path);
		exit(0);
	}

	double scale = 1.3;

	// 영상 준비
	//
	IplImage* gray = cvCreateImage( cvSize(m_pImage->width,m_pImage->height), 8, 1 );
	IplImage* small_img = cvCreateImage(
		cvSize( cvRound(m_pImage->width/scale), cvRound(m_pImage->height/scale)), 8, 1
		);
	cvCvtColor( m_pImage, gray, CV_BGR2GRAY );
	cvResize( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );

	// 객체 검출
	//
	cvClearMemStorage( storage );
	CvSeq* objects = cvHaarDetectObjects(
		small_img,
		cascade,
		storage,
		1.1,
		2,
		0   /*CV_HAAR_DO_CANNY_PRUNING*/,
		cvSize(30, 30)
		);

	// 객체를 찾고 박스를 그린다.
	//
	for( int i = 0 ; i < (objects ? objects->total : 0) ; i++ )
	{
		CvRect* r = (CvRect*)cvGetSeqElem( objects, i );
		CvPoint center;
		int radius;
		center.x = cvRound((r->x + r->width*0.5)*scale);
		center.y = cvRound((r->y + r->height*0.5)*scale);
		radius = cvRound((r->width + r->height)*0.25*scale);
		cvCircle( m_pImage, center, radius, cvScalar(255, 0, 0), 3, 8, 0 );
	}

	//이미지 저장
	cvSaveImage(save_path, m_pImage);


	// 이미지 정리
	cvReleaseImage( &gray );
	cvReleaseImage( &small_img );
	cvReleaseMemStorage( &storage );

	// 리소스 정리
	cvReleaseMemStorage(&storage);
	cvReleaseHaarClassifierCascade(&cascade);

	return 0;
}

int main(int argc, char* argv[]) {
	facedetect_test( argv[1] );
	facedetect_test_img_binary( argv[1] );
	return 0;
}
