#include <iostream>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;


int main(int argc, char* argv[])  {

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

		IplImage* m_pImage =cvLoadImage( "./res/a.jpeg"  );
		if(!m_pImage){
			printf("Could not load image file: %s\n",argv[1]);
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

		cvReleaseImage( &gray );
		cvReleaseImage( &small_img );
		cvReleaseMemStorage( &storage );

		//Window에 frame 출력
		cvShowImage("haar example (exit = esc)",m_pImage);


		while( true ){
			//haar를 이용해 얼굴 영역 검출
			if( cvWaitKey(50) == 27 ) break;
		}

		/*
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
		*/
		cvReleaseMemStorage(&storage);
		cvReleaseHaarClassifierCascade(&cascade);
		cvDestroyWindow("haar example (exit = esc)");

	return 0;
}
