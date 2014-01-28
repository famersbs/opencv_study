//============================================================================
// Name        : leaning_opencv_exma.cpp
// Author      : famersbs
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

#include <iostream>
using namespace std;


#include "example5/example5.h"

int main() {

	int number = 0;

	cout << "input example number : " << endl; // prints !!!Hello World!!!

	cin >> number ;

	cout << "your selected example number is : " << number << endl;

	getchar();

	switch( number ){
	case 5:
		run_example5();
		break;
	default:
		cout << "not found Example number " << number << endl;
		break;
	}


	return 0;
}
