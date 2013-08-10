#include <iostream>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int facedetect_test( const char* load_image, const char* result_window_id );
int facedetect_test_img_binary( const char* load_image, const char* result_window_id );
int facedetect_test_img_Laplacian( const char* load_image, const char* result_window_id );
