#pragma once

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

typedef struct 
{
	bool	bVisitedFlag;
	CvPoint ptReturnPoint;
} Visited;

class CBlobLabeling
{
public:
	CBlobLabeling(void);
	~CBlobLabeling(void);

public:
	IplImage*	m_Image;				// 레이블링을 위한 이미지
	int			m_nThreshold;			// 레이블링 스레스홀드(임계값)
	Visited*	m_vPoint;				// 레이블링시 방문정보
	int			m_nBlobs;				// 레이블의 갯수
	CvRect*		m_recBlobs;				// 각 레이블 정보


public:
	// 레이블링 이미지 선택(이미지, 임계값)
	void		SetParam(IplImage* image, int nThreshold);

	// 레이블링(실행)
	void		DoLabeling();

private:
	// 레이블링(이미지, 임계값)
	int		 Labeling(IplImage* image, int nThreshold);

	// 포인트 초기화(너비, 높이)
	void	 InitvPoint(int nWidth, int nHeight);
	void	 DeletevPoint();

	// 레이블링 결과 얻기(레이블 번호, 임시버퍼, 너비, 높이)
	void	 DetectLabelingRegion(int nLabelNumber, unsigned char *DataBuf, int nWidth, int nHeight);

	// 레이블링(실제 알고리즘) (버퍼, 너비, 높이, 임계값)
	int		_Labeling(unsigned char *DataBuf, int nWidth, int nHeight, int nThreshold);
	
	// _Labling 내부 사용 함수 
	// 인접 영역 설정(버퍼, 너비, 높이, X,Y,시작X점, 시작 Y점, 끝X점, 끝 Y점)
	int		__NRFIndNeighbor(unsigned char *DataBuf, int nWidth, int nHeight, int nPosX, int nPosY, int *StartX, int *StartY, int *EndX, int *EndY );
	// 지역 설정 (버퍼, 시작X점, 시작 Y점, 끝X점, 끝Y점, 너비, 레벨)
	int		__Area(unsigned char *DataBuf, int StartX, int StartY, int EndX, int EndY, int nWidth, int nLevel);

	// 가로, 세로 크기가 비교값 보다 작은 레이블 제거
	// 지정한 값보다 작은 레이블 제거
public:
	void	BlobSmallSizeConstraint(int nWidth, int nHeight);
private:
	void	_BlobSmallSizeConstraint(int nWidth, int nHeight, CvRect* rect, int* nRecNumber);

	// 가로, 세로 크기가 비교값 보다 큰 레이블 제거
	// 지정한 값보다 큰 레이블 제거
public:
	void	BlobBigSizeConstraint(int nWidth, int nHeight);
private:
	void	_BlobBigSizeConstraint(int nWidth, int nHeight, CvRect* rect, int* nRecNumber);



	// 레이블 머징
public:
	void	MergeLabel( int mergerange );
};
