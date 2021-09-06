//bmp.cpp
//CBMp class의 구현
//24bits RGB bmp file을 읽고
//DirectX에서는 32bits mode Surface에 복사시키는 기능이 있음.

#include <windows.h>//api프로그래맹시 선언하는 헤더
#include <windowsx.h>//필요한 기능들을 미리 선언해놓은 헤더
#include "bmp.h"//사용자 정의 헤더 생성

CBMP::CBMP() //class cbmp구조체 안의 cbmp(생성자)
{
	m_pBuffer = NULL; //초기값 null
	m_nWidth = 0; //길이값 초기값0
	m_nHeight = 0; //높이 초기값0
}
CBMP::~CBMP()//소멸자 - 버퍼가 포인터로 되있어서 해제필요
{
	if(m_pBuffer)//m_pbuffer가 null값일때
	{
		delete[] m_pBuffer;//m_pbuffer의 소멸자 호출
	}
}
int CBMP::GetHeight()//cbmp구조체 안에서 height값 가져옴
{
	return (m_nHeight);//가져온 nHeight값 리턴
}
int CBMP::GetWidth()
{
	return (m_nWidth);
}

//24bits bmp file을 읽는다. 이것을 내부 버퍼에 넣을 때 32bit ARGB 형식으로 바꾼다.
//자롯된 경우가 발생하면 false값을 반환한다.
//성공하면 true값을 반환한다.

bool CBMP::LoadBMPFile(char *filename)//캐릭터형의 파일이름을 받아와서 파일염
{
	// 파일 열기
	HANDLE hfile;//hfile 객체핸들 선언
	DWORD actualRead;//더블워드(4바이트)

	hfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	//파일 입출력 장치를 만들거나 연다
	//createfile(파일이름에대한포인터,접근모드,공유모드,보안속성에대한포인터,생성하는방식,파일속성,복사하기위한속성을가진파일의핸들)
	if(hfile == INVALID_HANDLE_VALUE)//invalid~value로 유효값 판단
	{
		return false;//false로 반환
	}

	// 파일 내용 읽기, 만일 24bit bmp file이 아니라면 false값을 반환
	// 24bit bmp file 내용을 32bit ARGE 형식으로 바꿀 것.
	//bottom-up으로 저장된 bmp라면 top-down으로 버퍼에 기록하도록 할 것.
	// 1.BITMAPFILEHEADER 읽기

	BITMAPFILEHEADER bmpfilehead;//비트맵 파일의 헤더를 구현한 구조체

	if(!ReadFile(hfile, &bmpfilehead,sizeof(bmpfilehead), &actualRead, NULL))//http://m.blog.naver.com/power2845/50144349557  안전성 검사
	{
		CloseHandle(hfile);//hfile객체 핸들을 닫음
		return false;//false로 반환
	}
	// bitmap file인지 점검한다.
	if(bmpfilehead.bfType != 0x4D42)
	{
		CloseHandle(hfile);
		return false;
	}
	// 2.BITMAPINFOHEADER 읽기
	BITMAPINFOHEADER bmpinfohead;//비트맵(dib)의 색상 형식에 대한 정보를 가짐

	if(!ReadFile(hfile, &bmpinfohead, sizeof(bmpinfohead), &actualRead, NULL))
	{
		CloseHandle(hfile);
		return false;
	}
	//24bits RGB bmp file인지 점검한다.
	if(bmpinfohead.biBitCount != 24)//비트맵 형식 24빗 아니면
	{
		CloseHandle(hfile);//hfile객체핸들종료
		return false;
	}
	//bottom-up 순서로 저장하고 있는 bmp file 인가를 점검한다
	int nHeight = bmpinfohead.biHeight;	
	bool bBottomUp; // bmp file	저장순서가 bottom-up이면 true, 아니면 false

	if(nHeight > 0)//이미지가 정상
	{
		bBottomUp = true;
	}
	else//반대로 되있으면
	{
		bBottomUp = false;
		nHeight = -nHeight;//돌려줌
	}

	m_nHeight = nHeight;
	m_nWidth = bmpinfohead.biWidth;

	// 3.Pixel data를 buffer에 읽어 들이기
	// buffer를 생성하고 pixel data를 읽을 준비를 한다.
	if(m_pBuffer)
	{
		delete[] m_pBuffer;//버퍼 있으면 지움
	}

	m_pBuffer = new unsigned char[m_nHeight *m_nWidth *4];

	// bmp파일로부터 pixel data를 읽는다.
	struct RGBstruct
	{
		unsigned char B;//unsigned char=부호가없는정수=양수(0~256)
		unsigned char G;
		unsigned char R;
	};

	struct RGBstruct rgb24;//rgb24 구조체 선언

	int nStoredLine = (m_nWidth * 3 + 3)&~3;		//bmp file에 저장된 한 줄의 byte 수
	unsigned char temp[4];							//bmp file의 한 줄의 끝 부분 값 읽음.
	int nRemainder = nStoredLine - (m_nWidth *3);	//bmp file의 한 줄 끝 부분에 추가한 byte 수

	int nDestY, nDeltaY;							//bottom-up or top-down에 따라 값 결정.

	if(bBottomUp)//버튼업 상태일때
	{
		nDestY = nHeight -1;
		nDeltaY = -1;
	}
	else
	{
		nDestY = 0;
		nDeltaY = 1;
	}

	for(int y=0; y<nHeight; y++)
	{
		for(int x=0; x<m_nWidth; x++)
		{
			if(!ReadFile(hfile,&rgb24,3,&actualRead,NULL))
			{
				CloseHandle(hfile);
				delete []m_pBuffer;
				return false;
			}
			CopyMemory(m_pBuffer + (x<<2) + nDestY *(m_nWidth<<2), &rgb24,3);
			*(m_pBuffer + (x<<2) + nDestY *(m_nWidth<<2)+3) = 0;				// Alpha 값을 0으로
		}
		nDestY += nDeltaY;

		if(!ReadFile(hfile, temp, nRemainder, &actualRead, NULL))
		{
			CloseHandle(hfile);
			delete []m_pBuffer;
			return false;
		}
	}
	// 파일 닫기
	CloseHandle(hfile);
	return true;
}

bool CBMP::CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface)
{
	// 만일 버퍼가 비었다면 동작 중지
	if(!m_pBuffer)
	{
		return false;
	}
	// 1. Lock()
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);

	if(FAILED(lpSurface->Lock(NULL,&ddsd, DDLOCK_WAIT, NULL)))
	{
		return false;
	}
	// 2. buffer에서 Surface로 복사
	// 복사
	unsigned char *pDest, *pSrc;

	pDest = (unsigned char *)ddsd.lpSurface;
	pSrc = m_pBuffer;

	for(int y=0; y<m_nHeight; y++)
	{
		CopyMemory(pDest,pSrc,m_nWidth<<2);
		pDest += ddsd.lPitch;
		pSrc += (m_nWidth << 2);
	}
	// 3. Unlock()
	lpSurface->Unlock(NULL);

	return true;
}
