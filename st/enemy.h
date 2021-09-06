//class CEnemy
#ifndef _enemy_h
#define _enemy_h_

#include "gobject.h"

class CEnemy : public CGObject
{
private:
	int m_nLastMoveTime;//마지막으로 움직인 시간
	int m_nMoveInterval;//움직임의 시간 간격

public:
	int m_speedx;//적기 좌우움직임 제어
	int m_speedy;//양쪽중 하나의 벽이라도 충돌하였을경우 아래로 이동 하기 윟마

public:
	CEnemy();//적기 스피드 초기화
	~CEnemy();
	void Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval);//적기 초기화
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);//적기를 그려줌
	void DrawMissile(LPDIRECTDRAWSURFACE7 lpSurface);//쓰이는 곳이 없음(?)
	void CreateMissile();//쓰이는 곳이 없음(?)
	void Move();//적기 움직임 제어
};
#endif