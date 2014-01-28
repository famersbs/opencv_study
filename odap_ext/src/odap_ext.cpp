#include <iostream>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

#include "BlobLabeling.h"
#include "Thinning.h"

#include "OdapMetaBuilder.h"
#include "OdapUtil.h"

/**
 * 아래 케이스를 늘려 가는형태가 되면 될것 같은데... ~!
 */
void procOdap( const char* image_path, OdapMetaBuilder* builder, bool isDebug ){
	CvFont font;  //축에 글자 쓰기용
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 0.7, 0.7, 0, 1);


	const char* window_name = image_path;
	if( isDebug ){
		cvNamedWindow( window_name, 0 ); // 결과 영상을 띄울 윈도우
	}

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
	setAllBlack( gray3);
	cvThreshold(gray3, gray, 1, 255, CV_THRESH_BINARY_INV );	// 반전 후 라벨링
	//cvThreshold(gray3, gray, 1, 255, CV_THRESH_BINARY );	// 반전 후 라벨링


	if( isDebug ){
		showImage( gray, window_name, 1 );
	}

	//cout<< "labeling after1" << endl;

	// 레이블링 알고리즘 사용
	CBlobLabeling blob;
	blob.SetParam(gray,20 );	// 레이블링 할 이미지와 최소 픽셀수 등을 설정
	//blob.SetParam(gray, 10 );	// 레이블링 할 이미지와 최소 픽셀수 등을 설정

	blob.DoLabeling(); //레이블링 실시

	cout<< "labeling first "<< blob.m_nBlobs << endl;

		// 절반 이상 크기의 라벨은 제거
	blob.BlobBigSizeConstraint( m_pImage->width, m_pImage->height / 2  );
		// 레이블 머지
	blob.MergeLabel( 23 ); 	// 몇 픽샐 이상 안떨어져있다면 합친다. ( 즉 겹쳐져 있거나, 얼마 떨어지지 않은 녀석  옆으로든 아래로든 )

	if( isDebug ){
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		// 잡영 제거 전 레이블링
		cout << "clone before" << endl;
		m_cloneImage = m_pImage;
		cout << "clone after" << endl;
			// 레이블링 표시
		for( int i = 0 ; i < blob.m_nBlobs; ++ i ){

			CvPoint	pt1 = cvPoint(blob.m_recBlobs[i].x, blob.m_recBlobs[i].y);
			CvPoint pt2 = cvPoint(pt1.x + blob.m_recBlobs[i].width, pt1.y + blob.m_recBlobs[i].height);

			cvDrawRect(m_cloneImage, pt1, pt2, CV_RGB(255,0,0), 1, 8, 0 );
		}

		showImage( m_cloneImage, window_name, 0 );
		//cvReleaseImage(&m_cloneImage);
		///////////////////////////////////////////////////////////////////////////////////////////////////////
	}

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

	if( isDebug ){
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		// 잡영 제거 전 레이블링
		cout << "clone before" << endl;
		m_cloneImage = m_pImage;
		cout << "clone after" << endl;


			// 레이블링 표시
		for( int i = 0 ; i < blob.m_nBlobs; ++ i ){

			CvPoint	pt1 = cvPoint(blob.m_recBlobs[i].x, blob.m_recBlobs[i].y);
			CvPoint pt2 = cvPoint(pt1.x + blob.m_recBlobs[i].width, pt1.y + blob.m_recBlobs[i].height);

			if( blob.m_recBlobs[i].type == 0 ){
				cvDrawRect(m_cloneImage, pt1, pt2, CV_RGB(255,0,0), 1, 8, 0 );
				builder->addProblem( blob.m_recBlobs[i].x, blob.m_recBlobs[i].y, blob.m_recBlobs[i].width, blob.m_recBlobs[i].height );
			}
			else{
				cvDrawRect(m_cloneImage, pt1, pt2, CV_RGB(0,255,0), 1, 8, 0 );
				builder->addExam( blob.m_recBlobs[i].type, blob.m_recBlobs[i].x, blob.m_recBlobs[i].y, blob.m_recBlobs[i].width, blob.m_recBlobs[i].height );
			}
		}

		showImage( m_cloneImage, window_name, 0 );
		//cvReleaseImage(&m_cloneImage);
		///////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	// 문제영역을 구분 지어서 XML로 저장
	for( int i = 0 ; i < blob.m_nBlobs; ++ i ){
		if( blob.m_recBlobs[i].type == 0 ){
			builder->addProblem( blob.m_recBlobs[i].x, blob.m_recBlobs[i].y, blob.m_recBlobs[i].width, blob.m_recBlobs[i].height );
		}
		else{
			builder->addExam( blob.m_recBlobs[i].type, blob.m_recBlobs[i].x, blob.m_recBlobs[i].y, blob.m_recBlobs[i].width, blob.m_recBlobs[i].height );
		}
	}

}

