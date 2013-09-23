#include <iostream>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

#include "BlobLabeling.h"
#include "Thinning.h"

#define MAX_IMAGE_SIZE 15

/*
	번호판 판별 알고리즘의 핵심!!!

		1. 이진화처리를 한다.

		2. 라벨링을 수행 한다.

		3. 라벨링 된 영역을 검증 한다.
			-> 원본 이미지를 통해 테두리는 흰색, 내부는 녹색 같은 형태로 검증한다. ( 검은 계열이 내부에 있고, 퍼센트를 따져서 몇퍼센트가 하얀색 혹은 녹색이고, 몇퍼센트는 검은 색인지--> 약 50% 비율로 ?
			-> 검증 과장을 통해 잡영을 제거한다. ( 잡다한 레이블을 )

		4. 마커 즉 번호판의 꼭지점을 추축 한다.

		5. 각형 마커를 추출 하였으면, 회전등을 통해 글씨의 이미지를 제대로 잡아 본다.

		6. 각 글짜들을 맵핑 하여 가장 우선순위가 높은 글짜 아스키 코드로 정의 내린다.
 */

// 기울어진 이미지의 경우  바로 세우는 로직 ( 이미지 테그 공식 ) 을 이용해서 인식 하면 인식 률이  많이 올라 갈 것 같음

void showImage( IplImage* image, const char* window_name, int ispause ){
	cvShowImage( window_name, image);

	// 사용자 입력 대기
	while( ispause == 1 ){
		//haar를 이용해 얼굴 영역 검출
		if( cvWaitKey(50) == 27 ) break;
	}
}

char* ImageToString( int blub_count, IplImage* img[]  )
{
	static char result[1024] = {0,};

	IplImage* C[MAX_IMAGE_SIZE];
	for(int i=0; i<MAX_IMAGE_SIZE; i++)
	{
		C[i] = cvCreateImage(cvSize(30,45), IPL_DEPTH_8U, 1);
	}
	int count=0;
	int tempnum;
	//정렬을 위한 배열 2개
	int number_temp[MAX_IMAGE_SIZE];
	int number_num[MAX_IMAGE_SIZE];
	int number_temp_temp;
	int number_num_temp;

	char final[MAX_IMAGE_SIZE]="";

	IplImage* res[10];

	res[0] = cvLoadImage("ocr/0.bmp", -1);
	res[1] = cvLoadImage("ocr/1.bmp", -1);
	res[2] = cvLoadImage("ocr/2.bmp", -1);
	res[3] = cvLoadImage("ocr/3.bmp", -1);
	res[4] = cvLoadImage("ocr/4.bmp", -1);
	res[5] = cvLoadImage("ocr/5.bmp", -1);
	res[6] = cvLoadImage("ocr/6.bmp", -1);
	res[7] = cvLoadImage("ocr/7.bmp", -1);
	res[8] = cvLoadImage("ocr/8.bmp", -1);
	res[9] = cvLoadImage("ocr/9.bmp", -1);

	Thinning th;

	for(int i=0; i <10; i++)
	{// 받은 이미지 이진화
		cvThreshold(res[i], res[i], 1, 255, CV_THRESH_BINARY);
	}
	for(int i=0; i <blub_count; i++)
	{// 번호 이미지 이진화
		cvThreshold(img[i], img[i], 1, 255, CV_THRESH_BINARY);
	}
	for(int i=0; i<blub_count; i++)
	{// 번호 이미지 세선화 실시
		img[i] = th.Thin(img[i]);
	}
	for(int i=0; i < blub_count; i++)
	{// 이미지 반전
		cvThreshold(img[i], img[i], 1, 255, CV_THRESH_BINARY_INV );
	}

	for(int a=0; a<blub_count; a++)
	{
		for(int i=0; i<10; i++)
		{
			count=0;
			for(int x=0; x<30; x++)
			{
				for(int y=0; y<45; y++)
				{
					int index = x + y * res[i]->widthStep;
					unsigned char value = res[i]->imageData[index];

					int index2 = x + y *img[0]->widthStep;
					unsigned char value2 = img[a]->imageData[index2];

					if(value < value2)
					{
						count++;
					}
				}
			}
			number_temp[i] = count;
			number_num[i]= i;
		}

		//버블정렬 결과
		for(int i=0; i<10; i++)
		{
			for(int j=0; j<10-i-1; j++)
			{
				if(number_temp[j] > number_temp[j+1])
				{
					number_num_temp = number_num[j];
					number_num[j] = number_num[j+1];
					number_num[j+1] = number_num_temp;

					number_temp_temp = number_temp[j];
					number_temp[j] = number_temp[j+1];
					number_temp[j+1] = number_temp_temp;
				}
			}
		}

		final[a] = number_num[0];


	}

	if(blub_count == 6)
	{
		sprintf(result, "%d%d %d %d%d%d",final[0],final[1],final[2],final[3],final[4],final[5]);
	}
	else if(blub_count == 7)
	{
		sprintf(result, "%d%d %d %d%d%d%d",final[0],final[1],final[2],final[3],final[4],final[5],final[6]);
	}
	else if (blub_count == 8)
	{
		sprintf(result,"%d%d %d%d %d%d%d%d",final[0],final[1],final[2],final[3],final[4],final[5],final[6],final[7]);
	}
	else if (blub_count == 9)
	{
		sprintf(result,"%d%d %d%d %d%d%d%d%d",final[0],final[1],final[2],final[3],final[4],final[5],final[6],final[7],final[8]);
	}else if(blub_count == 10){
		sprintf(result,"%d%d %d%d %d%d%d%d%d %d",final[0],final[1],final[2],final[3],final[4],final[5],final[6],final[7],final[8], final[9]);
	}else if(blub_count == 11){
		sprintf(result,"%d%d %d%d %d%d%d%d%d %d %d",final[0],final[1],final[2],final[3],final[4],final[5],final[6],final[7],final[8], final[9], final[10]);
	}
	else{

		printf( "[blubcnt : %d] \n", blub_count );
		for( int i = 0; i< blub_count ; ++ i ){
			printf("%d ", final[i] );
		}

	}


	return result;
}



