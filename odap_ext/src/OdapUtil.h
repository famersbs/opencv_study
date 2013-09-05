#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


// 흑과 백을 완벽하게 나눈다~!!
void setAllBlack( IplImage* img );

// 팽창 처리
IplImage* FilterMax(IplImage *src,int nX, int nY);

// 탬플릿 메칭
CvPoint templateMatching( const char* src, const char* t );

// 이미지 보여주기
void showImage( IplImage* image, const char* window_name, int ispause );
