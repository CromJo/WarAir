#ifndef _sprite_h_
#define _sprite_h_

#include <ddraw.h>
#include "bmp.h"

class CSprite
{
private:
	int m_nFrame;
	int m_nWidth;
	int m_nHeight;
	LPDIRECTDRAWSURFACE7 *m_ppSurface;
	CBMP *m_pBMPArray;//bmparray 배열로 그림들 있음
public:
	CSprite();
	~CSprite();
	int GetNumberOfFrame();
	bool InitSprite(int nFrame, int nWidth, int nHeight, int nColorKey, LPDIRECTDRAW7 pDirectDraw);
	bool LoadFrame(int nFrame, char *filename);
	bool Drawing(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true);
	bool Drawing2(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true);
	bool Restore();
	bool ReleaseAll();
};
#endif