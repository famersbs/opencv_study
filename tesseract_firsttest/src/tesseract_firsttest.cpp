//============================================================================
// Name        : tesseract_firsttest.cpp
// Author      : famersbs
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>

#include "leptonica/allheaders.h"
#include "tesseract/baseapi.h"

int main() {
	// [1]
	tesseract::TessBaseAPI *myOCR =
			new tesseract::TessBaseAPI();

	// [2]
	printf("Tesseract-ocr version: %s\n",
		   myOCR->Version());
	printf("Leptonica version: %s\n",
		   getLeptonicaVersion());


	// [3]
	if (myOCR->Init( 0 , "eng")) {
	  fprintf(stderr, "Could not initialize tesseract.\n");
	  exit(1);
	}

	// [4]
	Pix *pix = pixRead("./res/input.png");
	myOCR->SetImage(pix);

	// [5]
	char* outText = myOCR->GetUTF8Text();
	printf("OCR output:\n\n");
	printf("%s", outText);

	// [6]
	myOCR->Clear();
	myOCR->End();
	delete [] outText;
	pixDestroy(&pix);
	return 0;
}

// 3.0.2 버전으로 업그레이드 해야 한글을 사용할 수 있다~!!!
