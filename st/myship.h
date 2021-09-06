#ifndef _myship_h_
#define _myship_h_

#include "GObject.h"

class CMyShip:public CGObject
{
private:
	int m_nLastMoveTime;
	int	m_nMoveInterval;



public:
	CMyShip();
	~CMyShip();
	void Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void DrawFrame(LPDIRECTDRAWSURFACE7 lpSurface, int Frame);
	void Left();
	void Right();
};
#endif