#include <iostream>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

#include "BlobLabeling.h"
#include "Thinning.h"

void showImage( IplImage* image, const char* window_name, int ispause ){
	cvShowImage( window_name, image);

	// 사용자 입력 대기
	while( ispause == 1 ){
		//haar를 이용해 얼굴 영역 검출
		if( cvWaitKey(50) == 27 ) break;
	}
}

// 글자 단위 라벨링을 한 뒤에,

// 팽창 처리
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

	/*
	cvRectangle(image3, max_loc, cvPoint(max_loc.x+templat->width,
	max_loc.y+templat->height), cvScalar(0,1,1), 1);

	cvShowImage("src", image3);
	//cvShowImage("result image", result);
	cvWaitKey(0);
	*/

	return max_loc;

	/*
	double min, max;
	CvPoint left_top;
	IplImage *A = cvLoadImage( src, -1); // 책상(A)을 먼저 읽고
	IplImage *B = cvLoadImage( t , -1); // 스테이플러(B)를 읽는다.
	//IplImage* C = cvCreateImage( cvSize( A->width - B->width+1, A->height - B->height+1 ), IPL_DEPTH_8U, 1 ); // 상관계수를 구할 이미지(C)
	//IplImage* C = cvCreateImage( cvSize( A->width - B->width+1, A->height - B->height+1 ),
	IplImage* C = cvCreateImage( cvSize( 1024, 1024 ),
								IPL_DEPTH_8U, 1 ); // 상관계수를 구할 이미지(C)
								//IPL_DEPTH_32F, 1 ); // 상관계수를 구할 이미지(C)

	 IplImage *gA = cvCreateImage(cvGetSize(A), IPL_DEPTH_8U, 1);
	 IplImage *gB = cvCreateImage(cvGetSize(B), IPL_DEPTH_8U, 1);

	 cvCvtColor(A, gA, CV_RGB2GRAY);
	 cvCvtColor(B, gB, CV_RGB2GRAY);

	//cvMatchTemplate(gA, gB, C, CV_TM_SQDIFF_NORMED); // 상관계수를 구하여 C 에 그린다.
	cvMatchTemplate(gA, gB, C, CV_TM_SQDIFF_NORMED); // 상관계수를 구하여 C 에 그린다.
	cvMinMaxLoc(C, &min, &max, NULL, &left_top); // 상관계수가 최대값을 값는 위치 찾기

	// 모든 이미지 릴리즈
	cvReleaseImage(&A);
	cvReleaseImage(&B);
	cvReleaseImage(&gA);
	cvReleaseImage(&gB);
	cvReleaseImage(&C);
	// 모든 윈도우 제거
	cvDestroyAllWindows();

	return left_top;
	*/
}

/*
 * 희지 않은 부분은 모두 검다라는 공식을 적용
 */
