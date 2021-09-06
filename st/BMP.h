//CBMP class
//24bits Bitmap file을 읽는 기능을 수행한다
//32bits DirectDrawSurface에 복사할 수 있는 기능이 있음
#ifndef _bmp_h_
#define _bmp_h_

#include <ddraw.h>

class CBMP
{
private:
	int m_nWidth;
	int m_nHeight;
	unsigned char *m_pBuffer;

public:
	CBMP();
	~CBMP();
	int GetWidth();
	int GetHeight();
	bool LoadBMPFile(char* filename);
	bool CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface);
};
#endif