int main() {

	CvFont font;  //축에 글자 쓰기용
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 0.7, 0.7, 0, 1);

	const char* window_name = "car_num_test";
	cvNamedWindow( window_name, 0 ); // 결과 영상을 띄울 윈도우
	const char* window_name_r1 = "car_num_test_r1";
	cvNamedWindow( window_name_r1, 0 ); // 결과 영상을 띄울 윈도우
	const char* window_name_r2 = "car_num_test_r2";
	cvNamedWindow( window_name_r2, 0 ); // 결과 영상을 띄울 윈도우


	IplImage*	m_cloneImage;
	IplImage*	m_pImage;
	IplImage*	gray;
	IplImage*	binary;
	IplImage*	end1;
	IplImage*   img[MAX_IMAGE_SIZE];
	int blub_count = 0;


	m_pImage = cvLoadImage("res/aae.jpg", -1);
	gray = cvCreateImage(cvGetSize(m_pImage), IPL_DEPTH_8U, 1);
	cvCvtColor(m_pImage, gray, CV_RGB2GRAY);

	cvThreshold(gray, gray, 1, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);//이진화
	IplImage* labeled = cvCreateImage( cvSize( gray->width, gray->height ), 8, 3 );//레이블링 이미지
	cvCvtColor(gray, labeled, CV_GRAY2BGR );//그레이를 RGB로 바꿔준다.

	// 레이블링 알고리즘 사용
	CBlobLabeling blob;
	blob.SetParam(gray, 50 );	// 레이블링 할 이미지와 최소 픽셀수 등을 설정
	//임의로 50 픽셀수 설정

	blob.DoLabeling(); //레이블링 실시

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// 잡영 제거 전 레이블링
	m_cloneImage = cvCloneImage(m_pImage);
		// 레이블링 표시
	for( int i = 0 ; i < blob.m_nBlobs; ++ i ){
		CvPoint	pt1 = cvPoint(blob.m_recBlobs[i].x, blob.m_recBlobs[i].y);
		CvPoint pt2 = cvPoint(pt1.x + blob.m_recBlobs[i].width, pt1.y + blob.m_recBlobs[i].height);
		cvDrawRect(m_cloneImage, pt1, pt2, CV_RGB(255,0,0), 1, 8, 0 );
	}

	showImage( m_cloneImage, window_name, 0 );
	cvReleaseImage(&m_cloneImage);
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	int  opt[100];

	// 잡영 제거 부분.
	//int nMaxWidth	= gray->width  * 3 / 10;	// 영상 가로 전체 크기의 50% 이상인 레이블은 제거
	//int nMaxHeight	= gray->height * 3 / 10;	// 영상 세로 전체 크기의 50% 이상인 레이블은 제거

	int nMaxWidth	= gray->width;	// 영상 가로 전체 크기의 50% 이상인 레이블은 제거
	int nMaxHeight	= gray->height;	// 영상 세로 전체 크기의 50% 이상인 레이블은 제거
	blob.BlobSmallSizeConstraint( 15, 15 );
	blob.BlobBigSizeConstraint( nMaxWidth, nMaxHeight );
	// 픽셀 사이즈 30 미만은 제거


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// 잡영 제거 후 레이블링
	m_cloneImage = cvCloneImage(m_pImage);
		// 레이블링 표시
	for( int i = 0 ; i < blob.m_nBlobs; ++ i ){
		CvPoint	pt1 = cvPoint(blob.m_recBlobs[i].x, blob.m_recBlobs[i].y);
		CvPoint pt2 = cvPoint(pt1.x + blob.m_recBlobs[i].width, pt1.y + blob.m_recBlobs[i].height);
		cvDrawRect(m_cloneImage, pt1, pt2, CV_RGB(255,0,0), 1, 8, 0 );
	}

	showImage( m_cloneImage, window_name, 1 );
	cvReleaseImage(&m_cloneImage);
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Debug Msg : 1 \n" );

	for( int i=0; i < blob.m_nBlobs ; i++ )
	{


		//레이블링된 각 위치값을 잡아주고
		CvPoint	pt1 = cvPoint(blob.m_recBlobs[i].x, blob.m_recBlobs[i].y);
		CvPoint pt2 = cvPoint(pt1.x + blob.m_recBlobs[i].width, pt1.y + blob.m_recBlobs[i].height);

		// 이미지 관심영역 설정
		cvSetImageROI(gray, blob.m_recBlobs[i]);
		IplImage* sub_gray = cvCreateImage(cvSize(blob.m_recBlobs[i].width, blob.m_recBlobs[i].height), 8, 1);
		cvThreshold(gray, sub_gray, 1, 255, CV_THRESH_BINARY_INV );

		// 관심영역 해제
		cvResetImageROI(gray);

		////////////////////////////
		// 레이블링
		CBlobLabeling inner;
		inner.SetParam(sub_gray, 5);

		inner.DoLabeling();

		int nSubMaxWidth	= sub_gray->width  * 3 / 10;
		int nSubMaxHeight	= sub_gray->height * 8 / 10;

		inner.BlobSmallSizeConstraint( 2, 2 );	// 글씨는 5 픽샐 이하로 표현 할 수 없다.	// min 으로 빼서는 안되고, 평균 값이나, 분포 값으로 빼야 한다.
		inner.BlobBigSizeConstraint( nSubMaxWidth, nSubMaxHeight );

		/*
		 레이블 검증 에 추가할 두단계
			1. 전체 크기중 평균을 내고, 그 평균에 약 70%에 속하는 녀석만
			2. 레이블 중에서 처음과 끝 여백을 제외한 레이블간 간격의 평균의 절반 이하는 제거 하거나 하나로 합친다.
			3. 만약 영역이 겹쳐 있을 경우는 어떻게 할까?
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		// 잡영 제거 후 레이블링
		m_cloneImage = cvCloneImage(m_pImage);
			// 레이블링 표시
		for( int i = 0 ; i < inner.m_nBlobs; ++ i ){
			CvPoint	pt1_s = cvPoint(pt1.x + inner.m_recBlobs[i].x, pt1.y + inner.m_recBlobs[i].y);
			CvPoint pt2_s = cvPoint(pt1_s.x + inner.m_recBlobs[i].width, pt1_s.y + inner.m_recBlobs[i].height);
			cvDrawRect(m_cloneImage, pt1_s, pt2_s, CV_RGB(255,0,0), 1, 8, 0 );
		}

		showImage( m_cloneImage, window_name, 1 );
		cvReleaseImage(&m_cloneImage);

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		printf("Debug Msg : 2 [%d]\n", i );

		if( inner.m_nBlobs >= 6 && inner.m_nBlobs <= 10 )
		{
			binary = cvCreateImage(cvSize( (sub_gray->width)*3, (int)(sub_gray->height)*3 ), IPL_DEPTH_8U, 1);
			cvResize(sub_gray,binary,CV_INTER_LINEAR );	// 일단 세배로 키웠다...

			end1 = cvCreateImage( cvSize(binary->width, binary->height ), 8, 3 );//레이블링 이미지
			cvCvtColor(binary, end1, CV_GRAY2BGR );//그레이를 RGB로 바꿔준다.

			cvThreshold(binary, binary, 1, 255, CV_THRESH_BINARY);

			CBlobLabeling fire;
			fire.SetParam(binary, 100);
			fire.DoLabeling();
			int nwidth	= binary->width  * 3 / 10;	// 영상 가로 전체 크기의 80% 이상인 레이블은 제거
			int nheight	= binary->height * 9 / 10;	// 영상 세로 전체 크기의 80% 이상인 레이블은 제거

			fire.BlobSmallSizeConstraint( 10, 10 );
			fire.BlobBigSizeConstraint( nwidth, nheight );

			printf("Debug Msg : 3 \n" );

			// 픽셀 사이즈 30 미만은 제거
			int count = 0;

			// 구역이 너무 많을 경우 번호판이 아니라고 판단하자~!
			if( fire.m_nBlobs > MAX_IMAGE_SIZE ) continue;	// 다른걸

			for( int i=0; i < fire.m_nBlobs; i++ )
			{
				//이미지는 템플릿 사이즈로 조정해준다.
				//버퍼에 이미지를 받는다.
				//교환해서 리사이즈한다.
				img[i] = cvCreateImage(cvSize(30, 45), IPL_DEPTH_8U, 1);
				cvSetImageROI(binary,cvRect(fire.m_recBlobs[i].x,fire.m_recBlobs[i].y,fire.m_recBlobs[i].width,fire.m_recBlobs[i].height));
				cvResize((IplImage*)cvClone(binary),img[i],CV_INTER_LINEAR );

				opt[i] = fire.m_recBlobs[i].x;

				//레이블링된 각 위치값을 잡아주고
				CvPoint	fire_pt1 = cvPoint(	fire.m_recBlobs[i].x, fire.m_recBlobs[i].y );
				CvPoint fire_pt2 = cvPoint(	fire_pt1.x + fire.m_recBlobs[i].width, fire_pt1.y + fire.m_recBlobs[i].height );
				// 각 레이블 표시
				CvScalar color	= cvScalar( 0, 0, 255 );
				cvDrawRect(end1, fire_pt1, fire_pt2, color );
			}

			showImage( end1, window_name , 1);

			printf("Debug Msg : 4 \n" );

			IplImage* temp1;
			int tem;
			blub_count = fire.m_nBlobs; //번호판 블럽의 숫자 저장.

			//레이블링 된 이미지의 x점을 기준으로 정렬한다.
			for(int i=0; i < fire.m_nBlobs; i++)
			{
				for(int j=i+1; j < fire.m_nBlobs; j++)
				{
					//printf("Debug Msg : 4.5 [%d %d] \n", i , j  );
					if(opt[i] > opt[j])
					{
						tem = opt[i];
						opt[i] = opt[j];
						opt[j] = tem;
						temp1 = (IplImage*)cvClone(img[i]);
						img[i] = (IplImage*)cvClone(img[j]);
						img[j] = (IplImage*)cvClone(temp1);
					}
				}
			}


			printf("Debug Msg : 5 \n" );

			///////////////////////////////////////////////////////////////////////////////////////////////////////
			// 각 라벨링 영역별 서브 추가 라벨링 ( 글짜 구분 하기 위함 )
			m_cloneImage = cvCloneImage(m_pImage);

				// 레이블링 표시
			for( int i = 0 ; i < inner.m_nBlobs; ++ i ){
				CvPoint	pt1s = cvPoint(inner.m_recBlobs[i].x + pt1.x, inner.m_recBlobs[i].y + pt1.y);
				CvPoint pt2s = cvPoint(pt1s.x + inner.m_recBlobs[i].width, pt1s.y + inner.m_recBlobs[i].height);
				cvDrawRect(m_cloneImage, pt1s, pt2s, CV_RGB(0,255,0), 1, 8, 0 );
			}


			cvPutText( m_cloneImage, ImageToString( blub_count, img ),
						cvPoint(20, 30), &font, CV_RGB(0,0,255));

			showImage( m_cloneImage, window_name_r2, 1 );
			cvReleaseImage(&m_cloneImage);
			///////////////////////////////////////////////////////////////////////////////////////////////////////


			printf("Debug Msg : 6 \n" );

			cvReleaseImage(&sub_gray);
			cvReleaseImage(&temp1);

		}
	}

	cvDestroyAllWindows();

	return 0;
}
