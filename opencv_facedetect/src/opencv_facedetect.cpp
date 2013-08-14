#include "opencv_facedetect.h"

/*
int cam_capture(){

		capture = cvCaptureFromCAM(-1);
		if(!capture){
			std::cerr<<"error: Cannot open init webcam!"<<std::endl;
			return -3;
		}

		cvNamedWindow("haar example (exit = esc)",CV_WINDOW_AUTOSIZE);

		while(true){
			frame = cvQueryFrame(capture);
			if(!frame||cvWaitKey(50)==27) { break; }

			//haar를 이용해 얼굴 영역 검출
			CvSeq *faces = 0;
			faces = cvHaarDetectObjects(frame, cascade, storage, 2.0, 1, 0);

			//검출된 모든 face에 대한 반복문
			for(int i=0; i<faces->total; i++){
				//face 영역 가져오기
				CvRect *r = 0;
				r = (CvRect*) cvGetSeqElem(faces, i);

				//frame에 face 영역 그리기
				cvRectangle(frame, cvPoint(r->x, r->y), cvPoint(r->x+r->width, r->y+r->height), cvScalar(0,255,0), 3, CV_AA, 0);
			}

			//Window에 frame 출력
			cvShowImage("haar example (exit = esc)",frame);
		}

		//자원 해제
		cvReleaseCapture(&capture);

}
*/

int facedetect_test( const char* load_image, const char* result_window_id )  {

	const char *classifer = "./res/haarcascade_frontalface_alt_tree.xml";

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

	//const char* load_image =  "./res/b.jpeg";

	IplImage* m_pImage =cvLoadImage( load_image  );
	if(!m_pImage){
		printf("Could not load image file: %s\n", load_image);
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

	//Window에 frame 출력
	cvShowImage( result_window_id, m_pImage);

	// 이미지 정리
	cvReleaseImage( &gray );
	cvReleaseImage( &small_img );
	cvReleaseMemStorage( &storage );

	// 리소스 정리
	cvReleaseMemStorage(&storage);
	cvReleaseHaarClassifierCascade(&cascade);

	return 0;
}


int facedetect_test_img_binary( const char* load_image, const char* result_window_id ){

	int threshold = 128; // 임계값(Threshold) 설정
	IplImage* output = 0;
	IplImage* gray = 0;

	//const char* load_image =  "./res/b.jpeg";

	IplImage* image = cvLoadImage( load_image  );
	if(!image){
		printf("Could not load image file: %s\n", load_image);
		exit(0);
	}

	gray = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1); // 흑백 이미지 생성
	output = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1); // 흑백 이미지 생성
	cvCvtColor(image, gray, CV_RGB2GRAY); // 컬러를 흑백으로 변환


	// 영상의 각 픽셀(x,y) 값이 threshold 값의 초과는 255 로, 그 이하는 0 으로 변환
	cvThreshold(gray, output, threshold, 255, CV_THRESH_BINARY);
	output->origin = image->origin; // 방향이 뒤집어 진것을 바로 잡아줌
	cvShowImage( result_window_id, output );


	cvReleaseImage( &gray );
	cvReleaseImage( &output );
	cvReleaseImage( &image );

	return 0;

}

struct stMap{
 unsigned char r;
 unsigned char g;
 unsigned char b;
} Map[1024][1024];
void Img2Aray(IplImage* img);

int facedetect_test_img_Laplacian( const char* load_image, const char* result_window_id ){

	IplImage* img = cvLoadImage( load_image  );
	if(!img){
		printf("Could not load image file: %s\n", load_image);
		exit(0);
	}

	int di[8] = {-1,-1,-1, 0, 0, 1, 1, 1},
		dj[8] = {-1, 0, 1,-1, 1,-1, 0, 1};
	int mask[3][3]={{ -1, -1, -1 },
				   { -1,  8, -1 },
				   { -1, -1, -1 }};

	int p;
	Img2Aray(img);

	for(int i=1; i<img->height-1; i++){
	   for(int j=1; j<img->width-1; j++){
		   p = Map[i][j].r;
		   p *= mask[1][1];
		   for(int k=0; k<8; k++){
			   p = p + ( (unsigned char)Map[i+di[k]][j+dj[k]].r * mask[1+di[k]][1+dj[k]] );
		   }
		   if(p > 255)  p = 255;
		   else if(p < 0) p = 0;
		   img->imageData[i*img->widthStep + j ] = (unsigned char)p;
	   }
	}

	cvShowImage( result_window_id, img );

	return 0;
}

void Img2Aray(IplImage* img) {

 printf( "width : %d  height : %d\n", img->width, img->height );

 for(int i=0; i<img->height/*240*/; i++){
  for(int j=0; j<img->width/*320*/; j++){

   Map[i][j].r = img->imageData[i*img->widthStep + j*img->nChannels + 2];
   Map[i][j].g = img->imageData[i*img->widthStep + j*img->nChannels + 1];
   Map[i][j].b = img->imageData[i*img->widthStep + j*img->nChannels + 0];

  }
 }
}
