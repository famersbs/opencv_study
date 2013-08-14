#include "opencv_facedetect.h"


int main(int argc, char* argv[])  {

	const char* load_image = "./res/b.jpeg";

	// Facedetect Detect
	cvNamedWindow( "face_detect", 0 ); // 결과 영상을 띄울 윈도우
	facedetect_test( load_image,"face_detect");

	// 이미지 이진화
	cvNamedWindow( "img_binary", 0 ); // 결과 영상을 띄울 윈도우
	facedetect_test_img_binary( load_image, "img_binary");

	// 윤곽 추출
	cvNamedWindow( "img_Laplacian", 0 ); // 결과 영상을 띄울 윈도우
	facedetect_test_img_Laplacian( load_image, "img_Laplacian" );

	// 사용자 입력 대기
	while( true ){
		//haar를 이용해 얼굴 영역 검출
		if( cvWaitKey(50) == 27 ) break;
	}

	// 윈도우 삭제
	cvDestroyAllWindows();

	// 이 객체를 이용해서 연속 이미지 저장이 가능하다... 흠... 그렇구만 ~!! )
	//CvVideoWriter* VideoOut = NULL;


}

