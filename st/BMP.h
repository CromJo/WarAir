//CBMP class
//24bits Bitmap file�� �д� ����� �����Ѵ�
//32bits DirectDrawSurface�� ������ �� �ִ� ����� ����
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