//GObject.h
//class CGObject

#ifndef _gobject_h_
#define _gobject_h_

#include "sprite.h"
#include "timer.h"
#include <ddraw.h>

class CGObject
{
	//protected;
public:
	int m_x;
	int m_y;
protected:
	CSprite *m_pSprite;
	int m_nFrameInterval;
	int m_nCurrentFrame;
	int m_nLastFrameTime;
	CTimer *m_pTimer;
	bool m_bIsLive;
public:
	CGObject();
	~CGObject();
	void Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval);
	void Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface);
	void DrawFrame(int x, int y, int nFrame, LPDIRECTDRAWSURFACE7 lpSurface);
	bool IsLive();
	void Kill();
	int GetX();
	int GetY();
};
#endif

