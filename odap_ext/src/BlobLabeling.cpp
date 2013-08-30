#include "BlobLabeling.h"

#define _DEF_MAX_BLOBS		10000
#define _DEF_MAX_LABEL		  100

CBlobLabeling::CBlobLabeling(void)
//생성자
//레이블 변수들의 초기값 설정
//임계값 0 
//블롭 갯수 10000개
//이미지 없음
//레이블 정보 없음
{ 
	m_nThreshold	= 0;
	m_nBlobs		= _DEF_MAX_BLOBS;
	m_Image			= NULL;
	m_recBlobs		= NULL;
	m_vPoint		= NULL;
}
CBlobLabeling::~CBlobLabeling(void)
//파괴자
//메모리를 전부 해제해준다.
{
	if( m_Image != NULL )	cvReleaseImage( &m_Image );	

	if( m_recBlobs != NULL )
	{
		delete m_recBlobs;
		m_recBlobs = NULL;
	}
}
void CBlobLabeling::SetParam(IplImage* image, int nThreshold)
//레이블들의 임계값 설정.
//임계값으로 들어오는 값은 픽셀수.
//레이블로 저장되는 최소 픽셀 수다.
//값이 크면 잡영들이 제거되고, 값이 작으면 세세한 정보를 얻어올 수 있다.
{
	if( m_recBlobs != NULL )
	{
		delete m_recBlobs;

		m_recBlobs	= NULL;
		m_nBlobs	= _DEF_MAX_BLOBS;
	}

	if( m_Image != NULL )	cvReleaseImage( &m_Image );

	m_Image			= cvCloneImage( image );

	m_nThreshold	= nThreshold;
}
void CBlobLabeling::DoLabeling()
//레이블링을 호출하는 함수
{
	m_nBlobs = Labeling(m_Image, m_nThreshold);
}

int CBlobLabeling::Labeling(IplImage* image, int nThreshold)
//레이블링을 실행.
//이미지와 임계값을 받아서 실행한다.
//이미지의 높이와 너비를 받아서 두 값을 곱한 만큼의 버퍼를 설정한다.
{
	if( image->nChannels != 1 ) 	return 0;

	int nNumber;
	
	int nWidth	= image->width;
	int nHeight = image->height;
	
	unsigned char* tmpBuf = new unsigned char [nWidth * nHeight];

	int i,j;

	for(j=0;j<nHeight;j++)
	for(i=0;i<nWidth ;i++)
		tmpBuf[j*nWidth+i] = (unsigned char)image->imageData[j*image->widthStep+i];
	
	// 레이블링을 위한 포인트 초기화
	InitvPoint(nWidth, nHeight);

	// 레이블링
	nNumber = _Labeling(tmpBuf, nWidth, nHeight, nThreshold);

	// 포인트 메모리 해제
	DeletevPoint();

	if( nNumber != _DEF_MAX_BLOBS )		m_recBlobs = new CvRect [nNumber];

	if( nNumber != 0 )	DetectLabelingRegion(nNumber, tmpBuf, nWidth, nHeight);

	for(j=0;j<nHeight;j++)
	for(i=0;i<nWidth ;i++)
		image->imageData[j*image->widthStep+i] = tmpBuf[j*nWidth+i];

	delete tmpBuf;
	return nNumber;
}
void CBlobLabeling::InitvPoint(int nWidth, int nHeight)
// m_vPoint 초기화 함수
//m_vPoint에는 방문한 픽셀을 체크해준다.
//너비와 높이를 곱한만큼의 방문 배열을 생성.
{
	int nX, nY;

	m_vPoint = new Visited [nWidth * nHeight];

	for(nY = 0; nY < nHeight; nY++)
	{
		for(nX = 0; nX < nWidth; nX++)
		{
			m_vPoint[nY * nWidth + nX].bVisitedFlag		= false;
			m_vPoint[nY * nWidth + nX].ptReturnPoint.x	= nX;
			m_vPoint[nY * nWidth + nX].ptReturnPoint.y	= nY;
		}
	}
}

