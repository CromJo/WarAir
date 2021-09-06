#ifndef _BossMissile_h_
#define _BossMissile_h_

#include "gobject.h"

class CBossMissile :public  CGObject
{
private:
	int m_nLastMoveTime;//마지막 움직인 시간
	int m_nMoveInterval;//움직임 간격

public:
	int m_speedx;//x좌표 속도

public:
	CBossMissile();//생성자
	~CBossMissile();//소멸자
	void Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval);//이니셜라이즈(초기화)-객체만듬
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);//그려주기위해
	void Move();//움직이기위해
};
#endif