//class CMyMissile
#ifndef _mymissile_h_
#define _mymissile_h_

#include "gobject.h"

class CMyMissile :public CGObject
{
private:
	int m_nLastMoveTime;//마지막으로 움직인 시간
	int m_nMoveInterval;//움직인 시간 간격

public:
	CMyMissile();
	~CMyMissile();
	void Initialize(CSprite *pSprite, int, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval);//플레이어 미사일 초기화
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);//플레이어 미사일 그려줌
	void Move(); //플레이어 미사일 움직임 제어
};
#endif