void CBlobLabeling::DeletevPoint()
//m_vPoint 파괴자
{
	delete m_vPoint;
}
int CBlobLabeling::_Labeling(unsigned char *DataBuf, int nWidth, int nHeight, int nThreshold)
// Size가 nWidth이고 nHeight인 DataBuf에서 
// nThreshold(임계값)보다 작은 영역을 제외한 나머지를 blob으로 획득
{
	//int Index = 0,
	int num = 0;
	int nX, nY, k, l;
	int StartX , StartY, EndX , EndY;
	
	//연결된 컴포넌트를 찾는다.
	//y축은 높이, x축은 너비
	//x축부터 확인
	for(nY = 0; nY < nHeight; nY++)
	{
		for(nX = 0; nX < nWidth; nX++)
		{
			if(DataBuf[nY * nWidth + nX] == 255)
			//새로운 컴포넌트인가 확인.
			//오브젝트 == 255(흰색)
			{
				num++;

				DataBuf[nY * nWidth + nX] = num;
				
				StartX = nX, StartY = nY, EndX = nX, EndY= nY;

				__NRFIndNeighbor(DataBuf, nWidth, nHeight, nX, nY, &StartX, &StartY, &EndX, &EndY);

				if(__Area(DataBuf, StartX, StartY, EndX, EndY, nWidth, num) < nThreshold)
				{
		 			for(k = StartY; k <= EndY; k++)
					{
						for(l = StartX; l <= EndX; l++)
						{
							if(DataBuf[k * nWidth + l] == num)
								DataBuf[k * nWidth + l] = 0;
						}
					}
					--num;

					if(num > 250)
						return  0;
				}
			}
		}
	}

	return num;	
}

 
void CBlobLabeling::DetectLabelingRegion(int nLabelNumber, unsigned char *DataBuf, int nWidth, int nHeight)
// Blob labeling해서 얻어진 결과의 rec을 얻어냄
{
	int nX, nY;
	int nLabelIndex ;

	bool bFirstFlag[255] = {false,};
	
	for(nY = 1; nY < nHeight - 1; nY++)
	{
		for(nX = 1; nX < nWidth - 1; nX++)
		{
			nLabelIndex = DataBuf[nY * nWidth + nX];

			if(nLabelIndex != 0)	// Is this a new component?, 255 == Object
			{
				if(bFirstFlag[nLabelIndex] == false)
				{//플래그가 false면 초기화
					m_recBlobs[nLabelIndex-1].x			= nX;
					m_recBlobs[nLabelIndex-1].y			= nY;
					m_recBlobs[nLabelIndex-1].width		= 0;
					m_recBlobs[nLabelIndex-1].height	= 0;
				
					bFirstFlag[nLabelIndex] = TRUE;
				}
				else
					//플래그가 트루면
					// 왼쪽 오른쪽 가장 상위, 가장 아래 부분의 값을 설정
				{
					int left	= m_recBlobs[nLabelIndex-1].x;
					int right	= left + m_recBlobs[nLabelIndex-1].width;
					int top		= m_recBlobs[nLabelIndex-1].y;
					int bottom	= top + m_recBlobs[nLabelIndex-1].height;

					//각 방향값에 따라 x와 y좌표를 재설정

					if( left   >= nX )	left	= nX;
					if( right  <= nX )	right	= nX;
					if( top    >= nY )	top		= nY;
					if( bottom <= nY )	bottom	= nY;

					//설정된 방향값을 바탕으로 블롭 사각형의 좌표 설정.

					m_recBlobs[nLabelIndex-1].x			= left;
					m_recBlobs[nLabelIndex-1].y			= top;
					m_recBlobs[nLabelIndex-1].width		= right - left;
					m_recBlobs[nLabelIndex-1].height	= bottom - top;

				}
			}
				
		}
	}
	
}


