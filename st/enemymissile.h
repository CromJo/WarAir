//enemymissile.h
#ifndef __EnemyMissile_h__
#define __EnemyMissile_h__

#include "gobject.h"

class CEnemyMissile : public CGObject
{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;

public:
	CEnemyMissile();
	~CEnemyMissile();
	void Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame,
		int FrameInterval, int MoveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void Move();
};
#endif