void setAllBlack( IplImage* img ){

	cout << img->widthStep << " " << (int) (unsigned char)img->imageData[1] << endl;

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

void procOdap( const char* image_path ){
	CvFont font;  //축에 글자 쓰기용
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 0.7, 0.7, 0, 1);

	const char* window_name = image_path;
	cvNamedWindow( window_name, 0 ); // 결과 영상을 띄울 윈도우

	IplImage*	m_cloneImage;
	IplImage*	m_pImage;
	IplImage*	gray;
	IplImage*	gray2;
	IplImage*	gray3;

	m_pImage = cvLoadImage(image_path, -1);

	gray = cvCreateImage(cvGetSize(m_pImage), IPL_DEPTH_8U, 1);
	gray2 = cvCreateImage(cvGetSize(m_pImage), IPL_DEPTH_8U, 1);

	cvCvtColor(m_pImage, gray2, CV_RGB2GRAY);

	gray3 = cvCreateImage(cvGetSize(m_pImage), IPL_DEPTH_8U, 1);
	cvErode( gray2, gray3 );	// 팽창 연산
	setAllBlack( gray3 );
	cvThreshold(gray3, gray, 1, 255, CV_THRESH_BINARY_INV );	// 반전 후 라벨링
	showImage( gray, window_name, 0 );

	//cout<< "labeling after1" << endl;

	// 레이블링 알고리즘 사용
	CBlobLabeling blob;
	blob.SetParam(gray, 1 );	// 레이블링 할 이미지와 최소 픽셀수 등을 설정

	blob.DoLabeling(); //레이블링 실시

	cout<< "labeling first "<< blob.m_nBlobs << endl;

		// 절반 이상 크기의 라벨은 제거
	blob.BlobBigSizeConstraint( m_pImage->width, m_pImage->height / 2  );
		// 레이블 머지
	blob.MergeLabel( 23 ); 	// 몇 픽샐 이상 안떨어져있다면 합친다. ( 즉 겹쳐져 있거나, 얼마 떨어지지 않은 녀석  옆으로든 아래로든 )

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// 잡영 제거 전 레이블링
	cout << "clone before" << endl;
	m_cloneImage = m_pImage;
	cout << "clone after" << endl;
		// 레이블링 표시
	for( int i = 0 ; i < blob.m_nBlobs; ++ i ){


		CvPoint	pt1 = cvPoint(blob.m_recBlobs[i].x, blob.m_recBlobs[i].y);
		CvPoint pt2 = cvPoint(pt1.x + blob.m_recBlobs[i].width, pt1.y + blob.m_recBlobs[i].height);

		//cout<< "labeling after3 "<< i << " " << pt1.x << " " << pt1.y
			//	<< " " << pt2.x << " " << pt2.y << endl;

		cvDrawRect(m_cloneImage, pt1, pt2, CV_RGB(255,0,0), 1, 8, 0 );
	}

	showImage( m_cloneImage, window_name, 0 );
	//cvReleaseImage(&m_cloneImage);
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	// 번호 탬플릿 매칭을 통한 보기 영역 추축

	const char* template_img_path[] = {
			"res/template/1.png",
			"res/template/2.png",
			"res/template/3.png",
			"res/template/4.png",
			"res/template/5.png"
	};

	for( int i = 0 ; i < 5 ; ++ i ){

		CvPoint point = templateMatching( image_path, template_img_path[i] );

		CvRect find_range;
		find_range.x = point.x;
		find_range.y = point.y;
		find_range.width = 10;
		find_range.height = 10;


		blob.setType_HitRange( find_range, i + 1 );	// 해당 영역과 겹치는 레이블의 타입을 설정 한다.
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// 잡영 제거 전 레이블링
	cout << "clone before" << endl;
	m_cloneImage = m_pImage;
	cout << "clone after" << endl;
		// 레이블링 표시
	for( int i = 0 ; i < blob.m_nBlobs; ++ i ){


		CvPoint	pt1 = cvPoint(blob.m_recBlobs[i].x, blob.m_recBlobs[i].y);
		CvPoint pt2 = cvPoint(pt1.x + blob.m_recBlobs[i].width, pt1.y + blob.m_recBlobs[i].height);

		//cout<< "labeling after3 "<< i << " " << pt1.x << " " << pt1.y
			//	<< " " << pt2.x << " " << pt2.y << endl;

		if( blob.m_recBlobs[i].type == 0 )
			cvDrawRect(m_cloneImage, pt1, pt2, CV_RGB(255,0,0), 1, 8, 0 );
		else
			cvDrawRect(m_cloneImage, pt1, pt2, CV_RGB(0,255,0), 1, 8, 0 );
	}

	showImage( m_cloneImage, window_name, 0 );
	//cvReleaseImage(&m_cloneImage);
	///////////////////////////////////////////////////////////////////////////////////////////////////////


}

/**
 *
 * 1) 레이블링 ( 무제 영역과, 답 영역, 예시 영역으로 나눈다 )
 *
 * 2) 각 영역을 구분해 낸다.
 * 		문제영역
 * 		예시 영역 ( 문제영역과 답 영역의 중간에 나온다. )
 * 		답영역 ( 동그라미 + 숫자 가 제일 처음에 나오는 녀석
 *
 *
 */

int main() {

	procOdap("res/case1.png");

	/*
	procOdap("res/case2a.png");		// 실패
	procOdap("res/case2b.png");		// 실패
	procOdap("res/case2c.png");		// 실패
	procOdap("res/case2d.png");		// 실패
	procOdap("res/case2e.png");		// 보기만 분리 실패
	procOdap("res/case2f.png");		// 보기만 분리 실패
	procOdap("res/case3a.png");		// 보기만 분리 실패	--> merge를 30으로 하면 좋다.
	procOdap("res/case3b.png");		// 보기만 분리 실패	--> merge를 25으로 하면 좋다.
	*/
	procOdap("res/case3c.png");		// --> merge를 20으로 해야 한다.
	procOdap("res/case4a.png");		// --> merge를 23 정도?
	/*
	procOdap("res/case4b.png");		// 답이 통합으로 나옴 ( 간격만으로는 구분 불가 )
	procOdap("res/case4c.png");		// --> merge = 23 good ~ ^^
	procOdap("res/case5.png");		// 실패 ..
	procOdap("res/case6.png");		// 문제만 분리됨
	*/

	// 사용자 입력 대기
	while( true ){
		//haar를 이용해 얼굴 영역 검출
		if( cvWaitKey(50) == 27 ) break;
	}
	return 0;
}