int CBlobLabeling::__NRFIndNeighbor(unsigned char *DataBuf, int nWidth, int nHeight, int nPosX, int nPosY, int *StartX, int *StartY, int *EndX, int *EndY )
// Blob Labeling을 실제 행하는 function
{
	CvPoint CurrentPoint;
	
	CurrentPoint.x = nPosX;
	CurrentPoint.y = nPosY;

	m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x].bVisitedFlag    = TRUE;
	m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x].ptReturnPoint.x = nPosX;
	m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x].ptReturnPoint.y = nPosY;
			
	while(1)
	{
		if( (CurrentPoint.x != 0) && (DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x - 1] == 255) )   // -X 방향
		{
			if( m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x - 1].bVisitedFlag == false )
			{
				DataBuf[CurrentPoint.y  * nWidth + CurrentPoint.x  - 1]					= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x - 1].bVisitedFlag	= TRUE;
				m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x - 1].ptReturnPoint	= CurrentPoint;
				CurrentPoint.x--;
				
				if(CurrentPoint.x <= 0)
					CurrentPoint.x = 0;

				if(*StartX >= CurrentPoint.x)
					*StartX = CurrentPoint.x;

				continue;
			}
		}

		if( (CurrentPoint.x != nWidth - 1) && (DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x + 1] == 255) )   // -X 방향
		{
			if( m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x + 1].bVisitedFlag == false )
			{
				DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x + 1]					= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x + 1].bVisitedFlag	= TRUE;
				m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x + 1].ptReturnPoint	= CurrentPoint;
				CurrentPoint.x++;

				if(CurrentPoint.x >= nWidth - 1)
					CurrentPoint.x = nWidth - 1;
				
				if(*EndX <= CurrentPoint.x)
					*EndX = CurrentPoint.x;

				continue;
			}
		}

		if( (CurrentPoint.y != 0) && (DataBuf[(CurrentPoint.y - 1) * nWidth + CurrentPoint.x] == 255) )   // -X 방향
		{
			if( m_vPoint[(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x].bVisitedFlag == false )
			{
				DataBuf[(CurrentPoint.y - 1) * nWidth + CurrentPoint.x]					= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_vPoint[(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x].bVisitedFlag	= TRUE;
				m_vPoint[(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x].ptReturnPoint = CurrentPoint;
				CurrentPoint.y--;

				if(CurrentPoint.y <= 0)
					CurrentPoint.y = 0;

				if(*StartY >= CurrentPoint.y)
					*StartY = CurrentPoint.y;

				continue;
			}
		}
	
		if( (CurrentPoint.y != nHeight - 1) && (DataBuf[(CurrentPoint.y + 1) * nWidth + CurrentPoint.x] == 255) )   // -X 방향
		{
			if( m_vPoint[(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x].bVisitedFlag == false )
			{
				DataBuf[(CurrentPoint.y + 1) * nWidth + CurrentPoint.x]					= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_vPoint[(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x].bVisitedFlag	= TRUE;
				m_vPoint[(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x].ptReturnPoint = CurrentPoint;
				CurrentPoint.y++;

				if(CurrentPoint.y >= nHeight - 1)
					CurrentPoint.y = nHeight - 1;

				if(*EndY <= CurrentPoint.y)
					*EndY = CurrentPoint.y;

				continue;
			}
		}
		
		if(		(CurrentPoint.x == m_vPoint[CurrentPoint.y * nWidth + CurrentPoint.x].ptReturnPoint.x) 
			&&	(CurrentPoint.y == m_vPoint[CurrentPoint.y * nWidth + CurrentPoint.x].ptReturnPoint.y) )
		{
			break;
		}
		else
		{
			CurrentPoint = m_vPoint[CurrentPoint.y * nWidth + CurrentPoint.x].ptReturnPoint;
		}
	}

	return 0;
}

// 영역중 실제 blob의 칼라를 가진 영역의 크기를 획득
int CBlobLabeling::__Area(unsigned char *DataBuf, int StartX, int StartY, int EndX, int EndY, int nWidth, int nLevel)
{
	int nArea = 0;
	int nX, nY;

	for (nY = StartY; nY < EndY; nY++)
		for (nX = StartX; nX < EndX; nX++)
			if (DataBuf[nY * nWidth + nX] == nLevel)
				++nArea;

	return nArea;
}
/******************************************************************************************************/
////////////////////////////////////////////
// 잡영 제거를 위한 레이블링 루틴///////////
////////////////////////////////////////////
// nWidth와 nHeight보다 작은 rec을 모두 삭제
void CBlobLabeling::BlobSmallSizeConstraint(int nWidth, int nHeight)
{
	_BlobSmallSizeConstraint(nWidth, nHeight, m_recBlobs, &m_nBlobs);
}

void CBlobLabeling::_BlobSmallSizeConstraint(int nWidth, int nHeight, CvRect* rect, int *nRecNumber)
{
	if(*nRecNumber == 0)	return;

	int nX;
	int ntempRec = 0;

	CvRect* temp = new CvRect[*nRecNumber];

	for(nX = 0; nX < *nRecNumber; nX++)
	{
		temp[nX] = rect[nX];
	}

	for(nX = 0; nX < *nRecNumber; nX++)
	{
		if( (rect[nX].width > nWidth) && (rect[nX].height > nHeight) )
		{
			temp[ntempRec] = rect[nX];
			ntempRec++;
		}
	}

	*nRecNumber = ntempRec;

	for(nX = 0; nX < *nRecNumber; nX++)
		rect[nX] = temp[nX];

	delete temp;
}
// nWidth와 nHeight보다 큰 rec을 모두 삭제

void CBlobLabeling::BlobBigSizeConstraint(int nWidth, int nHeight)
{
	_BlobBigSizeConstraint(nWidth, nHeight, m_recBlobs, &m_nBlobs);
}

void CBlobLabeling::_BlobBigSizeConstraint(int nWidth, int nHeight, CvRect* rect, int* nRecNumber)
{
	if(*nRecNumber == 0)	return;

	int nX;
	int ntempRec = 0;

	CvRect* temp = new CvRect [*nRecNumber];

	for(nX = 0; nX < *nRecNumber; nX++)
	{
		temp[nX] = rect[nX];
	}

	for(nX = 0; nX < *nRecNumber; nX++)
	{
		if( (rect[nX].width < nWidth) && (rect[nX].height < nHeight) )
		{
			temp[ntempRec] = rect[nX];
			ntempRec++;
		}
	}

	*nRecNumber = ntempRec;

	for(nX = 0; nX < *nRecNumber; nX++)
		rect[nX] = temp[nX];

	delete temp;
}

#define	ABS(x)						( x > 0 ? x : x * -1 )
#define CENTER_X(rec) 				( (rec).x + ( (rec).width /2 ) )
#define CENTER_Y(rec) 				( (rec).y + ( (rec).height /2 ) )

bool crash_rect( CvRect rec1, CvRect rec2, int distance ){

	// 단순한 선분으로 만들자
	int rec1_x1 = rec1.x - distance;
	int rec1_x2 = rec1.x + rec1.width + distance;
	int rec1_y1 = rec1.y - distance;
	int rec1_y2 = rec1.y + rec1.height + distance;

	int rec2_x1 = rec2.x;
	int rec2_x2 = rec2.x + rec2.width;
	int rec2_y1 = rec2.y;
	int rec2_y2 = rec2.y + rec2.height;

	// x축 비교
	if( !( rec1_x1 > rec2_x2 || rec1_x2 < rec2_x1 ) &&			// x1 의 외곽이면 안되는거야
		   !( rec1_y1 > rec2_y2 || rec1_y2 < rec2_y1 )) return true;

	return false;


}

// 레이블 머징 !!!!
#include <iostream>
using namespace std;

void paint_rect(CvRect rec ){
	cout << rec.x << " " << rec.y << " " << rec.width << " " << rec.height <<" ";
}

void CBlobLabeling::MergeLabel( int mergerange ){

	/*
	// 테스트 코드
	CvRect t1, t2;
	t1.x = 0; t1.y = 0; t1.width = 10; t1.height = 10;t2.x = 5; t2.y = 5; t2.width = 10; t2.height = 3;
	cout << "test " << crash_rect( t1, t2 ) << endl;
	t1.x = 2; t1.y = 0; t1.width = 10; t1.height = 10;t2.x = 0; t2.y = 5; t2.width = 10; t2.height = 3;
	cout << "test " << crash_rect( t1, t2 ) << endl;

	t1.x = 0; t1.y = 5; t1.width = 25; t1.height = 30;t2.x = 5; t2.y = 0; t2.width = 10; t2.height = 10;
	cout << "test " << crash_rect( t1, t2 ) << endl;
	*/

	if( m_nBlobs <= 0 ) return;

	int	curr_index = 0;
	int curcle_cnt = 0;

	while( curr_index < m_nBlobs - 1 ){

		int mergecnt = 0;
		int ntempRec = 0;
		CvRect* temp = new CvRect [m_nBlobs];
		CvRect	target = m_recBlobs[curr_index];

		// 일단 겹치는 것만 ^^
		for( int i = 0 ; i < m_nBlobs ; ++ i ){

			if( i == curr_index ) continue;	// 자기 자신과의 비교는 하지 않는다.

			if( crash_rect( target, m_recBlobs[i], mergerange ) ){

				// 영역을 합칩니다~!
				int cur_x1 = target.x;
				int cur_x2 = target.x + target.width;
				int cur_y1 = target.y;
				int cur_y2 = target.y + target.height;

				int t_x1 = m_recBlobs[i].x;
				int t_x2 = m_recBlobs[i].x + m_recBlobs[i].width;
				int t_y1 = m_recBlobs[i].y;
				int t_y2 = m_recBlobs[i].y + m_recBlobs[i].height;

					// x1 선정
				target.x 		= ( t_x1 < cur_x1 ? t_x1 : cur_x1 );			// x1 좌표는 작은게 좋음
				target.width 	= ( t_x2 > cur_x2 ? t_x2 : cur_x2 ) - target.x;	// x1 좌표는 작은게 좋음

				target.y 		= ( t_y1 < cur_y1 ? t_y1 : cur_y1 );			// x1 좌표는 작은게 좋음
				target.height 	= ( t_y2 > cur_y2 ? t_y2 : cur_y2 ) - target.y;	// x1 좌표는 작은게 좋음

				/*
				cout<< "is carsh ";
				paint_rect( m_recBlobs[i]);
				paint_rect( target );
				cout<< endl;
				*/

				++ mergecnt;

			}else{
				// 그냥 넣습니다.
				temp[ntempRec] = m_recBlobs[i];
				++ ntempRec;
			}
		}

		temp[ntempRec] = target;	// 타겟을 마지막에 넣음
		++ ntempRec;

		// 교체
		m_nBlobs = ntempRec;
		for( int i = 0; i < m_nBlobs; i++){
			m_recBlobs[i] = temp[i];
		}
		delete temp;

		// 다음 인덱스
		if( 0 == mergecnt ){
			// 대상과 머지할 것이 더이상 없을 경우에만 다음으로 진행 할 수 있다!!!
			++curr_index;
		}

		++ curcle_cnt;
	}

	cout<< "count " << curcle_cnt << endl;


}