void procNumbering( const char* image_path, OdapMetaBuilder* builder, bool isDebug ){
	const char* window_name = image_path;
		if( isDebug ){
			cvNamedWindow( window_name, 0 ); // 결과 영상을 띄울 윈도우
		}

		IplImage*	m_cloneImage;
		IplImage*	m_pImage;
		IplImage*	gray;

		m_pImage = cvLoadImage(image_path, -1);

		gray = cvCreateImage(cvGetSize(m_pImage), IPL_DEPTH_8U, 1);

		cvCvtColor(m_pImage, gray, CV_RGB2GRAY);

		////////////////////////////////////////////////////////////////
		// 번호 탬플릿 매칭을 통한 보기 영역 추축

		const char* template_img_path[] = {
				"res/template/1.png",
				"res/template/2.png",
				"res/template/3.png",
				"res/template/4.png",
				"res/template/5.png"
		};
		CvRect find_range[5];

		for( int i = 0 ; i < 5 ; ++ i ){
			CvPoint point = templateMatching( image_path, template_img_path[i] );
			find_range[i].x = point.x;
			find_range[i].y = point.y;
			find_range[i].width = 10;
			find_range[i].height = 10;

			builder->addExam( i+1, find_range[i].x, find_range[i].y, find_range[i].width, find_range[i].height );
		}

		if( isDebug ){
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			// 잡영 제거 전 레이블링
			cout << "clone before" << endl;
			m_cloneImage = m_pImage;
			cout << "clone after" << endl;


				// 레이블링 표시
			for( int i = 0 ; i < 5; ++ i ){

				CvPoint	pt1 = cvPoint( find_range[i].x, find_range[i].y );
				CvPoint pt2 = cvPoint( find_range[i].x + find_range[i].width, find_range[i].y + find_range[i].height);

				cvDrawRect(m_cloneImage, pt1, pt2, CV_RGB(0,255,0), 1, 8, 0 );
			}

			showImage( m_cloneImage, window_name, 0 );
			//cvReleaseImage(&m_cloneImage);
			///////////////////////////////////////////////////////////////////////////////////////////////////////
		}

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
#include <stdio.h>
#include "example/example5.h"
int main( int argc, char* argv[] ) {

	run_example5();

	return 0;

	if( argc < 3 ){
		printf( "odap_ext [img path] [result path]\n");
		return -1;
	}

	printf("RunPath : %s\n", argv[0] );
	printf("ImagePath : %s\n", argv[1] );
	printf("ResultPath : %s\n", argv[2] );

	bool isDebug = false;

	OdapMetaBuilder builder;
	builder.setProperty( "filename", argv[1] );

	//procOdap(argv[1], &builder, isDebug);
	procNumbering( argv[1], &builder, isDebug );
	// 사용자 입력 대기

	while( isDebug ){
		//haar를 이용해 얼굴 영역 검출
		if( cvWaitKey(50) == 27 ) break;
	}

	if( builder.checkIsCorrect() ){

		//printf("%s", builder.toXMLString() );
		FILE *fp;
		if( fp = fopen( argv[2], "w" ) ){
			fprintf( fp, "%s", builder.toXMLString() );
		}
		fclose( fp );

		printf("Is not Problem... Write Success [%s] \n", argv[2] );
	}else{
		printf("Is Problem... \n" );
		return -1;
	}
	/*
	procOdap("res/case2a.png");		// 실패
	procOdap("res/case2b.png");		// 실패
	procOdap("res/case2c.png");		// 실패
	procOdap("res/case2d.png");		// 실패
	procOdap("res/case2e.png");		// 보기만 분리 실패
	procOdap("res/case2f.png");		// 보기만 분리 실패
	procOdap("res/case3a.png");		// 보기만 분리 실패	--> merge를 30으로 하면 좋다.
	procOdap("res/case3b.png");		// 보기만 분리 실패	--> merge를 25으로 하면 좋다.
	procOdap("res/case3c.png");		// --> merge를 20으로 해야 한다.
	procOdap("res/case4a.png");		// --> merge를 23 정도?
	procOdap("res/case4b.png");		// 답이 통합으로 나옴 ( 간격만으로는 구분 불가 )
	procOdap("res/case4c.png");		// --> merge = 23 good ~ ^^
	procOdap("res/case5.png");		// 실패 ..
	procOdap("res/case6.png");		// 문제만 분리됨
	*/



	return 0;
}
